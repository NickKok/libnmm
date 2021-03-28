#include "ReflexController.hh"


#include <string>
#include <math.h>
#include <vector>
#include <fstream>
#include <map>
#include <array>

//#include <boost/algorithm/string.hpp>

#include <dynamics/link.hh>
#include <timer/clock.hh>

#include <sml/types/types.h>
#include <sml/neuralSystem/Interneuron.hh>
#include <sml/neuralSystem/Motoneuron.hh>
#include <sml/neuralSystem/Entity.hh>
#include <sml/neuralSystem/Connection.hh>

#include <sml/sml-tools/EventManager.hh>
#include <sml/sml-tools/OnlineCtrl.hh>
#include <sml/sml-tools/Settings.hh>

extern EventManager* eventManager;
namespace nmm {
  extern SmlParameters parameters;
}
using namespace nmm;
using namespace std;


ReflexController::ReflexController(): Controller(){

}

void ReflexController::update(){
    step_INsen_to_IN();
    print_debug("[ok] : step_INsen_to_IN (ReflexController.cc)");
    step_INcpg_to_IN();
    print_debug("[ok] : step_INcpg_to_IN (ReflexController.cc)");
    step_IN_to_MN();
    print_debug("[ok] : step_IN_to_MN (ReflexController.cc)");
    step_MN_to_output();
    print_debug("[ok] : step_MN_to_output (ReflexController.cc)");
}


//DEPRECATED TO BE REMOVED;
void ReflexController::initialiseFeedbacks(SIDE::Side side){
}

void ReflexController::initialiseFeedbacks(){
    for(auto &kv: SENSORY_INTERNEURON::latentConnections){
        if(kv.inputs.size() == 1){
            INsens[kv.id] = new SensoryInterneuron(
                kv.id,
                getInput(kv.inputs[0]),
                motoneurons[kv.output], 
                kv.weight,
                kv.constantFactor);
        }
        if(kv.inputs.size() == 2){
            INsens[kv.id] = new SensorySensoryInterneuron(
                kv.id,
                getInput(kv.inputs[0]),
                getInput(kv.inputs[1]),
                motoneurons[kv.output], 
                kv.weight,
                kv.constantFactor);
        }
    }
    using namespace SENSORY_INTERNEURON;
    if(Settings::get<int>("notf_trunklean") == 1){
        INsens[RIGHT_HF_TRUNK_LEAK_SWING]->no_tf=true;
        INsens[RIGHT_HAB_TRUNK_LEAK_SWING]->no_tf=false;
        INsens[RIGHT_HAD_TRUNK_LEAK_SWING]->no_tf=false;
        INsens[RIGHT_HAB_TRUNK_ACC_SWING]->no_tf=false;
        INsens[RIGHT_HAD_TRUNK_ACC_SWING]->no_tf=false;
        INsens[LEFT_HF_TRUNK_LEAK_SWING]->no_tf=true;
        INsens[LEFT_HAB_TRUNK_LEAK_SWING]->no_tf=false;
        INsens[LEFT_HAD_TRUNK_LEAK_SWING]->no_tf=false;
        INsens[LEFT_HAB_TRUNK_ACC_SWING]->no_tf=false;
        INsens[LEFT_HAD_TRUNK_ACC_SWING]->no_tf=false;
    }
}

void ReflexController::initCpgParameters(){
    std::vector<std::string> strs;
    print_verbose("[ReflexController] Cpg scheme variable initialisation");
    ifstream myfile (Settings::get<string>("config")+"cpg_gate/"+Settings::get<string>("config_cpg_scheme"));
    string line ; // the string containing the line
    if (myfile.is_open()){
        while( std::getline( myfile, line ) )
        {
            boost::split(strs, line, boost::is_any_of("\t "));
            string name;
            bool modeled=false;
            if(strs[0][0]!='#'){
                name = strs[0];
                modeled=true;
            }
            else{
                name = strs[0].substr(1);
            }

            parameters[2][name]=0.0; // pure feedback
            parameters[3][name]=0.0; // constant cpg


            if(modeled){
                modelized_sensory_interneurons.push_back(name);
            }



        }

        myfile.close();
    }
    else{
        cerr << "ERROR: unable to open file '" << Settings::get<string>("config")+"cpg_gate/" << Settings::get<string>("config_cpg_scheme") << "'. In:Parameters.cc, constructor(string, string)"<<endl;
    }
}
void ReflexController::loadCpgFile(){
    std::vector<std::string> strs; // the vector containing the key->value pair
    print_verbose("[CPG Predictor] Loading cpg scheme : " + Settings::get<string>("config_cpg_scheme"));
    ifstream myfile (Settings::get<string>("config")+"cpg_gate/"+Settings::get<string>("config_cpg_scheme"));
    string line ; // the string containing the line
    if (myfile.is_open()){
        while( std::getline( myfile, line ) )
        {
            boost::split(strs, line, boost::is_any_of("\t "));
            string name;
            bool modeled=false;
            if(strs[0][0]!='#'){
                name = strs[0];
                modeled=true;
            }
            else{
                name = strs[0].substr(1);
            }

            parameters[2][name]=0.0; // pure feedback
            parameters[3][name]=0.0; // constant cpg

            if(modeled){
                modelized_sensory_interneurons.push_back(name);
                string type = "cpg";
                if(strs.size()>1){
                    if(strs[1] == "cst"){
                        parameters[3][name]=1.0;
                        type = "cst";
                    }
                    else if(strs[1] == "cpg"){
                        parameters[3][name]=0.0;
                    }
                    else{
                        type = "unknown";
                        cerr << "unknown cpg type : ''" << strs[1] << "'', should be either ''cst'' or ''cpg''" << endl;
                    }
                }
                if(strs.size()>2){
                    parameters[2][name]=convertToDouble(strs[2]);
                }
                else{
                    parameters[2][name]=1.0;
                }
                stringstream ss;
                ss << name << "\t" << parameters[3][name] << ", type=" << type << ", contribution=" << parameters[2][name];
                print_verbose(ss.str());
            }



        }

        myfile.close();
    }
    else{
        cerr << "ERROR: unable to open file '" << Settings::get<string>("config")+"cpg_gate/" << Settings::get<string>("config_cpg_scheme") << "'. In:Parameters.cc, constructor(string, string)"<<endl;
    }

}


void ReflexController::initialiseCpgs(Foot * left_foot, Foot * right_foot){
    using namespace SENSORY_INTERNEURON;

    CPGInterneuron * cs;
    /*
     * load interneurons to be modelized
     */
    initCpgParameters();
    for (int INsen=FIRST; INsen<=LAST; INsen++){
        if(SENSORY_INTERNEURON::getSide(INsen) == SIDE::RIGHT){
            Foot * foot = right_foot;
            INcpgs[INsen] = new CPGInterneuron((SensoryInterneuron* )(INsens[INsen]),foot);
        }
        else if(SENSORY_INTERNEURON::getSide(INsen) == SIDE::LEFT){
            Foot * foot = left_foot;
            INcpgs[INsen] = new CPGInterneuron((SensoryInterneuron* )(INsens[INsen]),foot);
        }
        else{
            // Not Implemented yet.
            Foot * foot = left_foot;
            INcpgs[INsen] = new CPGInterneuron((SensoryInterneuron* )(INsens[INsen]),foot);
        }


    }
}

void ReflexController::initialiseMotoneuronsFromStringVec(){
    std::vector<std::string> muscles = inputs.get<std::vector<std::string>>("muscles");
    for(auto& kv: muscles){
        std::string muscle = kv;
        motoneurons["left_" + muscle] = new Motoneuron(SIDE::LEFT,muscle,getOutput("left_" + muscle));
        motoneurons["right_" + muscle] = new Motoneuron(SIDE::RIGHT,muscle,getOutput("right_" + muscle));
    }
}


void ReflexController::initialiseMotoneurons(){
    MUSCLES_TYPES muscles = inputs.get<MUSCLES_TYPES>("muscles");
    for(auto& kv: muscles){
        Muscle * muscle = kv.second;
        std::string muscleStr = kv.first;
        motoneurons[muscleStr] = new Motoneuron(muscle->side,muscle->name,getOutput(muscleStr));
    }
}


/*
 * load all feedbacks (as defined in initialiseFeedbacks) in the interneurones network so that they act on motoneurons
 */
void ReflexController::loadFeedback(SIDE::Side side){ //TODO : Change
    using namespace SENSORY_INTERNEURON;
    if(side == SIDE::RIGHT){
#ifndef REAL_TIME
        print_verbose("    right feedback_loaded");
#endif
        feedback_right_loaded = true;
    }
    else if(side == SIDE::LEFT){
#ifndef REAL_TIME
        print_verbose("    left feedback_loaded");
#endif
        feedback_left_loaded = true;
    }
    print_verbose("Loading Feedback");
    static int withDelay = Settings::get<int>("delay");
    for (int INsen=FIRST; INsen<=LAST; INsen++){
        for(auto& kv: motoneurons){
            std::string MN = kv.first;
            Motoneuron * motoneuron = kv.second;
            if(motoneuron->side == side && CONNECTION_TO_MOTONEURONS[INsen][MN] != 0.0){
                if(withDelay == 1){
                    int delay = CONNECTION_TO_MOTONEURONS[INsen][MN];
                    __INsen_MN[INsen][MN] = new DiscreteDelayConnection(INsens[INsen],motoneurons[MN],delay);
                }
                else{
                    __INsen_MN[INsen][MN] = new NoDelayConnection(INsens[INsen],motoneurons[MN]);
                }
            }
        }

        for(auto& kv: motoneurons){
            std::string MN = kv.first;
            Motoneuron * motoneuron = kv.second;
            if(motoneuron->side == side && CONNECTION_TO_MOTONEURONS[INsen][MN] != 0.0){
                if(withDelay == 1){
                    int delay = CONNECTION_TO_MOTONEURONS[INsen][MN];
                    __INsen_MN[INsen][MN] = new DiscreteDelayConnection(INsens[INsen],motoneurons[MN],delay);
                }
                else{
                    __INsen_MN[INsen][MN] = new NoDelayConnection(INsens[INsen],motoneurons[MN]);
                }
            }
        }
    }
    print_verbose("Feedback loaded");
}
// Load cpg replace feedback
void ReflexController::loadCpg(SIDE::Side side){
    using namespace SENSORY_INTERNEURON;
    if(side == SIDE::RIGHT){
#ifndef REAL_TIME
        print_verbose("    right cpgs loaded");
#endif
        cpg_right_loaded = true;
    }
    else if(side == SIDE::LEFT){
#ifndef REAL_TIME
        print_verbose("    left cpgs loaded");
#endif
        cpg_left_loaded = true;
    }

    for (int INcpg=FIRST; INcpg<=LAST; INcpg++){
        for(auto& kv: motoneurons){
            std::string MN = kv.first;
            Motoneuron * motoneuron = kv.second;
            if(motoneuron->side == side && CONNECTION_TO_MOTONEURONS[INcpg][MN]){
                __INcpg_MN[INcpg][MN] = new NoDelayConnection(INcpgs[INcpg],motoneurons[MN]);
            }
#ifndef REAL_TIME
            print_verbose("          - loading : " + toString(INcpg));
#endif
        }
    }
}
// /*
//  * replace feedbacks by their cpgs counterpart (for cpgs defined in initialiseCpgs)
//  */
void ReflexController::loadPartialCpg(SIDE::Side side){
    using namespace SENSORY_INTERNEURON;
    if(side == SIDE::RIGHT){
#ifndef REAL_TIME
        print_verbose("    right cpgs loaded");
#endif
        cpg_right_loaded = true;
    }
    else if(side == SIDE::LEFT){
#ifndef REAL_TIME
        print_verbose("    left cpgs loaded");
#endif
        cpg_left_loaded = true;
    }

    for (int INcpg=FIRST; INcpg<=LAST; INcpg++){
        for(auto& kv: motoneurons){
            std::string MN = kv.first;
            Motoneuron * motoneuron = kv.second;
            if(motoneuron->side == side && CONNECTION_TO_MOTONEURONS[INcpg][MN]){
                auto cc = count_if(begin(modelized_sensory_interneurons),
                    end(modelized_sensory_interneurons),
                    [&](const string& str) {
                      return toString(INcpg).find(str) != std::string::npos;
                    });
                if( cc != 0 ){
                    __INcpg_MN[INcpg][MN] = new NoDelayConnection(INcpgs[INcpg],motoneurons[MN]);
#ifndef REAL_TIME
                    print_verbose("        - loading : " + toString(INcpg));
#endif
                }
            }
        }
    }
}



CYCLE_PHASE::CyclePhase ReflexController::getLimbState(SIDE::Side side){
    CYCLE_PHASE::CyclePhase limb_state;
    if(side == SIDE::RIGHT){
        limb_state = (CYCLE_PHASE::CyclePhase(eventManager->get<int>(STATES::RIGHT_FOOT)));
    }
    else{
        limb_state = (CYCLE_PHASE::CyclePhase(eventManager->get<int>(STATES::LEFT_FOOT)));
    }
    return limb_state;
}
string ReflexController::getLimbState_str(SIDE::Side side){
    return CYCLE_PHASE::toString(getLimbState(side));
}
bool ReflexController::isGateOpen(Interneuron * interneuron){
    static bool add;
    add=false;
    /*** FEEDBACK ALWAYS ACTIVE */
    if(interneuron->activeDuring() == ACTIVE_DURING::WHOLE_CYCLE) add=true;
    /*** FEEDBACK ACTIVE DURING STANCE / SWING */
    //if(getLimbState_str(interneuron->getOutputEntity()->side).find(interneuron->activeDuring()) != string::npos) add=true;
    if(
        ACTIVE_DURING::isActive(
            interneuron->activeDuring(),
            getLimbState(interneuron->getOutputEntity()->side)
        )
    ){
        add=true;
    }
    /*** FEEDBACK ANTI OVER EXTENSION */
    if(
        interneuron->getOutputEntity()->side == SIDE::LEFT &&
        interneuron->activeDuring() == ACTIVE_DURING::ANGLE_OFFSET
        ) {
            add=true;
    }
    if(
        interneuron->getOutputEntity()->side == SIDE::RIGHT &&
        interneuron->activeDuring() == ACTIVE_DURING::ANGLE_OFFSET
        ) {
            add=true;
    }
    if(add){ return true; }
    else return false;
}
void ReflexController::applyGateNeuron(Interneuron * interneuron){
    //change : was
    if(isGateOpen(interneuron)) interneuron->getOutputEntity()->add(interneuron->get());
    //interneuron->getOutputEntity()->add(interneuron->get());
}


void ReflexController::step_INcpg_to_IN(){
    using namespace SENSORY_INTERNEURON;
    static Interneuron * in;
    static double o;
    static double a;
    static int INcpg;
    double total_error = 0;
    for (INcpg=FIRST; INcpg<=LAST; INcpg++){
        in = (Interneuron *)INcpgs[INcpg];

        double o=0.0;
        double a=1.0;
        //o=parameters[5][((Interneuron *) in)->file];
        //a=1.0+parameters[4][((Interneuron *) in)->file];

        in->apply(o,a);
        in->step();

        double acc = in->get()-((Interneuron *)INsens[INcpg])->get();
        if(acc > 1000){
            CPGInterneuron * cpg = ((CPGInterneuron *)in);
            int update_pos = (int)(cpg->yi[0]*cpg->kNot);
            cerr << SENSORY_INTERNEURON::toString(INcpg) << " " << cpg->yi[1] << " (ReflexController.cc:423) " << endl;
        }
        total_error+=acc*acc;
    }
    print_debug("[ok] : compute cpg interneuron state (ReflexController.cc)");
    static long i=0;
    i++;
    eventManager->set<double>(STATES::PREDICTION_ERROR, total_error);
    if(i%1000==0){
        stringstream ss;
        ss << "    Prediction error: " << total_error;
        print_verbose(ss.str());
    }
}

void ReflexController::step_INsen_to_IN(){
    using namespace SENSORY_INTERNEURON;
    static int INsen;
    static double o;
    static double a;
    for (INsen=FIRST; INsen<=LAST; INsen++){
        stringstream ss; 
        ss << SENSORY_INTERNEURON::toString(INsen);
        print_debug(ss.str());
        Interneuron * in = (Interneuron *)INsens[INsen];
        o=parameters[7][((Interneuron *) in)->file];
        a=1.0+parameters[6][((Interneuron *) in)->file];
        in->apply(o,a);
    }
    print_debug("[ok] : compute sensory interneuron state (ReflexController.cc)");
}


/**
    Returns the contribution of the Interneuron
*/
double ReflexController::get_IN_contribution(int IN,bool cpg){
    CPGInterneuron * in = (CPGInterneuron *)INcpgs[IN];
    static double contribution;

    if(cpg)
        contribution = in->getParameter_level2();
    else{
        contribution = 1-in->getParameter_level2();
    }

    return contribution;
}

void ReflexController::step_IN_to_MN(std::map<int,std::map<std::string,Connection*>> __IN_MN,bool cpg){
    using namespace SENSORY_INTERNEURON;
    static int jwang = Settings::get<int>("jWang_changes");
    static string gating_mechanism = Settings::get<string>("GatingMechanism"); // TODO: Make it a typedef
    static bool skip;
    static Connection * connection;
    skip = false;
    for (int INsen=FIRST; INsen<=LAST; INsen++){
        for(auto& kv: motoneurons){
            std::string MN = kv.first;
            print_debug(MN);
            connection = __IN_MN[INsen][MN];
            if(connection != 0){
                // Here is where we can decide how the contribution of a Sensory Interneuron will affect the motoneurons.
                // By default a state machine decides if the
                // The connection step will add the output of the interneuron to the motoneuron input.
                // This happens only if the gate is open. What opens and closes the Gate is some magic signal.
                if(gating_mechanism == "state_machine"){
                    if(connection->isGateOpen(this)){
                        if(jwang == 1)
                        {
                            if(getInput<SIDE::Side>("finishing_stance") == SIDE::RIGHT)
                            {
                                if(INsen == RIGHT_HAM_TRUNK_PD_STANCE || INsen == RIGHT_HF_TRUNK_PD_STANCE || INsen == RIGHT_GLU_TRUNK_PD2_STANCE)
                                    skip = true;
                            }
                            if(getInput<SIDE::Side>("finishing_stance") == SIDE::LEFT)
                            {
                                if(INsen == LEFT_HAM_TRUNK_PD_STANCE || INsen == LEFT_HF_TRUNK_PD_STANCE || INsen == LEFT_GLU_TRUNK_PD2_STANCE)
                                    skip = true;
                            }
                            if(!skip)
                                connection->step(get_IN_contribution(INsen,cpg));
                            skip = false;
                        }
                        else{
                            connection->step(get_IN_contribution(INsen,cpg));
                        }
                    }
                }
                // TODO : All this below is not compatible with jWang = 0
                else if(gating_mechanism == "cpg_based"){

                }
                else if(gating_mechanism == "sensory_based"){
                    Interneuron * interneuron = (Interneuron *)(connection->input);
                    if(
                        ACTIVE_DURING::isActive(
                            interneuron->activeDuring(),
                            getLimbState(interneuron->getOutputEntity()->side)
                        )
                    ){
                        double contribution = 1.0;
                        if(false){//interneuron->activeDuring() == ACTIVE_DURING::SWING){
                        // for connections active during swing : indirect inhibitory connection
                        // free nerve endings connect to a interneurons inhibiting the sensory interneurons.

                        }
                        else if(false){//interneuron->activeDuring() == ACTIVE_DURING::STANCE){
                        // for connections active during stance : direct excitatory connection
                        // free nerve endings directly connect to the sensory interneurons to excite it.

                        }
                        else{
                            if(connection->isGateOpen(this)){
                                contribution = 1.0;
                            }
                            else{
                                contribution = 0.0;
                            }
                        }
                        connection->step(contribution*get_IN_contribution(INsen,cpg));
                    }
                }
                else{
                    print_debug("[error] unknown gating_mechanism");
                }
            }
        }
    }
}


void ReflexController::step_IN_to_MN(){
    using namespace SENSORY_INTERNEURON;
    static bool cpg=true;
    static int INsen;
    for(auto& kv: motoneurons){// Motoneuron input has to be reset at every step
        Motoneuron * motoneuron = kv.second;
        motoneuron->input = 0;         // to get input of current time step.
    }
    print_debug("[ok] : motoneurons reset input (ReflexController.cc)");

    for (INsen=FIRST; INsen<=LAST; INsen++){
        INsens[INsen]->step();
    }
    print_debug("[ok] : step INsens (ReflexController.cc)");
    step_IN_to_MN(__INsen_MN,!cpg);
    print_debug("[ok] : step_IN_to_MN (ReflexController.cc)");

    for (INsen=FIRST; INsen<=LAST; INsen++){
        INcpgs[INsen]->step();
    }
    step_IN_to_MN(__INcpg_MN,cpg);


    print_debug("[ok] : compute interneuron state (ReflexController.cc)");
}


void ReflexController::step_MN_to_output(){
    for(auto& kv: motoneurons){
        Motoneuron * motoneuron = kv.second;
        motoneuron->apply();
        motoneuron->step();
        motoneuron->muscle = motoneuron->get();
    }
}

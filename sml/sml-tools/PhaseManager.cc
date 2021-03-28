#include "PhaseManager.hh"

extern CentralClock * centralclock;
extern EventManager * eventManager;
namespace nmm {
  extern SmlParameters parameters;
}
using namespace nmm;

Phase::Phase(string name, Sml* sml, int nb_cycle): name(name), sml(sml), nb_cycle(nb_cycle), restart_state(false){};
Phase::Phase(string name, Sml* sml, int nb_cycle, string option): name(name), sml(sml), nb_cycle(nb_cycle){
    if(option.find("restart state") != string::npos)
        restart_state = true;
    else
        restart_state = false;
    eventManager->set(STATES::STEADY_STATE_WAITFOR,true);
    eventManager->set(STATES::STEADY_STATE,false);
    last = false;


};
void Phase::print_start(){
    cout << "[Loading] " << name << " phase" << endl;
}
void Phase::print_end(){
    print_verbose("[Starting] " + name + " phase");
}

InitialPhase::InitialPhase(Sml* sml): Phase("Initial", sml, 0){}
bool InitialPhase::rule(vector<Phase *>::iterator * it){
	this->print_start();
    eventManager->set<bool>(STATES::IS_LAUNCHING_PHASE,true);
    loadParameters();

    if(!((GeyerSml* ) sml)->geyerController.feedback_right_loaded)
        ((GeyerSml* ) sml)->geyerController.loadFeedback(SIDE::RIGHT);
    if(!((GeyerSml* ) sml)->geyerController.feedback_left_loaded)
        ((GeyerSml* ) sml)->geyerController.loadFeedback(SIDE::LEFT);
    //exp->color = 0xcc0000;
    (*it)++;
	this->print_end();
    return true;
}
void InitialPhase::loadParameters(){
	ParameterManager::loadCpgParametersFromFile(Settings::get<string>("config_cpg_parameters"));
    ParameterManager::loadLaunchingReflexParameters(Settings::get<string>("launching_gate"));
}
InitialPhaseFinder::InitialPhaseFinder(Sml* sml): Phase("Initial", sml, 0){};
bool InitialPhaseFinder::rule(vector<Phase *>::iterator * it){
	this->print_start();
    eventManager->set<bool>(STATES::IS_LAUNCHING_PHASE,false);

    loadParameters();

    if(!((GeyerSml* ) sml)->geyerController.feedback_right_loaded)
        ((GeyerSml* ) sml)->geyerController.loadFeedback(SIDE::RIGHT);
    if(!((GeyerSml* ) sml)->geyerController.feedback_left_loaded)
        ((GeyerSml* ) sml)->geyerController.loadFeedback(SIDE::LEFT);

    (*it)++;
	this->print_end();
    return true;
}
void InitialPhaseFinder::loadParameters(){
    ParameterManager::loadCpgParametersFromFile(Settings::get<string>("config_cpg_parameters"));
    ParameterManager::loadLaunchingReflexParameters(Settings::get<string>("launching_gate"));
    ParameterManager::loadReflexParametersFromOpti();
}


FullReflexPhase::FullReflexPhase(Sml* sml, string parameters, int nb_cycle): Phase("FullReflex", sml, nb_cycle), parameters(parameters){};
FullReflexPhase::FullReflexPhase(Sml* sml, int nb_cycle): Phase("FullReflex", sml, nb_cycle), parameters(""){};
FullReflexPhase::FullReflexPhase(Sml* sml, string parameters, int nb_cycle, string option): Phase("FullReflex", sml, nb_cycle, option), parameters(parameters){};
FullReflexPhase::FullReflexPhase(Sml* sml, int nb_cycle, string option,string parameters): Phase("FullReflex", sml, nb_cycle,option), parameters(parameters){};
FullReflexPhase::FullReflexPhase(Sml* sml, int nb_cycle, string option): Phase("FullReflex", sml, nb_cycle,option), parameters(""){};
bool FullReflexPhase::rule(vector<Phase *>::iterator * it){
    if(sml->left_foot->justTouchTheGround() && eventManager->get<int>(STATES::CYCLE_COUNT)>=nb_cycle){
		this->print_start();
        if(restart_state)
            eventManager->resetState();
        loadParameters();

        (*it)++;

        //exp->color = 0xcc0000;

        if(Settings::get<int>("reload_param_on_touchdown") == 1 && last){
            //eventManager->doIt_passByRef(&(eventManager->onLeftFootTouchDown), &test); // simple function
            eventManager->doIt_passByRef<FullReflexPhase>(&(eventManager->onRightFootTouchDown), &FullReflexPhase::loadParameters, *this);
        }
        this->print_end();
        return true;
    }
    else
        return false;
}
void FullReflexPhase::loadParameters(){
	eventManager->set<bool>(STATES::IS_LAUNCHING_PHASE,false);
    if(parameters == "")
        ParameterManager::loadReflexParameters();
    else
        ParameterManager::loadReflexParametersFromFile(parameters);
    ParameterManager::loadCpgParametersFromFile(Settings::get<string>("config_cpg_parameters"));
}


SemiReflexPhase::SemiReflexPhase(Sml* sml, std::string parameters, int nb_cycle):Phase("SemiReflex",sml, nb_cycle),end(false),parameters(parameters){};
SemiReflexPhase::SemiReflexPhase(Sml* sml, int nb_cycle):Phase("SemiReflex",sml, nb_cycle),end(false),parameters(""){};
SemiReflexPhase::SemiReflexPhase(Sml* sml, string parameters, int nb_cycle, string option): Phase("SemiReflex", sml, nb_cycle, option), end(false), parameters(parameters){};
SemiReflexPhase::SemiReflexPhase(Sml* sml, int nb_cycle, std::string option):Phase("SemiReflex",sml, nb_cycle, option),end(false),parameters(""){};
SemiReflexPhase::SemiReflexPhase(Sml* sml, int nb_cycle, std::string parameters, std::string option):Phase("SemiReflex",sml, nb_cycle, option),end(false),parameters(parameters){};
bool SemiReflexPhase::rule(vector<Phase *>::iterator * it){
    if(sml->right_foot->justTouchTheGround() && eventManager->get<int>(STATES::CYCLE_COUNT)>=nb_cycle && !end)
    {
        this->print_start();
		loadCpg();
        loadParameters();
        end = true;
    }

    if(end)
    {
        //exp->color = 0xffff00;
        (*it)++;
        eventManager->set<bool>(STATES::IS_LAUNCHING_PHASE,false);
		if(Settings::get<int>("reload_param_on_touchdown") == 1 && last){
            //eventManager->doIt_passByRef(&(eventManager->onLeftFootTouchDown), &test); // simple function
            eventManager->doIt_passByRef<SemiReflexPhase>(&(eventManager->onRightFootTouchDown), &SemiReflexPhase::loadCpg, *this);
        }
        if(Settings::get<int>("reload_param2_on_touchdown") == 1 && last){
            eventManager->doIt_passByRef<SemiReflexPhase>(&(eventManager->onRightFootTouchDown), &SemiReflexPhase::loadParameters, *this);
        }
		this->print_end();
        return true;
    }
    else
        return false;

}
void SemiReflexPhase::loadCpg(){
    ((GeyerSml* ) sml)->geyerController.loadCpgFile();
    ((GeyerSml* ) sml)->geyerController.loadPartialCpg(SIDE::RIGHT);
    ((GeyerSml* ) sml)->geyerController.loadPartialCpg(SIDE::LEFT);
}
void SemiReflexPhase::loadParameters(){
	cout << "    loading cpg" << endl;
	eventManager->set<bool>(STATES::IS_LAUNCHING_PHASE,false);
    if(parameters == "")
        ParameterManager::loadCpgParameters();
    else
		ParameterManager::loadCpgParametersFromFile(parameters);



	eventManager->set<bool>(STATES::LEFT_UP_EFX_PARAMETERS,true);
	eventManager->set<bool>(STATES::RIGHT_UP_EFX_PARAMETERS,true);
    //((GeyerSml* ) sml)->geyerController.loadPartialCpg(SIDE::RIGHT,"cpg");
    //((GeyerSml* ) sml)->geyerController.loadPartialCpg(SIDE::LEFT,"cpg");
}



vector<Phase *> PhaseManager::phases;
vector<Phase *>::iterator PhaseManager::phase;


void PhaseManager::update(){
    if(phase < phases.end())
    {
        if((*phase)->rule(&phase)){
            //_size ++;
            //cout << "Phase " << _size << " ended" << endl;
        }
    }
    if(phase == phases.end())
        eventManager->set<bool>(STATES::IS_LAST_PHASE,true);
    else if ((*phase)->last)
        eventManager->set<bool>(STATES::IS_LAST_PHASE,true);
}

void PhaseManager::init(Sml* sml){
    if(Settings::get<string>("experiment") == "initialPhase"){
        phases.push_back(new InitialPhaseFinder(sml));
    }
    else{
        if(Settings::get<int>("force_launching_gate_from_opti") == 1){
            phases.push_back(new InitialPhaseFinder(sml));
        }
        else{
            phases.push_back(new InitialPhase(sml));
        }

        string expString = Settings::get<string>("experiment");
        boost::trim(expString);
        boost::replace_all(expString, "\t", "");
        boost::replace_all(expString, " ", "");
        boost::replace_all(expString, "\n", "|");
        istringstream f(expString);
        string p;
        while (getline(f, p, '|')) {

            map<string, string> options = {
                {"start_after_step","3"},
                {"p_file",""},
                {"restart_state","restart state"}
            };

            vector<string> strs;
            boost::split(strs, p, boost::is_any_of("+"));
            if(strs.size() > 1){
                options["start_after_step"] = strs[1];
                p = strs[0];
                strs.clear();
            }

            boost::split(strs, p, boost::is_any_of("{"));



            // If option
            if(strs.size() > 1)
            {
                vector<string> strs2;
                boost::split(strs2, strs[1], boost::is_any_of("}"));
                vector<string> strs3;
                boost::split(strs3, strs2[0], boost::is_any_of(","));

                for(auto &it:strs3){
                    vector<string> strs4;
                    boost::split(strs4, it, boost::is_any_of("="));
                    options[strs4[0]] = strs4[1];
                }
            }

            int start_after_step = stoi( options["start_after_step"]);
            string p_file = options["p_file"];
            string restart_state = options["restart_state"] != "restart state" ? "" : "restart state";


            if(strs[0] == "fullReflex"){
                phases.push_back(new FullReflexPhase(sml,p_file,start_after_step,restart_state));
            }

            else if(strs[0]=="semiCpg"){
                phases.push_back(new SemiReflexPhase(sml,p_file,start_after_step,restart_state));
            }
            //else if(strs[0]=="systematicStudy"){
            //  phases.push_back(new SystematicStudyPhase(sml, start_after_step, restart_state));
            //}
            else
            {
                cout << "    [warning] unknown phase : " << p << endl;
            }
        }
    }
    phases[phases.size()-1]->last=true;
    phase = phases.begin();
}

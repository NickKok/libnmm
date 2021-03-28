#include "CentralClock.hh"
#include "Interneuron.hh"
#include <sml/types/types.h>
#include <sml/sml-tools/Settings.hh>
#include <boost/algorithm/string.hpp>
#include <iomanip>
#include <efx/rungekutta.h>

using namespace std;
extern EventManager* eventManager;
extern CentralClock * centralclock;
namespace nmm {
  extern SmlParameters parameters;
}
using namespace nmm;

double weight_const = 0.0;

double interpolate(double pos, std::vector<double> & y){
    double a;
    double b;
    a=(y[ceil(pos)]-y[floor(pos)]);
    b=y[floor(pos)]-a*floor(pos);

    return a*pos+b;
}


double Interneuron::tf(double x){
    // tf from entities overwritten because we don't implement inhibitory connection for now:
    // --> The inhibitory connection are defined by factor given in the construction of the interneuron
    // so here we trick the transfer function.
    double y;
    int type;
    type = 0;
	if (no_tf) // TODO remove
		return 2.0*factor*(Entity::tf(factor*x));
	else
		return factor*(Entity::tf(factor*x));
}



vector<double> CPGInterneuron::dydt(double t, vector<double> y, vector<double> param, vector<double> input){
	//dydt(1) = w;
	//dydt(2) = tau*(in(y(1))-y(2))+(in(y(1)+h/2)-in(y(1)-h/2))/(h)*dydt(1);
    double f,tau,dt,offset,amplitude;
    int cpg_strong_sync = Settings::get<int>("cpg_strong_sync");
    vector<double> dydt(y.size(),0.0);
	f   = param[0];
	tau = param[1];
	dt  = param[2];
	offset = param[3];
	amplitude = param[4];

	dydt[0] = f;
	if(cpg_strong_sync==1){
        if(fmod(y[0],1.0) >= 0.9)
            dydt[0] = 10*f*exp(-10*f*t+9-log(10));
	}
	dydt[1] = tau*(
		offset+
		amplitude*getG(y[0])-y[1])+
		amplitude*getDG(y[0])*dydt[0];
	return dydt;
}
double CPGInterneuron::getG(double val){
	double Knot = (double)teacher.size()-1.0;
	return interpolate(fmod(val,period)*Knot,teacher);
}
double CPGInterneuron::getDG(double val){
	double Knot = (double)teacher_d.size()-1.0;
	return interpolate(fmod(val,period)*Knot,teacher_d);
}

CPGInterneuron::CPGInterneuron(SensoryInterneuron* insen, Foot * foot):
    CPGInterneuron(
        insen->active_during,
        insen->output_neuron,
        foot,
        insen->file){
            this->insen = insen;
    }
CPGInterneuron::CPGInterneuron(
    ACTIVE_DURING::ActiveDuring active_during,
    Entity * output_neuron,
    Foot * foot,
    string file):
        Interneuron(output_neuron, active_during, file),
        dt(double(Settings::get<int>("time_step"))/1000.0),
        t(0.0),
        tau(200.0),
        freq(1.0),
        period(1.0),
        file(file),
        foot(foot),
        kNot(Settings::get<int>("cpg_knodes")),
        state_(-1)
        {
    parameter_level2 = &parameters[2][file];
	parameter_level3 = &parameters[3][file];

    mean = 0;
    amplitude = 1;
    offset = 0;
    int cpg_load_from_file = Settings::get<int>("cpg_load_from_file");
    if(cpg_load_from_file == 0){ // Learning enabled

        // We initialize every CPG to zero
        for(int i=0; i<this->kNot;i++){
            teacher.push_back(0.0);
            teacher_d.push_back(0.0);
            teacher_first.push_back(true);
        }
        if(foot->getName().find("LEFT") != string::npos){
            yi.push_back(0.0);
            yi.push_back(0.0);
        }
        else{
            yi.push_back(0.5);
            yi.push_back(0.0);
        }
    }
    else{
        string path = Settings::get<string>("config")+
                "cpg_gate/cpg_data/" +
                Settings::get<string>("config_cpg_data") +
                "/" + file + foot->getName() + ".txt";
        ifstream myfile (Settings::get<string>("config")+
                "cpg_gate/cpg_data/" +
                Settings::get<string>("config_cpg_data") +
                "/" + file + foot->getName() + ".txt");
        string line ; // the string containing the line
        vector<string> strs; // the vector containing the key->value pair
        if (myfile.is_open()){
            print_verbose("[CPG Predictor]   Loading :" + file);
            while( getline( myfile, line ) )
            {
                boost::split(strs, line, boost::is_any_of("\t "));
                if(strs.size()==2)
                {
                   teacher.push_back(convertToDouble(strs[0]));
                   teacher_d.push_back(convertToDouble(strs[1]));
                }
                else
                    cerr << "     Error: number of element in every row of cpg file wrong" << endl;
            }
            if(foot->getName().find("LEFT") != string::npos){
                yi.push_back(0.0);
                yi.push_back(offset+amplitude*teacher[0]);
            }
            else{
                yi.push_back(0.5);
                yi.push_back(offset+amplitude*teacher[0]);
            }
            myfile.close();
        }
        else{
            cerr<<"     ERROR: unable to open CPG file '" << path << "'. In:Interneuron.hh" << endl;
            cerr<<"            You might need to run first with the setting cpg_save_to_file=1 '" << endl;
            cerr<<"            Check your Config file '" << endl;
            exit (EXIT_FAILURE);
            yi.push_back(0.5);
            yi.push_back(0.0);
        }
    }

}
double CPGInterneuron::getParameter_level2(){
	return *parameter_level2;
}
double CPGInterneuron::getParameter_level3(){
	return *parameter_level3;
}
void CPGInterneuron::initialize(){
	initialize(offset, amplitude);
}
void CPGInterneuron::initialize(double offset, double amplitude){
    yi[0] = 0.0;
    yi[1] = teacher[0];
    t=0.0;
	f = foot->duration_last_step;
}
bool CPGInterneuron::isCst(){
    try
    {
        return getParameter_level3()==1.0;
    }
    catch (int e)
    {
        cerr << "An exception occurred with cpg " << file << ". Exception Nr. " << e << '\n';
        return 0;
    }
	
}
void CPGInterneuron::update_state(){
	freq = centralclock->getFrequency();//1./duration;
}

static int new_state = 0; // share the state between different CPGInterneuron
static int flag = 0;
static int flag2 = 0;

int CPGInterneuron::learn(){
    static int cpg_start_learning_at_cycle = Settings::get<int>("cpg_start_learning_at_cycle");
    static int cpg_start_learnining_cycle_number = Settings::get<int>("cpg_start_learnining_cycle_number");
    static int cpg_save_to_file = Settings::get<int>("cpg_save_to_file");
    // Update teaching signal state based on current sensory input value and phase.
    if(eventManager->get<int>(STATES::CYCLE_COUNT) < cpg_start_learning_at_cycle){
        new_state = 0;
    }
    else if(
       state_ == 0
        &&
       eventManager->get<int>(STATES::CYCLE_COUNT) >= cpg_start_learning_at_cycle
        &&
       eventManager->get<int>(STATES::CYCLE_COUNT) < cpg_start_learning_at_cycle + cpg_start_learnining_cycle_number)
    {
        new_state = 1;
        if(flag == 0) {
            stringstream ss;
            ss << "[CPG Predictor]    Start learning for " << cpg_start_learnining_cycle_number << " cycles";
            print_verbose(ss.str());
            flag++;
        }
    }
    else if(
        state_ == 1
         &&
        eventManager->get<int>(STATES::CYCLE_COUNT) >= cpg_start_learning_at_cycle + cpg_start_learnining_cycle_number
    )
    {
        new_state = 2;
        if(flag2 == 0) {
            print_verbose("[CPG Predictor]    Saving ");

            flag2++;
        }
    }

    if(state_ == 1){
        int update_pos = (int)(yi[0]*kNot);
        if(teacher_first[update_pos]){
            teacher_first[update_pos] = false;
            teacher[update_pos] = insen->get();
        }
        else{
            teacher[update_pos] = 0.9*teacher[update_pos] + 0.1*insen->get();
        }
        if(update_pos > 0){
            teacher_d[update_pos] = (teacher[update_pos]-teacher[update_pos-1])/dt/kNot;
        }
        else{
            teacher_d[update_pos] = (teacher[update_pos]-teacher[teacher.size()-1])/dt/kNot;
        }
    }
    else if(state_ == 2){
        new_state = 3;
        if(cpg_save_to_file == 1){
            print_verbose("[CPG Predictor]   Saving :" + file);
            // Save CPG signal to file
            string path = Settings::get<string>("config")+"cpg_gate/cpg_data/" +
                          Settings::get<string>("config_cpg_data") + "/" + file
                          + foot->getName() + ".txt";
            std::ofstream *file;
            file = new ofstream();
            file->open(path);
            for(int i = 0; i < teacher.size(); i++ ){
                *file << setprecision(16) << teacher[i] << " " << setprecision(16) << teacher_d[i] << endl;
            }
            file->close();
        }
    }
    if(state_ != new_state){
        print_debug("[CPG Predictor] Entering cpg state " + new_state);
        state_ = new_state;
    }
    return state_;
}

void CPGInterneuron::apply(double o, double a){
    static int cpg_load_from_file = Settings::get<int>("cpg_load_from_file");

    // Learn if not loading from file
    // If we are learning, we set the frequency to the frequency from descending drive.
    switch ( cpg_load_from_file == 0 ? learn() : -1 ){
        case 1:
            freq = eventManager->get<double>(STATES::CURRENT_CYCLE_DURATION);
            break;
        case 2:
        case 3:
            freq = parameters[1]["freq_change"]; // TODO get frequency from global clock. And update frequency in global clock.
            break;
    }

    // Update the CPG dynamical system
	apply();

    static int supraspinal_separatedCtrl = Settings::get<int>("supraspinal_separatedCtrl");
    if(supraspinal_separatedCtrl == 0){
        // Parameter adaptation
        if(isCst()){
            o=parameters[1]["offset_change_cst"];
            a=parameters[1]["amp_change_cst"];
        }
        else{
            o=parameters[1]["offset_change_cpg"];
            a=parameters[1]["amp_change_cpg"];
        }
    }
	state = o+a*state;
}
void CPGInterneuron::apply(){
    static int cpg_strong_sync = Settings::get<int>("cpg_strong_sync");
    static int cpg_biPhasic = Settings::get<int>("cpg_biPhasic");
    static vector<double> param(5,0.0);
	if(isCst()){
		state = mean;
		return;
	}

    if(cpg_biPhasic){
        if(active_during == ACTIVE_DURING::SWING){
            if(foot->justGetOffGround()){
                if(cpg_strong_sync==1) initialize();
            }
        }
        else{
            if(foot->justTouchTheGround()){
                if(cpg_strong_sync==1) initialize();
            }
        }
    }
    else if(foot->justTouchTheGround()){
		if(cpg_strong_sync==1) initialize();
	}

	param[0] = freq;
	param[1] = tau;
	param[2] = dt;
	param[3] = offset;
	param[4] = amplitude;
	yi = rungeKutta4(t,dt,yi,param,teacher,(Derivatives *)this);
	//yi = rungeKutta4(t,dt,yi,param,teacher,(*dydtF));
	t+=dt;
	state = yi[1];
}

SensorySensoryInterneuron::SensorySensoryInterneuron(
        SENSORY_INTERNEURON::SensoryInterneurons ID,
        const double& input1,
        const double& input2,
        Entity * output,
        std::string param_str,
        double type
        ):SensorySensoryInterneuron(
                SENSORY_INTERNEURON::toString(ID).erase(0, SENSORY_INTERNEURON::toString(ID).find("_") + 1),
                SENSORY_INTERNEURON::getGate(ID),
                input1,
                input2,
                output,
                parameters[0][param_str],
                parameters[8][param_str],
                type){
        }


void SensorySensoryInterneuron::apply(){
    static int jwang = Settings::get<int>("jWang_changes");
    if(jwang == 1)
        state = factor*weight*(1.0+weight_modulator)*sensor2;
    else
        state = factor*weight*(1.0+weight_modulator)*sensor1*sensor2;
}
void SensorySensoryInterneuron::apply(double o, double a){
	apply();
	state = o+a*state;
}
double SensorySensoryInterneuron::tf(double x){
    return Interneuron::tf(x);
}

SensoryInterneuron::SensoryInterneuron(
        SENSORY_INTERNEURON::SensoryInterneurons ID,
        const double& input,
        Entity * output,
        std::string param_str,
        double type
        ):SensoryInterneuron(
                SENSORY_INTERNEURON::toString(ID).erase(0, SENSORY_INTERNEURON::toString(ID).find("_") + 1),
                SENSORY_INTERNEURON::getGate(ID),
                input,
                output,
                parameters[0][param_str],
                parameters[8][param_str],
                type){
        }

void SensoryInterneuron::apply(){
	state = factor*weight*(1.0+weight_modulator)*sensor;
}
void SensoryInterneuron::apply(double o, double a){
	apply();
    static int supraspinal_separatedCtrl = Settings::get<int>("supraspinal_separatedCtrl");
    if(supraspinal_separatedCtrl == 0){
        o=parameters[1]["offset_change_reflex"];
        a=parameters[1]["amp_change_reflex"];
    }
	state = o+a*state;
}

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "Settings.hh"
#include "Optimization.hh"
using boost::property_tree::ptree;

#ifdef OPTIMIZATION
using namespace optimization::messages::task;
#endif
using namespace std;

namespace nmm {
  extern SmlParameters parameters;
}
using namespace nmm;
extern double convertToDouble(const string&);

class is_int : public boost::static_visitor<bool>
{
public:
  bool operator()(double & i) const{ return false;}
  bool operator()(int & i) const{ return true;}
  bool operator()(const std::string & str) const{return false;}
};
class is_double : public boost::static_visitor<bool>
{
public:
  bool operator()(double & i) const{ return true;}
  bool operator()(int & i) const{ return false;}
  bool operator()(const std::string & str) const{return false;}
};

class is_string : public boost::static_visitor<bool>
{
public:
  bool operator()(double & i) const{ return false;}
  bool operator()(int & i) const{ return false;}
  bool operator()(const std::string & str) const{return true;}
};
//constructor

double_int_string Settings::load_setting_from_xml(std::string setting_name, double_int_string setting_value){
  double_int_string OUT_ = setting_value;
  if (boost::apply_visitor( is_double(), setting_value )){
    OUT_ = pt.get("settings."+setting_name, boost::get<double>(setting_value));
  }
  else if(boost::apply_visitor( is_int(), setting_value )){
    OUT_ =pt.get("settings."+setting_name, boost::get<int>(setting_value));
  }
  else if(boost::apply_visitor( is_string(), setting_value )){
    OUT_ =pt.get("settings."+setting_name, boost::get<string>(setting_value));
  }
  return OUT_;
}
double_int_string Settings::load_setting_from_opti(std::string setting_name, double_int_string setting_value){
  string IN_;
  stringstream s;
  optimizer &opti = tryGetOpti();
  double_int_string OUT_=setting_value;
  if (boost::apply_visitor( is_double(), setting_value )){
    double temp = boost::get<double>(setting_value);
    if(opti){if ( opti.Setting(setting_name,IN_) ){
      s << IN_;
      s >> temp;
    }}
    OUT_ = temp;
  }
  else if (boost::apply_visitor( is_int(), setting_value )){
    int temp = boost::get<int>(setting_value);
    if(opti){if ( opti.Setting(setting_name,IN_) ){
      s << IN_;
      s >> temp;
    }}
    OUT_ = temp;
  }
  else if (boost::apply_visitor( is_string(), setting_value )){
    string temp = boost::get<string>(setting_value);
    if(opti){if ( opti.Setting(setting_name,IN_) ){
      s << IN_;
      s >> temp;
    }}
    OUT_ = temp;
  }
  return OUT_;
}
//settings
std::string Settings::loadedFrom = "defaultValue";
std::string Settings::filename = "settings.xml";
std::string Settings::prefix = "";
std::string Settings::config_path = "../../../config/";

Settings::Settings(){
  load_default_settings();
  init();

}
Settings::Settings(std::string str){
  this->filename = str;
  load_default_settings();
  init();
}
void Settings::load_default_settings(){
  /**
  * TIME STEP Management
  * time_step : control the global time_step in ms
  * time_step_eventManager : control the event manager's time_step (in multiple of time_step)
  */
  this->sets["robot_name"] = "REGIS";
  this->sets["time_step"] = 1; //ms
  this->sets["time_step_eventManager"] = 1;
  this->sets["number_of_repeat"] = 1;
  this->sets["muscle_activation_noise_max"] = 0.2;
  this->sets["randomized_initial_condition"] = 0;
  this->sets["max_fall_time"] = 5;
  this->sets["number_of_cycles"] = 50;
  this->sets["energylimit"] = 5000.0;
  this->sets["angle_transfer_function"] = 0;
  this->sets["optimization_scheme"] = 0;
  this->sets["theta|trunkref"] = 0.105;
  this->sets["backward"] = -1;
  this->sets["rescale_parameters"] = 0;
  this->sets["muscle_activation_noise"] = 0;
  this->sets["distance_max"] = 40.0;
  this->sets["duration_max"] = 32.0;
  this->sets["parameter_loading_scheme"] = 1;
  this->sets["amp_change"] = 1.0;
  this->sets["freq_change"] = 1.0;
  this->sets["speed_step_switch"] = 7;
  this->sets["log_webots"] = 0;
  this->sets["systematic_search_step_number"] = 10;
  this->sets["systematic_search_stepsize_step_number"] = 5; // maybe not use
  this->sets["systematic_search_prefix"] = "";
  this->sets["systematic_search_type"] = "normal"; // recursive;
  this->sets["systematic_search_recordStep"] = 10;
  this->sets["force"] = 0.0;
  this->sets["extract_video"] = 0;
  this->sets["automatic_systematic_search"] = 0;
  this->sets["config"] = "";
  this->sets["config_nmm"] = "nmm";
  this->sets["config_rfx_parameters"] = "gaits"; // parameters
  this->sets["fullReflex_gate"] = "fullReflex";
  this->sets["launching_gate"] = "launching";

  /** Obstacles management
  *
  */
  this->sets["bumper"] = 0;
  this->sets["obstacle_1_pos"] = 0.0;

  /** Musculoskeletal system settings
  *
  */
  this->sets["body_scheme"] = "body.yaml";
  /** RL Brain settings
  *
  */
  this->sets["brain_modulator_scheme"] = "brain.yaml";
  this->sets["brain_modulator_reflex"] = 0;
  this->sets["brain_modulator_motoneuron"] = 0;


  this->sets["neverstop"] = 0;
  this->sets["2D"] = 0;
  this->sets["debug"] = 0;
  this->sets["verbose"] = 0;
  this->sets["coronal_linear"] = 0; // Whether to use linear velocity instead of angle for coronal plane control

  this->sets["systematic_control_rule_file"] = "systematic_control_rule.txt";
  this->sets["config_systematic_parameters"] = "default.txt";
  /**
  * Controls if the cpg pattern of the predictor are loaded from a file or learned on the fly
  * 0 for : learn on the fly (default behavior)
  * 1 for : load from a file
  */
  this->sets["cpg_load_from_file"] = 0;
  /**
  * Whether to save the currently learned CPG to file.
  * This makes sense only if the cpg_load_from_file is set to 0, otherwise the same content will be written
  * to the file than the one used when loading the cpg.
  */
  this->sets["cpg_save_to_file"] = 0;
  this->sets["cpg_start_learning_at_cycle"] = 3;
  this->sets["cpg_start_learnining_cycle_number"] = 5;
  this->sets["cpg_strong_sync"] = 1;
  this->sets["cpg_knodes"] = 500;
  this->sets["config_cpg_scheme"] = "cpg_interneurons.txt";
  this->sets["config_cpg_parameters"] = "cpg_parameters.txt";
  this->sets["config_cpg_type"] = "full"; /** DEPRECATED : have no effect */
  /**
  * Only meaningfull when cpg_load_from_file=1, defines the name of the folder (located in [CONFIG]/cpg/cpg_gate)
  */
  this->sets["config_cpg_data"] = "sample";
  /**
  * Controls if the cpg are restarted at the same moment for both stance and swing cpgs, or if stance are restarted at touchdown and
  * swing at lift off
  * 0 for : one restart for all
  * 1 for : two restart for all
  */
  this->sets["cpg_biPhasic"] = 0;
  /**
  * 1 if you want the ctrl of the network to be separated (not controlled by variable amp_change_cpg, offset_change_cpg, ...
  */
  this->sets["supraspinal_separatedCtrl"] = 0;
  this->sets["experiment"] = "fullReflex";
  /**
  * Allows to force the use of parameters from the optimization for the launching gate even if the experiments
  * Includes a semiCPG phase
  */
  this->sets["force_launching_gate_from_opti"] = 0;
  this->sets["modeltype"] = "M1";
  this->sets["experimenttype"] = "flat";
  this->sets["extract"] = "off";
  this->sets["extractoutput"] = "output.txt";
  this->sets["worldname"] = "base.wbt";
  this->sets["fixedViewPoint"] = 1;
  this->sets["feedback2study"] = "";
  this->sets["video_name"] = "video";

  /**
  * Can be fabs or sqrt.
  * However it seems that the right way of doing it (according to the article on signal dependent noise) is with sqrt
  */
  this->sets["noise_type"] = "fabs";
  /**
  * Can be normal / revert / constant
  */
  this->sets["noise_class"] = "normal";
  this->sets["noise_variationCoefficient"] = 0.01;
  this->sets["noise_muscle"] = 0;
  this->sets["noise_sensor"] = 0;
  this->sets["noise_entity"] = 0;
  this->sets["muscle_step_number"] = 1;

  this->sets["kphiknee_off"] = 0; // phi_knee_offset parameter optimization

  /**
  * Parameter controlling the muscle parameters.
  * If set to 1 then parameters should be defined for left and right leg (example hf_fmax_left or hf_fmax_right)
  * If set to 0 then parameters should be defined once for both legs (example hf_fmax)
  */
  this->sets["assymetric_gait"] = 0;
  /**
  * If = 1, jWang modification of Geyer's original model
  * 1) No modulation of PD feedbacks with ground reaction forces, pd feedback always assigned to leading leg (not the one in finishing stance)
  * 2) Entering in swing phase with d > d_lim also (not yet implemented)
  */
  this->sets["jWang_changes"] = 0;
  /**
  * Error in jWang_changes in  Sml::step_IN_to_MN
  *  1) Forgot to put the hf pd feedback. And used the positive force feedback of the ankle instead (working on the leading leg
  which actually does not make sense) Might be interesting to actually try it.
  --> not used after git : 86983ebccaf72891d05fdea638fffa4a02a06e95
  */
  //this->sets["buggyJwang_changes"] = 0;
  /**
  * TODO Remove
  */
  this->sets["notf_trunklean"] = 1;
  /**
  * When set to one the clamping (between 0.01 and 1) of signals sent to the motoneuron is done
  * at the level of the muscle activation (in the original model this is done at the level of muscle stimulation, i.e. before the
  * low pass filter of muscle activation (modelling the muscle plate transfer function)).
  */
  this->sets["buggyMuscle_tone"] = 0;
  this->sets["delay"] = 1;
  /**
  * The transfer function acts on any entity (i.e. interneuron sensory interneuron)
  * The transfer_function_type is the type of transfer_function to be used
  * 1 : threshold transfer function
  * 2 : sigmoid transfer function (not yet implemented)
  */
  this->sets["transfer_function"] = 0;
  this->sets["transfer_function_type"] = 1;
  this->sets["transfer_function_motoneurons"] = 0;
  this->sets["assume_stabilized_after"] = 0;

  this->sets["running_mode"] = "normal", // execution mode

  this->sets["motor_primitives_folder"] = "MP_test";
  /**
  * Simulation and phases control
  * reload_param_on_touchdown : reloads the parameters associated to the cpg interneurons (percentage of cpg/cst)
  * reload_param2_on_touchdown : reloads the parameters associated to cpg parameters files
  */
  this->sets["restart_simulation_at_end"] = 0;
  this->sets["reload_param_on_touchdown"] = 0;
  this->sets["reload_param2_on_touchdown"] = 0;


  this->sets["noAngleOffset"] = 1; // TODO: Remove, changing this parameter has no effect anymore.
  this->sets["parameterSets_number"] = 9;
  this->sets["torque_soft_limit"] = 1;

  /**
  * Perturbation control
  *
  */
  this->sets["perturbation_start_at"] = 10.0;
  this->sets["perturbation_ends_after"] = 20.0;
  this->sets["perturbation_type"] = "linearDecreasingRamp";
  this->sets["perturbation_fileName"] = "perturbation";
  this->sets["perturbation_number"] = -1;
  this->sets["perturbation"] = 0;
  this->sets["perturbation_amplitude"] = 2.0;
  this->sets["perturbation_duration"] = 0.1;
  this->sets["perturbation_phase"] = "swing";
  this->sets["perturbation_recovery_step"] = 0; // Number of step after perturbation to let the system recover.

  this->sets["perturbation_startingMode"] = "randInt";
  //randIntParameters
  this->sets["perturbation_minTimeBetween"] = 10; // Minimum time (ms) between perturbation
  this->sets["perturbation_maxTimeBetween"] = 40; // Maximum time (ms) between perturbation
  //gaitPercentageParameters
  this->sets["perturbation_stepPercentageBetween"] = 10; // Step percentage of gait phase between perturbation


  this->sets["rescaling_mass_ratio"] = 1.0;
  this->sets["rescaling_length_ratio"] = 1.0;
  this->sets["rescaling_velocity_ratio"] = 1.0;

  /** DEPRECATED
  * Stimulation scheme
  * 1 : Normal reflex model
  * 2 : Stimulation each muscle on after each other
  * 3 :
  * 4 : No stimulation
  */
  this->sets["stimulation_scheme"] = 1;
  this->sets["stimulation_scheme_muscle"] = "LEFT_HF";
  this->sets["coman"] = 0;

  /**
  * Libcoman specific settings
  */
  this->sets["libcoman"] = 0; //libcoman flag
  this->sets["coman_mass_ratio"] = 1;
  this->sets["coman_length_ratio"] = 1;
  this->sets["coman_velocity_ratio"] = 1;

  /**
  * Delay
  */
  this->sets["delay_vestibular"] = 5.0;
  this->sets["delay_short"] = 5.0;
  this->sets["delay_medium"] = 10.0;
  this->sets["delay_long"] = 20.0;

  /**
  * Twitching controller
  */
  this->sets["sma_start_time"] = 2.0;
  this->sets["sma_twitch_duration"] = 0.1; // 0.25
  this->sets["sma_post_twitch_duration"] = 5.9;
  this->sets["sma_twitch_activity_level"] = 0.1;

  /** Logger settings
  *
  */
  this->sets["log_nmm"] = 0;
  this->sets["log_nmm_folder"] = "";
  this->sets["log_nmm_path"] = "../../../log/";
  this->sets["log_nmm_overwrite"] = 0;


  /** Neural control settings
  *
  */
  this->sets["NeuralControl"] = "geyer";
  this->sets["GatingMechanism"] = "state_machine"; // Can also be "cpg_based" or "sensory_based"


  this->sets["ParameterPrecision"] = 8; // number of digit to use for the parameter value precision.
}

void Settings::printAll(){
  cout << "[Current settings]" << endl;
  for (auto &kv: sets){
    cout << "     " << kv.first << "=" << kv.second << endl;
  }
}

void Settings::init(){
  optimizer &opti = tryGetOpti();
  print_verbose("[Loading Settings]");
  if(opti){
    this->loadedFrom = "optimizationFile";
    print_verbose("    loading settings from optimization settings file");
    if(!opti.Setting("optiextractor"))
    this->sets["running_mode"] = "optimization";
    else
    this->sets["running_mode"] = "optiextractor";
  }
  else{
    this->loadedFrom = "externalFile";
    string path = this->config_path+this->prefix+"/";
    print_verbose("    loading settings from external settings file : " + path + filename);
    read_xml(path+filename, pt);
  }

  this->sets["config"] = this->config_path + this->prefix + "/";


  for(auto &kv : this->sets)
  {
    if(opti)
    {
      kv.second = load_setting_from_opti(kv.first, kv.second);
    }
    else
    {
      kv.second = load_setting_from_xml(kv.first, kv.second);
    }
  }

  parameters = SmlParameters(get<int>("parameterSets_number"));
  parameters.init();
}

bool Settings::isOptimizing(){
  if ( get<string>("running_mode") == "optimization")
  	return true;
  return false;
}
bool Settings::isOptimization(){
  optimizer &opti = tryGetOpti();
  if(opti)
  return true;
  else
  return false;
  //if ( get<string>("running_mode") != "normal")
  //    return true;
  //return false;
}


void SmlParameters::init(){
  for(int i=0;i<numberOfSets;i++){
    parameters.push_back(parametersSet());
    initDefaultValue(i);
  }
  // Initialization of RL Brain parameters
  //initDefaultValue(100);

}
SmlParameters::SmlParameters():
numberOfSets(4)
{}

SmlParameters::SmlParameters(int n):
numberOfSets(n)
{}

parametersSet&  SmlParameters::operator [](int i){
  return parameters[i];
}

void SmlParameters::printAll(){
  cout << "[Current parameters]" << endl;
  for(int i=0;i<numberOfSets;i++){
    std::vector<string> vec;
    getNames(i,vec);
    for(auto&kv:vec){
      string str = kv;
      cout << std::setprecision(16) << "    " << i << "-" << kv << " : " << parameters[i][str] << endl;
    }


  }
}

void SmlParameters::loadRange_fromfile(string file, int i)
{
  std::vector<std::string> strs;
  ifstream myfile (file);
  string line ;
  if (myfile.is_open()){
    while( std::getline( myfile, line ) )
    {
      boost::split(strs, line, boost::is_any_of("\t "));
      this->min(i)[strs[0]] = convertToDouble(strs[1]);
      this->max(i)[strs[0]] = convertToDouble(strs[2]);
    }
    myfile.close();
  }
  else{
    cerr<<"ERROR: unable to open file '" << file << "'. In:Parameters.cc, constructor(string, file)"<<endl;
  }
}

void SmlParameters::loadAssymetricParamValue(int level, string name, double value){
  static int parameter_precision = Settings::get<int>("ParameterPrecision");
  if(this->parameters[level].find(name+"_left") != this->parameters[level].end()){
    stringstream tmp;
    tmp << setprecision(parameter_precision) << fixed << value;
    this->parameters[level][name+"_left"] = stod(tmp.str());
  }
  if(this->parameters[level].find(name+"_right") != this->parameters[level].end()){
    stringstream tmp;
    tmp << setprecision(parameter_precision) << fixed << value;
    this->parameters[level][name+"_right"] = stod(tmp.str());
  }
}
void SmlParameters::loadParamValue(int level, string name, double value){
  static int parameter_precision = Settings::get<int>("ParameterPrecision");
  if(this->parameters[level].find(name) != this->parameters[level].end()){
    stringstream tmp;
    tmp << setprecision(parameter_precision) << fixed << value;
    this->parameters[level][name] =  stod(tmp.str());
  }
}

void SmlParameters::loadParam_fromvec(std::vector<double> parameters, std::vector<std::string> parametersName, int i){
  for(int i=0;i<parameters.size();i++){
    loadAssymetricParamValue(0,parametersName[i],parameters[i]);
  }
}

void SmlParameters::loadParam_fromfile(string file, int i)
{
  std::vector<std::string> strs; // the vector containing the key->value pair
  ifstream myfile (file);
  string line; // the string containing the line
  if (myfile.is_open()){
    while( std::getline( myfile, line ) )
    {
      boost::split(strs, line, boost::is_any_of("\t "));
      if(strs.size() == 2){ // if the line is separable by space or tab
        loadAssymetricParamValue(i,strs[0],convertToDouble(strs[1]));
      }
    }
    myfile.close();
    myfile.open(file);
    while( std::getline( myfile, line ) )
    {
      boost::split(strs, line, boost::is_any_of("\t "));
      if(strs.size() == 2){ // if the line is separable by space or tab
        loadParamValue(i,strs[0],convertToDouble(strs[1]));
      }
    }
    myfile.close();
  }
  else{
    cerr<<"ERROR: unable to open file '" << file << "'. In:Parameters.cc, constructor(string, string)"<<endl;
  }
}
void SmlParameters::loadParam_fromopti(int i){

  if(Settings::isOptimization()){
    optimizer &opti = tryGetOpti();
    optimizerParameters param;
    std::stringstream ss;
    ss << i;
    for(auto& kv : parameters[i]) {
      if(opti.Parameter(kv.first, param)){
        loadAssymetricParamValue(i,kv.first,param.value());
      }
      if(opti.Parameter(kv.first+"|level"+ss.str(), param)){
        loadAssymetricParamValue(i,kv.first,param.value());
      }
    }
    for(auto& kv : parameters[i]) {
      if(opti.Parameter(kv.first, param)){
        loadParamValue(i,kv.first,param.value());
      }
      if(opti.Parameter(kv.first+"|level"+ss.str(), param)){
        loadParamValue(i,kv.first,param.value());
      }
    }
  }
  else{
    cerr << "You want to load parameters from optimization framework, but optimization framework was not found" << endl;
  }
}
void SmlParameters::initDefaultValue(int i){
  std::vector<string> vec;
  getNames(i,vec);
  for(auto&kv:vec){
    string str = kv;
    parameters[i][str] = 0.0;
  }
  switch(i){
    case 0:
    parameters[i]["zero"] = 0.0;
    parameters[i]["one"] = 1.0;
    break;
    case 1:
    parameters[1]["swing_end"] = 0.5;
    parameters[1]["stance_end"] = -0.5;
    break;
    case 2:
    break;
    case 3:
    break;
    case 4:
    break;
    case 5:
    break;
    case 6:
    break;
    case 7:
    break;
  }
}

void SmlParameters::getNames(int i,std::vector<string>& vec){
  switch(i){
    case 0:
    vec = {
      "zero",
      "one",
      "solsol_wf",
      "solsol_wf_left",
      "solsol_wf_right",
      "tata_wl",
      "tata_wl_left",
      "tata_wl_right",
      "solta_wf",
      "solta_wf_left",
      "solta_wf_right",
      "gasgas_wf",
      "gasgas_wf_left",
      "gasgas_wf_right",
      "vasvas_wf",
      "vasvas_wf_left",
      "vasvas_wf_right",
      "hamham_wf",
      "hamham_wf_left",
      "hamham_wf_right",
      "gluglu_wf",
      "gluglu_wf_left",
      "gluglu_wf_right",
      "hfhf_wl",
      "hfhf_wl_left",
      "hfhf_wl_right",
      "hamhf_wl",
      "hamhf_wl_left",
      "hamhf_wl_right",
      "ta_bl",
      "ta_bl_left",
      "ta_bl_right",
      "hf_bl",
      "hf_bl_left",
      "hf_bl_right",
      "ham_bl",
      "ham_bl_left",
      "ham_bl_right",
      "kphiknee",
      "kbodyweight",
      "kbodyweight_left",
      "kbodyweight_right",
      "kp",
      "kp_cor",
      "kd",
      "kd_cor",
      "kpHipCor",
      "kdHipCor",
      "cpHipCor",
      "cdHipCor",
      "crefHipCor",
      "deltas",
      "sol_activitybasal",
      "sol_activitybasal_left",
      "sol_activitybasal_right",
      "ta_activitybasal",
      "ta_activitybasal_left",
      "ta_activitybasal_right",
      "gas_activitybasal",
      "gas_activitybasal_left",
      "gas_activitybasal_right",
      "vas_activitybasal",
      "vas_activitybasal_left",
      "vas_activitybasal_right",
      "ham_activitybasal",
      "ham_activitybasal_left",
      "ham_activitybasal_right",
      "glu_activitybasal",
      "glu_activitybasal_left",
      "glu_activitybasal_right",
      "hf_activitybasal",
      "hf_activitybasal_left",
      "hf_activitybasal_right",
      "hab_activitybasal",
      "rf_activitybasal",
      "hab_activitybasal_left",
      "rf_activitybasal_left",
      "hab_activitybasal_right",
      "rf_activitybasal_right",
      "had_activitybasal",
      "had_activitybasal_left",
      "had_activitybasal_right",
      "vas_activitybasal_stance",
      "vas_activitybasal_stance_left",
      "vas_activitybasal_stance_right",
      "bal_activitybasal_stance",
      "bal_activitybasal_stance_left",
      "bal_activitybasal_stance_right",
      "klean",
      "kref",
      "kref_cor",
      "kref_cor_linear",
      "klean_cor_hab",
      "klean_cor_had",
      "accum_cor_had",
      "accum_cor_hab",
      "stance_end",
      "swing_end",
      "freq"
    };
    break;
    case 1:
    vec = {
      "offset_change_cpg",
      "offset_change_cst",
      "offset_change_reflex",
      "amp_change_cpg",
      "amp_change_cst",
      "amp_change_reflex",
      "trunk_ref",
      "trunk_ref_cor",
      "freq_change",
      "stance_end",
      "swing_end",
      "noise_level",
    };
    break;
    case 2: // mixing parameters [0,1] real number
    case 3: // cpg-cst = 0, cpg-osc = 1
    case 4: // cpg amplitude control (term added to default value for amplitude)
    case 5: // cpg offset control (term added to default value for offset)
    case 6: // rfx amplitude control (term added to default value for amplitude)
    case 7: // rfx offset control (term added to default value for offset)
    vec = {
      "sol__mff_stance",
      "ta__mlf_cycle",
      "sol_ta_mff_stance",
      "gas__mff_stance",
      "vas__mff_stance",
      "vas_pko_angleoffset",
      "vas_gcf_finishingstance",
      "ham_gif_stance",
      "had_gif_stance",
      "hab_gif_stance",
      "rf_gif_stance",
      "ham__mff_swing",
      "glu_gif_stance",
      "glu__mff_swing",
      "hf_tl_swing",
      "hf_gif_stance",
      "hf__mlf_swing",
      "ham_hf_mlf_swing"
      #if VERSION == 6
      ,
      "hab_trunk_pd_stance",
      "rf_trunk_pd_stance",
      "had_trunk_pd_stance",
      "hab_trunk_leak_swing",
      "rf_trunk_leak_swing",
      "had_trunk_leak_swing"
      #endif
    };
    break;
    case 8:  // modulation from RL brain
    vec = {
      "trunk_ref",
      "trunk_ref_cor",
      "zero",
      "one",
      "solsol_wf",
      "solsol_wf_left",
      "solsol_wf_right",
      "tata_wl",
      "tata_wl_left",
      "tata_wl_right",
      "solta_wf",
      "solta_wf_left",
      "solta_wf_right",
      "gasgas_wf",
      "gasgas_wf_left",
      "gasgas_wf_right",
      "vasvas_wf",
      "vasvas_wf_left",
      "vasvas_wf_right",
      "hamham_wf",
      "hamham_wf_left",
      "hamham_wf_right",
      "gluglu_wf",
      "gluglu_wf_left",
      "gluglu_wf_right",
      "hfhf_wl",
      "hfhf_wl_left",
      "hfhf_wl_right",
      "hamhf_wl",
      "hamhf_wl_left",
      "hamhf_wl_right",
      "ta_bl",
      "ta_bl_left",
      "ta_bl_right",
      "hf_bl",
      "hf_bl_left",
      "hf_bl_right",
      "ham_bl",
      "ham_bl_left",
      "ham_bl_right",
      "kphiknee",
      "kbodyweight",
      "kbodyweight_left",
      "kbodyweight_right",
      "kp",
      "kp_cor",
      "kd",
      "kd_cor",
      "kpHipCor",
      "kdHipCor",
      "cpHipCor",
      "cdHipCor",
      "crefHipCor",
      "deltas",
      "sol_activitybasal",
      "sol_activitybasal_left",
      "sol_activitybasal_right",
      "ta_activitybasal",
      "ta_activitybasal_left",
      "ta_activitybasal_right",
      "gas_activitybasal",
      "gas_activitybasal_left",
      "gas_activitybasal_right",
      "vas_activitybasal",
      "vas_activitybasal_left",
      "vas_activitybasal_right",
      "ham_activitybasal",
      "ham_activitybasal_left",
      "ham_activitybasal_right",
      "glu_activitybasal",
      "glu_activitybasal_left",
      "glu_activitybasal_right",
      "hf_activitybasal",
      "hf_activitybasal_left",
      "hf_activitybasal_right",
      "hab_activitybasal",
      "rf_activitybasal",
      "hab_activitybasal_left",
      "rf_activitybasal_left",
      "hab_activitybasal_right",
      "rf_activitybasal_right",
      "had_activitybasal",
      "had_activitybasal_left",
      "had_activitybasal_right",
      "vas_activitybasal_stance",
      "vas_activitybasal_stance_left",
      "vas_activitybasal_stance_right",
      "bal_activitybasal_stance",
      "bal_activitybasal_stance_left",
      "bal_activitybasal_stance_right",
      "klean",
      "kref",
      "kref_cor",
      "kref_cor_linear",
      "klean_cor_hab",
      "klean_cor_had",
      "accum_cor_had",
      "accum_cor_hab",
      "stance_end",
      "swing_end",
      "freq"
    };
    break;
    default:
    cerr << "No parameter name defined for set " << i << endl;
  }
}

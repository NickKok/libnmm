#include "ParameterManager.hh"
#include "Settings.hh"

namespace nmm {
  extern SmlParameters parameters;
}
using namespace nmm;
using namespace std;

/**
 * Load parameters from launching gate if the file is not found the launching gate at 1.3 m/s is used
 *
 */
void ParameterManager::loadLaunchingReflexParameters(string name){
	string path = Settings::get<string>("config") + Settings::get<string>("config_rfx_parameters") + "/" + name.c_str();
	print_verbose("    External Reflex Launching Parameter loading :" + path);
	parameters.loadParam_fromfile(path,0);
}
/**
 * Load parameters from
 * - the optimization framework if present
 * - and from the file parameters.txt if not
 *
 */
void ParameterManager::loadCpgParameters(){
	// load parameters from file
	loadCpgParametersFromFile(Settings::get<string>("config_cpg_parameters"));
	// overwrite parameters coming from optimization framework
	if(Settings::isOptimization()){
		loadCpgParametersFromOpti();
	}

}
void ParameterManager::loadCpgParametersFromOpti(){
	print_verbose("External CPG parameter loading from optimization framework");
    parameters.loadParam_fromopti(0);
    parameters.loadParam_fromopti(1);
    parameters.loadParam_fromopti(2);
    parameters.loadParam_fromopti(4);
    parameters.loadParam_fromopti(5);
    parameters.loadParam_fromopti(6);
    parameters.loadParam_fromopti(7);


}
void ParameterManager::loadCpgParametersFromFile(std::string file){
	print_verbose("External CPG parameter loading from file " + file);
	string path = Settings::get<string>("config")+"cpg_gate/"+file;
    parameters.loadParam_fromfile(path,0);
	parameters.loadParam_fromfile(path,1);
	parameters.loadParam_fromfile(path,2);

}
void ParameterManager::loadReflexParameters(){
	// First load from file
	loadReflexParametersFromFile(Settings::get<string>("fullReflex_gate"));
	// Override the parameter we want.
	if(Settings::isOptimization()){
		loadReflexParametersFromOpti();
	}
}
void ParameterManager::loadReflexParametersFromOpti(){
	print_verbose("External Reflex parameter loading from optimization framework");

	parameters.loadParam_fromopti(0);
}

void ParameterManager::loadReflexParametersFromFile(std::string file){
	print_verbose("External Reflex parameter loading from file " + file);
	string path = Settings::get<string>("config") + Settings::get<string>("config_rfx_parameters") + "/" + file;

	parameters.loadParam_fromfile(path,0);
    //parameters.loadParam_fromfile(path,1);
}

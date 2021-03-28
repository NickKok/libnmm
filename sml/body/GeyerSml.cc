#include "GeyerSml.hh"

#include <algorithm>
#include <sml/neuralSystem/Interneuron.hh>
#include <sml/neuralSystem/Motoneuron.hh>
#include <sml/musculoSkeletalSystem/Muscle.hh>
#include <sml/musculoSkeletalSystem/Joint.hh>
#include <sml/musculoSkeletalSystem/Sensor.hh>
#include <sml/sml-tools/Settings.hh>
#include <sml/sml-tools/EventManager.hh>
#include <sml/sml-tools/OnlineCtrl.hh>

#include <boost/xpressive/xpressive.hpp>
#include <stdexcept>


extern EventManager* eventManager;
namespace nmm {
  extern SmlParameters parameters;
}
using namespace nmm;
using namespace std;
using namespace boost::xpressive;


void GeyerSml::initialiseSpinalControl(){
  print_debug("[ok]");
  initialiseSpinalControl_twitching();
  print_debug("[ok] : initialisation of twitching");
  initialiseSpinalControl_geyer();
  print_debug("[ok] : initialisation of geyer");
  initialiseModulators();
  print_debug("[ok] : initialisation of modulators");
}

void GeyerSml::initialiseModulators(){
  string folder = Settings::get<string>("config") + "external_modulator/";
  string file = folder+Settings::get<string>("brain_modulator_scheme");

  print_verbose("Loading modulators from file : " + file);
  std::ifstream f(file);
  if (f) {
    f.close();
    brainModulationConfig = Parameters::CreateFromFile(file);
    if (brainModulationConfig.getVector<std::string>("reflexes").size() > 0) {
      print_verbose("    Found the following reflex modulation");
      for(auto &kv : brainModulationConfig.getVector<std::string>("reflexes")){
        reflex_modulated.push_back(kv);
        if ( parameters[8].find(kv) == parameters[8].end() ) {
          cerr << "Parameters: " << kv << " not found in " << file << " (GeyerSml.cc)\n" <<
                "Are you sure this parameter exists ?" << endl;
          throw std::invalid_argument(
                "Parameters: " + kv + " not found in initialiseModulators (GeyerSml.cc)\n" +
                "Are you sure this parameter exists ?\n"
                );
        }
      }
      //cout << endl;
    }
    motoneuron_scaling = brainModulationConfig.get<double>("motoneuron_scaling");
    motoneuron_scaling = (motoneuron_scaling == 0.0 ? 1.0 : motoneuron_scaling);

    reflex_scaling = brainModulationConfig.get<double>("reflex_scaling");
    reflex_scaling = (reflex_scaling == 0.0 ? 1.0 : reflex_scaling);

    if (brainModulationConfig.getVector<std::string>("motoneurons").size() > 0) {
      print_verbose("    Found the following motoneuron modulation");
      for(auto &kv : brainModulationConfig.getVector<std::string>("motoneurons")){
        motoneuron_modulated.push_back(kv);
        string str = kv;
        print_verbose(str);
        std::transform(str.begin(), str.end(),str.begin(), ::toupper);
      }
    }



    int i=0;
    for(auto &sensor_name: brainModulationConfig.getVector<std::string>("sensor_name")){
      std::string sensor_cat = brainModulationConfig.getVector<std::string>("sensor_cat")[i];
      if(sensor_cat == "state"){
        std::transform(sensor_name.begin(), sensor_name.end(),sensor_name.begin(), ::toupper);
        sensor_to_modulator.push_back(eventManager->getP<double>((STATES::States)STATES::toID(sensor_name)));
        sensor_to_modulator_name.push_back(sensor_name);
      }
      if(sensor_cat == "input"){
        std::transform(sensor_name.begin(), sensor_name.end(),sensor_name.begin(), ::toupper);
        sensor_to_modulator.push_back(&Input[(INPUT::Inputs)INPUT::toID(sensor_name)]);
        sensor_to_modulator_name.push_back(sensor_name);
      }
      if(sensor_cat == "joint_angle"){
        std::transform(sensor_name.begin(), sensor_name.end(),sensor_name.begin(), ::toupper);
        if(sensor_name == "ALL"){
          for (unsigned joint_pos=INPUT::FIRST_JOINT, joint=JOINT::FIRST_JOINT; joint_pos<=INPUT::LAST_JOINT; joint_pos++, joint++){
              //cout << getServo("HIP_LEFT")->getPosition() << endl;
              sensor_to_modulator.push_back(&Input[joint_pos]);
              sensor_to_modulator_name.push_back(INPUT::toString(joint_pos));
          }
        }
        else{
          cout << "THE MODE " << sensor_name << " IS NOT IMPLEMENTED YET IN " << sensor_cat << " , (GeyerSml.cc)" << endl;
        }
      }
      if(sensor_cat == "sensor"){
        //if(sensor_name == "ALL"){
        bool f = false;
        for(auto&kv : sensors){
            //cout << getServo("HIP_LEFT")->getPosition() << endl;
            if(sensor_name == "all" || sensor_name == kv.first){
              sensor_to_modulator.push_back(kv.second->getP());
              sensor_to_modulator_name.push_back(kv.first);
              f = true;
            }
        }
        if(!f){
          cout << "THE MODE " << sensor_name << " IS NOT IMPLEMENTED YET IN" << sensor_cat << ", (GeyerSml.cc)" << endl;
        }
      }

      i++;
    }


  }
}

void GeyerSml::initialiseSpinalControl_twitching(){
    // for(auto&kv : sensors){
    //     twitchingController.inputs.get<const double*>(kv.first) = &kv.second->get();
    // }

    // for(int i=0; i< muscles.size(); i++){
    //     twitchingController.outputs.get<double*>(MUSCLES::toString(i)) = &muscles[i]->stim;
    // }
    // twitchingController.initialiseMotoneurons();
}

void GeyerSml::initialiseSpinalControl_geyer(){
    for(auto&kv : sensors){
        geyerController.inputs.get<const double*>(kv.first) = &kv.second->get();
    }
    geyerController.inputs.get<const SIDE::Side*>("finishing_stance") = (const SIDE::Side*)eventManager->getP<int>(STATES::FINISHING_STANCE);
    geyerController.inputs.get<MUSCLES_TYPES>("muscles") = muscles;

    for(auto& kv: muscles){
        Muscle * muscle = kv.second;
        std::string muscleStr = kv.first;
        geyerController.outputs.get<double*>(kv.first) = &muscle->stim;
        //geyerController.outputs.get<double*>(MUSCLES::toString(i)) = &muscles[i]->stim;
    }
    print_debug("[ok] : initialisation geyerControllerInputsOutputs");
    geyerController.initialiseMotoneurons();
    print_debug("[ok] : initialisation of motoneurons");
    geyerController.initialiseFeedbacks();
    print_debug("[ok] : initialisation of feedbacks");
    geyerController.initialiseCpgs(left_foot,right_foot);
    print_debug("[ok] : initialisation of cpgs");
}


/*
 * load all feedbacks (as defined in initialiseFeedbacks) in the interneurones network so that they act on motoneurons
 */
void GeyerSml::loadFeedback(SIDE::Side side){
    geyerController.loadFeedback(side);
}
// Load cpg replace feedback
void GeyerSml::loadCpg(SIDE::Side side){
    geyerController.loadCpg(side);
}

/*
 * replace feedbacks by their cpgs counterpart (for cpgs defined in initialiseCpgs)
 */
void GeyerSml::loadPartialCpg(SIDE::Side side){
    geyerController.loadPartialCpg(side);
}


std::string GeyerSml::getSensoryInformation(){
  // We send all the sensors data we have to the RL brain, see yaml files in external_modulator folder
  stringstream jsonLocalObservation;
  jsonLocalObservation << "[";

  int i=0;
  for(auto *kv: sensor_to_modulator){
    //cout << sensor_to_modulator_name[i] << " " << *kv << endl;
    if(i==0){
      jsonLocalObservation << *kv;
    }
    else{
      jsonLocalObservation << ", " << *kv;
    }
    i++;
  }

  jsonLocalObservation << "]";
  return jsonLocalObservation.str();
}

void GeyerSml::brainModulation_reflex(int& rlAction_index){
  // Here we can load parameters from RL Brain.
  if( Settings::get<int>("brain_modulator_reflex") == 1){
    if(this->rlActions.size() > 0) {
      for(auto&kv:reflex_modulated){
        print_debug("       loading " + kv);
        parameters[8][kv] = reflex_scaling * this->rlActions[rlAction_index];
        rlAction_index++;
      }
    }
    else{
      print_debug("    brain modulation of reflexes ON but rlActions empty");
    }
  }
}

void GeyerSml::brainModulation_motoneurons(int& rlAction_index){
  if( Settings::get<int>("brain_modulator_motoneuron") == 1){
    if(this->rlActions.size() > 0){
      for(auto&kv:motoneuron_modulated){
        string str = kv;
        std::transform(str.begin(), str.end(),str.begin(), ::tolower);
        muscles[str]->stim += motoneuron_scaling * this->rlActions[rlAction_index];
        rlAction_index++;
      }
    }
    else{
      print_debug("    brain modulation ON but rlActions empty");
    }
  }
}

void GeyerSml::addMusclePreStimulation(){
  static double pre_stimulation;
  if(right_foot->inStance() && left_foot->inStance()){
      if(eventManager->get<int>(STATES::FINISHING_STANCE) == SIDE::RIGHT){ //double support and right finish the stance
          muscles["right_glu"]->stim -= parameters[0]["deltas"];
          muscles["right_hf"]->stim += parameters[0]["deltas"];
      }
      if(eventManager->get<int>(STATES::FINISHING_STANCE) == SIDE::LEFT){ //double support and left finish the stance
          muscles["left_glu"]->stim -= parameters[0]["deltas"];
          muscles["left_hf"]->stim += parameters[0]["deltas"];
      }
  }

  //for(auto& muscle: muscles){
  for(auto& kv: muscles){
      Muscle * muscle = kv.second;
      string side;
      if (muscle->side == SIDE::LEFT)
          side = "left";
      else
          side = "right";

      pre_stimulation = parameters[0][muscle->name+"_activitybasal_"+side] == 0 ?
                        0.001 :
                        parameters[0][muscle->name+"_activitybasal_"+side]*(1+parameters[8][muscle->name+"_activitybasal"]);
      muscle->stim += pre_stimulation;
  }
}

void GeyerSml::step_SPINE_TO_MTU(double dt){
  int rlActionIdx = 0;
  print_debug("Going into spinal cord");
  this->brainModulation_reflex(rlActionIdx);
  print_debug("[ok] : brainModulation_reflex (GeyerSml.cc)");

  if(Settings::get<string>("NeuralControl") == "twitching") {
    twitchingController.update();
  }
  else {
    geyerController.update();
  }
  print_debug("[ok] : neural controller ok");

  this->brainModulation_motoneurons(rlActionIdx);
  print_debug("[ok] : brainModulation_motoneuron");
  this->addMusclePreStimulation();

  print_debug("[ok] : compute muscles stimulation (GeyerSml.cc)");
}

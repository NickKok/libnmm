#include "TwitchingController.hh"


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
#include <sml/musculoSkeletalSystem/Joint.hh>
#include <sml/musculoSkeletalSystem/Muscle.hh>
#include <sml/musculoSkeletalSystem/Sensor.hh>

#include <sml/sml-tools/EventManager.hh>
#include <sml/sml-tools/OnlineCtrl.hh>
#include <sml/sml-tools/Settings.hh>

extern EventManager* eventManager;
extern bool debug;
namespace nmm {
  extern SmlParameters parameters;
}
using namespace nmm;
using namespace std;


TwitchingControllerWbts::TwitchingControllerWbts(): Controller(), time_step(Settings::get<int>("time_step")){

}

void TwitchingControllerWbts::update(){
    // step_IN_to_MN();
    // step_MN_to_output();
}

static double inSession = false;
static double startSession = false;
static double counter = 0.0; // [s]
static double currentInput = 0.0;

void TwitchingControllerWbts::step_IN_to_MN(){

    // double start_time = 1.0; // [s]
    // double sessionDuration = 2.0;  // [s]
    // double time = eventManager->get<double>(STATES::TIME); // [s]
    // double twitchDuration = 0.1; // [s]

    // if( !inSession && time > start_time + sessionDuration*counter){
    //   inSession = true;
    //   startSession = true;
    // }

    // if(startSession){
    //   cout << "Twitching " << counter << endl;
    //   counter+=1.0;
    //   startSession = false;
    //   currentInput = 1.0;

    // }

    // if(inSession){
    //   currentInput += 0.01*(-currentInput);
    // }
    // if( time > start_time + sessionDuration*(counter) && inSession ){
    //   inSession = false;
    // }

    // int i = 0;
    // for(auto& motoneuron: motoneurons){
    //     motoneuron->input = 0.0;
    //     if(time > start_time && start_time + sessionDuration*(counter-1) == start_time + sessionDuration*(i)){
    //       motoneuron->input = currentInput;
    //     }
    //     i++;
    // }
}


void TwitchingControllerWbts::step_MN_to_output(){
    // for(auto& motoneuron: motoneurons){
    //     motoneuron->apply();
    //     motoneuron->step();
    //     motoneuron->muscle = motoneuron->get();
    // }
}


void TwitchingControllerWbts::initialiseMotoneurons(){
    // motoneurons[MUSCLES::LEFT_SOL]  = new Motoneuron(SIDE::LEFT,"sol",getOutput(MUSCLES::toString(MUSCLES::LEFT_SOL)));
    // motoneurons[MUSCLES::LEFT_TA]   = new Motoneuron(SIDE::LEFT,"ta",getOutput(MUSCLES::toString(MUSCLES::LEFT_TA)));
    // motoneurons[MUSCLES::LEFT_GAS]  = new Motoneuron(SIDE::LEFT,"gas",getOutput(MUSCLES::toString(MUSCLES::LEFT_GAS)));
    // motoneurons[MUSCLES::LEFT_VAS]  = new Motoneuron(SIDE::LEFT,"vas",getOutput(MUSCLES::toString(MUSCLES::LEFT_VAS)));
    // motoneurons[MUSCLES::LEFT_HAM]  = new Motoneuron(SIDE::LEFT,"ham",getOutput(MUSCLES::toString(MUSCLES::LEFT_HAM)));
    // motoneurons[MUSCLES::LEFT_GLU]  = new Motoneuron(SIDE::LEFT,"glu",getOutput(MUSCLES::toString(MUSCLES::LEFT_GLU)));
    // motoneurons[MUSCLES::LEFT_HF]   = new Motoneuron(SIDE::LEFT,"hf",getOutput(MUSCLES::toString(MUSCLES::LEFT_HF)));
    // motoneurons[MUSCLES::LEFT_HAB]  = new Motoneuron(SIDE::LEFT,"hab",getOutput(MUSCLES::toString(MUSCLES::LEFT_HAB)));
    // motoneurons[MUSCLES::LEFT_HAD]  = new Motoneuron(SIDE::LEFT,"had",getOutput(MUSCLES::toString(MUSCLES::LEFT_HAD)));

    // motoneurons[MUSCLES::RIGHT_SOL] = new Motoneuron(SIDE::RIGHT,"sol",getOutput(MUSCLES::toString(MUSCLES::RIGHT_SOL)));
    // motoneurons[MUSCLES::RIGHT_TA]  = new Motoneuron(SIDE::RIGHT,"ta",getOutput(MUSCLES::toString(MUSCLES::RIGHT_TA)));
    // motoneurons[MUSCLES::RIGHT_GAS] = new Motoneuron(SIDE::RIGHT,"gas",getOutput(MUSCLES::toString(MUSCLES::RIGHT_GAS)));
    // motoneurons[MUSCLES::RIGHT_VAS] = new Motoneuron(SIDE::RIGHT,"vas",getOutput(MUSCLES::toString(MUSCLES::RIGHT_VAS)));
    // motoneurons[MUSCLES::RIGHT_HAM] = new Motoneuron(SIDE::RIGHT,"ham",getOutput(MUSCLES::toString(MUSCLES::RIGHT_HAM)));
    // motoneurons[MUSCLES::RIGHT_GLU] = new Motoneuron(SIDE::RIGHT,"glu",getOutput(MUSCLES::toString(MUSCLES::RIGHT_GLU)));
    // motoneurons[MUSCLES::RIGHT_HF]  = new Motoneuron(SIDE::RIGHT,"hf",getOutput(MUSCLES::toString(MUSCLES::RIGHT_HF)));
    // motoneurons[MUSCLES::RIGHT_HAB] = new Motoneuron(SIDE::RIGHT,"hab",getOutput(MUSCLES::toString(MUSCLES::RIGHT_HAB)));
    // motoneurons[MUSCLES::RIGHT_HAD] = new Motoneuron(SIDE::RIGHT,"had",getOutput(MUSCLES::toString(MUSCLES::RIGHT_HAD)));
}

#include <sml/body/Sml.hh>
#include <sml/musculoSkeletalSystem/Muscle.hh>
#include <sml/musculoSkeletalSystem/MuscleJoint.hh>
#include <sml/musculoSkeletalSystem/Joint.hh>
#include <sml/musculoSkeletalSystem/Sensor.hh>
#include <sml/neuralSystem/Interneuron.hh>
#include <sml/neuralSystem/Motoneuron.hh>
#include <sml/sml-tools/Settings.hh>
#include <sml/sml-tools/EventManager.hh>
#include <sml/sml-tools/OnlineCtrl.hh>

#include <boost/xpressive/xpressive.hpp>

extern EventManager* eventManager;
extern OnlineCtrl* onlineCtrl;
namespace nmm {
  extern SmlParameters parameters;
}
using namespace nmm;
using namespace std;
using namespace boost::xpressive;


BasalGanglia::BasalGanglia(double dt, std::vector<double>& Input):dt(dt){
  trunk = new VestibularState(
      Input[INPUT::THETA_TRUNK],
      parameters[0]["kref"],
      parameters[8]["kref"]);
  print_debug("[ok] trunk");
  trunk_cor_linear = new VestibularState2(
      Input[INPUT::THETACOR_TRUNK],STATES::LATERALSPEED_TRUNK,
      parameters[0]["kref_cor_linear"],
      parameters[8]["kref_cor_linear"]);
  print_debug("[ok] trunk cor linear");
  trunk_cor = new VestibularState(
      Input[INPUT::THETACOR_TRUNK],
      parameters[0]["kref_cor"],
      parameters[8]["kref_cor"]);
  print_debug("[ok] trunk cor");

  print_debug("[ok] trunk initialization");

  positionControlSagital = new PositionSensor1(trunk, parameters[0]["kp"], parameters[0]["kd"]);
  positionControlSagital2 = new PositionSensor2(trunk, parameters[0]["kp"], parameters[0]["kd"]);
  positionControlCoronal = new PositionSensor1(trunk_cor, parameters[0]["kp_cor"], parameters[0]["kd_cor"]);

  swingCorrectionSagital = new FixedAtTakeOffSensor(trunk);
  swingCorrectionCoronal = new FixedAtTakeOffSensor(trunk_cor);

  coronalAccumulatorRight = new AccumulatorSensor(trunk_cor_linear,1);
  coronalAccumulatorLeft = new AccumulatorSensor(trunk_cor_linear,-1);



  print_debug("[ok] trunk sensor initialization");
}

void BasalGanglia::step(){
  trunk->step(dt);
  trunk_cor->step(dt);
  trunk_cor_linear->step(dt);
}

Sml::Sml(
    ) :
        Constant(CONSTANT::NUMBER,0.0),
        Input(INPUT::NUMBER,0.0),
        Output(OUTPUT::NUMBER,0.0),
        dt(double(Settings::get<int>("time_step"))/1000.0)
    {
    }

double Sml::getTime(){
    return eventManager->get<double>(STATES::TIME);
}

//--------------------------------------------------------------
int Sml::step(){
    print_debug("START SML STEP");
    //Timer timer;
    //timer.Start();
    InputUpdate();
    print_debug("[ok] : input update ok (Sml.cc)");
    /// Muscles to mechanical system
    step_JOINT(dt);
    basalGanglia->step();
    print_debug("[ok] : compute angle (Sml.cc)");
    step_MTU_to_TORQUE(dt);
    print_debug("[ok] : compute muscle (Sml.cc)");
    step_TORQUE_to_JOINT();
    print_debug("[ok] : compute torque (Sml.cc)");
    /// Sensors to Neural control
    step_SENSORS();
    eventManager->step();
    print_debug("[ok] : compute sensors (Sml.cc)");
    /// Neural control to Muscles
    step_SPINE_TO_MTU(dt);
    /// Perturbation
    step_PERTURBATION();


    print_debug("[ok] : eventManager (Sml.cc)");
    return (int)eventManager->isRunFinished();
}

//compute l_MTC
void Sml::step_MTU_to_TORQUE(double dt){
    static int muscle_step_number = Settings::get<int>("muscle_step_number");
    static int i;
    for(auto&  kv : muscles){
        Muscle * muscle = kv.second;
        muscle->ApplyForce();
        for(i=0;i<muscle_step_number;i++)
            muscle->step(dt/muscle_step_number);

    }

}
//compute all the joint angles
void Sml::step_JOINT(double dt){
    for(auto&  joint : joints){
        joint->step(dt);
    }
}


void Sml::step_TORQUE_to_JOINT(){
    static int torque_soft_limit = Settings::get<int>("torque_soft_limit");
    for(auto&  joint : joints){
        if (torque_soft_limit) joint->ComputeTorqueSoftLimit();
        joint->ComputeTorque();
    }

}

void Sml::step_SENSORS(){
    for(auto &sen: sensors){
        sen.second->step();
    }
}

void Sml::step_PERTURBATION(){
    int perturbation = Settings::get<int>("perturbation");
    if (perturbation != 1) return;

    bool finishedPert;
    bool startPert=false;
    bool finishedPertPrinted=false;
    finishedPert = true;
    for (
        //INIT
        unsigned joint=JOINT::FIRST_JOINT,
                output=OUTPUT::FIRST_JOINT;
        output<=OUTPUT::LAST_JOINT;

        joint++,
        output++)
    {
         if(
            perturbation == 1 &&
            perturbator.find(JOINT::Joints(joint))
            )
         {
             if(perturbator[JOINT::Joints(joint)]->doPerturbation() )
             {
                perturbator[JOINT::Joints(joint)]->updatePerturbation();
                Output[output] += perturbator[JOINT::Joints(joint)]->getPerturbation();

                startPert = true;
                finishedPert = false;
             }
         }
    }

    if(finishedPert && !finishedPertPrinted && startPert){
        finishedPertPrinted = true;
        cout << "perturbation on one joint ended" << endl;
        //eventManager->set(STATES::STAY_IN_LOOP,false);
    }
}

void Sml::step_SPINE_TO_MTU(double dt){
}

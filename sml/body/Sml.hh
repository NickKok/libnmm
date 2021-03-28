#pragma once
#ifndef __Sml_HH__
#define __Sml_HH__


#include <string>
#include <math.h>
#include <vector>
#include <fstream>
#include <map>
#include <array>

//#include <boost/algorithm/string.hpp>


#include <sml/types/types.h>
#include <sml/neuralSystem/Interneuron.hh>
#include <sml/neuralSystem/Motoneuron.hh>
#include <sml/neuralSystem/Entity.hh>
#include <sml/neuralSystem/Connection.hh>
#include <sml/musculoSkeletalSystem/Joint.hh>
#include <sml/musculoSkeletalSystem/Muscle.hh>
#include <sml/musculoSkeletalSystem/Sensor.hh>

#include <sml/sml-tools/EventManager.hh>
#include <sml/sml-tools/PerturbationManager.hh>
#include <sml/sml-tools/OnlineCtrl.hh>
#include <sml/neuralSystem/ReflexController.hh>
#include <sml/neuralSystem/TwitchingController.hh>

typedef std::map<std::string, Muscle *> MUSCLES_TYPES;

class BasalGanglia
{
private:
  double dt;
public:
  VestibularState * trunk;
	VestibularState * trunk_cor;
	VestibularState2 * trunk_cor_linear;
  PositionSensor1* positionControlSagital; // PositionSensor1
  PositionSensor2* positionControlSagital2; // PositionSensor2
  PositionSensor1* positionControlCoronal; // PositionSensor1
  FixedAtTakeOffSensor* swingCorrectionSagital; // FixedAtTakeOffSensor
  FixedAtTakeOffSensor* swingCorrectionCoronal; // FixedAtTakeOffSensor
  AccumulatorSensor* coronalAccumulatorRight; //AccumulatorSensor
  AccumulatorSensor* coronalAccumulatorLeft; //AccumulatorSensor
  BasalGanglia(double dt, std::vector<double>& Input);
  void step();
};

class Sml
{
private:
    void initialiseOnlineCtrl();
    void initialisePerturbation();
	void initialiseSensors();
	void initialiseMusculoSkeletalSystem();

    virtual void ConstantInit()=0;
    virtual void InputInit()=0;
    virtual void InputUpdate()=0;

	void step_JOINT(double dt);
	void step_MTU_to_TORQUE(double dt);
	void step_TORQUE_to_JOINT();
	void step_SENSORS();
	void step_PERTURBATION();

	double phi_knee_off;
	double soft_limit_stiffness;
	double theta_trunk_ref;
	double zero_length_offset;
	double one_gain;

	virtual void initialiseSpinalControl(){}
	virtual void step_SPINE_TO_MTU(double dt);

	Parameters bodyConfig;

protected:
  double getTime();

public:
	std::vector<double> rlActions;
    std::vector<double> Constant;
    std::vector<double> Input;
    std::vector<double> Output;

	double dt;
	int step();

	MUSCLES_TYPES muscles;

	std::map<std::string, Sensor *> sensors;
	std::array<Joint *, JOINT::NUMBER> joints;
	Foot * left_foot;
	Foot * right_foot;
  BasalGanglia * basalGanglia;
	PerturbatorManager perturbator;

	Sml();
	//destructor
	//~Sml();

    void init();
};

#endif /* __ROB_HH__ */

#pragma once
#ifndef __SENSOR_HH__
#define __SENSOR_HH__

/*!
 * \file Sensor.hh
 * \brief Sensor description (touch sensor / muscle length / muscle force / foot / ...)
 * \author efx
 * \version 0.1
 */

#include <vector>
#include <string>
#include <math.h>
#include <map>

class Muscle;
class Joint;

class Sensor {
public:
	double value;
    void step(){
    	value = update();
    };
    const double& get(){
    	return value;
    }
    const double* getP(){
    	return (const double*)(&value);
    }
    double tf(double);
private:
	virtual double update() = 0;
};

/**
 *
 * TouchSensor class
 *
 * 		Ground sensors (left teel, left ankle, right teel, right ankle) are Biosensors
 *
 *
 */
class TouchSensor : public Sensor, public Derivatives {
public:
  double& input;
	double lastValue;
	double dt;
	TouchSensor(double& input, double dt):
			input(input),lastValue(10.0),dt(dt){} //lastValue to TO_BE_SERIALIZED

    double getInput(); // To serialize
	std::vector<double> dydt(double x, std::vector<double> y, std::vector<double> param, std::vector<double> input);
private:
	double update();
};


/**
 *
 * MuscleLengthSensor class
 *
 *
 */
class MuscleLengthSensor: public Sensor{
public:
	Muscle * muscle;
	double& offset;
	double& offsetRL;
	double& gain;
	MuscleLengthSensor(Muscle * muscle, double &offset, double &offsetRL, double &gain):muscle(muscle),offset(offset),offsetRL(offsetRL), gain(gain){}
private:
	double update();
};

/**
 *
 * MuscleForceSensor class
 *
 *
 */
class MuscleForceSensor: public Sensor{
	Muscle * muscle;
	double& gain;
public:
	MuscleForceSensor(Muscle * muscle, double &gain):muscle(muscle), gain(gain){}
private:
	double update();
};


/**
 *
 * JointPositionSensor class
 *
 *
 */
class JointPositionSensor: public Sensor{
	Joint * joint;
	double& kp;
	double& kd;
	double& ref;
public:
	JointPositionSensor(Joint * joint, double& kp, double& kd, double & ref):joint(joint),kp(kp),kd(kd), ref(ref){}
private:
	double update();
};


/**
 *
 * VestibularState class, state is with respect to the vertical axes (the one // to gravity)
 *
 */
class VestibularState{
public:
	double& input;
	double theta;
	double& theta_ref;
	double& theta_offset;
	double d_theta;
	double theta_to;
	//VestibularState():theta(0.0),theta_ref(0.0),d_theta(0.0),theta_to(0.0){}
	VestibularState(
				double& input,
        double& theta_trunk_ref,
        double& theta_offset);
    virtual double get();
	void step(double dt);
	void setThetaRef(double t);
	double getThetaRef();
	void addThetaRef(double t);
	void updateTo();
};


class VestibularState2: public VestibularState{
public:
  STATES::States state_id;
	VestibularState2(double& input,
        STATES::States state_id,
        double& theta_trunk_ref,
        double& theta_offset);
    double get();
};

class PositionSensor1: public Sensor{
	VestibularState * trunk;
	double& kp;
	double& kd;
public:
	PositionSensor1(VestibularState * trunk, double& kp, double& kd):trunk(trunk),kp(kp),kd(kd){}
private:
	double update();
};
class PositionSensor2: public Sensor{
	VestibularState * trunk;
	double& kp;
	double& kd;
public:
	PositionSensor2(VestibularState * trunk, double& kp, double& kd):trunk(trunk),kp(kp),kd(kd){}
private:
	double update();
};
/**
 *
 * FixedAtTakeOffSensor class
 *
 *
 */
class FixedAtTakeOffSensor: public Sensor{
	VestibularState * trunk;
public:
	FixedAtTakeOffSensor(VestibularState * trunk): trunk(trunk){}
private:
	double update();
};
/**
 *
 * FixedAtTakeOffSensor class
 *
 *
 */
class AccumulatorSensor: public Sensor{
	VestibularState * trunk;
	int sign;
public:
	AccumulatorSensor(VestibularState * trunk, int sign);
private:
	double update();
	void reset();
};
/**
 *
 * AngleSensor class for OverExtensionReflex
 *
 *
 */
class AngleSensor: public Sensor{
	Joint * joint;
	double& offset;
public:
	AngleSensor(Joint * joint, double& offset): joint(joint), offset(offset){}
private:
	double update();
};
/**
 *
 * Foot class (special sensors that have to TouchSensor to feel the ground
 * and also a limb_state variable that says if the foot thinks is in stance or swing
 *
 *   This is a special sensor used by the Gate that turns the feedbacks on/off
 *   regarding the limb_state on which they need to be active.
 *
 */

class Foot: public Sensor
{
public:
	double scalingfactor;
	CYCLE_PHASE::CyclePhase limb_state;
	CYCLE_PHASE::CyclePhase lastlimb_state;
	double delay;
	TouchSensor * heel;
	TouchSensor * toe;
	double last_time_limb_state_changed;
	double last_time_foot_touch;
	double duration_last_step;
	double distance_last_step;
	double last_pos_foot_touch;
	double &off;
	SIDE::Side side;
	Foot(TouchSensor * heel, TouchSensor * toe, SIDE::Side side,double &off):  scalingfactor(784.8),limb_state(CYCLE_PHASE::STANCE), lastlimb_state(CYCLE_PHASE::STANCE), delay(0.05), heel(heel), toe(toe),last_time_limb_state_changed(-1.0),last_time_foot_touch(0.5),duration_last_step(0.5),distance_last_step(0.0), last_pos_foot_touch(0.0),off(off),side(side){}
	Foot(TouchSensor * heel, TouchSensor * toe, double time, SIDE::Side side, double &off):  scalingfactor(784.8),limb_state(CYCLE_PHASE::STANCE), lastlimb_state(CYCLE_PHASE::STANCE), delay(0.05), heel(heel), toe(toe), last_time_limb_state_changed(time),off(off),side(side){}
	std::string getName();
	bool inStance();
	bool justTouchTheGround();
	bool justGetOffGround();
	double getFeltForce();
	double getRealFeltForce();
	double getXForce();
	double getYForce();
	//double getZForce();
	CYCLE_PHASE::CyclePhase getLastState();
	CYCLE_PHASE::CyclePhase getState();
	void setState(CYCLE_PHASE::CyclePhase str);
	void updateState(double actual_time);
    bool touchedBefore(Foot * cfoot);
    bool changeStateAt();
private:
	double update();
};


#endif /* __SENSOR_HH__ */

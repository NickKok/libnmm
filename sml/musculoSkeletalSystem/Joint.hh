#pragma once
#ifndef __JOINT_HH__
#define __JOINT_HH__

/*!
 * \file Joint.hh
 * \brief Joint definition
 * \author efx
 * \version 0.1
 */
#include <string>
#include <math.h>
#include <map>


namespace JointMomentArms {
    enum MOMENT_ARM { CONSTANT, GEYER, GEOMETRIC};
    
}


class Sml;


class Joint {
public:
	Joint(
       JointMomentArms::MOMENT_ARM joint_type,
       double * input,
       double * output,
       double reference_angle, 
       double angle_min, 
       double angle_max,
       bool sagital
    );
    
    Joint(
       JointMomentArms::MOMENT_ARM joint_type,
       double reference_angle, 
       double angle_min, 
       double angle_max
    ) : Joint(joint_type, &d_input,&d_output,reference_angle, angle_min, angle_max, true){};
    
    
	virtual ~Joint() {}
	//---------------------------
	std::string name;
	JointMomentArms::MOMENT_ARM joint_type;
    double * input;
    double * output;
    double d_input;
    double d_output;
	//double position;
	//double dposition;
	double dt;
	double reference_angle;
	double angle;
	double d_angle;
	double angle_min;
	double angle_max;
	double angle_ref;
	double torque;
	double torque_soft_limit;
	double soft_limit_stiffness;
	bool sagital;
	//---------------------------
	void AddForce(double force);
	void step();
	void step(double dt);
	
	double angleTF(double angle_ref, double angle_max);
	double angleTFprime(double angle_max);
	void ComputeTorqueSoftLimit();
	double getAngle();
	void ComputeTorque();
    std::string getName();
	
};

#endif /* __JOINT_HH__ */

#include <fstream>
#include <iostream>

#include <sml/types/types.h>
#include <sml/sml-tools/Settings.hh>
#include <sml/sml-tools/EventManager.hh>

#include "MuscleJoint.hh"
#include "Muscle.hh"
#include "Joint.hh"

double MuscleJoint::getDelta_Length(){
    double delta_length = angleTF(joint->joint_type, joint->getAngle());
    return r_0*delta_length;
}

double MuscleJoint::getTorque(){
    double moment_arm = angleTFprime(joint->joint_type, joint->getAngle());
    double torque = r_0*muscle->getForce()*moment_arm;
    return -torque;
}

void MuscleJoint::addTorqueToJoint(){
	joint->AddForce(getTorque());
}

/**
	*  the theta_max corresponds to the angle at which the 
	*  force of the muscle applied to the joints is maximum.
	**/
double MuscleJoint::angleTFprime(JointMomentArms::MOMENT_ARM joint_type, double angle){
// H. Geyer angle transfer function
    static int angle_transfer_function = Settings::get<int>("angle_transfer_function");
    if(joint_type == JointMomentArms::CONSTANT)
        return 1.0;
    else if(angle_transfer_function == 0)
    {
        return cos(angle-theta_max);
    }
    else
    {
        double alpha = cos(theta_max);
        return sin(angle)/sqrt(1+alpha*alpha-2*alpha*cos(angle));
    }
}
/**
	* 	the theta_max corresponds to the angle at which the 
	* 	force of the muscle applied to the joints is maximum.
	**/
double MuscleJoint::angleTF(JointMomentArms::MOMENT_ARM joint_type, double angle)
{
    static int angle_transfer_function = Settings::get<int>("angle_transfer_function");
    static double alpha;
    if(
        joint_type == JointMomentArms::CONSTANT
	){
		return angle - theta_ref;
    }
	// H. Geyer angle transfer function
	else if(angle_transfer_function == 0)
	{
		return sin(angle - theta_max) - sin(theta_ref - theta_max);
	}
	else
	{
		alpha = cos(theta_max);
		return (sqrt(-2*alpha*cos(angle)+alpha*alpha+1)-sqrt(-2*alpha*cos(theta_ref)+alpha*alpha+1))/alpha;
	}
	
}

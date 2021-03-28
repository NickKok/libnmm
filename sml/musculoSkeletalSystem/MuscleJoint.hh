#ifndef __MUSCLEJOINT_HH__
#define __MUSCLEJOINT_HH__

class Muscle;
class Joint;

class MuscleJoint {
private:
	double angleTFprime(JointMomentArms::MOMENT_ARM joint_type, double angle);
	double angleTF(JointMomentArms::MOMENT_ARM joint_type, double angle);
    const double theta_max;
    const double theta_ref;
    const double r_0;
    Muscle* muscle;
    



public:
    Joint* joint;
	double getDelta_Length();
    double getTorque();
    void addTorqueToJoint();

    MuscleJoint(Joint* joint,Muscle* muscle,double r_0,double theta_ref):
    	joint(joint),
    	muscle(muscle),
    	theta_max(0.0),
    	theta_ref(theta_ref),
    	r_0(r_0) {
    }
    
    MuscleJoint(Joint* joint,Muscle* muscle,double r_0,double theta_ref,double theta_max):
    	joint(joint),
    	muscle(muscle),
    	theta_max(theta_max),
    	theta_ref(theta_ref),
    	r_0(r_0) {
    }

};

#endif

#include <sml/body/Sml.hh>
#include <sml/sml-tools/Settings.hh>
#include <boost/any.hpp>

#include "Joint.hh"

extern EventManager* eventManager;
using namespace std;

Joint::Joint(
    JointMomentArms::MOMENT_ARM joint_type,
    double * input,
    double * output,
    double reference_angle,
    double angle_min,
    double angle_max,
    bool sagital) : 
        dt(double(Settings::get<int>("time_step"))/1000.0),
        reference_angle(reference_angle),
        joint_type(joint_type),
        input(input),
        output(output),
        d_input(0.0),
        d_output(0.0),
        angle(*input),
        d_angle(0.0),
        angle_min(angle_min),
        angle_max(angle_max),
        angle_ref(0.0),
        torque(0.0),
        torque_soft_limit(0.0),
        soft_limit_stiffness(17.19),
        sagital(sagital)
{
    
}

double Joint::getAngle()
{
	return *input;
}
/**
 * 	the angle_max corresponds to the angle at which the 
 * 	force of the muscle applied to the joints is maximum.
 **/
double Joint::angleTF(double angle_ref, double angle_max)
{
    static int angle_transfer_function = Settings::get<int>("angle_transfer_function");
    static double alpha;
    if(
        joint_type == JointMomentArms::CONSTANT
	){
		return getAngle() - angle_ref;
    }
	// H. Geyer angle transfer function
	else if(angle_transfer_function == 0)
	{
		return sin(getAngle() - angle_max) - sin(angle_ref - angle_max);
	}
	else
	{
		alpha = cos(angle_max);
		return (sqrt(-2*alpha*cos(getAngle())+alpha*alpha+1)-sqrt(-2*alpha*cos(angle_ref)+alpha*alpha+1))/alpha;
	}
	
}

void Joint::step()
{
	step(dt);
}

void Joint::step(double dt)
{
    //cout << getName() << "\t" << sml->Input[input_id]*180/3.14 << endl;
    static double new_angle;
    new_angle = getAngle();
	d_angle = ( new_angle - angle)/dt;
	angle = new_angle;
	torque = 0.0;
    
}
/**
 * 	the angle_max corresponds to the angle at which the 
 * 	force of the muscle applied to the joints is maximum.
 **/
double Joint::angleTFprime(double angle_max){
// H. Geyer angle transfer function
    static int angle_transfer_function = Settings::get<int>("angle_transfer_function");
    if(joint_type == JointMomentArms::CONSTANT)
		return 1.0;
	else if(angle_transfer_function == 0)
	{
		return cos(getAngle()-angle_max);
	}
	else
	{
		double alpha = cos(angle_max);
		return sin(getAngle())/sqrt(1+alpha*alpha-2*alpha*cos(getAngle()));
	}
}
void Joint::AddForce(double force){
	torque += force;
}
int sign(double val){
	if(val < 0.0)
		return -1;
	else
		return 1;
}
void Joint::ComputeTorqueSoftLimit(){
     static int coman  = Settings::get<int>("coman");
     static double flag = (dt*1000.0);
     static double force_ratio =1.0;
     static double n_vLim = 1.0*0.0175;

     if(coman){
         force_ratio = 25.8/80.0;
     }
     
    if ( angle < angle_min ){
        if ( d_angle < n_vLim )
            torque_soft_limit = soft_limit_stiffness / flag * ( angle - angle_min) * ( 1.0 - d_angle / n_vLim / flag );
    }
    else if ( angle > angle_max){ 
        if (d_angle > -n_vLim )
            torque_soft_limit = soft_limit_stiffness / flag * ( angle - angle_max ) * ( 1.0 + d_angle / n_vLim / flag );
    }
    else 
            torque_soft_limit = 0.0;
     
 
     torque_soft_limit *= force_ratio;

        
}


void Joint::ComputeTorque(){
    static int is2D  = Settings::get<int>("2D");
    if(is2D == 1 && this->sagital == false){
        *output = 0.0;
    }
    else{
        *output = (torque - torque_soft_limit );
    }	
}

std::string Joint::getName(){
    return name;
}
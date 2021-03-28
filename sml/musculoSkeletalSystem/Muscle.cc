#include <fstream>
#include <iostream>

#include <sml/types/types.h>
#include <sml/sml-tools/Settings.hh>
#include <sml/sml-tools/EventManager.hh>
#include <sml/sml-tools/Helper.hh>

#include <efx/rungekutta.h>


#include "Muscle.hh"
#include "Joint.hh"
#include "MuscleJoint.hh"

extern EventManager* eventManager;

using namespace std;



Muscle::Muscle(SIDE::Side side, std::string name, double& l_slack, double& l_opt, double& v_max, double& F_max, double& pennation, double& typeI_fiber) :
// Constant
side(side),
name(name),
noise(0.0),
// Specific variable
l_slack(l_slack), l_opt(l_opt), v_max(v_max),mass((density*l_opt)*F_max/(F_per_m2 * pennation)),F_max(F_max), pennation(pennation), typeI_fiber(typeI_fiber),
// Muscle variables
stim(0.01), l_SE(0.0), l_CE(0.0), v_CE(0.0),
// Energy muscle variables
d_energyW(0.0), d_energyM(0.0), d_energyA(0.0), d_energyS(0.0),d_energy(0.0), cst_energyM(100.0)
{
	this->sagital = true;
	A = stim;
	initialise_muscle_length();
    //F_max = F_per_m2 * mass*pennation/(density*l_opt));
}

Muscle::Muscle(SIDE::Side side, std::string name, double& l_slack, double& l_opt, double& v_max, double& F_max, double& pennation, double& typeI_fiber, bool sagital) :
Muscle::Muscle(side, name, l_slack, l_opt, v_max, F_max, pennation, typeI_fiber)
{
	this->sagital = sagital;
}

//destructor
Muscle::~Muscle()
{}



string Muscle::getName(){
    static string side_str;
    side_str = "left_";
    if (side == SIDE::RIGHT) side_str = "right_";
    return side_str+name;
}


//--------------METHODES-------------------------------------------------------


double Muscle::_F_SE(double l_SE){
	double numerator 	= (l_SE-l_slack);
	double denominator 	= (l_slack*E_ref);
	return (l_SE > l_slack 		? 	F_max * ( numerator/denominator ) * ( numerator/denominator ) : 0.0);
}
double Muscle::_F_PE_star(double l_CE){
	double numerator	= (l_CE-l_opt);
	double denominator	= (l_opt*w);
	return  (l_CE > l_opt 	? 	F_max * ( numerator/denominator ) * ( numerator/denominator ) : 0.0);	
}
double Muscle::_F_BE(double l_CE){
	double numerator 	= (l_CE-l_opt*(1.0-w));
	double denominator 	= (l_opt*w/2.0);
	return (l_CE <= l_opt*(1.0-w) 	? 	F_max * ( numerator/denominator ) * ( numerator/denominator ) : 0.0);
}

double Muscle::_f_l(double l_CE){
	double numerator = (l_CE-l_opt);
	double denominator = (l_opt*w);
	double val = fabs(numerator/denominator);
	double exposant = c * val * val * val;
	return exp( exposant );
}
double Muscle::_f_v(double v_CE)
{
	if ( v_CE >= 0.0)
		return (v_max - v_CE)/(v_max + K*v_CE);
	else
		return N + (N - 1) * ( v_max + v_CE ) / (7.56 * K * v_CE - v_max);
}
double Muscle::_f_v(double F_SE, double F_BE, double a, double f_l, double F_PE_star)
{
	double f_v;

	if(F_max*a*f_l+F_PE_star == 0.0)
		f_v = 0.0;
	else
		f_v = (F_SE+F_BE)/((F_max*a*f_l)+F_PE_star); 

	f_v = (f_v > 1.5) ? 1.5 : f_v;
	f_v = (f_v < 0.0) ? 0.0 : f_v;
	return f_v;
}


double Muscle::_v_CE( double f_v){
	return (f_v < 1.0) ? v_max * l_opt * (1.0-f_v)/(1.0+f_v*K) : v_max * l_opt * (f_v-1.0)/( 7.56*K*(f_v-N)+1.0-N);
}

double Muscle::getMuscleActiveForce(){
	return A*_f_v(v_CE)*_f_l(l_CE)*F_max;
}
double Muscle::getMuscleActiveForce(double l_CE, double v_CE, double a){
	return a*_f_v(v_CE)*_f_l(l_CE)*F_max;
}

double Muscle::getMuscleTendonForce(double l_CE)
{
	return _F_SE(getTendonLength(l_CE));
}

double Muscle::getTendonLength(double l_CE)
{
	return getMuscleTendonLength() - l_CE;
}

double Muscle::getMuscleTendonLength()
{
	return l_MTC;
}


void Muscle::updateEnergyConsumption(double dt){

	auto fA = [&] () -> double {
		return 40 * typeI_fiber * sin( PI/2 * stim) + 133*(1 - typeI_fiber)*(1-cos(PI/2* stim));
	};
	auto fM = [&] () -> double {
		return 74 * typeI_fiber * sin( PI/2 * stim) + 111*(1 - typeI_fiber)*(1-cos(PI/2* stim));
	};
	auto g = [&] () -> double {
		double l = l_CE/l_opt;
		if(l >= 0 && l <= 0.5)
			return 0.5;
		else if(l > 0.5 && l <= 1.0)
			return l;
		else if(l > 1.0 && l <= 1.5)
			return -2*l+3;
		else
			return 0;
	};

	double F_MF = getMuscleActiveForce(l_CE, v_CE, A);
	double F_MTC = getMuscleTendonForce(l_CE);

	d_energyA = dt*mass*fA();
	d_energyM = dt*mass*g()*fM();
	
	d_energyS = -v_CE > 0.0 ? dt*0.25*F_MTC*(-v_CE) : 0.0;
	d_energyW = -v_CE > 0.0 ? dt*F_MF*(-v_CE) : 0.0;

	d_energy = d_energyA + d_energyM + d_energyS + d_energyW;

}
void Muscle::updateActivation(double dt){
    /**	Neural Input	*/
	static int noise_muscle = Settings::get<int>("noise_muscle");
	static int buggyMuscle_tone = Settings::get<int>("buggyMuscle_tone");

    noise = getSignalDependentNoise(stim);
    if(noise_muscle == 1 && eventManager->get<bool>(STATES::IS_LAST_PHASE)) stim+=noise;
	
    if(buggyMuscle_tone == 0) stim = max(0.01,min(1.0,stim));

	dA = (stim-A) * dt/tau_act;
	A+=dA;
	
	if(buggyMuscle_tone == 1) A = max(0.01,min(1.0,A));
}


//compute l_SE and l_CE for a given l_MTC at steady state with v_CE=0.0 and A=0.0
void Muscle::initialise_muscle_length(){
	ComputeMuscleLength();
	if(l_MTC < (l_slack + l_opt)){
		l_CE = l_opt;
		l_SE = l_MTC - l_CE;
	}
	else{
		if (l_opt*w + E_ref*l_slack != 0.0)
			l_SE = l_slack * ( (l_opt*w + E_ref*(l_MTC-l_opt)) / (l_opt*w + E_ref*l_slack) );
		else
			l_SE = l_slack;
		l_CE = l_MTC - l_SE;
	}
}

//compute the new muscle state for one time step
void Muscle::step(double dt){
    static double F_PE_star;

    updateActivation(dt);
	updateEnergyConsumption(dt);
	
	// muscle length
	ComputeMuscleLength();

	v_CE = _v_CE(
			_f_v(
				_F_SE(l_SE),
				_F_BE(l_CE), 
				A, 
				_f_l(l_CE), 
				_F_PE_star(l_CE)
				)
			);
	// active element activity
	l_CE = (l_CE > 0.0) ? l_CE - v_CE*dt : 0.0;
	
	// passive element length
	l_SE = l_MTC - l_CE;

}


void Muscle::ComputeMuscleLength()
{
    l_MTC = l_slack + l_opt;
    for (auto j_m_link : musclejoints)
    	l_MTC+=pennation*j_m_link->getDelta_Length();
}
void Muscle::ApplyForce()
{
    for (auto j_m_link : musclejoints)
        j_m_link->addTorqueToJoint();
}

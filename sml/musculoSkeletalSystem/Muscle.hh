#pragma once
#ifndef __MUSCLE_HH__
#define __MUSCLE_HH__

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <math.h>

class MuscleJoint;
class Joint;


class Muscle
{
private:
	//constante
	const double E_ref = 0.04;	//reference strain
	const double c = log(0.05);
	const double w = 0.56;
	const double N = 1.5;
	const double K = 5.0;
	const double tau_act = 0.01;	//time cst of the derivative of activation
	const double F_per_m2 = 250000; //force per m2 of muscle physiological cross sectional area
	const double density = 1060;	//density of muscle;
	const double mass;

	double l_CE;	//length of the contractil element
	double v_CE;	//speed of the contractile element
	double A;
	double l_SE;	//length of the serie elastic
	double l_MTC; //length of the muscle-tendon complex
	double dA;	 //dA/dt derivative of activation
	bool sagital;
	//------------ PRIVATE METHODES----------------------------------------

	void updateEnergyConsumption(double);
	void updateActivation(double dt);
	void initialise_muscle_length();//compute l_SE and l_CE for a given l_MTC at steady state
	void ComputeMuscleLength();

	double _F_SE(double l_SE);
	double _F_PE_star(double l_CE);
	double _F_BE(double l_CE);
	double _f_l(double l_CE);
	double _f_v(double v_CE);
	double _f_v(double F_SE, double F_BE, double a, double f_l, double F_PE_star);
	double _v_CE( double f_v);

	double getTendonLength(double l_CE);
	double getMuscleTendonForce(double l_SE);
	double getMuscleTendonLength();
	double getMuscleActiveForce(double l_CE, double v_CE, double a);
public:
	SIDE::Side side;
	std::string name;
	//muscle specific constant
	const double& l_slack; //rest length of the serie elastic
	const double& l_opt;
	const double& v_max;
	const double& F_max;
	const double& pennation;
	const double& typeI_fiber;
	//variables
	std::vector<MuscleJoint*> musclejoints;
	double noise; // noise applied to the muscle;
	double stim; //stimulation of the muscle
	double d_energyW; //energy used by the muscle in a single time step
	double d_energyM;
	double d_energyA;
	double d_energyS;
	double d_energy;
	double cst_energyM;
	
	//------------ PUBLIC METHODES----------------------------------------
	
	//constructor
	Muscle(SIDE::Side side, std::string name, double& l_slack, double& l_opt, double& v_max, double& F_max, double& pennation,double& typeI_fiber);
	Muscle(SIDE::Side side, std::string name, double& l_slack, double& l_opt, double& v_max, double& F_max, double& pennation,double& typeI_fiber, bool sagital);
	//destructor
	~Muscle();
	
    std::string getName();
		bool isSagital(){
			return this->sagital;
		}
	void step(double dt);//compute the new muscle state for one time step
	double getV_CE(){ return v_CE;}
	double getL_CE(){ return l_CE;}
	double getL_SE(){ return l_SE;}
	double getA(){ return A;}
	double getF_v(){ return _f_v(v_CE);}
	double getNoise() { return noise;}
	void ApplyForce();
	double getForce(){ return getMuscleTendonForce(l_CE);}
	double getMuscleActiveForce();

};


#endif /* __MUSCLE_HH__ */

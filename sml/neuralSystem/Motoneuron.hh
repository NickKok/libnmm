#pragma once
#ifndef __MOTONEURON_HH__
#define __MOTONEURON_HH__

/*!
 * \file Motoneuron.hh
 * \brief Motoneuron definition
 * \author efx
 * \version 0.1
 */

#include <string>
#include <iostream>
#include <sml/sml-tools/RungeKutta.hh>

class Entity;
class Muscle;

class Motoneuron : public Entity{
	public:
	std::string name;
    double& muscle;
    
	double input;
	double tau;
    double dt;
	Motoneuron(SIDE::Side side, std::string name, double& muscle);
	std::string getName();
	void set(double value);
	void add(double value);
	void apply();
	void apply(double a, double o){};
    double tf(double x){return x;}
};

#endif /* __MOTONEURON_HH__ */
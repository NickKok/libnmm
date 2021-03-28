#pragma once
#ifndef __CONNECTION_HH__
#define __CONNECTION_HH__

/*!
 * \file Entity.hh
 * \brief Connection type
 * \author efx
 * \version 0.1
 */

#include <vector>
#include <string>

#include <sml/sml-tools/RungeKutta.hh>


class ReflexController;

class Connection{
public:
    Entity * input;
    Entity * output;
    double dt;
    double weight;
    double value;
    double amplitude;
    double offset;
    Connection(Entity * input, Entity * output): input(input), output(output), dt(0.001),weight(1.0),amplitude(1.0),offset(0.0){
        value = output->get();
    };
    
    virtual void step() = 0;
	virtual void step(double) = 0;
    virtual double getWeight();
    virtual void setWeight(double weight);
    virtual bool isGateOpen(ReflexController * body);
    void reset();
};


class NoDelayConnection : public Connection{
public:
    NoDelayConnection( Entity * input, Entity * output): Connection(input,output){};
    void step();
	void step(double);
};

class DiscreteDelayConnection : public Connection{
public:
    int delay;
    DiscreteDelayConnection( Entity * input, Entity * output, int delay): Connection(input,output), delay(delay) {};
    void step();
	void step(double);
};

class FirstOrderConnection : public Connection, public Derivatives{
private:
    double value;

public:
    FirstOrderConnection( Entity * input, Entity * output): Connection(input,output){};
    void step();
	void step(double);
    std::vector<double> dydt(double x, std::vector<double> y, std::vector<double> param, std::vector<double> input);
};

#endif
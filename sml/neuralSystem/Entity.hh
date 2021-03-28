#pragma once
#ifndef __ENTITY_HH__
#define __ENTITY_HH__

/*!
 * \file Entity.hh
 * \brief Entity definiton (base class of all neurons) and the different connection type
 * \author efx
 * \version 0.1
 */

#include <vector>
#include <string>
#include <boost/circular_buffer.hpp>

class Sml;

struct stateBuffer
{
        int length;
        boost::circular_buffer<double> cb;
        stateBuffer(int length): length(length), cb(length,0.0){};

        double operator()(double t); // add input to buffer
        double operator()(int t); // retrieve input with delay t (in time step)
        double operator()(); // retrieve input without delay

};
class Entity{
protected:
	double state;
    stateBuffer state_buffer;
public:
    bool no_tf;
	SIDE::Side side;
	ACTIVE_DURING::ActiveDuring active_during;
	Entity():state(0.0),state_buffer(BUFFER_SIZE),side(SIDE::ALL),active_during(ACTIVE_DURING::WHOLE_CYCLE){init();}
	Entity(double state):state(state),state_buffer(BUFFER_SIZE),side(SIDE::ALL),active_during(ACTIVE_DURING::WHOLE_CYCLE){init();}
	Entity(SIDE::Side side):state(0.0),state_buffer(BUFFER_SIZE),side(side),active_during(ACTIVE_DURING::WHOLE_CYCLE){init();}
	Entity(SIDE::Side side, double state):state(state),state_buffer(BUFFER_SIZE),side(side),active_during(ACTIVE_DURING::WHOLE_CYCLE){init();}
	Entity(SIDE::Side side, ACTIVE_DURING::ActiveDuring active_during):state(0.0),state_buffer(BUFFER_SIZE),side(side),active_during(active_during){init();}
	Entity(SIDE::Side side, ACTIVE_DURING::ActiveDuring active_during, double state):state(state),state_buffer(BUFFER_SIZE),side(side),active_during(active_during){init();}
    void init(){
		no_tf=false;
	}
    virtual double get(){
        return tf(state_buffer());
    }
    virtual double get(int delay){
        return tf(state_buffer(delay));
    }
    virtual double tf(double x);
	virtual void set(double state ){this->state = state;}
	virtual void add(double state )=0;
    virtual void step(){state_buffer(state);};
	virtual void apply() = 0;
	virtual void apply(double o, double a) = 0;
	virtual ACTIVE_DURING::ActiveDuring activeDuring(){ return active_during;}
};

#endif

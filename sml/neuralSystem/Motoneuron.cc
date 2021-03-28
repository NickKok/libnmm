#include "Motoneuron.hh"
#include "Entity.hh"
#include <sml/sml-tools/EventManager.hh>
#include <sml/neuralSystem/Interneuron.hh>

#include <fstream>
#include <iostream>
using namespace std;
namespace nmm {
  extern SmlParameters parameters;
}
using namespace nmm;

Motoneuron::Motoneuron(SIDE::Side side, string name, double& muscle):
    Entity(side), 
    name(name), 
    muscle(muscle),
    input(0.0),
    tau(0.01),
    dt(((double)Settings::get<int>("time_step"))/1000.0){}
string Motoneuron::getName(){
    static string side_str;
    side_str = "left_";
    if (side == SIDE::RIGHT) side_str = "right_";
    return side_str+name;
}
void Motoneuron::apply(){
    static int mn_tf = Settings::get<int>("transfer_function_motoneurons");
    static double dstate;
	if(mn_tf == 1)
	{
		state = tf(input);
	}
	else{
        dstate = (input-state) * dt/tau;
        state += dstate;
	}
	
} // to change, add a differential equation
void Motoneuron::set(double value){this->state = value;}
void Motoneuron::add(double value){this->input += value;}
#include "Connection.hh"

#include <sml/neuralSystem/ReflexController.hh>

extern EventManager* eventManager;

using namespace std;


void FirstOrderConnection::step(){
    step(1.0);
}
void FirstOrderConnection::step(double alpha){
    static vector<double> in(1,0.0);
    static vector<double> y(1,0.0);
    in.at(0) = amplitude*weight*this->input->get()+offset;
    y.at(0) = value;
    
    y=rungeKutta4(eventManager->get<double>(STATES::TIME),dt,y,in,(Derivatives *)this);
    value = y[0];
    output->add(alpha*value);
}

vector<double> FirstOrderConnection::dydt(double x, vector<double> y, vector<double> param, vector<double> input){
    static vector<double> dadt(1,0.0);
    dadt.at(0) = 130*(input[0]-y[0]);
    return dadt;
}

double Connection::getWeight(){
    return weight;
}
void Connection::setWeight(double weight){
    this->weight=weight;
}
bool Connection::isGateOpen(ReflexController * body){
    return body->isGateOpen((Interneuron *)input);
}
void Connection::reset(){
    weight = 1.0;
    amplitude = 1.0;
    offset = 0.0;
}
void NoDelayConnection::step(){
    step(1.0);
}
void NoDelayConnection::step(double alpha){
    output->add(alpha*(amplitude*weight*input->get()+offset));
}
void DiscreteDelayConnection::step(){
    step(1.0);
}


void DiscreteDelayConnection::step(double alpha){
    output->add(alpha*(amplitude*weight*input->get(delay)+offset));
}
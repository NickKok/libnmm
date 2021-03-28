#include <sml/neuralSystem/CentralClock.hh>

CentralClock::CentralClock(double& f):frequency(f){

};
double CentralClock::getFrequency(){
	return frequency;
}
void CentralClock::setFrequency(double& f){
	frequency = f;
}

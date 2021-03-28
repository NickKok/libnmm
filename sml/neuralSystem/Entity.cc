#include "Entity.hh"
#include <sml/sml-tools/Helper.hh>
#include <sml/sml-tools/EventManager.hh>

extern EventManager* eventManager;
using namespace std;

double Entity::tf(double x){
    static double y;
    static int noise_entity = Settings::get<int>("noise_entity");
    static int transfer_function = Settings::get<int>("transfer_function");
    static int transfer_function_type = Settings::get<int>("transfer_function_type");
    
    /*
        If noise active only adds it at the last phase (because we can not be sure that the other phase have been optimized for noise)
    */
    if(noise_entity == 1 && eventManager->get<bool>(STATES::IS_LAST_PHASE))
    {
        x += getSignalDependentNoise(x);
    }
    
    if(transfer_function == 1){
    
        switch(transfer_function_type){
            case 1:
                if(x>1.0)
                    y = 1.0;
                else if(x<0.0)
                    y = 0.0;
                else
                    y = x;
                break;
            default:
                y = x;
                break;
        }
    }
    else{
        y=x;
    }

    
    
    return y;
}
  


double stateBuffer::operator()(double t){
        cb.push_back(t);
        return t;
}

double stateBuffer::operator()(int t){
        return cb[cb.size()-1-min(t,length-1)];
}

double stateBuffer::operator()(){
        return cb[cb.size()-1];
}

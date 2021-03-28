#include "OnlineCtrl.hh"
//#include <sml/body/Sml.hh>


bool OnlineCtrl::Add(std::string str, double value, double inc){
    ctrlsInc[str] = inc;
    ctrlsValue[str] = value;
    ctrlsDefault[str] = value;
    ctrlsStep[str] = 0;
    ctrlsMinValue[str] = -10000;
    ctrlsMaxValue[str] = 10000;
    if(actual == "")
        actual = str;
    return true;
}
bool OnlineCtrl::Add(std::string str, double value, double inc, double min){
    Add(str,value,inc);
    ctrlsMinValue[str] = min;
    return true;
}
bool OnlineCtrl::Add(std::string str, double value, double inc, double min, double max){
    Add(str,value,inc);
    ctrlsMinValue[str] = min;
    ctrlsMinValue[str] = max;
    return true;
}
bool OnlineCtrl::Inc(std::string str){
    ctrlsStep[str] += 1;
    return true;
}
bool OnlineCtrl::Dec(std::string str){
    ctrlsStep[str] -= 1;
    return true;
}
double OnlineCtrl::Get(std::string str){
    return ctrlsDefault[str] + (double)ctrlsStep[str]*ctrlsInc[str];
}

bool OnlineCtrl::Select(std::string str){
    actual = str;
    return true;
}
double OnlineCtrl::Get(){
    return Get(actual);
}
bool OnlineCtrl::Dec(){
    return Dec(actual);
}
bool OnlineCtrl::Inc(){
    return Inc(actual);
}

OnlineCtrl * onlineCtrl;
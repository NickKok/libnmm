#pragma once
#ifndef __ONLINE_CTRL_HH__
#define __ONLINE_CTRL_HH__

#include <map>
#include <string>
class Sml;

class OnlineCtrl{
private:
    Sml * body;
    std::string actual;
    std::map<std::string,double> ctrlsDefault;
    std::map<std::string,double> ctrlsValue;
    std::map<std::string,double> ctrlsMinValue;
    std::map<std::string,double> ctrlsMaxValue;
    std::map<std::string,double> ctrlsInc;
    std::map<std::string,int> ctrlsStep;
public:
    OnlineCtrl(Sml * body): body(body){}; 
    bool Add(std::string, double value, double inc);
    bool Add(std::string, double value, double inc, double min);
    bool Add(std::string, double value, double inc, double min, double max);
    
    double Get(std::string str);
    bool Dec(std::string str);
    bool Inc(std::string str);
    
    bool Select(std::string str);
    double Get();
    bool Dec();
    bool Inc();
};

#endif
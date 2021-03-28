#ifndef __Perturbator_HH__
#define __Perturbator_HH__

#include <sml/types/types.h>

class Perturbator {
private:
    double  start;
    double  duration;
    double  current_perturbation_value;
    double  current_perturbation_step;
    double  perturbation_start_time;
    double  perturbation_start_percentage;
    double  perturbation_duration;
    int     d_perturbation_number;
    bool    perturbation_on_going;
    int     perturbation_cycle_num;
    
    void    initVariables();
    void    checkPerturbationStart();
    void    updatePerturbationValue();
    double  getPerturbationValue();
    bool    perturbationOnGoing();
    bool    inExpectedPhase();
    double  getEstimatedPercentage();
    void    resetPerturbation();
    
    
    void    checkPerturbationEnd();
public:
    Perturbator(double,double);
    bool    doPerturbation();
    double  getPerturbation();
    void    updatePerturbation();
    double  getCurrentPerturbation();
};


class PerturbatorManager {
private:
    double start_time;
    double duration;
    std::map<JOINT::Joints, Perturbator*> perturbator;
public:
    PerturbatorManager();
    PerturbatorManager(double start, double duration);
    std::map<JOINT::Joints, Perturbator*> getMap();
    void addInSerie(JOINT::Joints joint);
    void addInParallel(JOINT::Joints joint);
    Perturbator * operator[](JOINT::Joints joint);
    bool find(JOINT::Joints joint);
    
    std::string getHeader();
    std::string getRow();
    
    
};
#endif

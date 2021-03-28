#include <string>
#include <math.h>
#include <vector>
#include <fstream>
#include <map>

#include "PerturbationManager.hh"
#include <sml/sml-tools/Settings.hh>
#include <sml/types/types.h>
//#include <sml/body/Sml.hh>

using namespace std;
extern EventManager* eventManager;

Perturbator::Perturbator(double start, double duration):
    start(start),
    duration(duration),
    current_perturbation_value(0.0),
    current_perturbation_step(0.0),
    perturbation_start_time(0.0),
    perturbation_start_percentage(0.0),
    perturbation_duration(0.0),
    d_perturbation_number(0),
    perturbation_on_going(false),
    perturbation_cycle_num(0)
    {
        if(Settings::get<string>("perturbation_phase") == "swing")
        {
            //eventManager->doIt_passByRef<Perturbator>(&(eventManager->onLeftFootTakeOff), &Perturbator::resetPerturbation, *this);
        }
        else if(Settings::get<string>("perturbation_phase") == "stance")
        {
            //eventManager->doIt_passByRef<Perturbator>(&(eventManager->onLeftFootTouchDown), &Perturbator::resetPerturbation, *this);
        }
        else
        {
            //eventManager->doIt_passByRef<Perturbator>(&(eventManager->onLeftFootTouchDown), &Perturbator::resetPerturbation, *this);
        }
        initVariables();
    }

double Perturbator::getEstimatedPercentage(){
    if(Settings::get<string>("perturbation_phase") == "swing")
    {
        if(eventManager->get<double>(STATES::MEAN_LEFT_SWING_DURATION) != 0)
            return eventManager->get<double>(STATES::LEFT_SWING_DURATION)/eventManager->get<double>(STATES::MEAN_LEFT_SWING_DURATION);
    }
    else if(Settings::get<string>("perturbation_phase") == "stance")
    {
        if(eventManager->get<double>(STATES::MEAN_LEFT_STANCE_DURATION) != 0)
            return eventManager->get<double>(STATES::LEFT_STANCE_DURATION)/eventManager->get<double>(STATES::MEAN_LEFT_STANCE_DURATION);
    }
    else
    {
        if(eventManager->get<double>(STATES::MEAN_LEFT_CYCLE_DURATION) != 0)
            return eventManager->get<double>(STATES::LEFT_CYCLE_DURATION)/eventManager->get<double>(STATES::MEAN_LEFT_CYCLE_DURATION);
    }
    return -1.0;
}
/**
 * Reset perturbation state
 *
 * This is done at the end of one perturbation or at the biginning of the phase on which the prerturbation acts (swings / stance)
 *
 */
void Perturbator::resetPerturbation(){
    if ( !doPerturbation() )
        return;
    cout << "PERTURBATION ENDED" << endl;
    perturbation_on_going = false;
    current_perturbation_value = 0.0;
    current_perturbation_step = 0.0;
    d_perturbation_number++;

    if(Settings::get<string>("perturbation_startingMode") == "gaitPercentage"){
        perturbation_start_percentage = (perturbation_start_percentage >= 100.0) ? 0.0 : perturbation_start_percentage + Settings::get<int>("perturbation_stepPercentageBetween");
    }
    else if(Settings::get<string>("perturbation_startingMode") == "randInt"){
        static int min_space = Settings::get<int>("perturbation_minTimeBetween");
        static int max_space = Settings::get<int>("perturbation_maxTimeBetween");
        perturbation_start_time = eventManager->get<double>(STATES::TIME) + rand() % (max_space-min_space)/1000.0 + min_space/1000.0;
    }

}
/**
 * Returns true if perturbation session not finished
 *
 */
bool Perturbator::doPerturbation(){
    bool perturb = true;
    if (eventManager->get<double>(STATES::TIME) < start )
    {
        perturb = false;
    }
    if (eventManager->get<double>(STATES::TIME) > start + duration )
    {
        perturb = false;
    }
    if (Settings::get<int>("perturbation_number") != -1 && d_perturbation_number > Settings::get<int>("perturbation_number"))
    {
        perturb = false;
    }
    return perturb;
}
void Perturbator::updatePerturbation(){

    if (!perturbationOnGoing())
    {
        checkPerturbationStart();
        if(perturbationOnGoing()){
            perturbation_cycle_num = eventManager->get<int>(STATES::CYCLE_COUNT);
            perturbation_start_time = eventManager->get<double>(STATES::TIME);
            if(Settings::get<string>("perturbation_startingMode") == "gaitPercentage"){
                cout << "START NEW gaitPercentage PERTURBATION" << endl;
                cout << "\t           percentage : " << perturbation_start_percentage << " %" << endl;
            }
            else if(Settings::get<string>("perturbation_startingMode") == "randInt"){
                cout << endl << endl;
                cout << "START NEW randInt PERTURBATION" << endl;
            }
            cout << "\t           starts  at : " << perturbation_start_time << " s" << endl;
            cout << "\t           end   at : " << perturbation_start_time+perturbation_duration << " s" << endl;
            initVariables();
        }
    }

    checkPerturbationEnd();

    if(perturbationOnGoing())
    {
        updatePerturbationValue();
    }
}
double Perturbator::getPerturbation(){
    if (doPerturbation() && perturbationOnGoing())
        return getPerturbationValue();

    return 0.0;
}
void Perturbator::checkPerturbationEnd(){
    if (!perturbationOnGoing() )
        return;
    if (!inExpectedPhase() || eventManager->get<double>(STATES::TIME) - perturbation_start_time > perturbation_duration){
        //cout << this << " " << d_perturbation_number << endl;
        resetPerturbation();
    }
}

bool Perturbator::perturbationOnGoing(){
    return perturbation_on_going;
}

bool Perturbator::inExpectedPhase(){
    // When to start perturbation
    if(Settings::get<string>("perturbation_phase") == "swing")
    {
        if(eventManager->right_foot->justGetOffGround())
            return false;
        if(eventManager->get<int>(STATES::LEFT_FOOT) == CYCLE_PHASE::SWING_START || eventManager->get<int>(STATES::LEFT_FOOT) == CYCLE_PHASE::SWING_END)
            return true;
    }
    else if(Settings::get<string>("perturbation_phase") == "stance")
    {
        if(eventManager->right_foot->justTouchTheGround())
            return false;
        if(eventManager->get<int>(STATES::LEFT_FOOT) != CYCLE_PHASE::SWING_START && eventManager->get<int>(STATES::LEFT_FOOT) != CYCLE_PHASE::SWING_END)
            return true;
    }
    else
    {
        return true;
    }
    return false;
}

void Perturbator::initVariables(){
    double perturbation_amplitude = Settings::get<double>("perturbation_amplitude");
    double perturbation_direction = 1.0;//2*(rand() % 2)-1;
    static string perturbation_type = Settings::get<string>("perturbation_type");
    if(perturbation_type == "linearDecreasingRamp"){
        perturbation_duration = Settings::get<double>("perturbation_duration");
        current_perturbation_value = perturbation_direction*perturbation_amplitude;
        current_perturbation_step = perturbation_direction*perturbation_amplitude/perturbation_duration/1000.0;
    }
    else if(perturbation_type == "arashTorquePert"){

    }
    else if(perturbation_type == "pseudoRandomBinarySequence"){
        perturbation_duration = (rand()%90+10)/1000.0;
        current_perturbation_value = perturbation_direction*perturbation_amplitude;
        current_perturbation_step = 0.0;
    }
    else if(perturbation_type == "step"){
        perturbation_duration = Settings::get<double>("perturbation_duration");
        current_perturbation_value = perturbation_amplitude;
        current_perturbation_step = 0.0;
    }
    else
    {
        cout << "perturbation scheme ( "  << Settings::get<string>("perturbation_type") << " ) not implemented" << endl;
    }
}
/**
 * Starts new perturbation
 *
 */
void Perturbator::checkPerturbationStart(){
    //if (round((int(eventManager->get<double>(STATES::TIME))*1000)%50) == 0)
    //    cout << perturbation_start_time << endl;

    if(Settings::get<string>("perturbation_startingMode") == "gaitPercentage"){
        if (
            inExpectedPhase() &&
            getEstimatedPercentage() >= perturbation_start_percentage/100.0 &&
            getEstimatedPercentage() <= 1.02*perturbation_start_percentage/100.0 &&
            eventManager->get<int>(STATES::CYCLE_COUNT) > perturbation_cycle_num+Settings::get<int>("perturbation_recovery_step") ){
            perturbation_on_going = true;
        }
    }
    else if(Settings::get<string>("perturbation_startingMode") == "randInt"){
        if ( inExpectedPhase() && eventManager->get<double>(STATES::TIME) >= perturbation_start_time-1.0/1000.0 ){
            perturbation_on_going = true;
        }
    }
}

double Perturbator::getPerturbationValue(){
    return current_perturbation_value;
}
void Perturbator::updatePerturbationValue(){
    current_perturbation_value -= current_perturbation_step;
}

PerturbatorManager::PerturbatorManager(): start_time(9.5), duration(40.0){

}
PerturbatorManager::PerturbatorManager(double start_time, double duration): start_time(start_time), duration(duration){

}

std::map<JOINT::Joints, Perturbator*> PerturbatorManager::getMap(){
    return perturbator;
}

void PerturbatorManager::addInSerie(JOINT::Joints joint){
    perturbator[joint] = new Perturbator(start_time,duration);
    start_time = start_time+duration;
}
void PerturbatorManager::addInParallel(JOINT::Joints joint){
    perturbator[joint] = new Perturbator(start_time,duration);
}
bool PerturbatorManager::find(JOINT::Joints joint){
    return perturbator.find(joint) != perturbator.end();
}
Perturbator * PerturbatorManager::operator[](JOINT::Joints joint){
    return perturbator[joint];
}

string PerturbatorManager::getHeader(){
    stringstream str;
    for(auto &kv : perturbator){

        str << JOINT::toString(kv.first) << " ";
    }
    return str.str();
}
string PerturbatorManager::getRow(){
    stringstream str;
    for(auto &kv : perturbator){
        if(Settings::get<int>("perturbation") == 1) {
        str << (kv.second)->getPerturbation() << " ";
        }
        else
        str << "0.0" << " ";
    }
    return str.str();
}

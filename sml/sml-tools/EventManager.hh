#pragma once
#ifndef __EventManager_HH__
#define __EventManager_HH__

#include <setting-manager/parameters.hh>
#include <sml/types/types.h>
#include <boost/signals2.hpp>
#include <boost/statechart/event.hpp>
#include <boost/variant.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <array>

class Foot;
class Muscle;
typedef boost::variant< double, int, bool > state_type;

class EventManager{
public:
    std::map<std::string, Muscle *>* muscles;
    const std::vector<double>& Constant;
    const std::vector<double>& Input;
    Foot * left_foot;
  	Foot * right_foot;
    Parameters dynamicState;
    double dt_eventManager;
    typedef boost::signals2::signal<void()> EVENT;
    typedef const EVENT::extended_slot_type& EVENT_SLOT;

    EventManager(
        std::vector<double>& Constant,
        std::vector<double>& Input,
        Foot *left_foot,
        Foot *right_foot) :
          muscles(NULL),
          Constant(Constant),
          Input(Input),
          left_foot(left_foot),
          right_foot(right_foot)
    {
        init();
    };
    void doIt(
        EVENT* event,
        void (EventManager::*what)());
    void doIt(
        EVENT* event,
        void (*what)());
    template<class T> void doIt(
        EVENT* event,
        void (T::*what)(), T& t);
    template<class T> void doIt_passByRef(
        EVENT* event,
        void (T::*what)(), T& t);
    void init();
    void trigger();
    void initState();
    void resetState();
    void printEvents();
    template<class T> void onlineUpdateMeanVariance(T current, T mean, T var, int n);
    template<class T> void onlineCalcMeanVariance(T current, T& mean, T& var, int n);
    void updateOnLeftTouchDown();
    void updateOnRightTouchDown();
    void updateOnLeftTakeOff();
    void updateOnRightTakeOff();
    void updateOnLeftStance();
    void updateOnLeftStanceEnd();
    void updateOnRightStance();
    void updateOnRightStanceEnd();
    void updateOnLeftSwing();
    void updateOnRightSwing();


    void updateEnergy();
    void updatePerTimeStep();
    void updateContinuationCriterion();

    void updateMinRange(STATES::States s, double value);
    void updateMaxRange(STATES::States s, double value);
    void updateStabilization();
    void updateState();
    void updateFeet();
    void updateFeetState();
    void updateFeetDistanceToCM();
    void updateFootDistanceToCM(SIDE::Side side);


    double getAngle(JOINT::Joints j);
    boost::numeric::ublas::vector<double> getSegment(SEGMENTS::Segments s);



    bool isRunFinished();

    template<class T> T get(STATES::States state);
    template<class T> const T* getP(STATES::States state);
    template<class T> void add(STATES::States state, T in);
    template<class T> void set(STATES::States state, T in);
    template<class T> void onlineUpdateMeanVariance(T & current, T & mean, T & var, int n);
    void checkEndCriterion();
    void printAll();
    void step();

    std::map<std::string, double> getMap();


    std::array<state_type, STATES::NUMBER> states;

	EVENT onSimulationEnd;
    EVENT onLeftFootTouchDown;
    EVENT onLeftFootTakeOff;
    EVENT onRightFootTouchDown;
    EVENT onRightFootTakeOff;

    EVENT onLeftFootStanceEnd;
    EVENT onLeftFootSwing;
    EVENT onLeftFootStance;
    EVENT onRightFootStanceEnd;
    EVENT onRightFootSwing;
    EVENT onRightFootStance;



    void wrapper(const boost::signals2::connection &conn, int SIG_TYPE, boost::function<void()> fun);
private:
};


template<class T> T EventManager::get(STATES::States state)
{
    return (boost::get<T>(states[state]));
}

template<class T> const T* EventManager::getP(STATES::States state)
{
    return &(boost::get<T>(states[state]));
}

template<class T> void EventManager::set(STATES::States state, T in)
{
    states[state] = in;
}
template<class T> void EventManager::add(STATES::States state, T in)
{
    states[state] = boost::get<T>(states[state]) + in;
}

template<class T> void EventManager::onlineUpdateMeanVariance(T current, T mean, T var, int n){
    double delta = get<double>(current) - get<double>(mean);
    states[mean] = get<double>(mean) + delta/n;
    if(n > 2)
        states[var] = (n-2.0)/(n-1.0)*get<double>(var) + delta*(get<double>(current) - get<double>(mean))/(n-1.0);
    else
        states[var] = get<double>(var) + delta*(get<double>(current) - get<double>(mean));
}

template<class T> void EventManager::onlineCalcMeanVariance(T current, T& mean, T& var, int n){
    double delta = current - mean;
    mean = mean + delta/n;
    if(n > 2)
        var = (n-2.0)/(n-1.0)*var + delta*(current - mean)/(n-1.0);
    else
        var = var + delta*(current - mean);
}

template<class T> void EventManager::doIt(
        EVENT* event,
        void (T::*what)(), T& t)
{
    //event->connect_extended(boost::bind(what,boost::ref(t)));
    event->connect_extended(boost::bind(what,t));

}
template<class T> void EventManager::doIt_passByRef(
        EVENT* event,
        void (T::*what)(), T& t)
{
    event->connect_extended(boost::bind(what,boost::ref(t)));
    //event->connect_extended(boost::bind(what,t));

}
#endif

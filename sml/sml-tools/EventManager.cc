
#include "EventManager.hh"
#include <boost/bind/protect.hpp>
#include <iostream>
#include <sml/types/types.h>
using namespace STATES;
void empty(std::string name){
#ifndef REAL_TIME
    std::cout << "[EventManager] Event : " << name << " triggered !" << std::endl;
#endif
}

void EventManager::wrapper(const boost::signals2::connection &conn, int SIG_TYPE, boost::function<void()> fun){
    if(SIG_TYPE == EVENT_TYPE::UNIQUE)
        conn.disconnect();
    fun();
}

void EventManager::doIt(
        EVENT* event,
        void (EventManager::*what)())
{
    event->connect_extended(boost::bind(what,this));

}

void EventManager::doIt(
        EVENT* event,
        void (*what)())
{
    event->connect_extended(boost::bind(what));

}

void EventManager::trigger(){
    if( right_foot->justTouchTheGround() )             this->onRightFootTouchDown();
    if( left_foot->justTouchTheGround() )              this->onLeftFootTouchDown();
    if( right_foot->justGetOffGround() )               this->onRightFootTakeOff();
    if( left_foot->justGetOffGround() )                this->onLeftFootTakeOff();

	if(!get<bool>(STAY_IN_LOOP))                            this->onSimulationEnd();

    if ( get<int>(LEFT_FOOT) == CYCLE_PHASE::STANCE_END )   this->onLeftFootStanceEnd();
    if ( get<int>(RIGHT_FOOT) == CYCLE_PHASE::STANCE_END )  this->onRightFootStanceEnd();

    if ( get<int>(LEFT_FOOT) == CYCLE_PHASE::SWING_START  || get<int>(LEFT_FOOT) == CYCLE_PHASE::SWING_END )    this->onLeftFootSwing();
    if ( get<int>(LEFT_FOOT) == CYCLE_PHASE::STANCE_START || get<int>(LEFT_FOOT) == CYCLE_PHASE::STANCE_END )   this->onLeftFootStance();
    if ( get<int>(RIGHT_FOOT) == CYCLE_PHASE::SWING_START || get<int>(RIGHT_FOOT) == CYCLE_PHASE::SWING_END )   this->onRightFootSwing();
    if ( get<int>(RIGHT_FOOT) == CYCLE_PHASE::STANCE_START || get<int>(RIGHT_FOOT) == CYCLE_PHASE::STANCE_END ) this->onRightFootStance();
}

void EventManager::updateOnLeftStance(){
    add<double>(LEFT_STANCE_DURATION,dt_eventManager);
}
void EventManager::updateOnLeftStanceEnd(){
    add<double>(LEFT_STANCE_END_DURATION,dt_eventManager);
}
void EventManager::updateOnRightStance(){
    add<double>(RIGHT_STANCE_DURATION,dt_eventManager);
}
void EventManager::updateOnRightStanceEnd(){
    add<double>(RIGHT_STANCE_END_DURATION,dt_eventManager);
}
void EventManager::updateOnLeftSwing(){
    add<double>(LEFT_SWING_DURATION,dt_eventManager);
}
void EventManager::updateOnRightSwing(){
    add<double>(RIGHT_SWING_DURATION,dt_eventManager);
}


void EventManager::init(){
    static int time_step_eventManager = Settings::get<int>("time_step_eventManager");
    static int time_step = Settings::get<int>("time_step");
    dt_eventManager = (double)(time_step_eventManager*time_step)/1000.0;
    // Set initial foot states

    initState();
    //printEvents();
    this->doIt(&onRightFootTouchDown,&EventManager::updateOnRightTouchDown);
    this->doIt(&onLeftFootTouchDown,&EventManager::updateOnLeftTouchDown);
    this->doIt(&onRightFootTakeOff,&EventManager::updateOnRightTakeOff);
    this->doIt(&onLeftFootTakeOff,&EventManager::updateOnLeftTakeOff);

    this->doIt(&onLeftFootStance,&EventManager::updateOnLeftStance);
    this->doIt(&onLeftFootStanceEnd,&EventManager::updateOnLeftStanceEnd);
    this->doIt(&onLeftFootSwing,&EventManager::updateOnLeftSwing);
    this->doIt(&onRightFootStance,&EventManager::updateOnRightStance);
    this->doIt(&onRightFootStanceEnd,&EventManager::updateOnRightStanceEnd);
    this->doIt(&onRightFootSwing,&EventManager::updateOnRightSwing);

}
void EventManager::updateOnRightTakeOff(){
    states[RIGHT_SWING_DURATION] = 0.0;
}
void EventManager::updateOnLeftTakeOff(){
    states[LEFT_SWING_DURATION] = 0.0;
}
void EventManager::updateOnRightTouchDown(){
    static double old_foot_position = 0.0;
    static double foot_position;
    static int speed_step_switch = Settings::get<int>("speed_step_switch");
    static int cc;


    add<int>(CYCLE_COUNT,1);
    if ( get<int>(CYCLE_COUNT) < 1 ) return;

    foot_position = get<double>(DISTANCE) + get<double>(RIGHT_X_DIST_NECK);
    set<double>(RIGHT_STEP_LENGTH, std::abs(foot_position - old_foot_position));
    old_foot_position = foot_position;

    set<double>(RIGHT_GOLDEN_RATIO_0, get<double>(RIGHT_CYCLE_DURATION) /  get<double>(RIGHT_STANCE_DURATION) );
    set<double>(RIGHT_GOLDEN_RATIO_1, get<double>(RIGHT_STANCE_DURATION) /  get<double>(RIGHT_SWING_DURATION) );
    set<double>(RIGHT_GOLDEN_RATIO_2, get<double>(RIGHT_SWING_DURATION) /  (get<double>(RIGHT_STANCE_END_DURATION)+get<double>(RIGHT_STANCE_END_DURATION)));

    if(!get<bool>(RIGHT_UP_EFX_PARAMETERS) && get<int>(CYCLE_COUNT) >= speed_step_switch)
        set<bool>(RIGHT_UP_EFX_PARAMETERS,true);



    if(get<bool>(STEADY_STATE)){
        cc = get<int>(CYCLE_COUNT)-get<int>(STEADY_STATE_REACHEDAT)-1;
        onlineUpdateMeanVariance(RIGHT_STEP_LENGTH, MEAN_RIGHT_STEP_LENGTH, VAR_RIGHT_STEP_LENGTH, cc);
        onlineUpdateMeanVariance(CYCLE_LENGTH, MEAN_CYCLE_LENGTH, VAR_CYCLE_LENGTH,cc);
        onlineUpdateMeanVariance(CYCLE_LENGTH, MEAN_CYCLE_LENGTH, VAR_CYCLE_LENGTH,cc);
        onlineUpdateMeanVariance(RIGHT_GOLDEN_RATIO_0,MEAN_RIGHT_GOLDEN_RATIO_0,VAR_RIGHT_GOLDEN_RATIO_0, cc);
        onlineUpdateMeanVariance(RIGHT_GOLDEN_RATIO_1,MEAN_RIGHT_GOLDEN_RATIO_1,VAR_RIGHT_GOLDEN_RATIO_1, cc);
        onlineUpdateMeanVariance(RIGHT_GOLDEN_RATIO_2,MEAN_RIGHT_GOLDEN_RATIO_2,VAR_RIGHT_GOLDEN_RATIO_2, cc);
        onlineUpdateMeanVariance(RIGHT_CYCLE_DURATION, MEAN_RIGHT_CYCLE_DURATION, VAR_RIGHT_CYCLE_DURATION,cc);
        onlineUpdateMeanVariance(RIGHT_STANCE_DURATION, MEAN_RIGHT_STANCE_DURATION, VAR_RIGHT_STANCE_DURATION,cc);
        onlineUpdateMeanVariance(RIGHT_STANCE_END_DURATION, MEAN_RIGHT_STANCE_END_DURATION, VAR_RIGHT_STANCE_END_DURATION,cc);
        onlineUpdateMeanVariance(RIGHT_SWING_DURATION, MEAN_RIGHT_SWING_DURATION, VAR_RIGHT_SWING_DURATION,cc);
        onlineUpdateMeanVariance(IN_VELOCITY_FILTERED, MEAN_VELOCITY, VAR_VELOCITY,cc);
        onlineUpdateMeanVariance(IN_ACCELERATION_FILTERED, MEAN_ACCELERATION, VAR_ACCELERATION,cc);
    }


    if(get<bool>(STEADY_STATE_WAITFOR))
        updateStabilization();
    /**
     *
     * Restart cycle variables
     *
     */
    states[CYCLE_LENGTH] = 0.0;
    states[RIGHT_CYCLE_DURATION] = 0.0;
    states[RIGHT_STANCE_DURATION] = 0.0;
    states[LEFT_STANCE_END_DURATION] = 0.0;

    states[TIME_CYCLE] = 0.0;
    states[MEAN_VELOCITY_CYCLE_OLD] = states[MEAN_VELOCITY_CYCLE];
    states[VAR_VELOCITY_CYCLE_OLD] = states[VAR_VELOCITY_CYCLE];
    states[MEAN_VELOCITY_CYCLE] = states[IN_VELOCITY_FILTERED];
    states[VAR_VELOCITY_CYCLE] = 0.0;

    //if ( get<bool>(STEADY_STATE_WAITFOR) ){
    //
    //}
}
void EventManager::updateOnLeftTouchDown(){

    /**
     *
     * Update heading position
     *
     This is done by comparing the position and last touchdown with the current position
     The heading is defined as a unit vector that points in the direction of the movement
     */

     // Test, heading always toward front
    set<double>(HEADING_X,0.0);
    set<double>(HEADING_Y,1.0);

    /**
     *
     * Update Golden rations and step length
     *
     */
    static double foot_position;
    static double old_foot_position = 0.0;
    static int cc;

    if ( get<int>(CYCLE_COUNT) < 1 ) return;

    foot_position = get<double>(DISTANCE) + get<double>(RIGHT_X_DIST_NECK);
    set<double>(LEFT_STEP_LENGTH, std::abs(foot_position - old_foot_position));
    old_foot_position = foot_position;

    set<double>(LEFT_GOLDEN_RATIO_0, get<double>(LEFT_CYCLE_DURATION) /  get<double>(LEFT_STANCE_DURATION) );
    set<double>(LEFT_GOLDEN_RATIO_1, get<double>(LEFT_STANCE_DURATION) /  get<double>(LEFT_SWING_DURATION) );
    set<double>(LEFT_GOLDEN_RATIO_2, get<double>(LEFT_SWING_DURATION) /  (
        get<double>(LEFT_STANCE_END_DURATION) + get<double>(RIGHT_STANCE_END_DURATION))
    );

    //if(!get<bool>(LEFT_UP_EFX_PARAMETERS) && get<int>(CYCLE_COUNT) >= Settings::get<int>("speed_step_switch"))
    //    set<bool>(LEFT_UP_EFX_PARAMETERS,true);//TODO

    set<double>(CURRENT_CYCLE_DURATION, get<double>(LEFT_CYCLE_DURATION));

    if(get<bool>(STEADY_STATE)){
        cc = get<int>(CYCLE_COUNT)-get<int>(STEADY_STATE_REACHEDAT);
        onlineUpdateMeanVariance(LEFT_GOLDEN_RATIO_0,MEAN_LEFT_GOLDEN_RATIO_0,VAR_LEFT_GOLDEN_RATIO_0, cc);
        onlineUpdateMeanVariance(LEFT_GOLDEN_RATIO_1,MEAN_LEFT_GOLDEN_RATIO_1,VAR_LEFT_GOLDEN_RATIO_1, cc);
        onlineUpdateMeanVariance(LEFT_GOLDEN_RATIO_2,MEAN_LEFT_GOLDEN_RATIO_2,VAR_LEFT_GOLDEN_RATIO_2, cc);

        onlineUpdateMeanVariance(LEFT_STEP_LENGTH, MEAN_LEFT_STEP_LENGTH, VAR_LEFT_STEP_LENGTH, cc);
        onlineUpdateMeanVariance(LEFT_CYCLE_DURATION, MEAN_LEFT_CYCLE_DURATION, VAR_LEFT_CYCLE_DURATION, cc);
        onlineUpdateMeanVariance(LEFT_STANCE_DURATION, MEAN_LEFT_STANCE_DURATION, VAR_LEFT_STANCE_DURATION, cc);
        onlineUpdateMeanVariance(LEFT_STANCE_END_DURATION, MEAN_LEFT_STANCE_END_DURATION, VAR_LEFT_STANCE_END_DURATION, cc);
        onlineUpdateMeanVariance(LEFT_SWING_DURATION, MEAN_LEFT_SWING_DURATION, VAR_LEFT_SWING_DURATION, cc);


    }

    onlineUpdateMeanVariance(LEFT_HIP_MIN, MEAN_LEFT_HIP_MIN, VAR_LEFT_HIP_MIN, get<int>(CYCLE_COUNT));
    onlineUpdateMeanVariance(RIGHT_HIP_MIN, MEAN_RIGHT_HIP_MIN, VAR_RIGHT_HIP_MIN, get<int>(CYCLE_COUNT));
    onlineUpdateMeanVariance(LEFT_HIPCOR_MIN, MEAN_LEFT_HIPCOR_MIN, VAR_LEFT_HIPCOR_MIN, get<int>(CYCLE_COUNT));
    onlineUpdateMeanVariance(RIGHT_HIPCOR_MIN, MEAN_RIGHT_HIPCOR_MIN, VAR_RIGHT_HIPCOR_MIN, get<int>(CYCLE_COUNT));
    onlineUpdateMeanVariance(LEFT_KNEE_MIN, MEAN_LEFT_KNEE_MIN, VAR_LEFT_KNEE_MIN, get<int>(CYCLE_COUNT));
    onlineUpdateMeanVariance(RIGHT_KNEE_MIN, MEAN_RIGHT_KNEE_MIN, VAR_RIGHT_KNEE_MIN, get<int>(CYCLE_COUNT));
    onlineUpdateMeanVariance(LEFT_ANKLE_MIN, MEAN_LEFT_ANKLE_MIN, VAR_LEFT_ANKLE_MIN, get<int>(CYCLE_COUNT));
    onlineUpdateMeanVariance(RIGHT_ANKLE_MIN, MEAN_RIGHT_ANKLE_MIN, VAR_RIGHT_ANKLE_MIN, get<int>(CYCLE_COUNT));

    onlineUpdateMeanVariance(LEFT_HIP_MAX, MEAN_LEFT_HIP_MAX, VAR_LEFT_HIP_MAX, get<int>(CYCLE_COUNT));
    onlineUpdateMeanVariance(RIGHT_HIP_MAX, MEAN_RIGHT_HIP_MAX, VAR_RIGHT_HIP_MAX, get<int>(CYCLE_COUNT));
    onlineUpdateMeanVariance(LEFT_HIPCOR_MAX, MEAN_LEFT_HIPCOR_MAX, VAR_LEFT_HIPCOR_MAX, get<int>(CYCLE_COUNT));
    onlineUpdateMeanVariance(RIGHT_HIPCOR_MAX, MEAN_RIGHT_HIPCOR_MAX, VAR_RIGHT_HIPCOR_MAX, get<int>(CYCLE_COUNT));

    onlineUpdateMeanVariance(LEFT_KNEE_MAX, MEAN_LEFT_KNEE_MAX, VAR_LEFT_KNEE_MAX, get<int>(CYCLE_COUNT));
    onlineUpdateMeanVariance(RIGHT_KNEE_MAX, MEAN_RIGHT_KNEE_MAX, VAR_RIGHT_KNEE_MAX, get<int>(CYCLE_COUNT));
    onlineUpdateMeanVariance(LEFT_ANKLE_MAX, MEAN_LEFT_ANKLE_MAX, VAR_LEFT_ANKLE_MAX, get<int>(CYCLE_COUNT));
    onlineUpdateMeanVariance(RIGHT_ANKLE_MAX, MEAN_RIGHT_ANKLE_MAX, VAR_RIGHT_ANKLE_MAX, get<int>(CYCLE_COUNT));

    /**
     *
     * Restart cycle variables
     *
     */
    states[RIGHT_STANCE_END_DURATION] = 0.0;
    states[LEFT_STANCE_DURATION] = 0.0;
    states[LEFT_CYCLE_DURATION] = 0.0;


}



void EventManager::updateState(){
    updateFeet();
    updateEnergy();
    updatePerTimeStep();
    checkEndCriterion();
}
bool EventManager::isRunFinished(){
    static int neverstop = Settings::get<int>("neverstop");
    if ( neverstop ){
        return false;
    }
    else{
        return !get<bool>(STATES::STAY_IN_LOOP);
    }
}
void EventManager::checkEndCriterion(){
    using namespace STOP_REASONS;

    if(
        get<int>(CYCLE_COUNT) > 5 &&
        get<double>(MEAN_VELOCITY_CYCLE) < 0.1
    ){
        set<bool>(STAY_IN_LOOP, false);
        set<int>(STOP_REASON, TO_SLOW);
    }
    // -----------------   will we get out of the loop ? --------------------

    if(
        get<bool>(FALLED)
    ){
        set<bool>(STAY_IN_LOOP, false);
        set<int>(STOP_REASON, FALL);
    }

    /*
     * Reason :
     *  1 : falled
     *  2 : height (max 3.0)
     *  3 : vertical distance from foot to cm (min 0.6)
     *  4 : stabilized confidence level to low (min -8.0)
     *  5 : too slow (min 0.1 m/s)
     *  0 : other
     */

    /*
    if(state.stabilized_confidence < -8.0){
        set<bool>(STAY_IN_LOOP, false);
        set<int>(STOP_REASON, UNSTABLE);
    }
    */
    static int libcoman = Settings::get<int>("libcoman");
    static int backward = Settings::get<int>("backward");
    static int optimization_scheme = Settings::get<int>("optimization_scheme");
    static double distance_max = Settings::get<double>("distance_max");
    static double duration_max = Settings::get<double>("duration_max");
    static double energylimit = Settings::get<double>("energylimit");


    if (libcoman == 0){
        if (
            //get<double>(LEFT_Y_DIST_NECK) < 0.25*Constant[CONSTANT::MODEL_HEIGHT]
            Input[INPUT::MIDHIP_Y]-Input[INPUT::TOE_LEFT_Y] < 0.25*Constant[CONSTANT::MODEL_HEIGHT]
        ){
            //std::cout << Input[INPUT::MIDHIP_Y]-Input[INPUT::TOE_LEFT_Y] << std::endl;
            //std::cout << get<double>(LEFT_Y_DIST_NECK) << std::endl;
            set<bool>(STAY_IN_LOOP, false);
            set<int>(STOP_REASON, TRUNK_LEANING_LIMIT);
            set<bool>(FALLED,true);
        }
    }
    else{
        if(Input[INPUT::THETA_TRUNK] > 1.2){
            set<int>(STOP_REASON, TRUNK_LEANING_LIMIT);
            set<bool>(FALLED,true);
        }
        if(Input[INPUT::THETA_TRUNK] < -1.2){
            set<int>(STOP_REASON, TRUNK_LEANING_LIMIT);
            set<bool>(FALLED,true);
        }
    }

    if(
        backward==1 &&
        get<double>(DISTANCE) > 1.0
    ){
        set<bool>(STAY_IN_LOOP, false);
        set<int>(STOP_REASON, WRONG_SIDE);
    }


    if(
        backward==-1 &&
        get<double>(DISTANCE) < -1.0
    ){
        set<bool>(STAY_IN_LOOP, false);
        set<int>(STOP_REASON, WRONG_SIDE);
    }


    switch(optimization_scheme){
        case 0:
            if(-backward*get<double>(DISTANCE) > distance_max){
                set<bool>(STAY_IN_LOOP, false);
                set<int>(STOP_REASON, OTHER);
            }
            if(get<double>(TIME) > duration_max){
                set<bool>(STAY_IN_LOOP, false);
                set<int>(STOP_REASON, OTHER);
            }
            break;
        case 1:
            if(get<double>(ENERGY) > energylimit){
                set<bool>(STAY_IN_LOOP, false);
                set<int>(STOP_REASON, OTHER);
            }
            break;
        case 2:
            if(get<int>(CYCLE_COUNT) > 80){
                set<bool>(STAY_IN_LOOP, false);
                set<int>(STOP_REASON, OTHER);
            }
            break;
        default:
            set<bool>(STAY_IN_LOOP, true);
            break;
    }
}
void EventManager::printAll(){
    for(int i=0; i< STATES::NUMBER; i++){
        std::cout << STATES::toString(i) << ":" << states[i] << std::endl;
    }
    // for(auto&kv : states){
    //     std::cout << kv << std::endl;
    // }
}

void EventManager::step(){
    static int n = Settings::get<int>("time_step_eventManager");;
    static int N = 1;
    if(N % n == 0){
        N = 1;
        updateState();
    }
    else{
        N++;
    }
    trigger();
}


double max(double a, double b){
    if ( a < b)
        return b;
    return a;
}

double min(double a, double b){
    if ( a > b)
        return b;
    return a;
}

std::map<std::string, double> EventManager::getMap(){
    std::map<std::string, double> fitnesses;
    fitnesses["distance"] = get<double>(DISTANCE);
    fitnesses["distance_lateral"] = get<double>(DISTANCE_LATERAL);
    fitnesses["distance_frontal"] = get<double>(DISTANCE_FRONTAL);
    fitnesses["duration"] = get<double>(TIME);
    fitnesses["energy"] = get<double>(ENERGY);
    fitnesses["energy_overextension"] = get<double>(ENERGY_OVEREXTENSION);
    fitnesses["fall"] = (double)(get<bool>(FALLED));
    fitnesses["cycle"] = (double)(get<int>(CYCLE_COUNT)-get<int>(RESET_AT));
    fitnesses["speed"] = get<double>(MEAN_VELOCITY);
    //fitnesses["angular_knee_velocity_change"] = nb_cycle != 0 ? (double)nb_knee_sign_changed/nb_cycle : 0;

    fitnesses["steadyState_reachedAt"] = (double)(get<int>(STEADY_STATE_REACHEDAT));
    fitnesses["duration"] = get<double>(TIME);
    fitnesses["muscle_activity_noise"] = get<double>(NOISE_MUSCLE);
    fitnesses["sensors_activity_noise"] = get<double>(NOISE_SENSORS);
    fitnesses["interneurons_activity_noise"] = get<double>(NOISE_INTERNEURONS);
    fitnesses["last_acceleration"] = get<double>(IN_ACCELERATION_FILTERED);
    fitnesses["last_speed"] =  get<double>(IN_VELOCITY_FILTERED);

    fitnesses["stop_reason"] =  (double)(get<int>(STOP_REASON));
    fitnesses["doublestance_duration"] = 0.5*(
        get<double>(MEAN_LEFT_STANCE_END_DURATION)
        +
        get<double>(MEAN_RIGHT_STANCE_END_DURATION));

    fitnesses["left_golden_ratio0"] = (
        get<double>(MEAN_LEFT_GOLDEN_RATIO_0));

    fitnesses["left_golden_ratio1"] = (
        get<double>(MEAN_LEFT_GOLDEN_RATIO_1));

    fitnesses["left_golden_ratio2"] = (
        get<double>(MEAN_LEFT_GOLDEN_RATIO_2));

    fitnesses["right_golden_ratio0"] = (
        get<double>(MEAN_RIGHT_GOLDEN_RATIO_0));

    fitnesses["right_golden_ratio1"] = (
        get<double>(MEAN_RIGHT_GOLDEN_RATIO_1));

    fitnesses["right_golden_ratio2"] = (
        get<double>(MEAN_RIGHT_GOLDEN_RATIO_2));

    fitnesses["doublestance_duration"] = get<double>(MEAN_DOUBLESTANCE_DURATION);


    fitnesses["hip_min"] = min(get<double>(MEAN_LEFT_HIP_MIN),get<double>(MEAN_RIGHT_HIP_MIN));
    fitnesses["hipcor_min"] = min(get<double>(MEAN_LEFT_HIPCOR_MIN),get<double>(MEAN_RIGHT_HIPCOR_MIN));
    fitnesses["knee_min"] = min(get<double>(MEAN_LEFT_KNEE_MIN),get<double>(MEAN_RIGHT_KNEE_MIN));
    fitnesses["ankle_min"] = min(get<double>(MEAN_LEFT_ANKLE_MIN),get<double>(MEAN_RIGHT_ANKLE_MIN));

    fitnesses["hip_max"] = max(get<double>(MEAN_LEFT_HIP_MAX),get<double>(MEAN_RIGHT_HIP_MAX));
    fitnesses["hipcor_max"] = max(get<double>(MEAN_LEFT_HIPCOR_MAX),get<double>(MEAN_RIGHT_HIPCOR_MAX));
    fitnesses["knee_max"] = max(get<double>(MEAN_LEFT_KNEE_MAX),get<double>(MEAN_RIGHT_KNEE_MAX));
    fitnesses["ankle_max"] = max(get<double>(MEAN_LEFT_ANKLE_MAX),get<double>(MEAN_RIGHT_ANKLE_MAX));

    fitnesses["hip_min_right"] = get<double>(MEAN_RIGHT_HIP_MIN);
    fitnesses["knee_min_right"] = get<double>(MEAN_RIGHT_KNEE_MIN);
    fitnesses["ankle_min_right"] = get<double>(MEAN_RIGHT_ANKLE_MIN);

    fitnesses["hip_max_right"] = get<double>(MEAN_RIGHT_HIP_MAX);
    fitnesses["knee_max_right"] = get<double>(MEAN_RIGHT_KNEE_MAX);
    fitnesses["ankle_max_right"] = get<double>(MEAN_RIGHT_ANKLE_MAX);

    fitnesses["hip_min_left"] = get<double>(MEAN_LEFT_HIP_MIN);
    fitnesses["knee_min_left"] = get<double>(MEAN_LEFT_KNEE_MIN);
    fitnesses["ankle_min_left"] = get<double>(MEAN_LEFT_ANKLE_MIN);

    fitnesses["hip_max_left"] = get<double>(MEAN_LEFT_HIP_MAX);
    fitnesses["knee_max_left"] = get<double>(MEAN_LEFT_KNEE_MAX);
    fitnesses["ankle_max_left"] = get<double>(MEAN_LEFT_ANKLE_MAX);

    //fitnesses["left_right_correlation"] = this->getJointsSimilarities();



    // Penalty for impossible solution (explosion of the robot model)
    if(fitnesses["distance"]/fitnesses["duration"] > 10.0){
        fitnesses["distance"] = 0.1;
    }
    return fitnesses;

}

void EventManager::updateEnergy(){
	static int muscle_step_number = Settings::get<int>("muscle_step_number");
    static double lower_angle_limit;
    static double penalty;
    states[ENERGY_DT] = 0.0;
    states[ENERGY_OE_DT] = 0.0;
    if(muscles != NULL){
      for(auto&  kv : *muscles){
          Muscle * muscle = kv.second;
          states[ENERGY_DT] = dt_eventManager*1000*muscle_step_number*muscle->d_energy;
          add<double>(ENERGY_W,dt_eventManager*1000*muscle_step_number*muscle->d_energyW);
          add<double>(ENERGY_M,dt_eventManager*1000*muscle_step_number*muscle->d_energyM);
          add<double>(ENERGY,dt_eventManager*1000*muscle_step_number*muscle->d_energy);
      }
    }
    lower_angle_limit = 0; //if knee extend more than 179°
    penalty = 50000.0; // [J/(rad * s)]

    if(Input[INPUT::ANGLE_KNEE_RIGHT] < lower_angle_limit){
        add<double>(ENERGY_OE_DT, penalty * dt_eventManager * std::abs(Input[INPUT::ANGLE_KNEE_RIGHT] - lower_angle_limit));
        add<double>(ENERGY_OVEREXTENSION,penalty * dt_eventManager * std::abs(Input[INPUT::ANGLE_KNEE_RIGHT] - lower_angle_limit));
        //using namespace std;
        //cout<<"knee right hyperextension: " << Input[INPUT::ANGLE_KNEE_RIGHT]*180/PI << endl;
    }

    //cout << Input[INPUT::ANGLE_KNEE_LEFT] << "\t" << lower_angle_limit << endl;
    if(Input[INPUT::ANGLE_KNEE_LEFT]  < lower_angle_limit){
        add<double>(ENERGY_OE_DT, penalty * dt_eventManager * std::abs(Input[INPUT::ANGLE_KNEE_LEFT] - lower_angle_limit));
        add<double>(ENERGY_OVEREXTENSION,penalty * dt_eventManager * std::abs(Input[INPUT::ANGLE_KNEE_LEFT] - lower_angle_limit));
        //cout<<"knee left hyperextension"<< Input[INPUT::ANGLE_KNEE_LEFT]*180/PI << endl;
    }
}

void EventManager::updateStabilization(){
    static int assume_stabilized_after = Settings::get<int>("assume_stabilized_after");

    static double v;
    static double v_old;
    static double stdV;
    static double stdV_old;


    v = get<double>(MEAN_VELOCITY_CYCLE) ;
    v_old = get<double>(MEAN_VELOCITY_CYCLE_OLD) ;
    stdV = sqrt(get<double>(VAR_VELOCITY_CYCLE)) ;
    stdV_old = sqrt(get<double>(VAR_VELOCITY_CYCLE_OLD)) ;


    static int waited = 0;
    static int wait_limit = 3;
    static double m1;
    static double m2;
    //using namespace std;
#ifndef REAL_TIME
    //cout << 100*std::abs(v-v_old)/v << " ";
#endif
#ifndef REAL_TIME
    //cout << 100*stdV << " " << m1 << " " << m2 << endl;
#endif
    if(waited==wait_limit){
        m1 = 100*stdV;
        m2 = 5.0/100.0;
        onlineCalcMeanVariance(100*stdV, m1, m2,  1);
    }
    if(waited>wait_limit){
        onlineCalcMeanVariance(100*stdV, m1, m2,  waited-wait_limit+1);
    }
    // If :
    // -- changes in velocity smaller than 1 percent
    // -- and

    if( assume_stabilized_after  != 0)
    {
        if(get<int>(CYCLE_COUNT) - 1 > assume_stabilized_after)
        {
            states[STEADY_STATE_REACHEDAT] = assume_stabilized_after;
            states[STEADY_STATE_WAITFOR] = false;
            states[STEADY_STATE] = true;
#ifndef REAL_TIME
            std::cout << "[EventManager] you stabilized after " << get<int>(CYCLE_COUNT) - 1 << std::endl;
#endif
            waited=0;
        }
    }
    else if(
        (100*std::abs(v-v_old)/v < 1.0 && 100*stdV < 3.0)
        ||
        (100*m2 < 3.0 && waited>wait_limit) // second level of steady stateness (if the first one is not reached in 3 steps)

    )
    {
        states[STEADY_STATE_REACHEDAT] = get<int>(CYCLE_COUNT) - 1;
        states[STEADY_STATE_WAITFOR] = false;
        states[STEADY_STATE] = true;
#ifndef REAL_TIME
        std::cout << "[EventManager] you stabilized after " << get<int>(CYCLE_COUNT) - 1 << std::endl;
#endif
        waited=0;
    }
    waited++;

    /*TODO
    double changesPercentage = 100*std::abs(v-v_old)/v;
    double threshold = 1/100.0;
    double stabilized_threshold = 3.0;
#ifndef REAL_TIME
    cout << "[EventManager] Confidence level: " << state.stabilized_confidence << endl;
#endif
#ifndef REAL_TIME
    cout << "[EventManager] changesPercentage: " << changesPercentage << endl;
#endif
    if ( !state.steady_state ){
        // maybe try to implement better smooth threshold :TODO:

        if ( changesPercentage < threshold ){
            state.stabilized_confidence+=1.0;
            if(state.steady_after_step == -1)
                state.steady_after_step = state.nb_cycle;
        }
        else if (changesPercentage < 5*threshold){
            state.stabilized_confidence+=1.0-2*changesPercentage/(5*threshold);
        }
        else{
            state.steady_after_step = -1;
            state.stabilized_confidence-=1.0;
        }
        if (state.stabilized_confidence > stabilized_threshold){
            state.stabilized_confidence = 0.0;
#ifndef REAL_TIME
            cout << "[EventManager] You stabilized after " << state.nb_cycle-state.steady_after_step << " cycle(s)" <<  endl;
#endif
            state.steady_state = true;
            state.steady_after_step = -1;
        }
    }
    */

}

void EventManager::updateMinRange(STATES::States s, double value){
    if (get<double>(s) > value)
        set<double>(s, value);
}
void EventManager::updateMaxRange(STATES::States s, double value){
    if (get<double>(s) < value){
        set<double>(s, value);
    }
}

void EventManager::updatePerTimeStep(){
    states[THETA_TRUNK] = Input[INPUT::THETA_TRUNK];
    states[THETACOR_TRUNK] = Input[INPUT::THETACOR_TRUNK];


    states[OLD_POSITION_X] =  states[POSITION_X];
    states[POSITION_X] =  Input[INPUT::NECK_X];
    states[OLD_POSITION_Y] =  states[POSITION_Y];
    states[POSITION_Y] =  Input[INPUT::NECK_Y];
    states[OLD_POSITION_Z] =  states[POSITION_Z];
    states[POSITION_Z] =  Input[INPUT::NECK_Z];

    add<double>(RIGHT_CYCLE_DURATION,dt_eventManager);
    add<double>(LEFT_CYCLE_DURATION,dt_eventManager);

	static double dz;
    static double dy;
    static double dx;
    static double dxz;
    static int cycle_steps;

    dz = get<double>(POSITION_Z)-get<double>(OLD_POSITION_Z);
    dy = get<double>(POSITION_Y)-get<double>(OLD_POSITION_Y);
    dx = get<double>(POSITION_X)-get<double>(OLD_POSITION_X);

    dxz = sqrt(dx*dx+dz*dz);

    double dPHx = get<double>(HEADING_Y);  // X Direction perpendicular to heading direction
    double dPHy = -get<double>(HEADING_X); // Y                  "

    double dCx = dz;
    double dCy = dx;

    double Cx = get<double>(POSITION_Z);
    double Cy = get<double>(POSITION_X);

    double lateralSpeed = (dCx*dPHx + dCy*dPHy)*10;
    set<double>(LATERALSPEED_TRUNK,lateralSpeed);
    add<double>(CYCLE_LENGTH,dx);
    add<double>(HEIGHT,dy);
    add<double>(DISTANCE,dxz);
    add<double>(DISTANCE_LATERAL,dz);
    add<double>(DISTANCE_FRONTAL,dx);
    add<double>(TIME,dt_eventManager);
    add<double>(TIME_CYCLE,dt_eventManager);
    states[OLD_IN_VELOCITY] = states[IN_VELOCITY];
    states[IN_VELOCITY] = sqrt(dx/dt_eventManager*dx/dt_eventManager+dy/dt_eventManager*dy/dt_eventManager);

    states[IN_ACCELERATION] = 1.0/dt_eventManager*(get<double>(IN_VELOCITY)-get<double>(OLD_IN_VELOCITY));

    add<double>(IN_VELOCITY_FILTERED,0.0025 * (get<double>(IN_VELOCITY)- get<double>(IN_VELOCITY_FILTERED)));
    add<double>(IN_ACCELERATION_FILTERED,0.0025 * (get<double>(IN_ACCELERATION)- get<double>(IN_ACCELERATION_FILTERED)));



    cycle_steps = (int)(get<double>(TIME_CYCLE)/dt_eventManager);


    onlineUpdateMeanVariance(IN_VELOCITY_FILTERED, MEAN_VELOCITY_CYCLE, VAR_VELOCITY_CYCLE,cycle_steps);
        //std::cout << get<int>(CYCLE_COUNT) << std::endl;

    int dts = (int)(get<double>(TIME)/dt_eventManager);
    onlineUpdateMeanVariance(THETA_TRUNK, MEAN_THETA_TRUNK, VAR_THETA_TRUNK,dts);
    onlineUpdateMeanVariance(THETACOR_TRUNK, MEAN_THETACOR_TRUNK, VAR_THETACOR_TRUNK,dts);
    //std::cout <<  get<double>(VAR_THETACOR_TRUNK) << std::endl;
    //std::cout << get<double>(RIGHT_HIPCOR_MIN) << " " << Input[INPUT::ANGLE_HIPCOR_RIGHT] << std::endl;
    updateMinRange(LEFT_HIPCOR_MIN,Input[INPUT::ANGLE_HIPCOR_LEFT]);
    updateMinRange(RIGHT_HIPCOR_MIN,Input[INPUT::ANGLE_HIPCOR_RIGHT]);
    //updateMinRange(LEFT_KNEE_MIN,Input[INPUT::ANGLE_KNEE_LEFT]);
    //updateMinRange(RIGHT_KNEE_MIN,Input[INPUT::ANGLE_KNEE_RIGHT]);
    //updateMinRange(LEFT_ANKLE_MIN,Input[INPUT::ANGLE_ANKLE_LEFT]);
    //updateMinRange(RIGHT_ANKLE_MIN,Input[INPUT::ANGLE_ANKLE_RIGHT]);

    updateMaxRange(LEFT_HIPCOR_MAX,Input[INPUT::ANGLE_HIPCOR_LEFT]);
    updateMaxRange(RIGHT_HIPCOR_MAX,Input[INPUT::ANGLE_HIPCOR_RIGHT]);
    //updateMaxRange(LEFT_KNEE_MAX,Input[INPUT::ANGLE_KNEE_LEFT]);
    //updateMaxRange(RIGHT_KNEE_MAX,Input[INPUT::ANGLE_KNEE_RIGHT]);
    //updateMaxRange(LEFT_ANKLE_MAX,Input[INPUT::ANGLE_ANKLE_LEFT]);
    //updateMaxRange(RIGHT_ANKLE_MAX,Input[INPUT::ANGLE_ANKLE_RIGHT]);
}


void EventManager::resetState(){
	states[TIME_AT_RESET] = states[TIME];
    states[HEIGHT] = 0.0;
    states[TIME_CYCLE] = 0.0;
    states[ENERGY_DT] = 0.0;
    states[ENERGY_OE_DT] = 0.0;
    states[ENERGY] = 0.0;
    states[ENERGY_W] = 0.0;
    states[ENERGY_M] = 0.0;
    states[ENERGY_OVEREXTENSION] = 0.0;

    states[STEADY_STATE_REACHEDAT] = -1;
    states[STEADY_STATE_WAITFOR] = true;
    states[STEADY_STATE] = false;
    states[FALLED] = false;
    states[STAY_IN_LOOP] = true;
    //states[IS_LAUNCHING_PHASE] = true;
    states[IS_LAUNCHING_PHASE] = false;
    states[IS_LAST_PHASE] = false;
    states[LEFT_UP_EFX_PARAMETERS] = false;
    states[RIGHT_UP_EFX_PARAMETERS] = false;
    states[SELF_CONTACT_COUNT] = 0;

    states[THETACOR_TRUNK] = 0.0;
    states[THETA_TRUNK] = 0.0;
    states[MEAN_THETACOR_TRUNK] = 0.0;
    states[MEAN_THETA_TRUNK] = 0.0;
    states[VAR_THETACOR_TRUNK] = 0.0;
    states[VAR_THETA_TRUNK] = 0.0;

    states[STOP_REASON] = -1;

    states[LEFT_STEP_LENGTH] = 0.0;
    states[RIGHT_STEP_LENGTH] = 0.0;
    states[MEAN_LEFT_STEP_LENGTH] = 0.0;
    states[MEAN_RIGHT_STEP_LENGTH] = 0.0;
    states[VAR_LEFT_STEP_LENGTH] = 0.0;
    states[VAR_RIGHT_STEP_LENGTH] = 0.0;
    states[OLD_POSITION_X] = 0.0;
    states[OLD_POSITION_Z] = 0.0;
    states[POSITION_Y] = 0.0;
    states[OLD_POSITION_Y] = 0.0;

    states[CURRENT_CYCLE_DURATION] = 1.0;
    states[CURRENT_LEFT_CYCLE_DURATION] = 0.0;
    states[CYCLE_COUNT] = 0;
    states[RESET_AT] = states[CYCLE_COUNT];
    states[CYCLE_LENGTH] = 0.0;
    states[LEFT_CYCLE_DURATION] = 0.0;
    states[LEFT_STANCE_DURATION] = 0.0;
    states[LEFT_STANCE_END_DURATION] = 0.0;
    states[LEFT_SWING_DURATION] = 0.0;
    states[RIGHT_CYCLE_DURATION] = 0.0;
    states[RIGHT_STANCE_DURATION] = 0.0;
    states[RIGHT_STANCE_END_DURATION] = 0.0;
    states[RIGHT_SWING_DURATION] = 0.0;

    states[MEAN_CYCLE_LENGTH] = 0.0;
    states[MEAN_LEFT_CYCLE_DURATION] = 0.0;
    states[MEAN_LEFT_STANCE_DURATION] = 0.0;
    states[MEAN_LEFT_STANCE_END_DURATION] = 0.0;
    states[MEAN_LEFT_SWING_DURATION] = 0.0;
    states[MEAN_RIGHT_CYCLE_DURATION] = 0.0;
    states[MEAN_RIGHT_STANCE_DURATION] = 0.0;
    states[MEAN_RIGHT_STANCE_END_DURATION] = 0.0;
    states[MEAN_RIGHT_SWING_DURATION] = 0.0;

    states[VAR_CYCLE_LENGTH] = 0.0;
    states[VAR_LEFT_CYCLE_DURATION] = 0.0;
    states[VAR_LEFT_STANCE_DURATION] = 0.0;
    states[VAR_LEFT_STANCE_END_DURATION] = 0.0;
    states[VAR_LEFT_SWING_DURATION] = 0.0;
    states[VAR_RIGHT_CYCLE_DURATION] = 0.0;
    states[VAR_RIGHT_STANCE_DURATION] = 0.0;
    states[VAR_RIGHT_STANCE_END_DURATION] = 0.0;
    states[VAR_RIGHT_SWING_DURATION] = 0.0;

    states[LEFT_GOLDEN_RATIO_0] = 0.0;
    states[LEFT_GOLDEN_RATIO_1] = 0.0;
    states[LEFT_GOLDEN_RATIO_2] = 0.0;
    states[RIGHT_GOLDEN_RATIO_0] = 0.0;
    states[RIGHT_GOLDEN_RATIO_1] = 0.0;
    states[RIGHT_GOLDEN_RATIO_2] = 0.0;
    states[MEAN_LEFT_GOLDEN_RATIO_0] = 0.0;
    states[MEAN_LEFT_GOLDEN_RATIO_1] = 0.0;
    states[MEAN_LEFT_GOLDEN_RATIO_2] = 0.0;
    states[MEAN_RIGHT_GOLDEN_RATIO_0] = 0.0;
    states[MEAN_RIGHT_GOLDEN_RATIO_1] = 0.0;
    states[MEAN_RIGHT_GOLDEN_RATIO_2] = 0.0;
    states[VAR_LEFT_GOLDEN_RATIO_0] = 0.0;
    states[VAR_LEFT_GOLDEN_RATIO_1] = 0.0;
    states[VAR_LEFT_GOLDEN_RATIO_2] = 0.0;
    states[VAR_RIGHT_GOLDEN_RATIO_0] = 0.0;
    states[VAR_RIGHT_GOLDEN_RATIO_1] = 0.0;
    states[VAR_RIGHT_GOLDEN_RATIO_2] = 0.0;

    states[IN_VELOCITY] = 0.0;
    states[OLD_IN_VELOCITY] = 0.0;
    states[IN_ACCELERATION] = 0.0;
    states[IN_VELOCITY_FILTERED] = 0.0;
    states[IN_ACCELERATION_FILTERED] = 0.0;
    states[MEAN_VELOCITY] = 0.0;
    states[MEAN_ACCELERATION] = 0.0;
    states[VAR_VELOCITY] = 0.0;
    states[VAR_ACCELERATION] = 0.0;
    states[MEAN_VELOCITY_CYCLE] = 0.0;
    states[MEAN_VELOCITY_CYCLE_OLD] = 0.0;
    states[DISTANCE] = 0.0;
    states[DISTANCE_LATERAL] = 0.0;
    states[DISTANCE_FRONTAL] = 0.0;

    states[LEFT_HIP_MIN] = 6.28;
    states[LEFT_HIPCOR_MIN] = 6.28;
    states[RIGHT_HIP_MIN] = 6.28;
    states[RIGHT_HIPCOR_MIN] = 6.28;
    states[LEFT_KNEE_MIN] = 6.28;
    states[RIGHT_KNEE_MIN] = 6.28;
    states[LEFT_ANKLE_MIN] = 6.28;
    states[RIGHT_ANKLE_MIN] = 6.28;

    states[LEFT_HIP_MAX] = -6.28;
    states[LEFT_HIPCOR_MAX] = -6.28;
    states[RIGHT_HIP_MAX] = -6.28;
    states[RIGHT_HIPCOR_MAX] = -6.28;
    states[LEFT_KNEE_MAX] = -6.28;
    states[RIGHT_KNEE_MAX] = -6.28;
    states[LEFT_ANKLE_MAX] = -6.28;
    states[RIGHT_ANKLE_MAX] = -6.28;

    states[VAR_LEFT_HIP_MIN] = 0.0;
    states[VAR_LEFT_HIPCOR_MIN] = 0.0;
    states[VAR_RIGHT_HIP_MIN] = 0.0;
    states[VAR_RIGHT_HIPCOR_MIN] = 0.0;
    states[VAR_LEFT_KNEE_MIN] = 0.0;
    states[VAR_RIGHT_KNEE_MIN] = 0.0;
    states[VAR_LEFT_ANKLE_MIN] = 0.0;
    states[VAR_RIGHT_ANKLE_MIN] = 0.0;

    states[VAR_LEFT_HIP_MAX] = 0.0;
    states[VAR_LEFT_HIPCOR_MAX] = 0.0;
    states[VAR_RIGHT_HIP_MAX] = 0.0;
    states[VAR_RIGHT_HIPCOR_MAX] = 0.0;
    states[VAR_LEFT_KNEE_MAX] = 0.0;
    states[VAR_RIGHT_KNEE_MAX] = 0.0;
    states[VAR_LEFT_ANKLE_MAX] = 0.0;
    states[VAR_RIGHT_ANKLE_MAX] = 0.0;

    states[MEAN_LEFT_HIP_MIN] = 0.0;
    states[MEAN_LEFT_HIPCOR_MIN] = 0.0;
    states[MEAN_RIGHT_HIP_MIN] = 0.0;
    states[MEAN_RIGHT_HIPCOR_MIN] = 0.0;
    states[MEAN_LEFT_KNEE_MIN] = 0.0;
    states[MEAN_RIGHT_KNEE_MIN] = 0.0;
    states[MEAN_LEFT_ANKLE_MIN] = 0.0;
    states[MEAN_RIGHT_ANKLE_MIN] = 0.0;

    states[MEAN_LEFT_HIP_MAX] = 0.0;
    states[MEAN_LEFT_HIPCOR_MAX] = 0.0;
    states[MEAN_RIGHT_HIP_MAX] = 0.0;
    states[MEAN_RIGHT_HIPCOR_MAX] = 0.0;
    states[MEAN_LEFT_KNEE_MAX] = 0.0;
    states[MEAN_RIGHT_KNEE_MAX] = 0.0;
    states[MEAN_LEFT_ANKLE_MAX] = 0.0;
    states[MEAN_RIGHT_ANKLE_MAX] = 0.0;

    states[HEADING_X] = 0.0;
    states[HEADING_Y] = 1.0;
    states[LATERALSPEED_TRUNK] = 0.0;
    states[PREDICTION_ERROR] = 0.0;
}
void EventManager::initState(){
    states[CYCLE_COUNT] = -1;
    resetState();
    states[POSITION_X] = 0.0;
    states[POSITION_Z] = 0.0;
    states[TIME] = 0.0;




    states[LEFT_FOOT] = CYCLE_PHASE::STANCE_START;
    states[RIGHT_FOOT] = CYCLE_PHASE::STANCE_START;
    states[FINISHING_STANCE] = SIDE::LEFT;

}
void EventManager::printEvents(){
    /*
    onLeftFootTouchDown.connect_extended(boost::bind(
            empty,
            "Take off right"
        ));
    onLeftFootTakeOff.connect_extended(
        boost::bind(
            empty,
            "Take off left"
        )
    );
    onRightFootStance.connect_extended(
        boost::bind(
            empty,
            "Stance right"
        )
    );
    onRightFootStanceEnd.connect_extended(
        boost::bind(
            empty,
            "Stance end"
        )
    );
    onRightFootSwing.connect_extended(
        boost::bind(
            empty,
            "Swing right"
        )
    );*/
    onRightFootStance.connect_extended(
        boost::bind(
            empty,
            "Stance right"
        )
    );
    onLeftFootStance.connect_extended(
        boost::bind(
            empty,
            "Stance left"
        )
    );
}
EventManager * eventManager;

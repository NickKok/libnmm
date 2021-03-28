#ifndef __AIRI_TYPES_STATES_H__
#define __AIRI_TYPES_STATES_H__

namespace STATES{
    typedef enum
    {
        //Variable constantly changing (current state)
        LEFT_FOOT, // Left foot states CYCLE_PHASE [STANCE_START, STANCE_END, SWING_START, SWING_END]
        RIGHT_FOOT,
        FINISHING_STANCE,
        HEADING_X,
        HEADING_Y,
        CURRENT_CYCLE_DURATION,
        LATERALSPEED_TRUNK,
        PREDICTION_ERROR,
        FALLED,
        NB_FALL, //TODO
        STAY_IN_LOOP,
        STEADY_STATE_WAITFOR,
        STEADY_STATE_REACHEDAT,
        STEADY_STATE,
        IS_LAUNCHING_PHASE,
        IS_LAST_PHASE,
        LEFT_UP_EFX_PARAMETERS,
        RIGHT_UP_EFX_PARAMETERS,
        SELF_CONTACT_COUNT,
        //STEADY_STATE_CONFIDENCE, //TODO
        THETA_TRUNK,
        THETACOR_TRUNK,
        MEAN_THETACOR_TRUNK,
        MEAN_THETA_TRUNK,
        VAR_THETACOR_TRUNK,
        VAR_THETA_TRUNK,

        LEFT_HIP_MAX,
        LEFT_HIP_MIN,
        LEFT_HIPCOR_MAX,
        LEFT_HIPCOR_MIN,
        RIGHT_HIP_MAX,
        RIGHT_HIP_MIN,
        RIGHT_HIPCOR_MAX,
        RIGHT_HIPCOR_MIN,

        LEFT_KNEE_MAX,
        LEFT_KNEE_MIN,
        RIGHT_KNEE_MAX,
        RIGHT_KNEE_MIN,

        LEFT_ANKLE_MAX,
        LEFT_ANKLE_MIN,
        RIGHT_ANKLE_MAX,
        RIGHT_ANKLE_MIN,

        VAR_LEFT_HIP_MIN,
        VAR_RIGHT_HIP_MIN,
        VAR_LEFT_HIPCOR_MIN,
        VAR_RIGHT_HIPCOR_MIN,
        VAR_LEFT_KNEE_MIN,
        VAR_RIGHT_KNEE_MIN,
        VAR_LEFT_ANKLE_MIN,
        VAR_RIGHT_ANKLE_MIN,

        VAR_LEFT_HIP_MAX,
        VAR_RIGHT_HIP_MAX,
        VAR_LEFT_HIPCOR_MAX,
        VAR_RIGHT_HIPCOR_MAX,
        VAR_LEFT_KNEE_MAX,
        VAR_RIGHT_KNEE_MAX,
        VAR_LEFT_ANKLE_MAX,
        VAR_RIGHT_ANKLE_MAX,

        MEAN_LEFT_HIP_MIN,
        MEAN_RIGHT_HIP_MIN,
        MEAN_LEFT_HIPCOR_MIN,
        MEAN_RIGHT_HIPCOR_MIN,
        MEAN_LEFT_KNEE_MIN,
        MEAN_RIGHT_KNEE_MIN,
        MEAN_LEFT_ANKLE_MIN,
        MEAN_RIGHT_ANKLE_MIN,

        MEAN_LEFT_HIP_MAX,
        MEAN_RIGHT_HIP_MAX,
        MEAN_LEFT_HIPCOR_MAX,
        MEAN_RIGHT_HIPCOR_MAX,
        MEAN_LEFT_KNEE_MAX,
        MEAN_RIGHT_KNEE_MAX,
        MEAN_LEFT_ANKLE_MAX,
        MEAN_RIGHT_ANKLE_MAX,


        STOP_REASON,
        CURRENT_LEFT_CYCLE_DURATION,
        CYCLE_COUNT,
        RESET_AT,
        CYCLE_LENGTH,

        COV_LEFT_RIGHT_HIP_ANGLE, //TODO
        COV_LEFT_RIGHT_KNEE_ANGLE, //TODO
        COV_LEFT_RIGHT_ANKLE_ANGLE, //TODO
        COV_LEFT_RIGHT_ANGLE, //TODO
        //Global variables (accumulated state)
        NOISE_MUSCLE, //TODO
        NOISE_SENSORS, //TODO
        NOISE_INTERNEURONS, //TODO
        LEFT_X_DIST_NECK,
        RIGHT_X_DIST_NECK,
        LEFT_Y_DIST_NECK,
        RIGHT_Y_DIST_NECK,
        LEFT_Z_DIST_NECK,
        RIGHT_Z_DIST_NECK,
        LEFT_STEP_LENGTH,
        RIGHT_STEP_LENGTH,
        MEAN_LEFT_STEP_LENGTH,
        MEAN_RIGHT_STEP_LENGTH,
        VAR_LEFT_STEP_LENGTH,
        VAR_RIGHT_STEP_LENGTH,
        LEFT_CYCLE_DURATION,
        LEFT_STANCE_DURATION,
        LEFT_STANCE_END_DURATION,
        LEFT_SWING_DURATION,
        RIGHT_CYCLE_DURATION,
        RIGHT_STANCE_DURATION,
        RIGHT_STANCE_END_DURATION,
        RIGHT_SWING_DURATION,
        LEFT_GOLDEN_RATIO_0, // Cycle / Stance
        LEFT_GOLDEN_RATIO_1, // Stance / Swing
        LEFT_GOLDEN_RATIO_2, // Swing / Stance_end
        RIGHT_GOLDEN_RATIO_0, // Cycle / Stance
        RIGHT_GOLDEN_RATIO_1, // Stance / Swing
        RIGHT_GOLDEN_RATIO_2, // Swing / Stance_end
        MEAN_LEFT_GOLDEN_RATIO_0, // Cycle / Stance
        MEAN_LEFT_GOLDEN_RATIO_1, // Stance / Swing
        MEAN_LEFT_GOLDEN_RATIO_2, // Swing / Stance_end
        MEAN_RIGHT_GOLDEN_RATIO_0, // Cycle / Stance
        MEAN_RIGHT_GOLDEN_RATIO_1, // Stance / Swing
        MEAN_RIGHT_GOLDEN_RATIO_2, // Swing / Stance_end
        VAR_LEFT_GOLDEN_RATIO_0, // Cycle / Stance
        VAR_LEFT_GOLDEN_RATIO_1, // Stance / Swing
        VAR_LEFT_GOLDEN_RATIO_2, // Swing / Stance_end
        VAR_RIGHT_GOLDEN_RATIO_0, // Cycle / Stance
        VAR_RIGHT_GOLDEN_RATIO_1, // Stance / Swing
        VAR_RIGHT_GOLDEN_RATIO_2, // Swing / Stance_end
        POSITION_X,
        OLD_POSITION_X,
        POSITION_Y,
        OLD_POSITION_Y,
        POSITION_Z,
        OLD_POSITION_Z,
        IN_VELOCITY, // Instantanoeus velocity
        OLD_IN_VELOCITY, // Instantanoeus velocity
        IN_ACCELERATION,
        IN_VELOCITY_FILTERED,
        IN_ACCELERATION_FILTERED,
        MEAN_VELOCITY,
        VAR_VELOCITY,
        MEAN_ACCELERATION,
        VAR_ACCELERATION,
        MEAN_VELOCITY_CYCLE,
        VAR_VELOCITY_CYCLE,
        MEAN_VELOCITY_CYCLE_OLD,
        VAR_VELOCITY_CYCLE_OLD,
        HEIGHT,
        DISTANCE,
        DISTANCE_LATERAL,
        DISTANCE_FRONTAL,
        TIME,
        TIME_AT_RESET,
        TIME_CYCLE,
        ENERGY_DT,
        ENERGY_OE_DT,
        ENERGY,
        ENERGY_W,
        ENERGY_M,
        ENERGY_OVEREXTENSION,
        //Cycle related variables (averaged state)
        MEAN_CYCLE_LENGTH,
        VAR_CYCLE_LENGTH,
        MEAN_DOUBLESTANCE_DURATION, //TODO
        VAR_DOUBLESTANCE_DURATION, //TODO
        MEAN_LEFT_CYCLE_DURATION,
        MEAN_LEFT_STANCE_DURATION,
        MEAN_LEFT_STANCE_END_DURATION,
        MEAN_LEFT_SWING_DURATION,
        MEAN_RIGHT_CYCLE_DURATION,
        MEAN_RIGHT_STANCE_DURATION,
        MEAN_RIGHT_STANCE_END_DURATION,
        MEAN_RIGHT_SWING_DURATION,
        VAR_LEFT_CYCLE_DURATION,
        VAR_LEFT_STANCE_DURATION,
        VAR_LEFT_STANCE_END_DURATION,
        VAR_LEFT_SWING_DURATION,
        VAR_RIGHT_CYCLE_DURATION,
        VAR_RIGHT_STANCE_DURATION,
        VAR_RIGHT_STANCE_END_DURATION,
        VAR_RIGHT_SWING_DURATION,

        FIRST = LEFT_FOOT,
        LAST = VAR_RIGHT_SWING_DURATION
    } States;
    const int NUMBER = STATES::LAST - STATES::FIRST + 1;
    int toID(std::string str);
    std::string toString(int id);

}

#endif

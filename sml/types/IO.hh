#ifndef __AIRI_TYPES_IO_H__
#define __AIRI_TYPES_IO_H__

namespace INPUT{
    typedef enum
    {
        ANGLE_HIP_LEFT,
        ANGLE_HIPCOR_LEFT,
        ANGLE_KNEE_LEFT,
        ANGLE_ANKLE_LEFT,
        ANGLE_HIP_RIGHT,
        ANGLE_HIPCOR_RIGHT,
        ANGLE_KNEE_RIGHT,
        ANGLE_ANKLE_RIGHT,
        NECK_Z,
        NECK_Y,
        NECK_X,
        THETA_TRUNK,
        THETACOR_TRUNK,
        TOE_LEFT_X,
        TOE_LEFT_Y,
        TOE_LEFT_Z,
        TOE_RIGHT_X,
        TOE_RIGHT_Y,
        TOE_RIGHT_Z,
        HEEL_LEFT_X,
        HEEL_LEFT_Y,
        HEEL_LEFT_Z,
        HEEL_RIGHT_X,
        HEEL_RIGHT_Y,
        HEEL_RIGHT_Z,
        MIDHIP_X,
        MIDHIP_Y,
        MIDHIP_Z,
        SENSOR_HEEL_LEFT,
        SENSOR_HEEL_RIGHT,
        SENSOR_TOE_LEFT,
        SENSOR_TOE_RIGHT,
        //LEFT_FOOT_ORIENTATION,
        //RIGHT_FOOT_ORIENTATION,
        FIRST = ANGLE_HIP_LEFT,
        LAST = SENSOR_TOE_RIGHT
    } Inputs;
    const int NUMBER = INPUT::LAST - INPUT::FIRST + 1;
    const unsigned FIRST_JOINT = ANGLE_HIP_LEFT;
    const unsigned LAST_JOINT = ANGLE_ANKLE_RIGHT;
    const unsigned FIRST_FORCE_SENSOR = SENSOR_HEEL_LEFT;
    const unsigned LAST_FORCE_SENSOR = SENSOR_TOE_RIGHT;
    std::string toString(int id);
    int toID(std::string str);
}


namespace OUTPUT{
    typedef enum
    {
        TORQUE_HIP_LEFT,
        TORQUE_HIPCOR_LEFT,
        TORQUE_KNEE_LEFT,
        TORQUE_ANKLE_LEFT,
        TORQUE_HIP_RIGHT,
        TORQUE_HIPCOR_RIGHT,
        TORQUE_KNEE_RIGHT,
        TORQUE_ANKLE_RIGHT,
        FIRST = TORQUE_HIP_LEFT,
        LAST = TORQUE_ANKLE_RIGHT
    } Outputs;
    const int NUMBER = OUTPUT::LAST - OUTPUT::FIRST + 1;
    const unsigned FIRST_JOINT = TORQUE_HIP_LEFT;
    const unsigned LAST_JOINT = TORQUE_ANKLE_RIGHT;
    std::string toString(int id);
    int toID(std::string str);
}

#endif

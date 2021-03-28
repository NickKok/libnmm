#ifndef __AIRI_TYPES_GENERAL_H__
#define __AIRI_TYPES_GENERAL_H__

#include <string>

namespace EVENT_TYPE{
    typedef enum
    {
        UNIQUE      = 1,
        LOOP        = -1,
    } EventType;
}


namespace CYCLE_PHASE{
    typedef enum
    {
        WHOLE_CYCLE     = -1,
        STANCE          = 1,
        STANCE_START    = 11,
        STANCE_END      = 12,
        SWING           = 2,
        SWING_START     = 21,
        SWING_END       = 23,
        ANGLE_OFFSET    = 3,
    } CyclePhase;
    std::string toString(int id);
}

namespace ACTIVE_DURING{
    typedef enum
    {
        WHOLE_CYCLE     = -1,
        STANCE          = 1,
        STANCE_START    = 11,
        STANCE_END      = 12,
        SWING           = 2,
        SWING_START     = 21,
        SWING_END       = 23,
        ANGLE_OFFSET    = 3,
    } ActiveDuring;
    std::string toString(int id);
    int toID(std::string);
    bool isActive(
            ACTIVE_DURING::ActiveDuring active_on,
            CYCLE_PHASE::CyclePhase current_limb_state
            );
}

namespace SIDE{
    typedef enum
    {
        LEFT        = 0,
        RIGHT       = 1,
        ALL         = 2,
        UNKNOWN     = -1,
    } Side;
    std::string toString(int id);
    int toID(std::string);
}
namespace STOP_REASONS{
    typedef enum
    {
        FALL,
        TRUNK_LEANING_LIMIT,
        EXPLOD,
        UNSTABLE,
        TO_SLOW,
        WRONG_SIDE,
        OTHER

    } StopReasons;
    std::string toString(int id);
}

namespace JOINT{
    typedef enum
    {
        HIP_LEFT,
        HIPCOR_LEFT,
        KNEE_LEFT,
        ANKLE_LEFT,
        HIP_RIGHT,
        HIPCOR_RIGHT,
        KNEE_RIGHT,
        ANKLE_RIGHT,
        FIRST = HIP_LEFT,
        LAST = ANKLE_RIGHT
    } Joints;
    const unsigned FIRST_JOINT = HIP_LEFT;
    const unsigned LAST_JOINT = ANKLE_RIGHT;
    std::string toString(int id);
    int toID(std::string s);
    const int NUMBER = JOINT::LAST - JOINT::FIRST + 1;
}


namespace SEGMENTS{
    const int NUMBER = 9;
    typedef enum
    {
        TRUNK   = 0,
        THIGH   = 1,
        SHIN    = 2,
        ANKLE   = 3,
        FOOT    = 4,
    } Segments;
}


void print_debug(std::string t);
void print_verbose(std::string t);

#endif

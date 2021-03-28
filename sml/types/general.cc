#include <sml/types/general.hh>
#include <string>
#include <iostream>
using namespace std;
namespace JOINT{
    string toString(int id){
        switch((Joints) (id)){
            case HIP_LEFT: return "HIP_LEFT"; break;
            case HIPCOR_LEFT: return "HIPCOR_LEFT"; break;
            case KNEE_LEFT: return "KNEE_LEFT"; break;
            case ANKLE_LEFT: return "ANKLE_LEFT"; break;
            case HIP_RIGHT: return "HIP_RIGHT"; break;
            case HIPCOR_RIGHT: return "HIPCOR_RIGHT"; break;
            case KNEE_RIGHT: return "KNEE_RIGHT"; break;
            case ANKLE_RIGHT: return "ANKLE_RIGHT"; break;
        }
        return "joint not found";;
    }
    int toID(string s){
        if( s=="HIP_LEFT"     ||    s=="hip_left") return JOINT::HIP_LEFT;
        if( s=="HIPCOR_LEFT"  ||    s=="hipcor_left") return JOINT::HIPCOR_LEFT;
        if( s=="KNEE_LEFT"    ||    s=="knee_left") return JOINT::KNEE_LEFT;
        if( s=="ANKLE_LEFT"   ||    s=="ankle_left") return JOINT::ANKLE_LEFT;
        if( s=="HIP_RIGHT"    ||    s=="hip_right") return JOINT::HIP_RIGHT;
        if( s=="HIPCOR_RIGHT" ||    s=="hipcor_right") return JOINT::HIPCOR_RIGHT;
        if( s=="KNEE_RIGHT"   ||    s=="knee_right") return JOINT::KNEE_RIGHT;
        if( s=="ANKLE_RIGHT"  ||    s=="ankle_right") return JOINT::ANKLE_RIGHT;
        return -1;
    }
}

namespace SIDE{
    string toString(int id){
        switch((Side) (id)) {
            case LEFT: return "LEFT"; break;
            case RIGHT: return "RIGHT"; break;
        }
        return "side not found";
    }
    int toID(string s){
        if( s=="LEFT"     ||    s=="left") return LEFT;
        if( s=="RIGHT"    ||    s=="right") return RIGHT;
        return -1;
    }
}

string CYCLE_PHASE::toString(int id){
    switch((CyclePhase) (id)) {
        case WHOLE_CYCLE: return "WHOLE_CYCLE"; break;
        case STANCE: return "STANCE"; break;
        case STANCE_START: return "STANCE_START"; break;
        case STANCE_END: return "STANCE_END"; break;
        case SWING: return "SWING"; break;
        case SWING_START: return "SWING_START"; break;
        case SWING_END: return "SWING_END"; break;
        case ANGLE_OFFSET: return "ANGLE_OFFSET"; break;
    }
    return "cycle phase not found";
}
namespace STOP_REASONS{
    string toString(int id){
        switch((StopReasons) (id)){
            case FALL: return "FALL"; break;
            case TRUNK_LEANING_LIMIT: return "TRUNK_LEANING_LIMIT"; break;
            case EXPLOD: return "EXPLOD"; break;
            case UNSTABLE: return "UNSTABLE"; break;
            case TO_SLOW: return "TO_SLOW"; break;
            case WRONG_SIDE: return "WRONG_SIDE"; break;
            case OTHER: return "OTHER"; break;
        }
        return "stop reason not found";
    }
}
namespace ACTIVE_DURING{
    string toString(int id){
        switch((ActiveDuring) (id)) {
            case WHOLE_CYCLE: return "WHOLE_CYCLE"; break;
            case STANCE: return "STANCE"; break;
            case STANCE_START: return "STANCE_START"; break;
            case STANCE_END: return "STANCE_END"; break;
            case SWING: return "SWING"; break;
            case SWING_START: return "SWING_START"; break;
            case SWING_END: return "SWING_END"; break;
            case ANGLE_OFFSET: return "ANGLE_OFFSET"; break;
        }
        return "active during not found";
    }
    int toID(string str){
        int result = -1;
        if(str=="STANCE_START" || str=="stancestart") return ACTIVE_DURING::STANCE_START;
        if(str=="STANCE_END" || str=="stanceend" || str=="finishingstance") return ACTIVE_DURING::STANCE_END;
        if(str=="STANCE" || str=="stance") return ACTIVE_DURING::STANCE;
        if(str=="SWING_START" || str=="swingstart") return ACTIVE_DURING::SWING_START;
        if(str=="SWING_END" || str=="swingend") return ACTIVE_DURING::SWING_END;
        if(str=="SWING" || str=="swing") return ACTIVE_DURING::SWING;
        if(str=="ANGLE_OFFSET" || str=="angleoffset") return ACTIVE_DURING::ANGLE_OFFSET;
        if(str=="WHOLE_CYCLE" || str=="cycle") return ACTIVE_DURING::WHOLE_CYCLE;
        return -1;
    }

    bool isActive(
                ACTIVE_DURING::ActiveDuring active_on,
                CYCLE_PHASE::CyclePhase current_limb_state
                )
    {
        bool ret = false;
        if(
            active_on == ACTIVE_DURING::STANCE
            &&
            ( current_limb_state == CYCLE_PHASE::STANCE_START
                ||
                current_limb_state == CYCLE_PHASE::STANCE_END
            )
        )
            ret = true;

        if(
            active_on == ACTIVE_DURING::SWING
            &&
            ( current_limb_state == CYCLE_PHASE::SWING_START
                ||
                current_limb_state == CYCLE_PHASE::SWING_END
            )
        )
            ret = true;

        if((int)current_limb_state == (int)active_on)
            ret = true;

        if(active_on == ACTIVE_DURING::WHOLE_CYCLE)
            ret = true;
        return ret;
    }
}


namespace nmm {
extern bool debug;
extern bool verbose;
}
void print_debug(std::string t){
#ifdef REAL_TIME
    return;
#endif
    if (nmm::debug){
        std::cout << t << std::endl;
    }
}

void print_verbose(std::string t){
#ifdef REAL_TIME
    return;
#endif
    if (nmm::verbose){
        std::cout << t << std::endl;
    }
}


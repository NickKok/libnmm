#include <sml/types/IO.hh>
#include <string>
using namespace std;
namespace INPUT{
    string toString(int id){
        string result="input not found";
        switch((Inputs) (id)) {
            case ANGLE_HIP_LEFT: return "ANGLE_HIP_LEFT"; break;
            case ANGLE_HIPCOR_LEFT: return "ANGLE_HIPCOR_LEFT"; break;
            case ANGLE_KNEE_LEFT: return "ANGLE_KNEE_LEFT"; break;
            case ANGLE_ANKLE_LEFT: return "ANGLE_ANKLE_LEFT"; break;
            case ANGLE_HIP_RIGHT: return "ANGLE_HIP_RIGHT"; break;
            case ANGLE_HIPCOR_RIGHT: return "ANGLE_HIPCOR_RIGHT"; break;
            case ANGLE_KNEE_RIGHT: return "ANGLE_KNEE_RIGHT"; break;
            case ANGLE_ANKLE_RIGHT: return "ANGLE_ANKLE_RIGHT"; break;
            case NECK_Y: return "NECK_Y"; break; // height
            case NECK_X: return "NECK_X"; break; // forward distance 
            case NECK_Z: return "NECK_Z"; break; // lateral distance 
            case THETA_TRUNK: return "THETA_TRUNK"; break;
            case THETACOR_TRUNK: return "THETACOR_TRUNK"; break;
            case TOE_LEFT_X: return "TOE_LEFT_X"; break;
            case TOE_LEFT_Y: return "TOE_LEFT_Y"; break;
            case TOE_LEFT_Z: return "TOE_LEFT_Z"; break;
            case TOE_RIGHT_X: return "TOE_RIGHT_X"; break;
            case TOE_RIGHT_Y: return "TOE_RIGHT_Y"; break;
            case TOE_RIGHT_Z: return "TOE_RIGHT_Z"; break;
            case HEEL_LEFT_X: return "HEEL_LEFT_X"; break;
            case HEEL_LEFT_Y: return "HEEL_LEFT_Y"; break;
            case HEEL_LEFT_Z: return "HEEL_LEFT_Z"; break;
            case HEEL_RIGHT_X: return "HEEL_RIGHT_X"; break;
            case HEEL_RIGHT_Y: return "HEEL_RIGHT_Y"; break;
            case HEEL_RIGHT_Z: return "HEEL_RIGHT_Z"; break;
            case MIDHIP_X: return "MIDHIP_X"; break;
            case MIDHIP_Y: return "MIDHIP_Y"; break;
            case MIDHIP_Z: return "MIDHIP_Z"; break;
            case SENSOR_HEEL_LEFT: return "SENSOR_HEEL_LEFT"; break;
            case SENSOR_HEEL_RIGHT: return "SENSOR_HEEL_RIGHT"; break;
            case SENSOR_TOE_LEFT: return "SENSOR_TOE_LEFT"; break;
            case SENSOR_TOE_RIGHT: return "SENSOR_TOE_RIGHT"; break;
            //case LEFT_FOOT_ORIENTATION: result="LEFT_FOOT_ORIENTATION";break;
            //case RIGHT_FOOT_ORIENTATION: result="RIGHT_FOOT_ORIENTATION"; break;
            }
        return result;
    }
    int toID(std::string s){
        if(s=="ANGLE_HIP_LEFT" || s=="angle_hip_left") return ANGLE_HIP_LEFT;
        if(s=="ANGLE_HIPCOR_LEFT" || s=="angle_hipcor_left") return ANGLE_HIPCOR_LEFT;
        if(s=="ANGLE_KNEE_LEFT" || s=="angle_knee_left") return ANGLE_KNEE_LEFT;
        if(s=="ANGLE_ANKLE_LEFT" || s=="angle_ankle_left") return ANGLE_ANKLE_LEFT;
        if(s=="ANGLE_HIP_RIGHT" || s=="angle_hip_right") return ANGLE_HIP_RIGHT;
        if(s=="ANGLE_HIPCOR_RIGHT" || s=="angle_hipcor_right") return ANGLE_HIPCOR_RIGHT;
        if(s=="ANGLE_KNEE_RIGHT" || s=="angle_knee_right") return ANGLE_KNEE_RIGHT;
        if(s=="ANGLE_ANKLE_RIGHT" || s=="angle_ankle_right") return ANGLE_ANKLE_RIGHT;
        if(s=="NECK_Y" || s=="neck_y") return NECK_Y;
        if(s=="NECK_X" || s=="neck_x") return NECK_X;
        if(s=="NECK_Z" || s=="neck_z") return NECK_Z;
        if(s=="THETA_TRUNK" || s=="theta_trunk") return THETA_TRUNK;
        if(s=="THETACOR_TRUNK" || s=="thetacor_trunk") return THETACOR_TRUNK;
        if(s=="TOE_LEFT_X" || s=="toe_left_x") return TOE_LEFT_X;
        if(s=="TOE_LEFT_Y" || s=="toe_left_y") return TOE_LEFT_Y;
        if(s=="TOE_LEFT_Z" || s=="toe_left_z") return TOE_LEFT_Z;
        if(s=="TOE_RIGHT_X" || s=="toe_right_x") return TOE_RIGHT_X;
        if(s=="TOE_RIGHT_Y" || s=="toe_right_y") return TOE_RIGHT_Y;
        if(s=="TOE_RIGHT_Z" || s=="toe_right_z") return TOE_RIGHT_Z;
        if(s=="HEEL_LEFT_X" || s=="heel_left_x") return HEEL_LEFT_X;
        if(s=="HEEL_LEFT_Y" || s=="heel_left_y") return HEEL_LEFT_Y;
        if(s=="HEEL_LEFT_Z" || s=="heel_left_z") return HEEL_LEFT_Z;
        if(s=="HEEL_RIGHT_X" || s=="heel_right_x") return HEEL_RIGHT_X;
        if(s=="HEEL_RIGHT_Y" || s=="heel_right_y") return HEEL_RIGHT_Y;
        if(s=="HEEL_RIGHT_Z" || s=="heel_right_z") return HEEL_RIGHT_Z;
        if(s=="MIDHIP_X" || s=="midhip_x") return MIDHIP_X;
        if(s=="MIDHIP_Y" || s=="midhip_y") return MIDHIP_Y;
        if(s=="MIDHIP_Z" || s=="midhip_z") return MIDHIP_Z;
        if(s=="SENSOR_HEEL_LEFT" || s=="sensor_heel_left") return SENSOR_HEEL_LEFT;
        if(s=="SENSOR_HEEL_RIGHT" || s=="sensor_heel_right") return SENSOR_HEEL_RIGHT;
        if(s=="SENSOR_TOE_LEFT" || s=="sensor_toe_left") return SENSOR_TOE_LEFT;
        if(s=="SENSOR_TOE_RIGHT" || s=="sensor_toe_right") return SENSOR_TOE_RIGHT;
    }
}

namespace OUTPUT {
    string toString(int id){
        string result="output not found";
        switch((Outputs) (id)) {
            case TORQUE_HIP_LEFT: return "TORQUE_HIP_LEFT"; break;
            case TORQUE_HIPCOR_LEFT: return "TORQUE_HIPCOR_LEFT"; break;
            case TORQUE_KNEE_LEFT: return "TORQUE_KNEE_LEFT"; break;
            case TORQUE_ANKLE_LEFT: return "TORQUE_ANKLE_LEFT"; break;
            case TORQUE_HIP_RIGHT: return "TORQUE_HIP_RIGHT"; break;
            case TORQUE_HIPCOR_RIGHT: return "TORQUE_HIPCOR_RIGHT"; break;
            case TORQUE_KNEE_RIGHT: return "TORQUE_KNEE_RIGHT"; break;
            case TORQUE_ANKLE_RIGHT: return "TORQUE_ANKLE_RIGHT"; break;
            }
        return result;
    }
    int toID(std::string s){
        if(s=="TORQUE_HIP_LEFT" or s=="torque_hip_left") return TORQUE_HIP_LEFT;
        if(s=="TORQUE_HIPCOR_LEFT" or s=="torque_hipcor_left") return TORQUE_HIPCOR_LEFT;
        if(s=="TORQUE_KNEE_LEFT" or s=="torque_knee_left") return TORQUE_KNEE_LEFT;
        if(s=="TORQUE_ANKLE_LEFT" or s=="torque_ankle_left") return TORQUE_ANKLE_LEFT;
        if(s=="TORQUE_HIP_RIGHT" or s=="torque_hip_right") return TORQUE_HIP_RIGHT;
        if(s=="TORQUE_HIPCOR_RIGHT" or s=="torque_hipcor_right") return TORQUE_HIPCOR_RIGHT;
        if(s=="TORQUE_KNEE_RIGHT" or s=="torque_knee_right") return TORQUE_KNEE_RIGHT;
        if(s=="TORQUE_ANKLE_RIGHT" or s=="torque_ankle_right") return TORQUE_ANKLE_RIGHT;
    }
}

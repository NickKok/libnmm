#include <sml/body/Sml.hh>
#include <sml/musculoSkeletalSystem/Muscle.hh>
#include <sml/musculoSkeletalSystem/MuscleJoint.hh>
#include <sml/musculoSkeletalSystem/Joint.hh>
#include <sml/musculoSkeletalSystem/Sensor.hh>
#include <sml/neuralSystem/Interneuron.hh>
#include <sml/neuralSystem/Motoneuron.hh>
#include <sml/sml-tools/Settings.hh>
#include <sml/sml-tools/EventManager.hh>
#include <sml/sml-tools/OnlineCtrl.hh>

#include <boost/xpressive/xpressive.hpp>

extern EventManager* eventManager;
extern OnlineCtrl* onlineCtrl;
namespace nmm {
  extern SmlParameters parameters;
}
using namespace nmm;
using namespace std;
using namespace boost::xpressive;

void Sml::init(){
    cout << endl;
    cout << "*********************************" << endl;
    cout << "  Human body from branch master  " << endl;
    cout << "*********************************" << endl;

    print_debug("[ok] : EventManager creation (Init.cc)");
    SENSORY_INTERNEURON::init();
    print_debug("[ok] : SENSORY_INTERNEURON Init (Init.cc)");
    ConstantInit();
    print_debug("[ok] : ConstantInit Init (Init.cc)");
    InputInit();
    print_debug("[ok] : Input Init (Init.cc)");

    //Constant initialisation
    initialisePerturbation();
    initialiseOnlineCtrl();
    /**
     *
     * intialise model features
     *
     */
    /**
     *  1/Musculo skeletal initialisation, joints, muscle, and connection between muscles and joints
     */
    initialiseMusculoSkeletalSystem();
    print_debug("[ok] : muscles initialisation (Init.cc)");
    /**
     *  2/Sensors initialisation (ground, foot, muscles, trunk)
     */
    initialiseSensors();
    print_debug("[ok] : sensors initialisation (Init.cc)");

    initialiseSpinalControl();
    print_debug("[ok] : initialisation of spinal cord controller (Init.cc)");
}

void Sml::initialiseOnlineCtrl()
{
    onlineCtrl = new OnlineCtrl(this);
    onlineCtrl->Add("MN_MTU_delay",0.0,5.0,0.0);

}

void Sml::initialisePerturbation(){
    if(Settings::get<int>("perturbation") == 1) {
        perturbator = PerturbatorManager(Settings::get<double>("perturbation_start_at"),Settings::get<double>("perturbation_ends_after"));
        // perturbator.addInSerie(JOINT::HIP_LEFT);
        // perturbator.addInSerie(JOINT::KNEE_LEFT);
        // perturbator.addInSerie(JOINT::ANKLE_LEFT);
        perturbator.addInParallel(JOINT::ANKLE_LEFT);
    }
    //perturbator[JOINT::KNEE_LEFT] = new Perturbator(start_time+duration,duration);
    //perturbator[JOINT::ANKLE_LEFT] = new Perturbator(start_time+2*duration,duration);



}


void Sml::initialiseMusculoSkeletalSystem(){
    string folder = Settings::get<string>("config") + Settings::get<string>("config_nmm") + "/";
    string file = folder+Settings::get<string>("body_scheme");

    print_verbose("Loading body from file : " + file );
    Muscle* m_l;
    Muscle* m_r;
    bodyConfig = Parameters::CreateFromFile(file);

     //TODO To create joints directly using the yaml file as well, to be tested
    if (bodyConfig.getVector<std::string>("joint_name").size() > 0) { // TODO TEST
        print_verbose("    Found the following joints");
        int i=0;
        for(auto &joint : bodyConfig.getVector<string>("joint_name")){
            print_verbose(joint);
            string  j_type    =  bodyConfig.getVector<string>("j_type")[i];
            double  j_ref_pos =  bodyConfig.getVector<double>("j_ref_pos")[i]/180.0*PI;
            double  j_min_pos =  bodyConfig.getVector<double>("j_min_pos")[i]/180.0*PI;
            double  j_max_pos =  bodyConfig.getVector<double>("j_max_pos")[i]/180.0*PI;
            bool    j_sagital =  bodyConfig.getVector<bool>("j_sagital")[i];
            JointMomentArms::MOMENT_ARM joint_type = j_type == "cst" ? JointMomentArms::CONSTANT : JointMomentArms::GEYER;

            string JOINT = joint;
            std::transform(JOINT.begin(), JOINT.end(),JOINT.begin(), ::toupper);


            JOINT::Joints   joint_left    = (JOINT::Joints)JOINT::toID(JOINT + "_LEFT" );
            INPUT::Inputs   input_left    = (INPUT::Inputs)INPUT::toID("ANGLE_" + JOINT + "_LEFT"  );
            OUTPUT::Outputs output_left   = (OUTPUT::Outputs)OUTPUT::toID("TORQUE_" + JOINT + "_LEFT"   );

            JOINT::Joints   joint_right   = (JOINT::Joints)JOINT::toID(JOINT + "_RIGHT");
            INPUT::Inputs   input_right   = (INPUT::Inputs)INPUT::toID("ANGLE_" + JOINT + "_RIGHT" );
            OUTPUT::Outputs output_right  = (OUTPUT::Outputs)OUTPUT::toID("TORQUE_" + JOINT + "_RIGHT"  );

            joints[joint_left] = new Joint(
                joint_type,
                &Input[input_left],
                &Output[output_left],
                j_ref_pos,
                j_min_pos,
                j_max_pos,
                j_sagital);

            joints[joint_right] = new Joint(
                joint_type,
                &Input[input_right],
                &Output[output_right],
                j_ref_pos,
                j_min_pos,
                j_max_pos,
                j_sagital);

            joints[joint_left]->name  = JOINT::toString((JOINT::Joints)joint_left);
            joints[joint_right]->name = JOINT::toString((JOINT::Joints)joint_right);
            i++;
        }
    }
    if (bodyConfig.getVector<std::string>("muscle_name").size() > 0) {
        print_verbose("    Found the following muscles" );
        int i=0;
        for(auto &name : bodyConfig.getVector<std::string>("muscle_name")){
            print_verbose(name);
            //TODO :
            // Add the ability to scale down the model using specific factor
            // m_fmax *= force_ratio;
            // m_l_opt *= length_ratio;
            // m_l_slack *= length_ratio;
            // m_v_max *= velocity_ratio;
            // m_mass *= force_ratio;

            double& m_fmax = bodyConfig.getVector<double>("muscle_fmax")[i];
            double& m_l_opt = bodyConfig.getVector<double>("muscle_l_opts")[i];
            double& m_l_slack = bodyConfig.getVector<double>("muscle_l_slacks")[i];
            double& m_v_max = bodyConfig.getVector<double>("muscle_v_maxes")[i];
            double& m_mass = bodyConfig.getVector<double>("muscle_masses")[i];
            double& m_pennation = bodyConfig.getVector<double>("muscle_pennation")[i];
            double& m_typeIfiber = bodyConfig.getVector<double>("muscle_typeIfiber")[i];
            bool   m_sagital = bodyConfig.getVector<bool>("sagital")[i];
            m_l = new Muscle(SIDE::LEFT, name,m_l_slack,m_l_opt,m_v_max,m_fmax, m_pennation,m_typeIfiber,m_sagital);
            m_r = new Muscle(SIDE::RIGHT,name,m_l_slack,m_l_opt,m_v_max,m_fmax, m_pennation,m_typeIfiber,m_sagital);
            int j=0;
            for(auto &muscle : bodyConfig.getVector<string>("muscle_joint_muscle")){
                if(muscle == name){
                    string  mj_joint = bodyConfig.getVector<string>("muscle_joint_joint")[j];
                    string  mj_muscle_joint_type = bodyConfig.getVector<string>("muscle_joint_type")[j];
                    double  mj_moment_arm = bodyConfig.getVector<double>("muscle_joint_moment_arm")[j];
                    double  mj_ref = bodyConfig.getVector<double>("muscle_joint_ref")[j]/180.0*PI;
                    double  mj_max = bodyConfig.getVector<double>("muscle_joint_max")[j]/180.0*PI;

                    if(mj_muscle_joint_type == "cst"){
                        m_l->musclejoints.push_back( new MuscleJoint(joints[JOINT::toID(mj_joint + "_left")], m_l, mj_moment_arm, mj_ref));
                        m_r->musclejoints.push_back( new MuscleJoint(joints[JOINT::toID(mj_joint + "_right")], m_r, mj_moment_arm, mj_ref));
                    }
                    else if(mj_muscle_joint_type == "geyer"){
                        m_l->musclejoints.push_back( new MuscleJoint(joints[JOINT::toID(mj_joint + "_left")], m_l, mj_moment_arm, mj_ref, mj_max));
                        m_r->musclejoints.push_back( new MuscleJoint(joints[JOINT::toID(mj_joint + "_right")], m_r, mj_moment_arm, mj_ref, mj_max));
                    }
                    else{
                        cout << "Unknown joint type : " << mj_muscle_joint_type << endl;
                    }
                }
                j++;
            }
            muscles["left_" + name] = m_l;
            muscles["right_" + name] = m_r;
            i++;
      }
    }

}

void Sml::initialiseSensors(){
    /**
     * Ground sensors (left_foot , right_foot sensors)
     */
    TouchSensor * sensor_heel;
    TouchSensor * sensor_toe;
    sensor_heel = new TouchSensor(Input[INPUT::SENSOR_HEEL_LEFT],dt);
    sensor_toe = new TouchSensor(Input[INPUT::SENSOR_TOE_LEFT],dt);
    left_foot = new Foot(sensor_heel,sensor_toe,SIDE::LEFT,parameters[1]["offset_change_reflex"]);
    sensors["left_heel_sensor"] = sensor_heel; // we add them to the sensors table so that we ensure
    sensors["left_toe_sensor"] = sensor_toe;   // that the step function will be run.

    sensor_heel = new TouchSensor(Input[INPUT::SENSOR_HEEL_RIGHT],dt);
    sensor_toe = new TouchSensor(Input[INPUT::SENSOR_TOE_RIGHT],dt);
    right_foot = new Foot(sensor_heel,sensor_toe,SIDE::RIGHT,parameters[1]["offset_change_reflex"]);
    sensors["right_heel_sensor"] = sensor_heel;
    sensors["right_toe_sensor"] = sensor_toe;


    sensors["left_foot"] = left_foot;
    sensors["right_foot"] = right_foot;
    print_debug("[ok] feet initialization");
    eventManager = new EventManager(Constant,Input,left_foot,right_foot);
    /**
     * Muscle sensors
     */
    MuscleLengthSensor * mls;
    MuscleForceSensor * mfs;
    zero_length_offset = 0.0;
    one_gain = 1.0;
    for(auto& kv: muscles){
        Muscle * muscle = kv.second;
        mfs = new MuscleForceSensor(muscle, one_gain);
        auto it = parameters[0].find(muscle->name+"_bl");
        auto itRL = parameters[8].find(muscle->name+"_bl");

        if(muscle->side == SIDE::LEFT){
            itRL = parameters[8].find(muscle->name+"_bl_left");
        }
        else{
            itRL = parameters[8].find(muscle->name+"_bl_right");
        }

        if(itRL != parameters[8].end()){
            if(it != parameters[0].end())
                mls = new MuscleLengthSensor(muscle, it->second, itRL->second, one_gain);
            else
                mls = new MuscleLengthSensor(muscle, zero_length_offset, itRL->second, one_gain);
        }
        else{
            if(it != parameters[0].end())
                mls = new MuscleLengthSensor(muscle, it->second, zero_length_offset, one_gain);
            else
                mls = new MuscleLengthSensor(muscle, zero_length_offset, zero_length_offset, one_gain);
        }

        sensors[muscle->getName()+"_length"] = mls;
        sensors[muscle->getName()+"_force"] = mfs;
    }
    print_debug("[ok] muscle sensors initialization");
    /**
     * VestibularState sensors (stability sensors)
     */

    basalGanglia = new BasalGanglia(dt,Input);
    sensors["trunk_pd"] = basalGanglia->positionControlSagital; // PositionSensor1
    sensors["trunk_pd2"] = basalGanglia->positionControlSagital2; // PositionSensor2
    sensors["trunk_pd_cor"] = basalGanglia->positionControlCoronal; // PositionSensor1
    sensors["trunk_leak"] = basalGanglia->swingCorrectionSagital; // FixedAtTakeOffSensor
    sensors["trunk_leak_cor"] = basalGanglia->swingCorrectionCoronal; // FixedAtTakeOffSensor
    sensors["trunk_accumulator_left"] = basalGanglia->coronalAccumulatorLeft; //AccumulatorSensor
    sensors["trunk_accumulator_right"] = basalGanglia->coronalAccumulatorRight; //AccumulatorSensor


    print_debug("[ok] trunk sensor initialization");
    /**
     * Angle sensors for the knee
     */
    phi_knee_off = 2.97;
    AngleSensor * as;

    if(Settings::get<int>("kphiknee_off") == 1){
        as = new AngleSensor(joints[JOINT::KNEE_RIGHT], parameters[0]["kphiknee_off"]);
        sensors["right_knee_angle"] = as;
        as = new AngleSensor(joints[JOINT::KNEE_LEFT], parameters[0]["kphiknee_off"]);
        sensors["left_knee_angle"] = as;
    }
    else
    {
        as = new AngleSensor(joints[JOINT::KNEE_RIGHT], phi_knee_off);
        sensors["right_knee_angle"] = as;
        as = new AngleSensor(joints[JOINT::KNEE_LEFT], phi_knee_off);
        sensors["left_knee_angle"] = as;
    }

    print_debug("[ok] angle sensor initialization");
}

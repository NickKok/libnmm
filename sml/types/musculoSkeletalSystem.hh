#ifndef __AIRI_TYPES_MUSCLES_H__
#define __AIRI_TYPES_MUSCLES_H__

namespace SENSORY_INTERNEURON{
    typedef enum
    {
        // the sensory interneurons name format is as follow
        // First element is the output muscle
        // Second is the sensors name (there can be several sensors combined)
        // Third is one this interneuron should be active

        // TRUNK RELATED INTERNEURONS
        RIGHT_HAM_TRUNK_PD_STANCE,
        RIGHT_HAB_TRUNK_PD_STANCE,
        RIGHT_HAD_TRUNK_PD_STANCE,
        RIGHT_HF_TRUNK_PD_STANCE,
        RIGHT_GLU_TRUNK_PD2_STANCE,
        RIGHT_HF_TRUNK_LEAK_SWING,
        RIGHT_HAB_TRUNK_LEAK_SWING,
        RIGHT_HAD_TRUNK_LEAK_SWING,
        RIGHT_HAB_TRUNK_ACC_SWING,
        RIGHT_HAD_TRUNK_ACC_SWING,
        LEFT_HAM_TRUNK_PD_STANCE,
        LEFT_HAB_TRUNK_PD_STANCE,
        LEFT_HAD_TRUNK_PD_STANCE,
        LEFT_HF_TRUNK_PD_STANCE,
        LEFT_GLU_TRUNK_PD2_STANCE,
        LEFT_HF_TRUNK_LEAK_SWING,
        LEFT_HAB_TRUNK_LEAK_SWING,
        LEFT_HAD_TRUNK_LEAK_SWING,
        LEFT_HAB_TRUNK_ACC_SWING,
        LEFT_HAD_TRUNK_ACC_SWING,
        // MUSCLE SENSORS
        RIGHT_GAS_Ib_STANCE,
        RIGHT_VAS_Ib_STANCE,
        RIGHT_SOL_Ib_STANCE,
        LEFT_GAS_Ib_STANCE,
        LEFT_VAS_Ib_STANCE,
        LEFT_SOL_Ib_STANCE,

        RIGHT_TA_II_CYCLE,
        RIGHT_SOL_RIGHT_TA_II_STANCE,
        RIGHT_HF_II_SWING,
        RIGHT_HAM_RIGHT_HF_II_SWING,
        LEFT_TA_II_CYCLE,
        LEFT_SOL_LEFT_TA_II_STANCE,
        LEFT_HF_II_SWING,
        LEFT_HAM_LEFT_HF_II_SWING,

        RIGHT_VAS_ANGLE_ANGLE_OFFSET,
        LEFT_VAS_ANGLE_ANGLE_OFFSET,

        RIGHT_VAS_LEFT_FOOT_STANCE_END,
        LEFT_VAS_RIGHT_FOOT_STANCE_END,

        RIGHT_HAM_Ib_SWING,
        RIGHT_GLU_Ib_SWING,
        LEFT_HAM_Ib_SWING,
        LEFT_GLU_Ib_SWING,



        FIRST = RIGHT_HAM_TRUNK_PD_STANCE,
        LAST = LEFT_GLU_Ib_SWING,
    } SensoryInterneurons;

    class LatentConnection{
        public:
        SensoryInterneurons id;
        std::string output;
        std::vector<std::string> inputs;
        std::string weight;
        double constantFactor; // Can be used to specify inhibitory connection
        LatentConnection(SensoryInterneurons id, std::string input, std::string output, std::string weight, double constantFactor):
            id(id),output(output), weight(weight), constantFactor(constantFactor){ 
                inputs.push_back(input);
            }
        LatentConnection(SensoryInterneurons id, std::string input1, std::string input2, std::string output, std::string weight, double constantFactor):
            id(id), output(output), weight(weight), constantFactor(constantFactor){ 
                inputs.push_back(input1);
                inputs.push_back(input2);
            }
    };

    const int NUMBER = LAST - FIRST + 1;
    extern int  GATE_MAP[NUMBER];
    extern std::map<int,std::string> strs;
    extern std::map<int,std::map<std::string,int>> CONNECTION_TO_MOTONEURONS;
    extern std::map<int,SIDE::Side> sides;
    extern std::vector<LatentConnection> latentConnections;


    void init();
    std::string toString(int id);
    ACTIVE_DURING::ActiveDuring getGate(int id);
    SIDE::Side getSide(int id);
}

#endif

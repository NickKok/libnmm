#include <sml/types/musculoSkeletalSystem.hh>
#include <sml/types/types.h>
#include <string>
using namespace std;

/*TODO
int SENSORY_INTERNEURON::OUTPUT_MUSCLE(int id){
    std::string str = SENSORY_INTERNEURON::toString(id);
    std::vector<std::string> strs; 
    boost::split(strs, line, boost::is_any_of("_"));
    
    return MUSCLES::toID(strs[0]+" "+strs[1]);
}
int SENSORY_INTERNEURON::ACTIVE_DURING(int id){
    std::string str = SENSORY_INTERNEURON::toString(id);
    std::vector<std::string> strs; 
    boost::split(strs, line, boost::is_any_of("_"));
    
    return ACTIVE_DURING::toID(strs[strs.size()-1]);
}
int SENSORY_INTERNEURON::INPUT_SENSOR(int id){
    std::string str = SENSORY_INTERNEURON::toString(id);
    std::vector<std::string> strs; 
    boost::split(strs, line, boost::is_any_of("_"));
    
    return ACTIVE_DURING::toID(strs[strs.size()-1]);
}
*/

namespace SENSORY_INTERNEURON{
    //int CONNECTION_TO_MOTONEURONS[NUMBER][MUSCLES::NUMBER];
    int GATE_MAP[NUMBER];
    std::map<int,std::map<std::string,int>> CONNECTION_TO_MOTONEURONS;
    std::map<int,std::string> strs;
    std::map<int,SIDE::Side> sides;
    std::vector<LatentConnection> latentConnections;
    void init(){
        memset(GATE_MAP, 0, sizeof(GATE_MAP[0]) * NUMBER);
        int ts = Settings::get<double>("delay_short"); //5
        int tm = Settings::get<double>("delay_medium"); //10
        int tl = Settings::get<double>("delay_long"); //20
        
        CONNECTION_TO_MOTONEURONS[RIGHT_HAM_TRUNK_PD_STANCE]["right_ham"] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_HAB_TRUNK_PD_STANCE]["right_hab"] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_HAD_TRUNK_PD_STANCE]["right_had"] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_HF_TRUNK_PD_STANCE]["right_hf"] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_GLU_TRUNK_PD2_STANCE]["right_glu"] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_HF_TRUNK_LEAK_SWING]["right_hf"] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_HAB_TRUNK_LEAK_SWING]["right_hab"] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_HAD_TRUNK_LEAK_SWING]["right_had"] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_HAB_TRUNK_ACC_SWING]["right_hab"] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_HAD_TRUNK_ACC_SWING]["right_had"] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_HAM_TRUNK_PD_STANCE]["left_ham"] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_HAB_TRUNK_PD_STANCE]["left_hab"] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_HAD_TRUNK_PD_STANCE]["left_had"] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_HF_TRUNK_PD_STANCE]["left_hf"] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_GLU_TRUNK_PD2_STANCE]["left_glu"] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_HF_TRUNK_LEAK_SWING]["left_hf"] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_HAB_TRUNK_LEAK_SWING]["left_hab"] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_HAD_TRUNK_LEAK_SWING]["left_had"] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_HAB_TRUNK_ACC_SWING]["left_hab"] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_HAD_TRUNK_ACC_SWING]["left_had"] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_GAS_Ib_STANCE]["right_gas"] = tl;
        CONNECTION_TO_MOTONEURONS[RIGHT_VAS_Ib_STANCE]["right_vas"] = tm;
        CONNECTION_TO_MOTONEURONS[RIGHT_SOL_Ib_STANCE]["right_sol"] = tl;
        CONNECTION_TO_MOTONEURONS[LEFT_GAS_Ib_STANCE]["left_gas"] = tl;
        CONNECTION_TO_MOTONEURONS[LEFT_VAS_Ib_STANCE]["left_vas"] = tm;
        CONNECTION_TO_MOTONEURONS[LEFT_SOL_Ib_STANCE]["left_sol"] = tl;
        CONNECTION_TO_MOTONEURONS[RIGHT_TA_II_CYCLE]["right_ta"] = tl;
        CONNECTION_TO_MOTONEURONS[RIGHT_SOL_RIGHT_TA_II_STANCE]["right_ta"] = tl;
        CONNECTION_TO_MOTONEURONS[RIGHT_HF_II_SWING]["right_hf"] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_HAM_RIGHT_HF_II_SWING]["right_hf"] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_TA_II_CYCLE]["left_ta"] = tl;
        CONNECTION_TO_MOTONEURONS[LEFT_SOL_LEFT_TA_II_STANCE]["left_ta"] = tl;
        CONNECTION_TO_MOTONEURONS[LEFT_HF_II_SWING]["left_hf"] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_HAM_LEFT_HF_II_SWING]["left_hf"] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_VAS_ANGLE_ANGLE_OFFSET]["right_vas"] = tm;
        CONNECTION_TO_MOTONEURONS[LEFT_VAS_ANGLE_ANGLE_OFFSET]["left_vas"] = tm;
        CONNECTION_TO_MOTONEURONS[RIGHT_VAS_LEFT_FOOT_STANCE_END]["right_vas"] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_VAS_RIGHT_FOOT_STANCE_END]["left_vas"] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_HAM_Ib_SWING]["right_ham"] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_GLU_Ib_SWING]["right_glu"] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_HAM_Ib_SWING]["left_ham"] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_GLU_Ib_SWING]["left_glu"] = ts;

        GATE_MAP[RIGHT_HAM_TRUNK_PD_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[RIGHT_HAB_TRUNK_PD_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[RIGHT_HAD_TRUNK_PD_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[RIGHT_HF_TRUNK_PD_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[RIGHT_GLU_TRUNK_PD2_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[RIGHT_HF_TRUNK_LEAK_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[RIGHT_HAB_TRUNK_LEAK_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[RIGHT_HAD_TRUNK_LEAK_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[RIGHT_HAB_TRUNK_ACC_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[RIGHT_HAD_TRUNK_ACC_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[LEFT_HAM_TRUNK_PD_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[LEFT_HAB_TRUNK_PD_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[LEFT_HAD_TRUNK_PD_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[LEFT_HF_TRUNK_PD_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[LEFT_GLU_TRUNK_PD2_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[LEFT_HF_TRUNK_LEAK_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[LEFT_HAB_TRUNK_LEAK_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[LEFT_HAD_TRUNK_LEAK_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[LEFT_HAB_TRUNK_ACC_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[LEFT_HAD_TRUNK_ACC_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[RIGHT_GAS_Ib_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[RIGHT_VAS_Ib_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[RIGHT_SOL_Ib_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[LEFT_GAS_Ib_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[LEFT_VAS_Ib_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[LEFT_SOL_Ib_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[RIGHT_TA_II_CYCLE] = ACTIVE_DURING::WHOLE_CYCLE;
        GATE_MAP[RIGHT_SOL_RIGHT_TA_II_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[RIGHT_HF_II_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[RIGHT_HAM_RIGHT_HF_II_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[LEFT_TA_II_CYCLE] = ACTIVE_DURING::WHOLE_CYCLE;
        GATE_MAP[LEFT_SOL_LEFT_TA_II_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[LEFT_HF_II_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[LEFT_HAM_LEFT_HF_II_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[RIGHT_VAS_ANGLE_ANGLE_OFFSET] = ACTIVE_DURING::STANCE_END;
        GATE_MAP[LEFT_VAS_ANGLE_ANGLE_OFFSET] = ACTIVE_DURING::STANCE_END;
        GATE_MAP[RIGHT_VAS_LEFT_FOOT_STANCE_END] = ACTIVE_DURING::STANCE_END;
        GATE_MAP[LEFT_VAS_RIGHT_FOOT_STANCE_END] = ACTIVE_DURING::STANCE_END;
        GATE_MAP[RIGHT_HAM_Ib_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[RIGHT_GLU_Ib_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[LEFT_HAM_Ib_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[LEFT_GLU_Ib_SWING] = ACTIVE_DURING::SWING;

        strs = {
        {SENSORY_INTERNEURON::RIGHT_SOL_Ib_STANCE,"right_sol__mff_stance"},
        {SENSORY_INTERNEURON::RIGHT_TA_II_CYCLE,"right_ta__mlf_cycle"},
        {SENSORY_INTERNEURON::RIGHT_SOL_RIGHT_TA_II_STANCE,"right_sol_ta_mff_stance"},
        {SENSORY_INTERNEURON::RIGHT_GAS_Ib_STANCE,"right_gas__mff_stance"},
        {SENSORY_INTERNEURON::RIGHT_VAS_Ib_STANCE,"right_vas__mff_stance"},
        {SENSORY_INTERNEURON::RIGHT_VAS_ANGLE_ANGLE_OFFSET,"right_vas_pko_angleoffset"},
        {SENSORY_INTERNEURON::RIGHT_VAS_LEFT_FOOT_STANCE_END,"right_vas_gcf_finishingstance"},
        {SENSORY_INTERNEURON::RIGHT_HAM_TRUNK_PD_STANCE,"right_ham_gif_stance"},
        {SENSORY_INTERNEURON::RIGHT_HAB_TRUNK_PD_STANCE,"right_hab_gif_stance"},
        {SENSORY_INTERNEURON::RIGHT_HAD_TRUNK_PD_STANCE,"right_had_gif_stance"},
        {SENSORY_INTERNEURON::RIGHT_HAM_Ib_SWING,"right_ham__mff_swing"},
        {SENSORY_INTERNEURON::RIGHT_GLU_TRUNK_PD2_STANCE,"right_glu_gif_stance"},
        {SENSORY_INTERNEURON::RIGHT_GLU_Ib_SWING,"right_glu__mff_swing"},
        {SENSORY_INTERNEURON::RIGHT_HF_TRUNK_PD_STANCE,"right_hf_gif_stance"},
        {SENSORY_INTERNEURON::RIGHT_HF_II_SWING,"right_hf__mlf_swing"},
        {SENSORY_INTERNEURON::RIGHT_HAM_RIGHT_HF_II_SWING,"right_ham_hf_mlf_swing"},
        {SENSORY_INTERNEURON::RIGHT_HF_TRUNK_LEAK_SWING,"right_hf_tl_swing"},
        {SENSORY_INTERNEURON::RIGHT_HAB_TRUNK_LEAK_SWING,"right_hab_tl_swing"},
        {SENSORY_INTERNEURON::RIGHT_HAD_TRUNK_LEAK_SWING,"right_had_tl_swing"},
        {SENSORY_INTERNEURON::RIGHT_HAB_TRUNK_ACC_SWING, "right_hab_trunk_acc_swing"},
        {SENSORY_INTERNEURON::RIGHT_HAD_TRUNK_ACC_SWING, "right_had_trunk_acc_swing"},
        {SENSORY_INTERNEURON::LEFT_SOL_Ib_STANCE,"left_sol__mff_stance"},
        {SENSORY_INTERNEURON::LEFT_TA_II_CYCLE,"left_ta__mlf_cycle"},
        {SENSORY_INTERNEURON::LEFT_SOL_LEFT_TA_II_STANCE,"left_sol_ta_mff_stance"},
        {SENSORY_INTERNEURON::LEFT_GAS_Ib_STANCE,"left_gas__mff_stance"},
        {SENSORY_INTERNEURON::LEFT_VAS_Ib_STANCE,"left_vas__mff_stance"},
        {SENSORY_INTERNEURON::LEFT_VAS_ANGLE_ANGLE_OFFSET,"left_vas_pko_angleoffset"},
        {SENSORY_INTERNEURON::LEFT_VAS_RIGHT_FOOT_STANCE_END,"left_vas_gcf_finishingstance"},
        {SENSORY_INTERNEURON::LEFT_HAM_TRUNK_PD_STANCE,"left_ham_gif_stance"},
        {SENSORY_INTERNEURON::LEFT_HAB_TRUNK_PD_STANCE,"left_hab_gif_stance"},
        {SENSORY_INTERNEURON::LEFT_HAD_TRUNK_PD_STANCE,"left_had_gif_stance"},
        {SENSORY_INTERNEURON::LEFT_HAM_Ib_SWING,"left_ham__mff_swing"},
        {SENSORY_INTERNEURON::LEFT_GLU_TRUNK_PD2_STANCE,"left_glu_gif_stance"},
        {SENSORY_INTERNEURON::LEFT_GLU_Ib_SWING,"left_glu__mff_swing"},
        {SENSORY_INTERNEURON::LEFT_HF_TRUNK_PD_STANCE,"left_hf_gif_stance"},
        {SENSORY_INTERNEURON::LEFT_HF_II_SWING,"left_hf__mlf_swing"},
        {SENSORY_INTERNEURON::LEFT_HAM_LEFT_HF_II_SWING,"left_ham_hf_mlf_swing"},
        {SENSORY_INTERNEURON::LEFT_HF_TRUNK_LEAK_SWING,"left_hf_tl_swing"},
        {SENSORY_INTERNEURON::LEFT_HAB_TRUNK_LEAK_SWING,"left_hab_tl_swing"},
        {SENSORY_INTERNEURON::LEFT_HAD_TRUNK_LEAK_SWING,"left_had_tl_swing"},
        {SENSORY_INTERNEURON::LEFT_HAB_TRUNK_ACC_SWING, "left_hab_trunk_acc_swing"},
        {SENSORY_INTERNEURON::LEFT_HAD_TRUNK_ACC_SWING, "left_had_trunk_acc_swing"},
        };

        auto addConnection = [] (SensoryInterneurons id, string a, string b, string c, int d) -> void {
            latentConnections.push_back(LatentConnection(id,a,b,c,d));
        };

        auto addConnection2 = [] (SensoryInterneurons id, string a, string b, string c, string c2, int d) -> void {
            latentConnections.push_back(LatentConnection(id,a,b,c,c2,d));
        };

        addConnection(RIGHT_SOL_Ib_STANCE, "right_sol_force", "right_sol", "solsol_wf_right", 1.0);
        addConnection(RIGHT_TA_II_CYCLE, "right_ta_length", "right_ta", "tata_wl_right", 1.0);
        addConnection(RIGHT_SOL_RIGHT_TA_II_STANCE, "right_sol_force", "right_ta", "solta_wf_right", -1.0);
        addConnection(RIGHT_GAS_Ib_STANCE, "right_gas_force", "right_gas", "gasgas_wf_right", 1.0);
        addConnection(RIGHT_VAS_Ib_STANCE, "right_vas_force", "right_vas", "vasvas_wf_right", 1.0);
        addConnection(RIGHT_VAS_LEFT_FOOT_STANCE_END, "left_foot", "right_vas", "kbodyweight_right", -1.0);
        addConnection(RIGHT_HAM_Ib_SWING, "right_ham_force", "right_ham", "hamham_wf_right", 1.0);
        addConnection(RIGHT_GLU_Ib_SWING, "right_glu_force", "right_glu", "gluglu_wf_right", 1.0);
        addConnection(RIGHT_HAM_RIGHT_HF_II_SWING, "right_ham_length", "right_hf", "hamhf_wl_right", -1.0);
        addConnection(RIGHT_HF_II_SWING, "right_hf_length", "right_hf", "hfhf_wl_right", 1.0);
        addConnection(RIGHT_VAS_ANGLE_ANGLE_OFFSET, "left_foot", "right_vas", "kbodyweight_right", -1.0);
        addConnection(RIGHT_HF_TRUNK_LEAK_SWING, "trunk_leak", "right_hf", "klean", 1.0);
        addConnection(LEFT_SOL_Ib_STANCE, "left_sol_force", "left_sol", "solsol_wf_left", 1.0);
        addConnection(LEFT_TA_II_CYCLE, "left_ta_length", "left_ta", "tata_wl_left", 1.0);
        addConnection(LEFT_SOL_LEFT_TA_II_STANCE, "left_sol_force", "left_ta", "solta_wf_left", -1.0);
        addConnection(LEFT_GAS_Ib_STANCE, "left_gas_force", "left_gas", "gasgas_wf_left", 1.0);
        addConnection(LEFT_VAS_Ib_STANCE, "left_vas_force", "left_vas", "vasvas_wf_left", 1.0);
        addConnection(LEFT_VAS_RIGHT_FOOT_STANCE_END, "right_foot", "left_vas", "kbodyweight_left", -1.0);
        addConnection(LEFT_HAM_Ib_SWING, "left_ham_force", "left_ham", "hamham_wf_left", 1.0);
        addConnection(LEFT_GLU_Ib_SWING, "left_glu_force", "left_glu", "gluglu_wf_left", 1.0);
        addConnection(LEFT_HAM_LEFT_HF_II_SWING, "left_ham_length", "left_hf", "hamhf_wl_left", -1.0);
        addConnection(LEFT_HF_II_SWING, "left_hf_length", "left_hf", "hfhf_wl_left", 1.0);
        addConnection(LEFT_VAS_ANGLE_ANGLE_OFFSET, "right_foot", "left_vas", "kbodyweight_left", -1.0);
        addConnection(LEFT_HF_TRUNK_LEAK_SWING, "trunk_leak", "left_hf", "klean", 1.0);
        
        addConnection2(RIGHT_HAM_TRUNK_PD_STANCE, "right_foot", "trunk_pd", "right_ham", "kbodyweight_right", 1.0);
        addConnection2(RIGHT_GLU_TRUNK_PD2_STANCE, "right_foot", "trunk_pd2", "right_glu", "kbodyweight_right", 1.0);
        addConnection2(RIGHT_HF_TRUNK_PD_STANCE, "right_foot", "trunk_pd", "right_hf", "kbodyweight_right", -1.0);
        
        addConnection2(LEFT_HAM_TRUNK_PD_STANCE, "left_foot", "trunk_pd", "left_ham", "kbodyweight_left", 1.0);
        addConnection2(LEFT_GLU_TRUNK_PD2_STANCE, "left_foot", "trunk_pd2", "left_glu", "kbodyweight_left", 1.0);
        addConnection2(LEFT_HF_TRUNK_PD_STANCE, "left_foot", "trunk_pd", "left_hf", "kbodyweight_left", -1.0);
        
        addConnection(RIGHT_HAB_TRUNK_ACC_SWING, "trunk_accumulator_right", "right_hab", "accum_cor_hab", 1.0);
        addConnection(RIGHT_HAD_TRUNK_ACC_SWING, "trunk_accumulator_left", "right_had", "accum_cor_had", 0.3);
        addConnection(RIGHT_HAB_TRUNK_LEAK_SWING, "trunk_leak_cor", "right_hab", "klean_cor_hab", 1.0);
        addConnection(RIGHT_HAD_TRUNK_LEAK_SWING, "trunk_leak_cor", "right_had", "klean_cor_had", -1.0);
        
        addConnection(LEFT_HAB_TRUNK_ACC_SWING, "trunk_accumulator_left", "left_hab", "accum_cor_hab", 1.0);
        addConnection(LEFT_HAD_TRUNK_ACC_SWING, "trunk_accumulator_right", "left_had", "accum_cor_had", 0.3);
        addConnection(LEFT_HAB_TRUNK_LEAK_SWING, "trunk_leak_cor", "left_hab", "klean_cor_hab", -1.0);
        addConnection(LEFT_HAD_TRUNK_LEAK_SWING, "trunk_leak_cor", "left_had", "klean_cor_had", 1.0);
        
        addConnection2(RIGHT_HAB_TRUNK_PD_STANCE, "right_foot", "trunk_pd_cor", "right_hab", "kbodyweight_right", -1.0);
        addConnection2(RIGHT_HAD_TRUNK_PD_STANCE, "right_foot", "trunk_pd_cor", "right_had", "kbodyweight_right", 1.0);
        addConnection2(LEFT_HAB_TRUNK_PD_STANCE, "left_foot", "trunk_pd_cor", "left_hab", "kbodyweight_left", 1.0);
        addConnection2(LEFT_HAD_TRUNK_PD_STANCE, "left_foot", "trunk_pd_cor", "left_had", "kbodyweight_left", -1.0);


        sides = {
        {SENSORY_INTERNEURON::RIGHT_SOL_Ib_STANCE,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_TA_II_CYCLE,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_SOL_RIGHT_TA_II_STANCE,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_GAS_Ib_STANCE,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_VAS_Ib_STANCE,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_VAS_ANGLE_ANGLE_OFFSET,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_VAS_LEFT_FOOT_STANCE_END,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_HAM_TRUNK_PD_STANCE,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_HAB_TRUNK_PD_STANCE,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_HAD_TRUNK_PD_STANCE,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_HAM_Ib_SWING,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_GLU_TRUNK_PD2_STANCE,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_GLU_Ib_SWING,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_HF_TRUNK_PD_STANCE,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_HF_II_SWING,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_HAM_RIGHT_HF_II_SWING,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_HF_TRUNK_LEAK_SWING,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_HAB_TRUNK_LEAK_SWING,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_HAD_TRUNK_LEAK_SWING,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_HAB_TRUNK_ACC_SWING,SIDE::RIGHT},
        {SENSORY_INTERNEURON::RIGHT_HAD_TRUNK_ACC_SWING,SIDE::RIGHT},
        {SENSORY_INTERNEURON::LEFT_SOL_Ib_STANCE,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_TA_II_CYCLE,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_SOL_LEFT_TA_II_STANCE,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_GAS_Ib_STANCE,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_VAS_Ib_STANCE,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_VAS_ANGLE_ANGLE_OFFSET,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_VAS_RIGHT_FOOT_STANCE_END,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_HAM_TRUNK_PD_STANCE,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_HAB_TRUNK_PD_STANCE,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_HAD_TRUNK_PD_STANCE,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_HAM_Ib_SWING,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_GLU_TRUNK_PD2_STANCE,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_GLU_Ib_SWING,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_HF_TRUNK_PD_STANCE,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_HF_II_SWING,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_HAM_LEFT_HF_II_SWING,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_HF_TRUNK_LEAK_SWING,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_HAB_TRUNK_LEAK_SWING,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_HAD_TRUNK_LEAK_SWING,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_HAB_TRUNK_ACC_SWING,SIDE::LEFT},
        {SENSORY_INTERNEURON::LEFT_HAD_TRUNK_ACC_SWING,SIDE::LEFT},
        };
    };
    
    ACTIVE_DURING::ActiveDuring getGate(int id){
        if (id >= SENSORY_INTERNEURON::FIRST && id <= SENSORY_INTERNEURON::LAST)
            return (ACTIVE_DURING::ActiveDuring) GATE_MAP[id];
        else
            return ACTIVE_DURING::WHOLE_CYCLE;
    }
    SIDE::Side getSide(int id){
        if (id >= SENSORY_INTERNEURON::FIRST && id <= SENSORY_INTERNEURON::LAST)
            return sides[id];
        else
            return SIDE::UNKNOWN;
    }
    std::string toString(int id){
        if (id >= SENSORY_INTERNEURON::FIRST && id <= SENSORY_INTERNEURON::LAST)
            return strs[id];
        else
            return "sensory interneuron not found";
    }
}


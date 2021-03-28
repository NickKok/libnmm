#ifndef __Phase_HH__
#define __Phase_HH__

#include <vector>
#include <sstream>
#include <fstream>
#include <sml/types/types.h>
#include <sml/neuralSystem/CentralClock.hh>
#include <sml/sml-tools/Settings.hh>
#include <sml/sml-tools/ParameterManager.hh>

#include <sml/body/Sml.hh>
#include <sml/body/GeyerSml.hh>




/*!
 * \file Phase.hh
 * \author efx
 * \version 0.1
 * \brief Experiment phase description
 */
using namespace std;

class Phase{
public:
    bool last;
    string name;
    Sml * sml;
    int nb_cycle;
    bool restart_state;
    Phase(string name, Sml * sml, int nb_cycle);
    Phase(string name, Sml * sml, int nb_cycle, string option);
    void print_start();
    void print_end();
    virtual bool rule(vector<Phase *>::iterator * it) = 0;
    virtual void loadParameters() = 0;

};
class InitialPhase : public Phase{
public:
    InitialPhase(Sml * sml);
    bool rule(vector<Phase *>::iterator * it);
    void loadParameters();
};

class InitialPhaseFinder : public Phase{
public:
    InitialPhaseFinder(Sml * sml);
    bool rule(vector<Phase *>::iterator * it);
    void loadParameters();
};
class FullReflexPhase : public Phase{
public:
    string parameters;
    FullReflexPhase(Sml * sml, string parameters, int nb_cycle);
    FullReflexPhase(Sml * sml, int nb_cycle);
    FullReflexPhase(Sml * sml, string parameters, int nb_cycle, string option);
    FullReflexPhase(Sml * sml, int nb_cycle, string option,string parameters);
    FullReflexPhase(Sml * sml, int nb_cycle, string option);
    bool rule(vector<Phase *>::iterator * it);
    void loadParameters();
};
class SemiReflexPhase : public Phase{
public:
    bool end;
    string parameters;
    SemiReflexPhase(Sml * sml, std::string parameters, int nb_cycle);
    SemiReflexPhase(Sml * sml, int nb_cycle);
    SemiReflexPhase(Sml * sml, string parameters, int nb_cycle, string option);
    SemiReflexPhase(Sml * sml, int nb_cycle, std::string option);
    SemiReflexPhase(Sml * sml, int nb_cycle, std::string parameters, std::string option);
    bool rule(vector<Phase *>::iterator * it);
    void loadCpg();
    void loadParameters();
};



class PhaseManager {
public:
	static vector<Phase *> phases;
	static vector<Phase *>::iterator phase;
	static void init(Sml* sml);
	static void update();
};

#endif

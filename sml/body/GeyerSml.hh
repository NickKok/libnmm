#pragma once
#ifndef __GeyerSml_HH__
#define __GeyerSml_HH__

#include <sml/body/Sml.hh>
#include <setting-manager/parameters.hh>

class GeyerSml: public Sml
{
private:
	std::vector<std::string> reflex_modulated;
	std::vector<std::string> motoneuron_modulated;
	std::vector<const double *> sensor_to_modulator;
    std::vector<std::string> sensor_to_modulator_name;
    double motoneuron_scaling;
    double reflex_scaling;
public:
	ReflexController geyerController;
	TwitchingControllerWbts twitchingController;
	void loadFeedback(SIDE::Side side);
	void loadCpg(SIDE::Side side);
	void loadPartialCpg(SIDE::Side side);
	virtual void initialiseSpinalControl();
	virtual void initialiseSpinalControl_geyer();
	virtual void initialiseSpinalControl_twitching();
	virtual void initialiseModulators();
	virtual void step_SPINE_TO_MTU(double dt);
	// RL BRAIN
	Parameters brainModulationConfig;
	void brainModulation_reflex(int& rlAction_index);
	void brainModulation_motoneurons(int& rlAction_index);
	void addMusclePreStimulation();
	std::string getSensoryInformation();


};


#endif

#pragma once
#ifndef __TwitchingController_HH__
#define __TwitchingController_HH__

#include <sml/sml-tools/Helper.hh>
#include <setting-manager/parameters.hh>
#include <dynamics/controller.hh>


class TwitchingControllerWbts : public Controller {

public:
	//using Controller::Controller;
	TwitchingControllerWbts();
	Sml* body;
	void update();
	void step_IN_to_MN();
	void step_MN_to_output();
	void initialiseMotoneurons();
	//std::array<Motoneuron *, MUSCLES::NUMBER> motoneurons;

private:
	int time_step;

};

#endif

#ifndef __LOGGER_HH__
#define __LOGGER_HH__

#include <sml/sml-tools/Settings.hh>


using namespace std;

class Logger;
class LoggerManager
{
public:
	static map<string, Logger*> loggers;
	static void init();
	static void step();
	static void end();
};


class Logger
{
protected:
	string path;
	map<string, ofstream*> raw_files;
	vector<string> raw_filesname;

	const string Path = Settings::get<string>("log_nmm_path");
	const string Folder = Settings::get<string>("log_nmm_folder");
	const int LogOverwrite =Settings::get<int>("log_nmm_overwrite");
	const int LogOn =Settings::get<int>("log_nmm");

public:
	Logger(){};
	void closeFiles();
	void createFiles();
	virtual void writeHeader() = 0;
	virtual void writeContent() = 0;
};

class ReflexControllerLogger: public Logger
{
private: 
	ReflexController& controller;
public:
	ReflexControllerLogger(ReflexController& controller): controller(controller){
		raw_filesname = {
		"motoneurons_activity",
		"feedbacks",
		"cpgs",
		"interneurons"
		};
	}
	void writeHeader();
	void writeContent();
};

class JointLogger: public Logger
{
private: 
	Sml& body;
public:
	JointLogger(Sml& body): body(body){
		raw_filesname = {
		"joints_angle"
		};
	}
	void writeHeader();
	void writeContent();
};


class MusculoSkeletalLogger: public Logger
{
private: 
	Sml& body;
public:
	MusculoSkeletalLogger(Sml& body): body(body){
		raw_filesname = {
		"muscles_force",
		"muscles_length",
		"muscles_l_se",
		"muscles_v_CE",
		"muscles_F_CE",
		"muscles_f_v",
		"muscles_activity",
		"muscles_noise",
		"muscles_stim",
		"joints_force",
		"joints_angle",
		"joints_position_y",
		"joints_position_z",
		"sensors_activity"
		};
	}
	void writeHeader();
	void writeContent();
};


#endif

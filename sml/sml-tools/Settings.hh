#pragma once
#ifndef __SETTINGS_HH__
#define __SETTINGS_HH__

#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/variant.hpp>
#include <boost/algorithm/string.hpp>
#include <sml/sml-tools/Optimization.hh>
#include <sml/types/types.h>

typedef boost::variant< double, int, std::string > double_int_string;
class Settings
{
private:
    boost::property_tree::ptree pt;
public:
	static std::map<std::string, double_int_string> sets;
	static std::string loadedFrom;
	static std::string config;
    static std::string filename;
	static std::string prefix;
  static std::string config_path;
    void load_default_settings();
    template<class T> static T get(std::string str);
	template<class T> static void set(std::string str, T);
	void static printAll();
	Settings();
	Settings(std::string str);

	void init();
	std::string load_setting_from_opti(std::string setting_name, std::string setting_value);
	double_int_string load_setting_from_opti(std::string setting_name,  double_int_string setting_value);
    double_int_string load_setting_from_xml(std::string setting_name,  double_int_string setting_value);
    static bool isOptimizing();
    static bool isOptimization();
};

template<class T> T Settings::get(std::string str)
{
    return boost::get<T>(sets[str]);
}
template<class T> void Settings::set(std::string str, T value)
{
	sets[str] = value;
}
typedef std::map<std::string, double> parametersSet;
class SmlParameters
{
private:
	std::vector<parametersSet> parameters;
	std::vector<parametersSet> parameters_min;
	std::vector<parametersSet> parameters_max;
public:

	int numberOfSets;
	SmlParameters();
	SmlParameters(int);
	void init();
	void initDefaultValue(int i);
	void getNames(int i,std::vector<std::string>&);
	void initNames(int i);
	parametersSet& operator [](int i);
	void printAll();
	parametersSet& min(int i){
		return parameters_min[i];
	}
	parametersSet& max(int i){
		return parameters_max[i];
	}
	void loadAssymetricParamValue(int,std::string, double);
    void loadParamValue(int,std::string, double);

  void loadParam_fromvec(std::vector<double> parameters, std::vector<std::string> parametersName, int i);
	void loadParam_fromfile(std::string file, int i);
	void loadRange_fromfile(std::string file, int i);
	void loadParam_fromopti(int i);
};

#endif /* __SETTINGS_HH__ */

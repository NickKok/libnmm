#ifndef __PARAMETERMANAGER_HH__
#define __PARAMETERMANAGER_HH__

#include <string>


class ParameterManager
{
private:
	ParameterManager();

public:
	static void loadLaunchingReflexParameters(std::string name);
	static void loadCpgParameters();
	static void loadCpgParametersFromOpti();
	static void loadCpgParametersFromFile(std::string file);
	static void loadReflexParameters();
	static void loadReflexParametersFromOpti();
	static void loadReflexParametersFromFile(std::string file);
};

#endif

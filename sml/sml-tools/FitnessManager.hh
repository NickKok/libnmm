#ifndef __FITNESSMANAGER_HH__
#define __FITNESSMANAGER_HH__

#include <map>
#include <vector>
#include <string>

using namespace std;

class Sml;
class FitnessManager
{
	static map<string, vector<double>> data_joint_angles;
	static map<string, vector<double>> data_joint_torques;
	static vector<double> data_leftTO;
	static vector<double> data_rightTO;
	static vector<double> data_time;
	static vector<double> data_steplength;
	static vector<double> data_steplength_left;
	static vector<double> data_steplength_right;


	static int changeVectorSize(vector<double>& out, vector<double>& x, vector<double>& y, long start, long end, int N_out);
	static double getJointsTorqueCorrelationWithHuman(string joint);
    static double getJointsAngleCorrelationWithHuman(string joint);
	static double getTorqueSum(string joint);
	static double getSNRWithHuman(string joint);
public:
	static void step(Sml * body);
	static std::map<std::string,double> buildFitness();
	static void saveToFile();	
};

#endif

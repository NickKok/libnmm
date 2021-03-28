#include "FitnessManager.hh"

#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <cmath>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/any.hpp>

#include <codecogs/statistics/moments/correlation.h>
#include <codecogs/statistics/moments/snr.h>


#include <sml/body/Sml.hh>

#include <sml/sml-tools/Settings.hh>
#include <sml/sml-tools/EventManager.hh>

extern EventManager * eventManager;



map<string, vector<double>> FitnessManager::data_joint_angles;
map<string, vector<double>> FitnessManager::data_joint_torques;
vector<double> FitnessManager::data_leftTO;
vector<double> FitnessManager::data_rightTO;
vector<double> FitnessManager::data_time;
vector<double> FitnessManager::data_steplength;
vector<double> FitnessManager::data_steplength_left;
vector<double> FitnessManager::data_steplength_right;


/**
 * HUMAN DATA
 */
#define ETH_WEROB2014
#ifdef WINTER
double human_hip_angles_right[51] = {-15.73, -14.69, -13.52, -12.49, -11.60, -10.55, -9.14, -7.54, -6.06, -4.77, -3.50, -2.19, -0.94, 0.16, 1.20, 2.27, 3.34, 4.35, 5.24, 6.07, 6.89, 7.69, 8.43, 9.12, 9.77, 10.39, 10.93, 11.27, 11.05, 9.95, 7.96, 5.27, 2.23, -0.86, -3.80, -6.45, -8.84, -11.04, -13.06, -14.80, -16.20, -17.27, -18.07, -18.53, -18.55, -18.17, -17.55, -16.88, -16.30, -15.90, -15.53};
double human_knee_angles_right[51] = {3.74, 5.96, 8.33, 10.88, 13.39, 15.31, 16.20, 16.20, 15.75, 15.07, 14.16, 13.10, 12.04, 11.10, 10.28, 9.54, 8.93, 8.47, 8.23, 8.21, 8.36, 8.71, 9.33, 10.25, 11.53, 13.21, 15.47, 18.54, 22.74, 28.32, 35.05, 42.28, 49.12, 54.89, 59.14, 61.71, 62.55, 61.77, 59.52, 55.96, 51.26, 45.57, 39.10, 31.99, 24.44, 16.91, 10.13, 4.98, 2.11, 1.73, 3.21};
double human_ankle_angles_right[51] = {0.57, 2.83, 5.27, 6.46, 5.91, 4.18, 2.19, 0.46, -0.96, -2.13, -3.07, -3.79, -4.35, -4.84, -5.34, -5.84, -6.29, -6.71, -7.16, -7.66, -8.11, -8.43, -8.56, -8.46, -8.03, -7.14, -5.55, -3.06, 0.34, 4.47, 8.91, 13.12, 16.27, 17.55, 16.64, 14.00, 10.48, 6.91, 3.81, 1.34, -0.51, -1.59, -1.72, -1.11, -0.25, 0.34, 0.45, 0.19, -0.14, -0.04, 0.89};

double human_hip_angles_left[51] = {-15.73, -14.69, -13.52, -12.49, -11.60, -10.55, -9.14, -7.54, -6.06, -4.77, -3.50, -2.19, -0.94, 0.16, 1.20, 2.27, 3.34, 4.35, 5.24, 6.07, 6.89, 7.69, 8.43, 9.12, 9.77, 10.39, 10.93, 11.27, 11.05, 9.95, 7.96, 5.27, 2.23, -0.86, -3.80, -6.45, -8.84, -11.04, -13.06, -14.80, -16.20, -17.27, -18.07, -18.53, -18.55, -18.17, -17.55, -16.88, -16.30, -15.90, -15.53};
double human_knee_angles_left[51] = {3.74, 5.96, 8.33, 10.88, 13.39, 15.31, 16.20, 16.20, 15.75, 15.07, 14.16, 13.10, 12.04, 11.10, 10.28, 9.54, 8.93, 8.47, 8.23, 8.21, 8.36, 8.71, 9.33, 10.25, 11.53, 13.21, 15.47, 18.54, 22.74, 28.32, 35.05, 42.28, 49.12, 54.89, 59.14, 61.71, 62.55, 61.77, 59.52, 55.96, 51.26, 45.57, 39.10, 31.99, 24.44, 16.91, 10.13, 4.98, 2.11, 1.73, 3.21};
double human_ankle_angles_left[51] = {0.57, 2.83, 5.27, 6.46, 5.91, 4.18, 2.19, 0.46, -0.96, -2.13, -3.07, -3.79, -4.35, -4.84, -5.34, -5.84, -6.29, -6.71, -7.16, -7.66, -8.11, -8.43, -8.56, -8.46, -8.03, -7.14, -5.55, -3.06, 0.34, 4.47, 8.91, 13.12, 16.27, 17.55, 16.64, 14.00, 10.48, 6.91, 3.81, 1.34, -0.51, -1.59, -1.72, -1.11, -0.25, 0.34, 0.45, 0.19, -0.14, -0.04, 0.89};

#else // ETH_WEROB2014
double human_hip_angles_left[51] = {-40.71, -40.77, -40.88, -40.83, -40.41, -39.46, -38.0, -36.14, -34.02, -31.81, -29.6, -27.46, -25.4, -23.39, -21.41, -19.41, -17.41, -15.4, -13.42, -11.48, -9.64, -7.95, -6.48, -5.32, -4.58, -4.35, -4.71, -5.66, -7.15, -9.11, -11.45, -14.08, -16.89, -19.79, -22.71, -25.58, -28.38, -31.07, -33.57, -35.78, -37.6, -38.92, -39.74, -40.13, -40.27, -40.37, -40.56, -40.77, -40.86, -40.82, -40.77};
double human_knee_angles_left[51] = {16.05, 19.05, 22.32, 25.21, 27.25, 28.25, 28.31, 27.62, 26.39, 24.83, 23.09, 21.24, 19.32, 17.33, 15.28, 13.23, 11.27, 9.49, 7.96, 6.75, 5.93, 5.57, 5.79, 6.74, 8.57, 11.38, 15.19, 19.84, 25.00, 30.24, 35.22, 39.82, 44.14, 48.18, 51.77, 54.59, 56.36, 56.92, 56.19, 54.12, 50.64, 45.80, 39.95, 33.75, 27.94, 22.90, 18.67, 15.43, 13.67, 13.73, 15.43};
double human_ankle_angles_left[51] = {5.34, 3.86, 1.43, -1.34, -3.93, -6.05, -7.6, -8.67, -9.39, -9.89, -10.25, -10.5, -10.67, -10.77, -10.84, -10.92, -11.04, -11.25, -11.58, -12.01, -12.51, -13.07, -13.66, -14.21, -14.61, -14.61, -13.83, -11.85, -8.46, -3.95, 0.84, 4.82, 7.21, 7.76, 6.77, 4.91, 2.97, 1.44, 0.34, -0.6, -1.63, -2.75, -3.74, -4.24, -3.97, -2.86, -1.06, 1.18, 3.45, 5.08, 5.4};

double human_hip_angles_right[51] = {-48.36, -47.61, -46.62, -45.39, -43.93, -42.26, -40.43, -38.51, -36.56, -34.64, -32.77, -30.95, -29.16, -27.38, -25.61, -23.84, -22.09, -20.34, -18.61, -16.87, -15.14, -13.42, -11.75, -10.2, -8.87, -7.88, -7.35, -7.39, -8.08, -9.46, -11.49, -14.1, -17.17, -20.57, -24.16, -27.8, -31.39, -34.83, -38.01, -40.83, -43.23, -45.18, -46.69, -47.79, -48.55, -49.03, -49.3, -49.38, -49.28, -49.0, -48.52};
double human_knee_angles_right[51] = {25.90, 27.42, 28.79, 29.72, 30.10, 29.93, 29.37, 28.59, 27.75, 26.92, 26.11, 25.27, 24.36, 23.34, 22.21, 21.01, 19.77, 18.52, 17.27, 16.05, 14.88, 13.84, 13.03, 12.59, 12.72, 13.60, 15.43, 18.31, 22.33, 27.45, 33.51, 40.19, 47.04, 53.52, 59.09, 63.32, 65.95, 66.92, 66.31, 64.24, 60.85, 56.29, 50.75, 44.59, 38.37, 32.75, 28.33, 25.47, 24.22, 24.32, 25.37};
double human_ankle_angles_right[51] = {-6.13, -6.24, -6.69, -7.46, -8.4, -9.36, -10.27, -11.13, -11.96, -12.78, -13.56, -14.28, -14.91, -15.46, -15.96, -16.42, -16.85, -17.26, -17.64, -17.98, -18.31, -18.64, -19.03, -19.51, -20.04, -20.47, -20.49, -19.66, -17.57, -14.06, -9.48, -4.77, -0.96, 1.36, 2.15, 1.76, 0.67, -0.71, -2.09, -3.36, -4.49, -5.45, -6.19, -6.67, -6.9, -6.93, -6.85, -6.7, -6.52, -6.3, -6.13};
#endif


double human_hip_torques_right[51] = {0.249, 0.600, 0.556, 0.416, 0.359, 0.305, 0.245, 0.159, 0.084, 0.000, -0.064, -0.092, -0.098, -0.092, -0.085, -0.088, -0.100, -0.130, -0.168, -0.199, -0.231, -0.269, -0.312, -0.364, -0.401, -0.404, -0.356, -0.262, -0.251, -0.310, -0.344, -0.295, -0.228, -0.169, -0.126, -0.089, -0.069, -0.057, -0.044, -0.026, -0.009, 0.008, 0.029, 0.060, 0.106, 0.170, 0.242, 0.296, 0.301, 0.237, 0.118};
double human_knee_torques_right[51] = {-0.196, -0.281, -0.090, 0.173, 0.362, 0.508, 0.593, 0.615, 0.556, 0.469, 0.362, 0.244, 0.141, 0.052, -0.019, -0.070, -0.114, -0.149, -0.181, -0.217, -0.247, -0.269, -0.270, -0.237, -0.171, -0.087, -0.004, 0.054, 0.116, 0.157, 0.156, 0.114, 0.080, 0.066, 0.064, 0.053, 0.037, 0.020, 0.004, -0.009, -0.023, -0.040, -0.059, -0.082, -0.114, -0.158, -0.211, -0.253, -0.263, -0.224, -0.147};
double human_ankle_torques_right[51] = {-0.009, -0.034, -0.064, -0.051, 0.028, 0.143, 0.260, 0.368, 0.469, 0.545, 0.601, 0.650, 0.692, 0.736, 0.780, 0.825, 0.881, 0.951, 1.037, 1.144, 1.260, 1.388, 1.513, 1.608, 1.628, 1.565, 1.388, 1.073, 0.690, 0.335, 0.102, -0.001, -0.028, -0.023, -0.019, -0.015, -0.012, -0.010, -0.010, -0.010, -0.011, -0.012, -0.013, -0.013, -0.011, -0.006, 0.001, 0.007, 0.011, 0.010, 0.004};

double human_hip_torques_left[51] = {0.249, 0.600, 0.556, 0.416, 0.359, 0.305, 0.245, 0.159, 0.084, 0.000, -0.064, -0.092, -0.098, -0.092, -0.085, -0.088, -0.100, -0.130, -0.168, -0.199, -0.231, -0.269, -0.312, -0.364, -0.401, -0.404, -0.356, -0.262, -0.251, -0.310, -0.344, -0.295, -0.228, -0.169, -0.126, -0.089, -0.069, -0.057, -0.044, -0.026, -0.009, 0.008, 0.029, 0.060, 0.106, 0.170, 0.242, 0.296, 0.301, 0.237, 0.118};
double human_knee_torques_left[51] = {-0.196, -0.281, -0.090, 0.173, 0.362, 0.508, 0.593, 0.615, 0.556, 0.469, 0.362, 0.244, 0.141, 0.052, -0.019, -0.070, -0.114, -0.149, -0.181, -0.217, -0.247, -0.269, -0.270, -0.237, -0.171, -0.087, -0.004, 0.054, 0.116, 0.157, 0.156, 0.114, 0.080, 0.066, 0.064, 0.053, 0.037, 0.020, 0.004, -0.009, -0.023, -0.040, -0.059, -0.082, -0.114, -0.158, -0.211, -0.253, -0.263, -0.224, -0.147};
double human_ankle_torques_left[51] = {-0.009, -0.034, -0.064, -0.051, 0.028, 0.143, 0.260, 0.368, 0.469, 0.545, 0.601, 0.650, 0.692, 0.736, 0.780, 0.825, 0.881, 0.951, 1.037, 1.144, 1.260, 1.388, 1.513, 1.608, 1.628, 1.565, 1.388, 1.073, 0.690, 0.335, 0.102, -0.001, -0.028, -0.023, -0.019, -0.015, -0.012, -0.010, -0.010, -0.010, -0.011, -0.012, -0.013, -0.013, -0.011, -0.006, 0.001, 0.007, 0.011, 0.010, 0.004};




std::map<std::string,double> FitnessManager::buildFitness(){
    std::map<std::string,double> fitnesses = eventManager->getMap();
	fitnesses["trunk_mean_angle"] = Stats::Moments::mean<double>(data_joint_angles["trunk"].size()-1,&data_joint_angles["trunk"][0]);
    fitnesses["trunk_variance_angle"] = Stats::Moments::variance<double>(data_joint_angles["trunk"].size()-1,&data_joint_angles["trunk"][0]);

	fitnesses["human_ankle_correlation_left"] = FitnessManager::getJointsAngleCorrelationWithHuman("ANKLE_LEFT");
	fitnesses["human_knee_correlation_left"] = FitnessManager::getJointsAngleCorrelationWithHuman("KNEE_LEFT");
	fitnesses["human_hip_correlation_left"] = FitnessManager::getJointsAngleCorrelationWithHuman("HIP_LEFT");
    fitnesses["human_ankle_correlation_right"] = FitnessManager::getJointsAngleCorrelationWithHuman("ANKLE_RIGHT");
    fitnesses["human_knee_correlation_right"] = FitnessManager::getJointsAngleCorrelationWithHuman("KNEE_RIGHT");
    fitnesses["human_hip_correlation_right"] = FitnessManager::getJointsAngleCorrelationWithHuman("HIP_RIGHT");

    fitnesses["human_ankle_correlation"] = 0.5* (fitnesses["human_ankle_correlation_right"] + fitnesses["human_ankle_correlation_left"]);
    fitnesses["human_knee_correlation"] = 0.5* (fitnesses["human_knee_correlation_right"] + fitnesses["human_knee_correlation_left"]);
    fitnesses["human_hip_correlation"] = 0.5* (fitnesses["human_hip_correlation_right"] + fitnesses["human_hip_correlation_left"]);

	fitnesses["ankle_torque_sum"] = (FitnessManager::getTorqueSum("ANKLE_LEFT") + FitnessManager::getTorqueSum("ANKLE_RIGHT"))/fitnesses["duration"]/1000.0;
	fitnesses["knee_torque_sum"] = (FitnessManager::getTorqueSum("KNEE_LEFT") + FitnessManager::getTorqueSum("KNEE_RIGHT"))/fitnesses["duration"]/1000.0;
	fitnesses["hip_torque_sum"] = (FitnessManager::getTorqueSum("HIP_LEFT") + FitnessManager::getTorqueSum("HIP_RIGHT"))/fitnesses["duration"]/1000.0;

	fitnesses["ankle_SNR"] = 0.0; //FitnessManager::getSNRWithHuman("ANKLE_LEFT");
	fitnesses["knee_SNR"] = 0.0; //FitnessManager::getSNRWithHuman("KNEE_LEFT");
	fitnesses["hip_SNR"] = 0.0; //FitnessManager::getSNRWithHuman("HIP_LEFT");
	if(data_steplength.size()-1 > 15){
		fitnesses["steplengthSNR"] = Stats::Moments::snr<double>(data_steplength.size()-1,&data_steplength[1]);

		double left_mean = Stats::Moments::mean<double>(data_steplength_left.size()-1,&data_steplength_left[1]);
		double right_mean = Stats::Moments::mean<double>(data_steplength_right.size()-1,&data_steplength_right[1]);
		double left_var = sqrt(Stats::Moments::variance<double>(data_steplength_left.size()-1,&data_steplength_left[1]));
		double right_var = sqrt(Stats::Moments::variance<double>(data_steplength_right.size()-1,&data_steplength_right[1]));
		fitnesses["symmetry"] = -log(1+fabs((left_mean - right_mean)/((left_mean + right_mean)/2)))-log(1+left_var)-log(1+right_var);
	}
	else{
		fitnesses["steplengthSNR"] = 0.0;
        fitnesses["symmetry"] = -1000;
		//state.steplengthSNR_leftright = 0.0;
	}
	for (auto &kv : fitnesses)
    {
        cout << kv.first << " : " << kv.second << endl;
    }
    return fitnesses;
}


void FitnessManager::step(Sml * body)
{
	if(!eventManager->get<bool>(STATES::IS_LAUNCHING_PHASE) && eventManager->get<bool>(STATES::IS_LAST_PHASE)){
		if(FitnessManager::data_time.size() != 0)
			FitnessManager::data_time.push_back(FitnessManager::data_time.back()+body->dt);
		else
			FitnessManager::data_time.push_back(0.0);
		if(body->left_foot->justTouchTheGround())
			FitnessManager::data_steplength.push_back( eventManager->get<double>(STATES::LEFT_STEP_LENGTH));

		FitnessManager::data_joint_angles["trunk"].push_back(body->basalGanglia->trunk->theta);
        for(auto& joint : body->joints){
            FitnessManager::data_joint_angles[joint->getName()].push_back(joint->getAngle()*180/PI);
            //if(kv.first.find("KNEE") != string::npos)
            FitnessManager::data_joint_torques[joint->getName()].push_back(1./80.*(joint->torque + joint->torque_soft_limit));
        }

		if(body->left_foot->justTouchTheGround()){
			//cout << "left " << body->left_foot->distance_last_step << endl;
			FitnessManager::data_leftTO.push_back(data_joint_angles["HIP_LEFT"].size()-1);
			FitnessManager::data_steplength_left.push_back(body->left_foot->duration_last_step);
		}
		if(body->right_foot->justTouchTheGround()){
			//cout << "right " << body->right_foot->distance_last_step << endl;
			FitnessManager::data_rightTO.push_back(data_joint_angles["HIP_LEFT"].size()-1);
			FitnessManager::data_steplength_right.push_back(body->right_foot->duration_last_step);
		}
	}
}
int FitnessManager::changeVectorSize(vector<double>& out, vector<double>& x, vector<double>& y, long start, long end, int N_out)
{

    double pos;
    double a;
    double b;
    for(int j=0;j<N_out;j++)
    {
        pos = start + (double)j*(double)(end-start)/(double)N_out;
        a=(y[ceil(pos)]-y[floor(pos)]);
        b=y[floor(pos)]-a*floor(pos);

        out.push_back(a*pos+b);
    }
    return 1;
}
double FitnessManager::getSNRWithHuman(std::string joint)
{
	// number of point of the human data
	int N_out = 51;
	double snr=-10.0;
	if( data_leftTO.size() <= 15 || data_rightTO.size() <= 15)
		return snr;
	double total_snr = 0.0;

	unsigned int start = 15;
	unsigned int i = start;
	int n = 0;
	while( i<=data_leftTO.size()-1){
		long start = data_leftTO[i-1];
		long end = data_leftTO[i]-1;
		vector<double> robot_angle;
		if(end-start > 100){
			double * human_angle = new double[N_out];
			if(changeVectorSize(robot_angle, data_time, data_joint_angles[joint], start, end, N_out) == 1){
				if( joint == "ANKLE_LEFT") 	    human_angle = human_ankle_angles_left;
				if( joint == "KNEE_LEFT")	    human_angle = human_knee_angles_left;
				if( joint == "HIP_LEFT")	    human_angle = human_hip_angles_left;
                if( joint == "ANKLE_RIGHT" )    human_angle = human_ankle_angles_right;
                if( joint == "KNEE_RIGHT" )     human_angle = human_knee_angles_right;
                if( joint == "HIP_RIGHT" )      human_angle = human_hip_angles_right;
				total_snr += Stats::Moments::snr(N_out, human_angle, &robot_angle[0]);
				n += 1;
			}
		}
		i++;
	}
	//1 Extract the cycle begin and end;

	//2 Extract the subpart of interest
	//changeVectorSize(robot_angle, data_time, data_joint_torques[joint], start, end, N_out);

	//cout << endl;
	//for(int i=0;i<N_out;i++)
	//    cout << robot_angle[i] << " ";
	//cout << endl;
	return total_snr/((double)(i-15));
}
/*
 * cout << joint << " : " << snr << endl ;
 * cout << joint << " = [ ";
 * for ( auto &it : robot_angle )
 *	cout << it << " " ;
 * cout << "]; " << endl;
 * cout << "h" << joint << " = [ ";
 * for (int i=0; i<N_out; i++ )
 *	cout <<  human_angle[i] << " ";
 * cout << "]; " << endl;
 */

double FitnessManager::getTorqueSum(std::string joint){
	std::vector<double> & torque = data_joint_torques[joint];
	double total_torque = 0.0;
	for(auto &kv: torque){
		total_torque += abs(kv);
	}
	return total_torque;
}
double FitnessManager::getJointsAngleCorrelationWithHuman(std::string joint)
{
    // number of point of the human data
    int N_out = 51;
    double corr=0.0;
    if( data_leftTO.size() <= 15 || data_rightTO.size() <= 15)
        return corr;
    double total_corr = 0.0;

    unsigned int start = 15;
    unsigned int i = start;
    int n = 0;

    //cout << joint << endl;
    vector<double>* p_data_sideTO;
    if (joint == "ANKLE_RIGHT" || joint == "KNEE_RIGHT" || joint == "HIP_RIGHT")
        p_data_sideTO = &data_rightTO;
    else
        p_data_sideTO = &data_leftTO;
    vector<double>& data_sideTO = *p_data_sideTO;

    while( i<=data_sideTO.size()-1){
        //1 Extract the cycle begin and end;
        long start = data_sideTO[i-1];
        long end = data_sideTO[i]-1;
        vector<double> robot_angle;
        //2 Extract the subpart of interest
        if(end-start > 100){
            if(changeVectorSize(robot_angle, data_time, data_joint_angles[joint], start, end, N_out) == 1){
                //3 Compute correlation
                if (joint == "ANKLE_LEFT")
                    corr = fabs(Stats::Moments::correlation<double>(N_out, &robot_angle[0] , human_ankle_angles_left));
                if (joint == "ANKLE_RIGHT")
                    corr = fabs(Stats::Moments::correlation<double>(N_out, &robot_angle[0] , human_ankle_angles_right));
                if (joint == "KNEE_LEFT")
                    corr = fabs(Stats::Moments::correlation<double>(N_out, &robot_angle[0] , human_knee_angles_left));
                if (joint == "KNEE_RIGHT")
                    corr = fabs(Stats::Moments::correlation<double>(N_out, &robot_angle[0] , human_knee_angles_right));
                if (joint == "HIP_LEFT")
                    corr = fabs(Stats::Moments::correlation<double>(N_out, &robot_angle[0] , human_hip_angles_left));
                if (joint == "HIP_RIGHT")
                    corr = fabs(Stats::Moments::correlation<double>(N_out, &robot_angle[0] , human_hip_angles_right));
                //cout << i << " " << corr << endl;
                //if(i==22){
                //    for(int j=0;j<N_out;j++)
                //        cout << data_joint_torques[joint][start+int(j*(end-start)/N_out)] << " " << human_ankle_torques_left[j] << endl;
                //}
                total_corr += corr;
                n += 1;
            }
        }
        i++;
    }
    if(n!=0)
        return total_corr/((double)(n));
    else
        return 0.0;
}
double FitnessManager::getJointsTorqueCorrelationWithHuman(std::string joint)
{
	// number of point of the human data
	int N_out = 51;
	double corr=0.0;
	if( data_leftTO.size() <= 15 || data_rightTO.size() <= 15)
		return corr;
	double total_corr = 0.0;

	unsigned int start = 15;
	unsigned int i = start;
	int n = 0;
    cout << joint << endl;
	while( i<=data_leftTO.size()-1){
		//1 Extract the cycle begin and end;
		long start = data_leftTO[i-1];
		long end = data_leftTO[i]-1;
		vector<double> robot_torque;
		//2 Extract the subpart of interest
		if(end-start > 100){
			if(changeVectorSize(robot_torque, data_time, data_joint_torques[joint], start, end, N_out) == 1){
				//3 Compute correlation
                if (joint == "ANKLE_LEFT")
                    corr = fabs(Stats::Moments::correlation<double>(N_out, &robot_torque[0] , human_ankle_torques_left));
                if (joint == "ANKLE_RIGHT")
                    corr = fabs(Stats::Moments::correlation<double>(N_out, &robot_torque[0] , human_ankle_torques_right));
                if (joint == "KNEE_LEFT")
                    corr = fabs(Stats::Moments::correlation<double>(N_out, &robot_torque[0] , human_knee_torques_left));
                if (joint == "KNEE_RIGHT")
                    corr = fabs(Stats::Moments::correlation<double>(N_out, &robot_torque[0] , human_knee_torques_right));
                if (joint == "HIP_LEFT")
                    corr = fabs(Stats::Moments::correlation<double>(N_out, &robot_torque[0] , human_hip_torques_left));
                if (joint == "HIP_RIGHT")
                    corr = fabs(Stats::Moments::correlation<double>(N_out, &robot_torque[0] , human_hip_torques_right));
                //cout << i << " " << corr << endl;
                if(i==22){
                    for(int j=0;j<N_out;j++)
                        cout << data_joint_torques[joint][start+int(j*(end-start)/N_out)] << " " << human_ankle_torques_left[j] << endl;

                }
				total_corr += corr;
				n += 1;
			}
		}
		i++;
	}
	if(n!=0)
		return total_corr/((double)(n));
	else
		return 0.0;
}

void FitnessManager::saveToFile(){
	//not compatible with number_of_repeat !=1

	/*
	 *	extract = base
	 *	>> cat extractoutput
	 *	experimenttype modeltype 	corr hip	corr knee	corr ankle	energy	distance
	 *
	 *	extract = force
	 *	>> cat extractoutput
	 *	experimenttype modeltype 	distance 	last force amp	last force pos		last force angle
	 *
	 *	extract = distance
	 *	>> cat extractoutput
	 *	experimenttype modeltype	distance
	 *
	 *	extract = rp_eval
	 *	experimenttype modeltype	force 	passed (yes/no)
	 *
	 *	extract = wg_eval
	 *	experimenttype modeltype	slope	passed (yes /no)
	 */
	string what = Settings::get<string>("extract");
	std::map<std::string,double> fitnesses = FitnessManager::buildFitness();
	ofstream *file;
	file = new ofstream();
	file->open("../../../evaluation/data/" + Settings::get<string>("extractoutput"), ios::app);
	*file << Settings::get<string>("experimenttype") << "\t" << Settings::get<string>("modeltype") << "\t" << Settings::get<string>("worldname") << "\t" ;
	if(what == "base"){
		*file <<
			fitnesses["distance"] << "\t" <<
			fitnesses["human_hip_correlation"] << "\t" <<
			fitnesses["human_knee_correlation"] << "\t" <<
			fitnesses["human_ankle_correlation"] << "\t" <<
			fitnesses["energy"];
	}
	else if(what == "force"){
		if(fitnesses["fall"])
			*file <<
			fitnesses["distance"] << "\t" <<
			eventManager->dynamicState.getVector<double>("lastForce")[0] << "\t" <<
			eventManager->dynamicState.getVector<double>("lastForce")[1] << "\t" <<
			eventManager->dynamicState.getVector<double>("lastPos")[0] << "\t" <<
			!fitnesses["fall"];
		else
			*file <<
				fitnesses["distance"] << "\t" <<
				eventManager->dynamicState.getVector<double>("currentForce")[0] << "\t" <<
				eventManager->dynamicState.getVector<double>("currentForce")[1] << "\t" <<
				eventManager->dynamicState.getVector<double>("currentPos")[0] << "\t" <<
				!fitnesses["fall"];
	}
	else if(what == "distance"){
		*file << fitnesses["distance"] << "\t" << !fitnesses["fall"];
	}
	else if(what == "rp_eval"){
		*file << Settings::get<double>("force") << "\t" << !fitnesses["fall"];
	}
	else if(what == "wg_eval"){
	}
	*file << endl;
	file->close();
}

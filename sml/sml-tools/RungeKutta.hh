#pragma once
#ifndef DERIVATIVES_HH
#define DERIVATIVES_HH // <-- same string as above line

/*!
 * \file RungeKutta.hh
 * \brief Numerical integrator (based on rungeKutta)
 * \author efx
 * \version 0.1
 */

#include <vector>

class Derivatives{
public:
	virtual std::vector<double> dydt(double x, std::vector<double> y, std::vector<double> param, std::vector<double> input) = 0;
};


inline std::vector<double> rungeKutta4(
	double xi,
	double dx,
	std::vector<double> yi,
	std::vector<double> param,
	std::vector<double> input,
	Derivatives *diff) {
	
	//	total number of elements in the vector
	int n=yi.size();
	
	//	first step
	std::vector<double> k1;
	k1=diff->dydt(xi, yi, param, input);
	for (int i=0; i<n; ++i) {
		k1.at(i)*=dx;
	}
	
	//	second step
	std::vector<double> k2(yi);
	for (int i=0; i<n; ++i) {
		k2.at(i)+=k1.at(i)/2.0;
	}
	k2=diff->dydt(xi+dx/2.0,k2, param, input);
	for (int i=0; i<n; ++i) {
		k2.at(i)*=dx;
	}
	
	//	third step
	std::vector<double> k3(yi);
	for (int i=0; i<n; ++i) {
		k3.at(i)+=k2.at(i)/2.0;
	}
	k3=diff->dydt(xi+dx/2.0,k3, param, input);
	for (int i=0; i<n; ++i) {
		k3.at(i)*=dx;
	}
	
	
	//	fourth step
	std::vector<double> k4(yi);
	for (int i=0; i<n; ++i) {
		k4.at(i)+=k3.at(i);
	}
	k4=diff->dydt(xi+dx,k4, param, input);
	for (int i=0; i<n; ++i) {
		k4.at(i)*=dx;
	}
	
	
	//	sum the weighted steps into yf and return the final y values
	std::vector<double> yf(yi);
	for (int i=0; i<n; ++i) {
		yf.at(i)+=(k1.at(i)/6.0)+(k2.at(i)/3.0)+(k3.at(i)/3.0)+(k4.at(i)/6.0);
	}
	
	return yf;
	}
	
	inline std::vector<double> rungeKutta4(
		double xi,
		double dx,
		std::vector<double> yi,
		std::vector<double> input,
		Derivatives *diff) {
		std::vector<double> param(1,0.0);
		return rungeKutta4(xi, dx, yi, param, input, diff);
		}
		
		#endif
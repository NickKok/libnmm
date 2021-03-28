// GNU General Public License Agreement
// Copyright (C) 2004-2010 CodeCogs, Zyba Ltd, Broadwood, Holford, TA5 1DU, England.
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by CodeCogs. 
// You must retain a copy of this licence in all copies. 
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
// ---------------------------------------------------------------------------------
//! Calculates the arithmetic mean of a given population

#ifndef STATS_MOMENTS_SNR_H
#define STATS_MOMENTS_SNR_H
#include <iostream>
namespace Stats
{

namespace Moments
{

//! Calculates the signal to noise ration given population
double * sub (int N, double * x1, double * x2)
{
	double *y  = new double[N];
	for (int i=0;i<N;i++){
		y[i] = x1[i]-x2[i];
	}
	return y;
}
double * sum (int N, double * x1, double * x2)
{
	double *y  = new double[N];
	for (int i=0;i<N;i++)
		y[i] = x1[i]+x2[i];
	return y;
}
double * mult(int N, double * x, int exp)
{
	double *y  = new double[N];
	for (int i=0;i<N;i++){
		y[i] = x[i];
		for (int j=0;j<exp-1;j++)
			y[i] *= x[i];
	}
	return y;
}

template <class T>double snr(int n, T *data)
{
	return log(mean<double>(n, data)/variance<double>(n, data));
}

template <class T>double snr(int n, T *x_true, T *x)
{
	return log(
		Stats::Moments::variance<double>(n, x_true)/
		Stats::Moments::mean<double>(n,mult(n, sub(n,x_true,x),2))
	);
}

}

}

#endif


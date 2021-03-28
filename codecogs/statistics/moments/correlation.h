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
//! Calculates the correlation of a given set of data.

#ifndef STATS_MOMENTS_CORRELATION_H
#define STATS_MOMENTS_CORRELATION_H

#include<math.h>
#include<codecogs/statistics/moments/variance.h>
#include<codecogs/statistics/moments/covariance.h>

namespace Stats
{

namespace Moments
{

//! Calculates the correlation of a given set of data.

template <class T>inline double correlation(int n ,T *data ,T *data1)
{
  double var=variance<T>(n,data);
  double var1=variance<T>(n,data1);
  double covar=covariance<T>(n,data,data1);
  return covar/sqrt(var*var1);
}

}

}

#endif


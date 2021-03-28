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
//! Calculates the covariance of a given set of data

#ifndef STATS_MOMENTS_COVARIANCE_H
#define STATS_MOMENTS_COVARIANCE_H

#include <codecogs/statistics/moments/mean.h>

namespace Stats
{

namespace Moments
{

//! Calculates the covariance of a given set of data

template <class T>double covariance(int n, T *data, T *data1)
{
  double
  med=Stats::Moments::mean<T>(n,data),
  med1=Stats::Moments::mean<T>(n,data1),
  m=0;

  int i=n/8;
  while (i>0)
  {
    m += data[0]*data1[0] - med*med1;
    m += data[1]*data1[1] - med*med1;
    m += data[2]*data1[2] - med*med1;
    m += data[3]*data1[3] - med*med1;
    m += data[4]*data1[4] - med*med1;
    m += data[5]*data1[5] - med*med1;
    m += data[6]*data1[6] - med*med1;
    m += data[7]*data1[7] - med*med1;
    data += 8;
    data1+= 8;
    i--;
  }

  switch (n%8)
  {
    case 7: m += data[6]*data1[6] - med*med1;
    case 6: m += data[5]*data1[5] - med*med1;
    case 5: m += data[4]*data1[4] - med*med1;
    case 4: m += data[3]*data1[3] - med*med1;
    case 3: m += data[2]*data1[2] - med*med1;
    case 2: m += data[1]*data1[1] - med*med1;
    case 1: m += data[0]*data1[0] - med*med1;
  }
  return m/n;
}

}

}

#endif


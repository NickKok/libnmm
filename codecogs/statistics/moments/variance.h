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
//! Calculates the variance of a given set of data.

#ifndef STATS_MOMENTS_VARIANCE_H
#define STATS_MOMENTS_VARIANCE_H

#include <codecogs/statistics/moments/mean.h>

namespace Stats
{

namespace Moments
{

//! Calculates the variance of a given set of data.

template <class T>double variance(int n, T *data, bool total = false)
{
  double m=0, med=mean<T>(n,data);
  int i=n/8;
  while (i>0)
  {
    m += (data[0] - med)*(data[0] - med);
    m += (data[1] - med)*(data[1] - med);
    m += (data[2] - med)*(data[2] - med);
    m += (data[3] - med)*(data[3] - med);
    m += (data[4] - med)*(data[4] - med);
    m += (data[5] - med)*(data[5] - med);
    m += (data[6] - med)*(data[6] - med);
    m += (data[7] - med)*(data[7] - med);
    data += 8;
    i--;
  }

  switch (n%8)
  {
    case 7: m += (data[6] - med)*(data[6] - med);
    case 6: m += (data[5] - med)*(data[5] - med);
    case 5: m += (data[4] - med)*(data[4] - med);
    case 4: m += (data[3] - med)*(data[3] - med);
    case 3: m += (data[2] - med)*(data[2] - med);
    case 2: m += (data[1] - med)*(data[1] - med);
    case 1: m += (data[0] - med)*(data[0] - med);
  }
  if (total) n++;
  return m/(n-1);
}

}

}

#endif


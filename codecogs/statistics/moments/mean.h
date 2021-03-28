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

#ifndef STATS_MOMENTS_MEAN_H
#define STATS_MOMENTS_MEAN_H

namespace Stats
{

namespace Moments
{

//! Calculates the arithmetic mean of a given population

template <class T>double mean(int n, T *data)
{
  double m=0;
  int i=n/8;
  while (i>0)
  {
    m += data[0];
    m += data[1];
    m += data[2];
    m += data[3];
    m += data[4];
    m += data[5];
    m += data[6];
    m += data[7];
    data += 8;
    i--;
  }

  switch (n%8)
  {
    case 7: m+=data[6];
    case 6: m+=data[5];
    case 5: m+=data[4];
    case 4: m+=data[3];
    case 3: m+=data[2];
    case 2: m+=data[1];
    case 1: m+=data[0];
  }
  return m/n;
}

}

}

#endif


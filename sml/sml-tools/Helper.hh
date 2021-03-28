#ifndef __HELPER_HH__
#define __HELPER_HH__


 // File: Helper.h
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

 class BadConversion : public std::runtime_error {
public:
  BadConversion(std::string const& s)
    : std::runtime_error(s)
    { }
};

 inline double convertToDouble(std::string const& s)
{
  std::istringstream i(s);
  double x;
  if (!(i >> x))
    throw BadConversion("convertToDouble(\"" + s + "\")");
  return x;
}
inline int convertToInt(std::string const& s)
{
  std::istringstream i(s);
  int x;
  if (!(i >> x))
    throw BadConversion("convertToInt(\"" + s + "\")");
  return x;
}

inline bool fexists(const char *filename)
{
 std::ifstream ifile(filename);
 return ifile.good();
}
inline bool fexists(std::string filename)
{
 std::ifstream ifile(filename.c_str());
 return (bool)ifile;
}
inline std::string ReplaceString(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}


double getSignalDependentNoise(double s);

template <class T, int m, int n> struct array2d{
    std::vector< std::vector<T> > array;
    array2d(){
        array.resize(m);
        for (int i = 0; i < m; ++i)
            array[i].resize(n);
    }
    std::vector<T>& operator[](int i){
        return array[i];
    }
};


/*
 * 
 * void filter(int,double *,double *,int,double *,double *);
 * 
 */
// void filter(int ord, double *a, double *b, int np, double *x, double *y)
// {
//   int i,j;
//   y[0]=b[0]*x[0];
//   for (i=1;i<ord+1;i++)
//   {
//     y[i]=0.0;
//     for (j=0;j<i+1;j++)
//       y[i]=y[i]+b[j]*x[i-j];
//     for (j=0;j<i;j++)
//       y[i]=y[i]-a[j+1]*y[i-j-1];
//   }
//   /* end of initial part */
//   for (i=ord+1;i<np+1;i++)
//   {
//     y[i]=0.0;
//     for (j=0;j<ord+1;j++)
//       y[i]=y[i]+b[j]*x[i-j];
//     for (j=0;j<ord;j++)
//       y[i]=y[i]-a[j+1]*y[i-j-1];
//   }
// } /* end of filter */
// void filtfilt(int ord, double *a, double *b, int np, double *x, double *y)
// {
//   int i;
//   filter(ord,a,b,np,x,y);
//   /* reverse the series for FILTFILT */
//   for (i=0;i<np;i++) x[i]=y[np-i-1];
//   /* do FILTER again */
//   filter(ord,a,b,np,x,y);
//   /* reverse the series back */
//   for (i=0;i<np;i++) x[i]=y[np-i-1];
//   for (i=0;i<np;i++) y[i]=x[i];
//   /* NOW y=filtfilt(b,a,x); boundary handling not included*/
// }
// void smooth(double * x, int np){
//   double y[np];
//   int ord=10;
//   double a[ord];
//   double b[ord];
//   for(int i=0;i<ord;i++) a[i] = 0.1;
//   for(int i=0;i<ord;i++) b[i] = 1.0;
  
//   filter(ord, a, b, np, x, y);
  
//   for(int i=0;i<np;i++){
//     x[i] = y[i];
//   }
  
//}

#endif

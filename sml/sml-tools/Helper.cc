#include "Helper.hh"


#include <boost/random/normal_distribution.hpp>
#include <boost/random.hpp>


//constructor
typedef boost::mt19937 RandomGenerator;
static RandomGenerator rng(static_cast<unsigned> (time(0)));

// Gaussian
typedef boost::normal_distribution<double> NormalDistribution;
typedef boost::variate_generator<RandomGenerator&, \
NormalDistribution> GaussianGenerator;


static NormalDistribution nd_amplitude(0.0, 1.0);
static GaussianGenerator gauss(rng, nd_amplitude);
 
extern EventManager* eventManager;
namespace nmm {
  extern SmlParameters parameters;
}
using namespace nmm;

using namespace std;

double getSignalDependentNoise(double s){
    // rescaling normal random variable
    
    // N(0,1)*A --> N(0,A*A). We want N(0,K*A*A) --> N(0,1)*sqrt(K*A*A)
    static double k = Settings::get<double>("noise_variationCoefficient");
    static string noise_level = "noise_level";
    static string noise_class = Settings::get<string>("noise_class");
    static string noise_type = Settings::get<string>("noise_type");
    
    k=k+parameters[1][noise_level];
    
    if(eventManager->get<int>(STATES::CYCLE_COUNT) < 1)
        return 0.0;;
    
    if(noise_class=="revert")
        s = 1.0-s;
    else if(noise_class=="constant")
        s = 1.0;
    
    //return gauss()*k*s*s;
    if(noise_type=="sqrt")
        return gauss()*sqrt(k*s*s);
    else
        return gauss()*k*sqrt(s*s);
}
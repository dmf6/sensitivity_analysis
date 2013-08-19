#ifndef __RANDOM_H
#define __RANDOM_H

#include "boost/random.hpp"
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>

using namespace boost;
using namespace std;



class Random {
    
  public:
    typedef boost::mt19937 RNGType;

    RNGType rg;
    Random() {};
    void setSeed(int seed);
    double nextDouble( double lowerLimit, double upperLimit );
    int nextInt(int lowerLimit, int upperLimit);
    std::ptrdiff_t operator()( std::ptrdiff_t arg ) { 
        return static_cast< std::ptrdiff_t >( rand() );
     } 
};

#endif

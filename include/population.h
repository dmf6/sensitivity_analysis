#ifndef __POPULATION_H
#define __POPULATION_H

#include <boost/foreach.hpp>
#include <vector>
#include <iterator>
#include <algorithm>
#include <math.h>
#include <fstream>
#include "individual.h"
#include "worker.h"

#define foreach         BOOST_FOREACH

using namespace std;
using namespace boost;

class Population {
  public:
    Individual **ind;
    int _popsize;
    int nreal;    /* number of real paramaters */
    boost::mutex lock;
    boost::thread_group g1;
    boost::thread_group g2;
      
    Population(int popsize, int nreal);
    ~Population();
    void initialize();
    void printPop();
    void readPars(ifstream &ifs);
    void simulate();
    void printFmax(ostream& os);
    void printZmaxSensitivity(ostream&);
    void printFmaxSensitivity(ostream&);
    void printQSensitivity(ostream&);
    void printFWidthSensitivity(ostream&);
    
};
#endif

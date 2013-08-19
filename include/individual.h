#ifndef __INDIVIDUAL_H
#define __INDIVIDUAL_H

#include <fstream>
#include <sstream>
#include <algorithm>    // std::copy
#include <vector>
#include <boost/smart_ptr.hpp>

#include "random.h"

using namespace std;
using namespace boost;


class Individual {
  public:
    double *vars; // stores params
    int numvars;
    Random *rand;
    
    double *fmax;
    vector<double> zmaxSensitivity;
    vector<double> fmaxSensitivity;
    vector<double> qSensitivity;
    vector<double> fWidthSensitivity;
    
    Individual(int nreal);
    ~Individual();
    void initialize();
    void evaluate();
    void simulate(int id);
    double getFmax(string myfile);
    double getAttribute(int id, int zfAttr_id, double *pars);
    void printZmaxSensitivity(ostream&);
    void printFmaxSensitivity(ostream&);
    void printQSensitivity(ostream&);
    void printFWidthSensitivity(ostream&);
    void qsort(double *x, double *y, int left, int right);
    int randPartition(double *x, double *y, int left, int right);
        //void swap( int *array, int a, int b );
    template <class T> void swap(T *, T, T);
    
    
};
#endif

#ifndef __INDIVIDUAL_H
#define __INDIVIDUAL_H



#include <fstream>
#include <sstream>
#include <algorithm>    // std::copy
#include <vector>
#include <boost/smart_ptr.hpp>

using namespace std;
using namespace boost;


class Individual {
  public:
    double *vars; // stores params
    int numvars;
    double sensitivity[5][15];
    
    double *fmax;
    double *fmaxSensitivity ;
    double *QSensitivity;
    double *fwidthSensitivity;
    double *ZmaxSensitivity;
    double *Z10Sensitivity;
    
    Individual(int nreal);
    ~Individual();
    void initialize();
    void evaluate();
    void simulate(int id);
    double getFmax(string myfile);
    double getAttribute(int id, int zfAttr_id, double *pars);
    void printZmaxSensitivity();
    void printFmaxSensitivity();
    void printQSensitivity();
    void printFWidthSensitivity();
};
#endif

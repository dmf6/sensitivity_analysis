#include <boost/foreach.hpp>
#include "population.h"

#define NUM_THREADS 1
#define NUM_SIMS 1

/* inline constructor */
Population::Population(int popsize, int nreal) : _popsize(popsize) {
        /* need to seed from the random number generator for the entire population */
    this->nreal = nreal;
    ind = new Individual*[_popsize];
  
    for (int i = 0; i < _popsize; i++) {
        ind[i] = new Individual(nreal);
    }

    cout << "popsize " << _popsize << endl;
}

Population::~Population() {
    for (int j = 0; j < _popsize; j++) {
        delete ind[j];
    }
    delete [] ind;
}

void Population::readPars(ifstream & ifs) {
    string line;
    double *buff = new double[15];
    int i, j;

    i = 0;
    if (ifs.is_open()) {  
         while (i < _popsize) {
            getline( ifs, line);
            stringstream sstr( line );
            double value;
            j = 0;
            while( sstr >> value) {
                buff[j++] = value;
            }
            memcpy(ind[i++]->vars, buff, 15*sizeof(double));
         }
    }
    delete [] buff;
    
}

void Population::printPop() {
    for (int i = 0; i < _popsize; i++) {
        for (int j = 0; j < 15; j++) {
            cout << ind[i]->vars[j] << "  ";
        }
        cout << "\n";
    }
}

               
void Population::simulate() {
        /* Parallel implementation */
    boost::thread *w1;
    
    //     /* 8 threads each running 2 simulations each = 16 simulations*/
    for (int i = 0, j = 0; i < NUM_THREADS; i++, j+=NUM_SIMS) {
        Worker w(i, &lock, j, j+NUM_SIMS, this);
        w1 =  new boost::thread(w);
        g1.add_thread(w1);
    }
    g1.join_all();   
        /* look into boost::bind */
}


void Population::printFmax(ostream& os) {
    for (int i = 0; i < _popsize; i++) {
        for (int j = 0; j < 3; j++) {
            os << ind[i]->fmax[j] << "  ";
        }
        os << "\n";
    }
}

void Population::printZmaxSensitivity(ostream& os) {
    for (int i = 0; i < _popsize; i++) {
        ind[i]->printZmaxSensitivity(os);
        
    }
}

void Population::printFmaxSensitivity(ostream& os) {
    for (int i = 0; i < _popsize; i++) {
        ind[i]->printFmaxSensitivity(os);
        
    }
}

void Population::printQSensitivity(ostream& os) {
    for (int i = 0; i < _popsize; i++) {
        ind[i]->printQSensitivity(os);
        
    }
}

void Population::printFWidthSensitivity(ostream& os) {
    for (int i = 0; i < _popsize; i++) {
        ind[i]->printFWidthSensitivity(os);
        
    }
}

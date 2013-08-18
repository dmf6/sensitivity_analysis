#include "individual.h"
#include <iostream>
#include <cstdlib>
#include <gsl/gsl_fit.h>

using namespace std;

Individual::Individual(int nreal) {
    numvars = nreal;
    vars = new double[nreal];
    fmax = new double[3];
}

Individual::~Individual() {
    delete [] vars;
    delete [] fmax;
}


// /* id is the thread number */
void Individual::simulate(int id) {
    int n = 0;
    double stderror = 0;
        /* copy contents of vars into pars vector
         *
         * pars vector is what we will use when we change the
         * parameters
         */
    vector<double> pars(15);
    vector<double> parVector;
    
    vector<double> zfAttrs;
    int flag = 0;
    double zf, zf0;

        /* variables for gsl_fit_linear */
    double c0, c1, cov00, cov01, cov11, chisq;

    double p0, p, pn;
    p0 = 0.001;
    
    int m = 0;

        /* for each attribute */
    for (int i =0; i < 1; i++) {
            // for each parameter
        for (int j = 1; j < 3; j++) {
            cout << "STARTING SENSITIVITY ANALYSIS OF ATTRIBUTE " << i << "TO CHANGES IN PARAMETER " << j << "\n\n\n\n\n";
            
            stderror = 0;
/* simulate control */
            zf0 = getAttribute(id, i, vars);

            for (int k = 0; k < 2; k++) {
                switch(k) {
                    case 0:
                        flag = -1;
                        break;
                    case 1:
                        flag = 1;
                        break;
                }
                copy (vars, vars+15, pars.begin());
                    /* set start value for parameter */
                p = exp(flag*p0)*pars[j];
                parVector.push_back(p);
                pars[j] = p;

                zf = getAttribute(id, i, pars.data());
                cout << zf << "\n";
                    /* push the relative change in the zf attribute value onto the vector */
                zfAttrs.push_back((zf-zf0)/zf0);
            }
            

            m = 1;

            while (stderror < 0.1) {
                
                    //while (stderror < 0.1) {
                    /* for +/-x% change par[i] and run simulation. Save percent value and relative change in x and y vectors */
                for (int l = 0; l < 2; l++) {
                    switch(l) {
                        case 0:
                            flag = -1;
                            break;
                        case 1:
                            flag = 1;
                            break;
                    }
                    copy (vars, vars+15, pars.begin());
                        // continue p on logarithmic scale...i chose 10*m because the stepping would be too fine otherwise
                    pn = pow(1.05, 10*m)*p0;
                    p = exp(flag*pn)*pars[j];
                    parVector.push_back(p);
                    pars[j] = p;
                    
                    zf = getAttribute(id, i, pars.data());
                    cout << zf << "\n";
                        /* push the realtive change in the zf attribute value onto the vector */
                    zfAttrs.push_back((zf-zf0)/zf0);
                }
               
                m++;
                
            
                for (vector<double>::iterator it = zfAttrs.begin(); it!=zfAttrs.end(); ++it) {
                    cout << ' ' << *it;
                }
                cout << "\n";
                
                for (vector<double>::iterator it = parVector.begin(); it!=parVector.end(); ++it) {
                    cout << ' ' << *it;
                }
                
                n = zfAttrs.size();
                cout << "\n";
                
                    /*perform linear fit */
                gsl_fit_linear (parVector.data(), 1, zfAttrs.data(), 1, n, 
                                &c0, &c1, &cov00, &cov01, &cov11, 
                                &chisq);
                
                    /* refer to the ANOVA table in R */
                stderror=sqrt(cov11);

                    /* Not sensitive */
                if (stderror == 0) {
                    break;
                }
                
                cout << "\n\n\n\n THE STANDARD ERROR IS " << stderror << "\n\n\n\n";
                cout << "\n\n\n\n THE SLOPE OF THE FIT IS " << c1 << "\n\n\n\n";
            }
                /* clear the x and y vectors ready for the next run */
            zfAttrs.clear();
            parVector.clear();
                /* save sensitivity (slope of linear fit) for attribute once we have finished with the parameter stepping */
            switch (i) {
                case 0:
                    zmaxSensitivity.push_back(c1);
                    cout << "THE SENSITIVITY " << c1 << " WAS SAVED!!!!!!!\n\n\n\n\n";
                    
                    break;
                case 1:
                    fmaxSensitivity.push_back(c1);
                    break;
                case 2:
                    qSensitivity.push_back(c1);
                    break;
                case 3:
                    fWidthSensitivity.push_back(c1);
                    break;
            }
        }
    }
}
    


/* given the parameter string return the ZF attribute value */
double Individual::getAttribute(int id, int zfAttr_id, double *pars) {
        //string prog, sid, string, params, par, prog2, myfile;
    string prog = "./neuralsim -i";
    string sid = to_string(id);
    string params = " -p ";
    string par;

    //     /* create parameter string */
    for (int j = 0; j < 15; j++) {
        par = to_string(pars[j]);
            //cout << pars[j] << "\n";
        params += " " + par;
    }
            
    prog += " " + sid + " -vlb -60 -vub -30";
    prog += params;
    cout << prog << "\n";
    
    boost::scoped_array<char> writable(new char[prog.size() + 1]);
    std::copy(prog.begin(), prog.end(), writable.get());
    writable[prog.size()] = '\0'; // don't forget the terminating 0
    int i;
            
    i = system(writable.get());
    if (i == 0) {
        cout << "Call to neuralsim successful\n";
    }
    //     /* call zfgenerator */
    string prog2 = "./zfgenerator -f ";
    string name = "SC";
    prog2 += name;
    prog2 += sid;
    prog2 += ".asc";

        //cout << prog2 << endl;
    
    boost::scoped_array<char> writable2(new char[prog2.size() + 1]);
    std::copy(prog2.begin(), prog2.end(), writable2.get());
    writable2[prog2.size()] = '\0'; // don't forget the terminating 0
    i = system(writable2.get());
    if (i == 0) {
        cout << "Call to zfgenerator successful\n";
    }

    string myfile = "SC";
    myfile += sid;
    myfile += "_zfstats.dat";
        //cout << myfile;

    cout << "Extracting zf curve attributes from " << myfile << "\n";
    
    string line;
    vector<double> zf_attrs;
    
    ifstream zfstats(myfile);
    if (zfstats.is_open()) {
        while ( zfstats.good()) {
            getline(zfstats, line);
            stringstream sstr2(line);
            double val;  
            while( sstr2 >> val) {
                zf_attrs.push_back(val);  /* stored as zmax, fmax, q, fwidth, z10 */
            }
        }    
    }

    zfstats.close();   
    
    return zf_attrs[zfAttr_id];
}



double Individual::getFmax(string myfile) {
    string line;
    ifstream zfstats(myfile);
    double *buff = new double[5];
    double fmax;
    
    if (zfstats.is_open()) {
        while ( zfstats.good()) {
            getline(zfstats, line);
            stringstream sstr2(line);
            double val;  
            int j = 0;
            
            while( sstr2 >> val) {
                buff[j++] = val;  /* stored as zmax,fmax, q, z10, fhalfwidth */
            }
        }    
    }
    zfstats.close();
    fmax = buff[1];
    delete [] buff;
    return fmax;
    
}

void Individual::printZmaxSensitivity(ostream& os) {
    for (vector<double>::iterator it = zmaxSensitivity.begin(); it!=zmaxSensitivity.end(); ++it) {
            os << ' ' << *it;
        }
             os << "\n";
}

void Individual::printFmaxSensitivity(ostream& os) {
    for (vector<double>::iterator it = fmaxSensitivity.begin(); it!=fmaxSensitivity.end(); ++it) {
            os << ' ' << *it;
        }
             os << "\n";
}

void Individual::printQSensitivity(ostream& os) {
    for (vector<double>::iterator it = qSensitivity.begin(); it!=qSensitivity.end(); ++it) {
            os << ' ' << *it;
        }
             os << "\n";
  
}

void Individual::printFWidthSensitivity(ostream& os) {
    for (vector<double>::iterator it = fWidthSensitivity.begin(); it!=fWidthSensitivity.end(); ++it) {
            os << ' ' << *it;
        }
             os << "\n";
         
}

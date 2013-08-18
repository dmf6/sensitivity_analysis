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
    double percent = 0.05;
        /* copy contents of vars into pars vector
         *
         * pars vector is what we will use when we change the
         * parameters
         */
    vector<double> pars(15);
    vector<double> percentChange;
    
    vector<double> zfAttrs;
    int flag = 0;
    double zf = 0.0;

        /* variables for gsl_fit_linear */
    double c0, c1, cov00, cov01, cov11, chisq;
    double s, stdev1; 
    
        /* for each attribute */
    for (int i =0; i < 5; i++) {
        stderror = 0;
        
            // for each parameter
        for (int j = 0; j < 15; j++) {
                /* simulate unchanged parameter string */
            percentChange.push_back(0);
            zf = getAttribute(id, i, vars);
            cout << zf << "\n";
                /* push the zf attribute value onto the vector */
            zfAttrs.push_back(zf);

            while (percent <= 0.2) {
                
                    //while (stderror < 0.1) {
                    /* for +/-x% change par[i] and run simulation. Save percent value and relative change in x and y vectors */
                for (int k = 0; k < 2; k++) {
                    switch(k) {
                        case 0:
                            flag = -1;
                            break;
                        case 1:
                            flag = 1;
                            break;
                    }
                        /* save the percent change value for the x vector used in the linear fit */
                    percentChange.push_back(flag*(percent)*100);
                    
                    copy (vars, vars+15, pars.begin());
                        /* now we can change (or not) the parameters in the pars vector and send it off for simulation */
                    pars[j] = pars[j] + flag*(percent)*pars[j];
                    
                    zf = getAttribute(id, i, pars.data());
                    cout << zf << "\n";
                        /* push the zf attribute value onto the vector */
                    zfAttrs.push_back(zf);
                }
                percent +=0.05;
            }
            
            for (vector<double>::iterator it = zfAttrs.begin(); it!=zfAttrs.end(); ++it) {
                cout << ' ' << *it;
            }
            cout << "\n";
            
            for (vector<double>::iterator it = percentChange.begin(); it!=percentChange.end(); ++it) {
                cout << ' ' << *it;
            }
            
            n = zfAttrs.size();
            cout << "\n";
            
                /*perform linear fit */
            gsl_fit_linear (percentChange.data(), 1, zfAttrs.data(), 1, n, 
                            &c0, &c1, &cov00, &cov01, &cov11, 
                            &chisq);
            
                /* refer to the ANOVA table in R */
                //stdev1=sqrt(cov11);
                //stderror=stdev1/sqrt(n);

                /* standard error of the slope??? */
            s = sqrt((cov00 -(pow(cov01, 2)/cov11))/(n-2));
            stderror = s/sqrt(cov11);
            sensitivity[i][j] = c1;
            
            // cout << "\n\n\n\n THE STANDARD ERROR IS " << stderror << "\n\n\n\n";
            // cout << "\n\n\n\n THE SLOPE OF THE FIT IS " << c1 << "\n\n\n\n";
            // cout  << "n\n\n\n R^2 IS " << cov11 << " " << cov00 << " " << cov01 << " " << n << " " << (pow(cov01, 2)/cov11) <<"\n\n\n\n";

            zfAttrs.clear();
            percentChange.clear();
            percent=0.05;
            
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

void Individual::printZmaxSensitivity() {
    for (int i = 0; i < 15; i++) {
        cout << sensitivity[0][i] << " ";
    }
    cout << "\n";
}

void Individual::printFmaxSensitivity() {
    for (int i = 0; i < 15; i++) {
        cout << sensitivity[1][i] << " ";
    }
    cout << "\n";
}

void Individual::printQSensitivity() {
    for (int i = 0; i < 15; i++) {
        cout << sensitivity[2][i] << " ";
    }
    cout << "\n";
}

void Individual::printFWidthSensitivity() {
    for (int i = 0; i < 15; i++) {
        cout << sensitivity[3][i] << " ";
    }
    cout << "\n";
}

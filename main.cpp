#include "population.h"

using namespace std;

int main() {
        /* create population object */
    Population p1(1, 15);
    
        /* read in parameters */
    ifstream test("optimalModels.dat");
    p1.readPars(test);
    test.close();

        //     Run thousands of simulations as fast as possible!!!!!
    p1.simulate();

        /* for each model, print fmax for each upper voltage bound */
    ofstream o1, o2, o3, o4;
    o1.open("zmax_sensitivity.dat");
    p1.printZmaxSensitivity(o1);
    o1.close();

    o2.open("fmax_sensitivity.dat");
    p1.printFmaxSensitivity(o2);
    o2.close();
    
    o3.open("q_sensitivity.dat");
    p1.printQSensitivity(o3);
    o3.close();

    o4.open("fwidth_sensitivity.dat");
    p1.printFWidthSensitivity(o4);
    o4.close();
    return 0;

}

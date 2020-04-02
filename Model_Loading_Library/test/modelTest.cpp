#include "model.h"
#include "vector.h"
#include "cell.h"
#include "material.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <sstream>
using namespace std;
int main(int argc, char *argv[])
{
    //test constructors
    Model testMod("ExampleModel2.mod");

    //test output operator 
    ostringstream ssModel;
    ssModel<<testMod;
    cout<<testMod;
    bool OUT_OP_STATUS = (ssModel.str() == 
        "There are 301 Vertices in this model\n"
        "There are 129 Cells in this model\n"
        "There are 2 Materials in this model\n"
        "The total weight of this model is 0.00228159\n"
        "The centre of this model is (0.00499457, 0.00252442, 0.00344344)\n");

    //test getters
    double epsilon = 0.001; //error margin for double comparisons
    Vector testModCentre(0.00499457, 0.00252442, 0.00344344);
    bool GET_STATUS = (abs(testMod.getWidth() - 0.01 ) < epsilon  && abs(testMod.getHeight() - 0.0046) < epsilon
                    && abs(testMod.getDepth() - 0.0325) < epsilon && testMod.getModelCentre() == testModCentre
                    && abs(testMod.getModelWeight() - 0.00228159) < epsilon && testMod.getNumberOfVertices() == 301 
                    && testMod.getNumberOfCells() == 129 && testMod.getNumberOfMaterials() == 2);
    //test model
    //test display functions
    testMod.displayDimensions();
    //testMod.displayVertices();
    //testMod.displayCells();
    //testMod.displayMaterials();
    //cout << "Saving to file..." << endl;
    //testMod.saveModel("newModel.mod");
    //cout << "Model saved as newModel.mod" << endl;
    return (OUT_OP_STATUS && GET_STATUS) ? 0 : 1;
}
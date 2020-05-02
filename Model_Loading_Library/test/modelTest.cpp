#include "model.h"
#include "vector.h"
#include "cell.h"
#include "material.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <string>
#include <fstream>
using namespace std;

template<typename InputIterator1, typename InputIterator2>
bool
rangeEqual(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
{
    while(first1 != last1 && first2 != last2)
    {
        if(*first1 != *first2)
            return false;
        ++first1;
        ++first2;
    }
    return (first1 == last1) && (first2 == last2);
}

bool compareFiles(const string& filename1, const string& filename2)
{
    ifstream file1(filename1);
    ifstream file2(filename2);

    istreambuf_iterator<char> begin1(file1);
    istreambuf_iterator<char> begin2(file2);

    istreambuf_iterator<char> end;

    return rangeEqual(begin1, end, begin2, end);
}


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
    testMod.saveModel("GoldenFile.mod");
    testMod.saveModel("tempTestFile.mod");
    bool SAVE_STATUS = compareFiles("GoldenFile.mod","tempTestFile.mod");
    remove("tempTestFile.mod");

    return (OUT_OP_STATUS && GET_STATUS && SAVE_STATUS) ? 0 : 1;
}

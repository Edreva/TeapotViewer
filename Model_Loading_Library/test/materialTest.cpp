#include "material.h"
#include <sstream>
using namespace std;
int main(int argc, char *argv[])
{
  //Test constructor
  Material testMaterial(125,"00FF00","ABS",3);
  bool GET_STATUS = (testMaterial.getColour() == "00FF00" && testMaterial.getID() == 3 &&
                     testMaterial.getDensity() == 125 && testMaterial.getName() == "ABS");
  //Test get functions                   
  std::ostringstream temp;
  temp << testMaterial;
  bool OUTPUT_OP_STATUS = (temp.str()=="\n  Name = ABS\n  Colour = 00FF00\n  Density = 125");
  return (GET_STATUS && OUTPUT_OP_STATUS) ? 0 : 1;
}

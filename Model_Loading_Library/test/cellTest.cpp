#include "cell.h"
#include "tetrahedron.h"
#include "pyramid.h"
#include "hexahedron.h"
#include "material.h"
#include "vector.h"
#include <sstream>
#include <cmath>
using namespace std;
int main(int argc, char *argv[])
{
  //Test constructors
  Vector point1(0,0,0);
  Vector point2(0,0,1);
  Vector point3(0,1,0);
  Vector point4(0,1,1);
  Vector point5(1,0,0);
  Vector point6(1,0,1);
  Vector point7(1,1,0);
  Vector point8(1,1,1);
  Material pyraMat(100,"00FF00","PyraMat",0);
  Material hexaMat(101,"A0FF00","HexaMat",1);
  Material tetraMat(102,"B0FF00","TetraMat",2);
  Pyramid testPyr(point1,point2,point3,point4,point5,pyraMat);
  Hexahedron testHex(point1,point2,point3,point4,point5,point6,point7,point8,hexaMat);
  Tetrahedron testTet(point1,point2,point3,point4,tetraMat);

  double epsilon = 0.001; //error margin for double comparisons

  //Test getWeight funtions
  bool GET_WEIGHT_STATUS = (abs(testPyr.getWeight() - 33.3333) < epsilon && abs(testHex.getWeight() - 67.3333) < epsilon && abs(testTet.getWeight() - 0) < epsilon);
  //Test getVolume functions
  bool GET_VOLUME_STATUS = (abs(testPyr.getVolume() - 0.333333) < epsilon && abs(testHex.getVolume() - 0.666667) < epsilon && abs(testTet.getVolume() - 0) < epsilon);
  //Test getType functions
  bool GET_TYPE_STATUS = (testPyr.getType() == "Pyramid" && testHex.getType() == "Hexahedron" && testTet.getType() == "Tetrahedron");
  //Test getGravityCenter functions
  bool GET_COG_STATUS = ((testPyr.getGravityCenter())[0] == 0.2 && (testPyr.getGravityCenter())[1] == 0.4 && (testPyr.getGravityCenter())[2] == 0.4 &&
                         (testHex.getGravityCenter())[0] == 0.5 && (testHex.getGravityCenter())[1] == 0.5 && (testHex.getGravityCenter())[2] == 0.5 &&
                         (testTet.getGravityCenter())[0] == 0   && (testTet.getGravityCenter())[1] == 0.5 && (testTet.getGravityCenter())[2] == 0.5);
  //Test getVertex functions
  bool GET_VERTEX_STATUS = (testPyr.getVertex()[0] == point1 && testHex.getVertex()[0] == point1 && testTet.getVertex()[0] == point1);
  //Test getColorRGB functions
  bool GET_COLOR_STATUS = (testPyr.getColorRGB()[1] == 255 && testHex.getColorRGB()[0] == 160 && testTet.getColorRGB()[0] == 176);


  return (GET_WEIGHT_STATUS && GET_VOLUME_STATUS && GET_TYPE_STATUS && GET_COG_STATUS && GET_VERTEX_STATUS && GET_COLOR_STATUS ) ? 0 : 1;
}

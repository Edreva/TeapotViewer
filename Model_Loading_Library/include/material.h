//material.h
#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED

///@file material.h This file contains the declarations for the model class

#include <string>
using namespace std;//for std::
/**
 * @brief Class for storing properties of a material
 * @author Vorrapol Gundthanachart
 * @author Ewan Drever-Smith
 * @version 1.0
 * @date 06/12/2019
 */
class Material {
private:
  //Variables
  double density;
  string colour;
  string name;
  int id;
public:
  ///Default Constructor
  Material();
 /**
  * @brief Construct Material given its parameters
  * @param d density of material
  * @param c colour of material in hex i.e 00FF00
  * @param n name of material
  * @param i id of material
  */
  Material (double d, string c, string n, int i);
  ///Default Destructor
  ~Material();
  ///Output stream overload
  friend ostream& operator<<(ostream& out,const Material& m);
  /**
  * @brief Set material parameters
  * @param d density of material
  * @param c colour of material in hex i.e 00FF00
  * @param n name of material
  * @param i id of material
  */
  void setMaterial( double d, string c, string n, double i);
  ///@brief get the density of material
  ///@return density as double
  double getDensity()const;
  ///@brief get the colour of material
  ///@return colour as hex string
  string getColour()const;
  ///@brief get the name of material
  ///@return name as string
  string getName()const;
  ///@brief get the ID of the material
  ///@return id as an int
  int getID()const;
  };

 #endif // End of material.h

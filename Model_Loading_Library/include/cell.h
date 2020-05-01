// cell.h
#ifndef CELL_H
#define CELL_H

///@file cell.h This file contains the declarations for the cell class

#include <vector>
#include <string>
#include <iostream>
#include "vector.h"
#include "material.h"

/**
 * @brief Class for creating cell objects made up of vector points. Parent to hexahedron, pyramid and tetrahedron.
 * @author Wenxiang LUO
 * @version 1.1
 * @date 05/12/2019
 */
class Cell
{
public:
	/// @brief Default Constructor
	Cell();
	/// @brief Default Destructor
	~Cell() = default;
	/// @brief Operator= overload
	Cell& operator=(const Cell& c);
	/// @brief Gets the volume of the cell
	/// @return returns volume of cell as double
	double getVolume() const;
	/// @brief Gets the weight of the cell
	/// @return returns weight of cell as double
	double getWeight() const;
	/// @brief Gets the type of the cell
	/// @return returns type of cell as string
	std::string getType() const;
	/// @brief gets the centre of gravity of the cell
	/// @return returns a vector of size 3 (x,y,z coords)
	const std::vector<double>& getGravityCenter() const;
	/// @brief gets all the vertexes making up the cell
	/// @return returns a vector list containing all the vector points making up this cell
	std::vector<Vector> getVertex() const;
  /// @brief gets the rgb colour of the cell
  /// @return returns a vector of 3 ints.
	std::vector<int> getColorRGB() const;
	/// @brief stream insertion overload
	friend std::ostream& operator<<(std::ostream& out, const Cell& c);
	/// @brief obtain the index of the vertex
	size_t getVectorNumber(vector<Vector>&, int) const;

protected:
	/// @brief a vector list containing all the vector points making up this cell
	std::vector<Vector> data;
	/// @brief a string storing the cell type
	std::string cellType;
	/// @brief a vector list of 3 doubles storing the point where the center of gravity of the cell resides
	/// @note This could be stored as a Vector object instead.
	std::vector<double> gravityCenter;
	/// @brief holds the volume of the cell
	double volume;
	/// @brief holds the weight of the cell
	double weight;
	/// @brief holds the density of the cell
	double density;
	/// @brief holds the color of the cell
	std::string color;
	/// @brief calculates the volume of the cell from its points
	/// @return void
	virtual void setVolume();
	/// @brief calculates the weight of the cell from its volume and density
	/// @return void
	void setWeight();
	/// @brief calculates the COG of the cell
	/// @return void
	void setGravityCenter();
	/// @brief converts a hexadecimal to decimal
	/// @param string the hexadecimal number as a string
	/// @return converted number as a decimal int
	int hexToDec(string) const;
};

#endif //CELL_H

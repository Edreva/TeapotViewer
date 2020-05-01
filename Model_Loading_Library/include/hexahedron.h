//hexahedron.h
#ifndef HEXAHEDRON_H
#define HEXAHEDRON_H

///@file hexahedron.h This file contains the declarations for the hexahedron class

#include "cell.h"
#include "vector.h"
#include "material.h"
#include <cmath>

/**
 * @brief Class for creating hexahedron cell objects made up of vector points. Child of Cell class.
 * @author Wenxiang LUO
 * @author Ewan Drever-Smith
 * @version 1.1
 * @date 05/12/2019
 */
class Hexahedron : public Cell
{
public:
	/// @brief Default Constructor
	Hexahedron();
	/// @brief Constructor that takes the 8 points making up the hexahedron cell and the material it is composed of
	Hexahedron(Vector& v0, Vector& v1, Vector& v2, Vector& v3, Vector& v4, Vector& v5, Vector& v6, Vector& v7, Material& m);
	/// @brief default destructor
	~Hexahedron() = default;
	/// @brief Overload assignment operator
	Hexahedron& operator=(const Hexahedron& h);

private:
	/// @brief determine the volume of the cell
	virtual void setVolume() override;
};

#endif // !HEXAHEDRON_H

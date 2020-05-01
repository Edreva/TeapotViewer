//tetrahedron.h
#pragma once
#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

///@file tetrahedron.h This file contains the declarations for the tetrahedron class

#include "cell.h"
#include "vector.h"
#include  "material.h"
#include <stdlib.h>
#include <cmath>

/**
 * @brief Class for creating tetrahedron cell objects made up of vector points. Child of Cell class.
 * @author Wenxiang LUO
 * @author Ewan Drever-Smith
 * @version 1.1
 * @date 05/12/2019
 */
class Tetrahedron : public Cell
{
public:
	/// @brief Default constructor
	Tetrahedron();
	/// @brief Constructor that takes the 4 points making up the Tetrahedron cell and the material it is composed of
	Tetrahedron(Vector& v0, Vector& v1, Vector& v2, Vector& v3, Material& m);
	/// @brief Default destructor
	~Tetrahedron() = default;
	/// @brief overload assignment operator
	Tetrahedron& operator=(const Tetrahedron& c);

private:
	/// @brief determine the volume of the tetrahedron cell
	void setVolume() override;
};

#endif // !TETRAHEDRON_H

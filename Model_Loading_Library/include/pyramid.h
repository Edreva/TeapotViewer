//pyramid.h
#ifndef PYRAMID_H
#define PYRAMID_H

///@file pyramid.h This file contains the declarations for the pyramid class

#include "cell.h"
#include "vector.h"
#include "material.h"
#include <cmath>

/**
 * @brief Class for creating pyramid cell objects made up of vector points. Child of Cell class.
 * @author Wenxiang LUO
 * @author Ewan Drever-Smith
 * @version 1.1
 * @date 05/12/2019
 */
class Pyramid : public Cell
{
public:
	/// @brief Default constructor
	Pyramid();
	/// @brief Constructor that takes the 5 points making up the Pyramid cell and the material it is composed of
	Pyramid(Vector& v0, Vector& v1, Vector& v2, Vector& v3, Vector& v4, Material& m);
	/// @brief Default Destructor
	~Pyramid() = default;
	/// @brief Overloaded assignment operator
	Pyramid& operator=(const Pyramid& p);

private:
	/// @brief determine the volume of the pyramid cell
	virtual void setVolume() override;
};

#endif // !PYRAMID_H

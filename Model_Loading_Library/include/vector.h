//vector.h
#ifndef Vector_H
#define Vector_H

/// @file vector.h This file contains the declarations for the vector class

#include <math.h>
#include <iostream>
#include <cmath>

using namespace std;
/**
 * @brief Class for creating vector point objects
 * @author Anthony Cooke
 * @author Ewan Drever-Smith
 * @author Wenxiang LUO
 * @version 1.1
 * @date 05/12/2019
 */
class Vector
{
public:
    ///@brief Default Constructor
    Vector();
    ///@brief Constructor which takes the 3-dimensional coordinates of the vector point
    ///@param i the x coordinate of the point
    ///@param j the y coordinate of the point
    ///@param k the z coordinate of the point
    Vector(float i, float j, float k);
    ///@brief Default Destructor
    ~Vector();
    ///@brief Copy constructor
    Vector(const Vector& _Vector);
    ///@brief operator= overload
    const Vector& operator=(const Vector& _Vector);
    ///@brief operator+ overload
    Vector operator+(const Vector& _Vector);
    ///@brief operator+= overload
    const Vector& operator+=(const Vector& _Vector);
    ///@brief operator-= overload
    const Vector& operator-=(const Vector& _Vector);
    ///@brief operator- overload
    Vector operator-(const Vector& _Vector);
    ///@brief operator* overload which scales the vector argument by the scaling factor
    friend Vector operator*(const float ScalingFactor, const Vector& v);
    ///@brief output stream operator overload which outputs the vectors parameters as a stream
    friend ostream& operator<<(ostream& out, const Vector& v);
    ///@brief operator[] overload that allows the vectors data to be accessed by giving an index, similarly to an array
    double operator[](int) const;
    ///@brief operator== overload that returns true if both vectors are equal, that is, their parameters are all equal (i,j,k,mag)
    bool operator==(const Vector&) const;

    ///@brief Set the value of this vector's i parameter
    ///@note Recalculates class values (mag)
    void set_i(const float& value);
    ///@brief Set the value of this vector's j parameter
    ///@note Recalculates class values (mag)
    void set_j(const float& value);
    ///@brief Set the value of this vector's k parameter
    ///@note Recalculates class values (mag)
    void set_k(const float& value);

    ///@brief Get the value of this vector's i parameter
    float get_i() const;
    ///@brief Get the value of this vector's j parameter
    float get_j() const;
    ///@brief Get the value of this vector's k parameter
    float get_k() const;
    ///@brief Get the value of this vector's mag parameter
    float get_mag() const;

    ///@brief Calculates the magnitude of the vector to store in the class
    void calc_mag(float _i,float _j,float _k);
    ///@brief Calculate the dot product of a a given Vector.
    float get_dot_p(const Vector& a);
    ///@brief Calculates the cross product of a Vector.
    Vector get_cross_p(const Vector& _Vector1);

private:
    ///@brief parameter storing the vector point's x coordinate
    float i;
    ///@brief parameter storing the vector point's y coordinate
    float j;
    ///@brief parameter storing the vector point's z coordinate
    float k;
    ///@brief parameter storing the vector's magnitude
    float mag;

};

#endif // Vector_H

#ifndef Vector_H
#define Vector_H

/// @file vector.h This file contains the declarations for the vector class

#include <math.h>
#include <iostream>
#include <cmath>

using namespace std;

class Vector
{
public:
    Vector(); //constructor
    Vector(float i, float j, float k);
    ~Vector(); //destruct

    Vector(const Vector& _Vector); //copy

    const Vector& operator=(const Vector& _Vector); //= (copies a Vector) //operators
    Vector operator+(const Vector& _Vector); //+ (adds two vectors)
    const Vector& operator+=(const Vector& _Vector); //+= (adds two vectors)
    const Vector& operator-=(const Vector& _Vector); //- (subtracts two vectors)
    Vector operator-(const Vector& _Vector); //-= (subtracts two vectors)


    friend Vector operator*(const float ScalingFactor, const Vector& v); //* (scales the vector using scalar product)


    //set functions, changes to value, then recalculates class. Nothing returned.
    void set_i(const float& value);
    void set_j(const float& value);
    void set_k(const float& value);


    //gets the value of variables.
    float get_i() const;
    float get_j() const;
    float get_k() const;
    float get_mag() const;

    double operator[](int) const;
    bool operator==(const Vector&) const;


    void calc_mag(float _i,float _j,float _k); //calculates the magnitude of the vector to store in the class.

    float get_dot_p(const Vector& a); //calculate the dot product of a a given Vector.

    Vector get_cross_p(const Vector& _Vector1); //Calculates the cross product of a Vector.



    //output stream operator
    friend ostream& operator<<(ostream& out, const Vector& v);


private:
    float i;
    float j;
    float k;
    float mag;

};

#endif // Vector_H

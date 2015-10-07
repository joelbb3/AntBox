#pragma once
#include "stdafx.h"

struct Vector {
    double x;
    double y;
    double z;
    
    Vector(double x, double y, double z) : x(x), y(y), z(z) {};
    Vector() : x(0), y(0), z(0) {};
    
    friend Vector operator+(const Vector& v1, const Vector& v2);
    friend Vector operator-(const Vector& v1, const Vector& v2);
    friend Vector operator/(const Vector& v1, const double& x);
    friend Vector operator*(const Vector& v1, const double& x);
    friend Vector operator*(const double& x, const Vector& v1);
    //friend Vector operator+=(const Vector& v1, const Vector& v2);
    
    Vector cross(const Vector& v);
    double dot(const Vector& v);
    double magnitude();
};

struct Plane {
    // ix + jy + kx = m.
    double i;
    double j;
    double k;
    double m;
    
    Plane(double i, double j, double k, double m);
    
    bool contains(Vector& point);
};

struct Box {
    Vector position;
    double width;
    double height;
    Vector topLeft;
    Vector topRight;
    Vector bottomLeft;
    Vector bottomRight;
    
    Box(Vector position, double width, double height);
    
    bool contains(Vector& point);
};

Vector cross(const Vector& v1, const Vector& v2);
#include <math.h>
#include <limits>
#include <algorithm>
#include "Geometry.h"


Vector cross(const Vector& v1, const Vector& v2) {
	return Vector((v1.y * v2.z) - (v1.z * v2.y), (v1.x * v2.z) - (v1.z * v2.x), (v1.x * v2.y) - (v1.y * v2.x));
}

bool flEquals(const double& d1, const double& d2){
    return fabs(d1-d2) <= std::numeric_limits<double>::epsilon() * std::max(fabs(d1), fabs(d2));
}

/* Vector */

Vector operator+(const Vector& v1, const Vector& v2) {
	return Vector(v1.x + v2.x, v1.y + v2.y, +v1.z + v2.z);
}

Vector operator-(const Vector& v1, const Vector& v2) {
	return Vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector operator/(const Vector& v1, const double& x){
    return Vector(v1.x / x, v1.y / x, v1.z / x);
}

Vector operator*(const Vector& v1, double x){
    return Vector(v1.x * x, v1.y * x, v1.z * x);
}

Vector operator*(const double& x, const Vector& v1){
    return Vector(v1.x * x, v1.y * x, v1.z * x);
}

Vector Vector::cross(const Vector& v) {
	return Vector((y * v.z) - (z * v.y), (x * v.z) - (z * v.x), (x * v.y) - (y * v.x));
}

double Vector::dot(const Vector& v) {
	return (x * v.x) + (y * v.y) + (z * v.z);
}

double Vector::magnitude() {
	return sqrt(x*x + y*y + z*z);
}

/* Plane */

Plane::Plane(double i, double j, double k, double m) : i(i), j(j), k(k), m(m){
};

bool Plane::contains(Vector& point){
    return flEquals(point.x * i + point.y * j + point.z * k, m);
}

/* Box */

Box::Box(Vector position, double width, double height) : position(position), width(width), height(height){
};

bool Box::contains(Vector& point){
    return point.x >= position.x && point.x <= position.x + height && point.y >= position.y && point.y <= position.y + width;
}
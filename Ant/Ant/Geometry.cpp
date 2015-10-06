#include "stdafx.h"
#include <math.h>
#include "Geometry.h"


Vector cross(const Vector& v1, const Vector& v2) {
	return Vector((v1.y * v2.z) - (v1.z * v2.y), (v1.x * v2.z) - (v1.z * v2.x), (v1.x * v2.y) - (v1.y * v2.x));
}


/* Vector */

Vector operator+(const Vector& v1, const Vector& v2) {
	return Vector(v1.x + v2.x, v1.y + v2.y, +v1.z + v2.z);
}

Vector operator-(const Vector& v1, const Vector& v2) {
	return Vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
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
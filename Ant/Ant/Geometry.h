#pragma once
#include "stdafx.h"

class Vector {
	double x;
	double y;
	double z;
public:
	Vector(double x, double y, double z) : x(x), y(y), z(z) {};
	Vector() : x(0), y(0), z(0) {};
	void update(double newX, double newY, double newZ) {
		x = newX;
		y = newY;
		z = newZ;
	}
};

class Plane {
	Vector i;
	Vector j;
	Plane(Vector i, Vector j) : i(i), j(j) {};
};

class Box {
	Vector i;
	Vector j;
	Vector k;
	Box(Vector i, Vector j, Vector k) : i(i), j(j), k(k) {};
};


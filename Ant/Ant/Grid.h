#pragma once
#include "stdafx.h"
#include <vector>
#include "Geometry.h"
#include <map>



bool indexInArray(int arrayWidth, int arrayHeight, int i, int j);

class TriangleFace {
	int ID;
	Vector pointOne;
	Vector pointTwo;
	Vector pointThree;
	std::map<int, std::vector<int>> neighbourList;
public:
	TriangleFace(int ID, Vector one, Vector two, Vector three, std::map<int, std::vector<int>> neighbourList) : ID(ID), pointOne(one), pointTwo(two), pointThree(three), neighbourList(neighbourList) {};
	void updateNeighbours(int distance, std::vector<int> neighbourIDs);
};

struct PointSquare {
public:
	int ID;
	Vector topLeft;
	Vector topRight;
	Vector bottomLeft;
	Vector bottomRight;
	PointSquare(int ID, Vector topLeft, Vector topRight, Vector bottomLeft, Vector bottomRight) : ID(ID), topLeft(topLeft), topRight(topRight), bottomLeft(bottomLeft), bottomRight(bottomRight) {};
};

class GridManager {
	int squaresPerRow;
	int squaresPerColumn;
	std::vector<TriangleFace> faceArray;
	double sandboxWidth;
	double sandboxHeight;
	double getDepth(double x, double y);
	void makeZones();
public:
	static std::vector<int> getNeighbours(int ID) { return std::vector<int>{}; };
};


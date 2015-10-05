#pragma once
#include "stdafx.h"
#include <vector>
#include <memory>
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
	void setID(int x) { ID = x; };
};

class Vertex : Vector{
    std::vector<std::weak_ptr<TriangleFace>> associatedFaces;
public:
    Vertex(Vector position) : Vector(position){};
	void associate(TriangleFace& face) {};
};

class SquareFace {
public:
	const int ID;
    // Vertices for the square.
    const std::shared_ptr<Vertex> topLeft;
    const std::shared_ptr<Vertex> topRight;
	const std::shared_ptr<Vertex> bottomLeft;
	const std::shared_ptr<Vertex> bottomRight;
    // Pointers to neighbouring squares.
    std::weak_ptr<const SquareFace> left;
    std::weak_ptr<const SquareFace> right;
    std::weak_ptr<const SquareFace> up;
    std::weak_ptr<const SquareFace> down;
    // Pointers to the two triangles encompassing the square.
    const std::shared_ptr<TriangleFace> top;
	const std::shared_ptr<TriangleFace> bottom;
	std::map<int, std::vector<int>> neighboursList;
    SquareFace(int ID, std::shared_ptr<Vertex> topLeft, std::shared_ptr<Vertex> topRight, std::shared_ptr<Vertex> bottomLeft, std::shared_ptr<Vertex> bottomRight):
        ID(ID), topLeft(topLeft), topRight(topRight),
        bottomLeft(bottomLeft), bottomRight(bottomRight)
    {
        split();
    }
	void split() {};
};

class GridManager {
	int squaresPerRow;
	int squaresPerColumn;
	std::vector<std::shared_ptr<TriangleFace>> faceArray;
	double sandboxWidth;
	double sandboxHeight;
	double getDepth(double x, double y);
	void makeZones();
public:
	static std::vector<int> getNeighbours(int ID) { return std::vector<int>{}; };
};


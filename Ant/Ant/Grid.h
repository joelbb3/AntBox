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
};

class Vertex : Vector{
    std::vector<std::weak_ptr<TriangleFace>> associatedFaces;
    Vertex(Vector position) : position(position), Vector(position){};
    void associate(TriangleFace& face);
};

class SquareFace {
public:
	const int ID;
    // Vertices for the square.
    const std::shared_ptr<Vertex> const topLeft;
    const std::shared_ptr<Vertex> const topRight;
	const std::shared_ptr<Vertex> const bottomLeft;
	const std::shared_ptr<Vertex> const bottomRight;
    // Pointers to neighbouring squares.
    const std::weak_ptr<SquareFace> left;
    const std::weak_ptr<SquareFace> right;
    const std::weak_ptr<SquareFace> up;
    const std::weak_ptr<SquareFace> down;
    // Pointers to the two triangles encompassing the square.
    const std::shared_ptr<TriangleFace> const top;
    const std::shared_ptr<TriangleFace> const down
    SquareFace(int ID, std::shared_ptr<Vertex> topLeft, std::shared_ptr<Vertex> topRight, std::shared_ptr<Vertex> bottomLeft, std::shared_ptr<Vertex> bottomRight):
        ID(ID), topLeft(topLeft), topRight(topRight),
        bottomLeft(bottomLeft), bottomRight(bottomRight)
    {
        split();
    }
    void split();
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


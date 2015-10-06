#pragma once
#include "stdafx.h"
#include <map>
#include <vector>
#include <memory>
#include "Geometry.h"

class Vertex;

class TriangleFace {
	friend class GridManager;
public:
	std::shared_ptr<Vertex> pointOne;
	std::shared_ptr<Vertex> pointTwo;
	std::shared_ptr<Vertex> pointThree;
	std::vector<std::vector<int>> neighbourList;
	bool contains(Vector& xyVec);
	const int ID;
	TriangleFace(int ID, std::shared_ptr<Vertex> one, std::shared_ptr<Vertex> two, std::shared_ptr<Vertex> three, std::vector<std::vector<int>> neighbourList);
};


class Vertex : public Vector{
    int ID;
    std::vector<std::weak_ptr<TriangleFace>> associatedFaces;
public:
    Vertex(int ID, Vector position);
	void associate(std::shared_ptr<TriangleFace> face);
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
    std::shared_ptr<TriangleFace> top;
	std::shared_ptr<TriangleFace> bottom;
	std::vector<std::vector<int>> neighbourList;
    SquareFace(int ID, std::shared_ptr<Vertex> topLeft, std::shared_ptr<Vertex> topRight, std::shared_ptr<Vertex> bottomLeft, std::shared_ptr<Vertex> bottomRight);
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
	double absoluteInterpolateHeight(Vector& xyVec); // Z-coordinate is modified in-place.
	double interpolateHeight(int faceID, Vector& xyVec);
	static std::vector<int> getNeighbours(int ID) { return std::vector<int>{}; };
};


bool indexInArray(int arrayWidth, int arrayHeight, int i, int j);


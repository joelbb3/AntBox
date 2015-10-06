#include "stdafx.h"
#include <algorithm>
#include <set>
#include <utility>
#include <exception>
#include <string>
#include "Grid.h"
#include "Geometry.h"

/* SquareFace */

SquareFace::SquareFace(int ID, std::shared_ptr<Vertex> topLeft, std::shared_ptr<Vertex> topRight, std::shared_ptr<Vertex> bottomLeft, std::shared_ptr<Vertex> bottomRight):
ID(ID), topLeft(topLeft), topRight(topRight),
bottomLeft(bottomLeft), bottomRight(bottomRight),
top(new TriangleFace(ID, bottomLeft, topLeft, topRight, std::vector<std::vector<int>>{})), bottom(new TriangleFace(ID + 1, bottomLeft, bottomRight, topRight, std::vector<std::vector<int>>{})) {
};



/* TriangleFace */

TriangleFace::TriangleFace(int ID, std::shared_ptr<Vertex> one, std::shared_ptr<Vertex> two, std::shared_ptr<Vertex> three, std::vector<std::vector<int>> neighbourList) : ID(ID), pointOne(one), pointTwo(two), pointThree(three), neighbourList(neighbourList), normal(cross(*one - *two, *one - *three)){
};


/* Vertex */

Vertex::Vertex(int ID, Vector position) : ID(ID), Vector(position){
};

void Vertex::associate(std::shared_ptr<TriangleFace> face){
    associatedFaces.push_back(std::weak_ptr<TriangleFace>(face));
}

void Vertex::calculateNormal(){
    int numFaces = associatedFaces.size();
    Vector n(0,0,0);
    for(auto x : associatedFaces){
        n = n + x.lock()->normal;
    }
    normal = n / numFaces;
}
                                                                                                                                                                                                                                                        



/* GridManager */

int GridManager::locatePoint(Vector& vec){
    double squareWidth = sandboxWidth / squaresPerRow;
    double squareHeight = sandboxHeight / squaresPerColumn;
    int predictedRow = static_cast<int>(vec.x / squareWidth);
    int predictedColumn = static_cast<int>(vec.y / squareHeight);
    int predictedSquareID = (predictedColumn * squaresPerRow) + predictedRow;
    int predictedTriangleID = predictedSquareID * 2;
    if (faceArray[predictedTriangleID]->contains(vec)) {
        return predictedTriangleID;
    } else {
        for (int i = 0; i < faceArray[predictedTriangleID]->neighbourList.size(); ++i) {
            for (auto x : faceArray[predictedTriangleID]->neighbourList[i]) {
                if (faceArray[x]->contains(vec)) {
                    return i;
                }
            }
        }
    }
    return false;

}

std::vector<double> GridManager::getBarycentricCoordinates(int faceID, Vector& vec){
    TriangleFace& triangle = *faceArray[faceID];
    Vector f1 = vec - *triangle.pointOne;
    Vector f2 = vec - *triangle.pointTwo;
    Vector f3 = vec - *triangle.pointThree;
    double triArea = cross(*triangle.pointOne - *triangle.pointTwo, *triangle.pointOne - *triangle.pointThree).magnitude();
    double a = cross(f2, f3).magnitude();
    double b = cross(f1, f3).magnitude();
    double c = cross(f1, f2).magnitude();
    return std::vector<double>{a, b, c};
}

double GridManager::absoluteInterpolateHeight(Vector& xyVec) {
    int faceID = locatePoint(xyVec);
    return interpolateHeight(faceID, xyVec);
}

double GridManager::interpolateHeight(int faceID, Vector& xyVec) {
    std::shared_ptr<TriangleFace> triangle = faceArray[faceID];
    std::vector<double> baryCoord = getBarycentricCoordinates(faceID, xyVec);
    xyVec.z = (baryCoord[0] * triangle->pointOne->z) + (baryCoord[1] * triangle->pointTwo->z) + (baryCoord[2] * triangle->pointThree->z);
    return xyVec.z;
}

Vector GridManager::absoluteInterpolateNormal(Vector& vec){
    int faceID = locatePoint(vec);
    return interpolateNormal(faceID, vec);
}

Vector GridManager::interpolateNormal(int faceID, Vector& vec){
    std::shared_ptr<TriangleFace> triangle = faceArray[faceID];
    std::vector<double> baryCoord = getBarycentricCoordinates(faceID, vec);
    return (baryCoord[0] * triangle->pointOne->normal) + (baryCoord[1] * triangle->pointTwo->normal) + (baryCoord[2] * triangle->pointThree->normal);
}

std::vector<int> GridManager::getNeighbours(int faceID,int radius){
    std::vector<int> neighbourList;
    for(int i = 0; i < radius; ++i){
        for(auto x : faceArray[faceID]->neighbourList[i]){
            neighbourList.push_back(x);
        }
    }
    return neighbourList;
}



void GridManager::makeZones() {
    // Set up.
	int ID = 0;
    std::vector<std::vector<std::shared_ptr<Vertex>>> vertexArray;
    std::vector<std::vector<std::shared_ptr<SquareFace>>> squareArray;
	double squareWidth = sandboxWidth / squaresPerRow;
	double squareHeight = sandboxHeight / squaresPerColumn;
    
    // Fill the vertex array.
    for (double i = 0; i < sandboxWidth; i += squareWidth) {
        std::vector<std::shared_ptr<Vertex>> vertexColumn;
        for (double j = 0; j < sandboxHeight; j += squareHeight) {
            vertexColumn.push_back(std::shared_ptr<Vertex>(new Vertex(ID++, Vector(i, j, getDepth(i, j)))));
        }
        vertexArray.push_back(vertexColumn);
    }
    
    // Fill the square array.
    ID = 0;
    for (int i = 0; i < vertexArray.size() - 1; ++i){
        std::vector<std::shared_ptr<SquareFace>> squareColumn;
        for (int j = 0; j < vertexArray[i].size() - 1; ++j){
            squareColumn.push_back(std::shared_ptr<SquareFace>(new SquareFace(ID, vertexArray[i][j], vertexArray[i+1][j], vertexArray[i][j+1], vertexArray[i+1][j+1])));
        }
		ID += 2;
        squareArray.push_back(squareColumn);
    }
    
    // Link squares.
    for (int i = 0; i < squareArray.size(); ++i){
        for (int j = 0; j < squareArray[i].size(); ++j){
            if(i > 0){
                squareArray[i][j]->left = squareArray[i-1][j];
            }
            if(i < squareArray.size() - 1){
                squareArray[i][j]->right = squareArray[i+1][j];
            }
            if(j > 0){
                squareArray[i][j]->up = squareArray[i][j-1];
            }
            if(j < squareArray[i].size()-1){
                squareArray[i][j]->down = squareArray[i][j+1];
            }
        }
    }
    
    // Associate vertices with faces.
	for (int i = 0; i < squareArray.size(); ++i) {
		for (int j = 0; j < squareArray[i].size(); ++j) {
			SquareFace& square = *squareArray[i][j].get();

			// We move from left to right across each row, associating the left-hand (top-left and bottom-left) vertices appropriately:

			// Every vertex is associated with the faces in its own square.
			square.topLeft->associate(square.top);
			square.bottomLeft->associate(square.top);
			square.bottomLeft->associate(square.bottom);

			// If there is a square to the left.
			if (i > 0) {
				square.topLeft->associate(square.left.lock()->top);
				square.topLeft->associate(square.left.lock()->bottom);
				square.bottomLeft->associate(square.left.lock()->top);
			}
			// If there is a square above.
			if (j > 0) {
				square.topLeft->associate(square.up.lock()->top);
				square.topLeft->associate(square.up.lock()->bottom);
				square.topRight->associate(square.up.lock()->bottom);
			}
			// If there is a square below.
			if (j < squareArray[i].size() - 1) {
				square.bottomLeft->associate(square.down.lock()->top);
				square.bottomRight->associate(square.up.lock()->top);
				square.bottomRight->associate(square.up.lock()->bottom);
			}
			// If there is a square to the diagonal top left.
			if (i > 0 && j > 0) {
				square.topLeft->associate(square.up.lock()->left.lock()->bottom);
			}
			// If there is a square to the diagonal bottom left.
			if (i > 0 && j < squareArray[i].size() - 1) {
				square.bottomLeft->associate(square.down.lock()->left.lock()->top);
				square.bottomLeft->associate(square.down.lock()->left.lock()->bottom);
			}

			// Finally, if we are at the last square in the row, we must associate the right-hand (top-right and bottom-right) vertices appropriately:

			// Every vertex is associated with the faces in its own square.
			square.topRight->associate(square.top);
			square.topRight->associate(square.bottom);
			square.bottomRight->associate(square.bottom);

			// If there is a square above.
			if (j > 0) {
				square.topRight->associate(square.up.lock()->bottom);
			}

			// If there is a square below.
			if (j < squareArray[i].size() - 1) {
				square.bottomRight->associate(square.down.lock()->top);
				square.bottomRight->associate(square.down.lock()->bottom);
			}
		}
	}
    
	// Set neighbours (within the squares).
	for (int i = 0; i < squareArray.size(); ++i) {
		for (int j = 0; j < squareArray[i].size(); ++j) {
			// Neighbours will be stored in a map where an integer k keys a list containing ID's for all squares within a radius of k (note here that 'radius' approximates boxes as circles; ie.
			// a 'radius' of one from some square S encompasses all squares within the box immediately surrounding S; a 'radius'of two encompasses the box immediately surrounding *that* box, etc.
			std::vector<std::vector<int>> neighboursList;
			for (int k = 1; k <= maxNeighbourRadius; ++k) {
				std::vector<int> neighboursAtRadiusK;
				for (int l = 0; l <= k * 2; ++l) { // Note that k gives the boundary of the neighbourhood region; l traverses the region.
					// Top row in the box at radius k.
					if (indexInArray(squareArray.size(), squareArray[i].size(), i - k + l, j - k)) {
						neighboursAtRadiusK.push_back(squareArray[i - k + l][j - k]->ID);
						neighboursAtRadiusK.push_back(squareArray[i - k + l][j - k]->ID + 1);
					}
					// Left column in the box at radius k.
					if (indexInArray(squareArray.size(), squareArray[i].size(), i - k, j - k + l)) {
						neighboursAtRadiusK.push_back(squareArray[i - k][j - k + l]->ID);
						neighboursAtRadiusK.push_back(squareArray[i - k][j - k + l]->ID + 1);
					}
					// Bottom row in the box at radius k.
					if (indexInArray(squareArray.size(), squareArray[i].size(), i + k - l, j + k)) {
						neighboursAtRadiusK.push_back(squareArray[i + k - l][j + k]->ID);
						neighboursAtRadiusK.push_back(squareArray[i + k - l][j + k]->ID + 1);
					}
					// Right column in the box at radius k.
					if (indexInArray(squareArray.size(), squareArray[i].size(), i + k, j + k - l)) {
						neighboursAtRadiusK.push_back(squareArray[i + k][j + k - l]->ID);
						neighboursAtRadiusK.push_back(squareArray[i + k][j + k - l]->ID + 1);
					}
				}
				neighboursList.push_back(neighboursAtRadiusK);
			}
			squareArray[i][j]->neighbourList = neighboursList;
		}
	}

	// Collect our finished triangle faces.
	for (auto& squareColumn : squareArray) {
		for (auto& square : squareColumn) {
			// Use the neighbour list in each square to pass neighbours to contained triangles.
			for (auto& x : square->neighbourList) {
				square->top->neighbourList.push_back(x);
				square->bottom->neighbourList.push_back(x);
			}
			// Finally, add to the face array.
			faceArray.push_back(std::shared_ptr<TriangleFace>(square->top));
			faceArray.push_back(std::shared_ptr<TriangleFace>(square->bottom));
		}
	}
};


bool indexInArray(int arrayWidth, int arrayHeight, int i, int j) {
	if (i < arrayWidth && i >= 0 && j < arrayHeight && j >= 0) {
		return true;
	}
	else {
		return false;
	}
};

double GridManager::getDepth(double x, double y) {
	return 5.0;
}
#include <algorithm>
#include <set>
#include <utility>
#include <exception>
#include <string>
#include "Grid.h"
#include <iostream>
#include <cmath>
#include "Geometry.h"
#include "SurfaceRenderer.h"

std::vector<std::shared_ptr<TriangleFace>> GridManager::faceArray{};
int GridManager::sandboxWidth;
int GridManager::sandboxHeight;
int Vertex::vertexIDGenerator = 0;

/* SquareFace */


SquareFace::SquareFace(int ID, std::shared_ptr<Vertex> topLeft, std::shared_ptr<Vertex> topRight, std::shared_ptr<Vertex> bottomLeft, std::shared_ptr<Vertex> bottomRight):
ID(ID), topLeft(topLeft), topRight(topRight),
bottomLeft(bottomLeft), bottomRight(bottomRight),
top(new TriangleFace(ID, bottomLeft, topLeft, topRight, std::vector<std::vector<int>>{})), bottom(new TriangleFace(ID + 1, bottomLeft, bottomRight, topRight, std::vector<std::vector<int>>{})) {
};


/* TriangleFace */


TriangleFace::TriangleFace(int ID, std::shared_ptr<Vertex> one, std::shared_ptr<Vertex> two, std::shared_ptr<Vertex> three, std::vector<std::vector<int>> neighbourList) : ID(ID), pointOne(one), pointTwo(two), pointThree(three), neighbourList(neighbourList){
    Vector preNormal = cross(*two - *one, *three - *one);
    if(preNormal.z < 0){
        normal = preNormal * -1;
    }else{
        normal = preNormal;
    }
};

bool SameSide(Vector p1, Vector p2, Vector a, Vector b){
    Vector cp1 = cross(b-a, p1-a);
    Vector cp2 = cross(b-a, p2-a);
    return cp1.dot(cp2) >= 0;
}

bool PointInTriangle(Vector p, Vector a, Vector b, Vector c){
    return(SameSide(p, a, b, c) && SameSide(p, b, a, c) && SameSide(p, c, a, b));
}

bool TriangleFace::contains(Vector& point){
    if(GridManager::debug){
        std::cout << "contains() called with vector: ";
        point.print();
        std::cout << "\nTriangle to be checked has points: "; pointOne->print(); std::cout << ";\n"; pointTwo->print(); std::cout << ";\n"; pointThree->print(); std::cout << "\n";
    }
    double m = std::abs((pointThree->y - pointOne->y) / (pointThree->x - pointOne->x));
    if(GridManager::debug){
        std::cout << "Gradient is: " << m << "\n";
        std::cout << "The value of the diagonal line from point one to point three at x = " << point.x << " is: " << pointOne->y - ((point.x - pointOne->x) * m) << "\n";
         if(point.y < pointOne->y - ((point.x - pointOne->x) * m)){
             std::cout << "The point is above this line!\n";
         }else{
             std::cout << "The point is below this line!\n";
         }
        if(point.x < pointOne->x || point.x > pointThree->x){
            std::cout << "The point's x value is out of bounds.\n";
        }
        if(point.y <= pointTwo->y){
            std::cout << "The point is above the triangle's second vertex\n";
        }else{
            std::cout << "The point is below the triangle's second vertex\n";
        }
        if( (point.x >= pointOne->x)
           && (point.x <= pointThree->x)
           && ((point.y >= pointOne->y - ((point.x - pointOne->x) * m) && point.y <= pointTwo->y)
               || (point.y <= pointOne->y - ((point.x - pointOne->x) * m) && point.y >= pointTwo->y))
           ){
            std::cout << "Returning true.\n";
        }else{
            std::cout << "Returning false.\n";
        }
        std::cout << "contains() exiting...\n\n";
    }
    return ( (point.x >= pointOne->x)
            && (point.x <= pointThree->x)
            && ((point.y >= pointOne->y - ((point.x - pointOne->x) * m) && point.y <= pointTwo->y)
                || (point.y <= pointOne->y - ((point.x - pointOne->x) * m) && point.y >= pointTwo->y))
            );
}

/* Vertex */


Vertex::Vertex(Vector position) : vertexID(vertexIDGenerator++), Vector(position){
};


void Vertex::associate(std::shared_ptr<TriangleFace> face){
    associatedFaces.push_back(std::weak_ptr<TriangleFace>(face));
}


void Vertex::calculateNormal(){
    //std::cout << "calculateNormal() called for vertex #" << vertexID << ".\n";
    if(associatedFaces.size() <= 0){
        throw std::logic_error("Vertex #" + std::to_string(vertexID) + " calculating normal with no associated faces.");
    }
    int numFaces = associatedFaces.size();
    //std::cout << "Number of faces associated: " << numFaces << "\n";
    Vector n(0,0,0);
    for(auto x : associatedFaces){
        n = n + x.lock()->normal;
    }
    //std::cout << "Sum vector is: "; n.print(); std::cout << "\n";
    normal = n / numFaces;
    //std::cout << "calcualteNormal() exiting...\n\n";
}




/* GridManager */


int GridManager::locatePoint(Vector& vec){              // Returns false if the point is not found.
    if(debug){
        std::cout << "locatePoint() called with vector: ";
        vec.print();
    }
    double squareWidth = SurfaceRenderer::getCurrentWidth() / squaresPerRow;
    double squareHeight = SurfaceRenderer::getCurrentHeight() / squaresPerColumn;
    int predictedRow = static_cast<int>(vec.x / squareWidth);
    int predictedColumn = static_cast<int>(vec.y / squareHeight);
    if(debug){
        std::cout << "\nSquare width: " << squareWidth << "\n";
        std::cout << "Square height: " << squareHeight << "\n";
        std::cout << "Predicted row is: " << predictedRow << "\n";
        std::cout << "Predicted column is: " << predictedColumn << "\n";
    }
    int predictedSquareID = (predictedRow * squaresPerColumn) + predictedColumn;
    int predictedTriangleID = predictedSquareID * 2;
    if(debug){
        std::cout << "Predicted: " << predictedTriangleID << "\n";
        std::cout << "(Predicted triangle has vertices: ";
        faceArray[predictedTriangleID]->pointOne->print();
        faceArray[predictedTriangleID]->pointTwo->print();
        faceArray[predictedTriangleID]->pointThree->print();
        std::cout << ")\n";
    }
    if (faceArray[predictedTriangleID]->contains(vec)) {
        if(debug){
            std::cout << "Returning the predicted triangle!\n";
            std::cout << "locatePoint() exiting...\n\n";
        }
        return predictedTriangleID;
    }
    if(debug){
        std::cout << "Testing other face in same square\n";
    }
    if(faceArray[predictedTriangleID+1]->contains(vec)) {
        std::cout << "Found in other face on same square!\n";
        std::cout << "locatePoint() exiting...\n\n";
        return predictedTriangleID+1;
    }
    else{
        for (int i = 0; i < faceArray[predictedTriangleID]->neighbourList.size(); ++i) {
            for (auto x : faceArray[predictedTriangleID]->neighbourList[i]) {
                if(debug){
                    std::cout << "Testing face #" << x << "...\n\n";
                }
                if (faceArray[x]->contains(vec)) {
                    if(debug){
                        std::cout << "Found!\n";
                        std::cout << "locatePoint() exiting...\n";
                    }
                    return x;
                }
                if(debug){
                    std::cout << "Point not contained in " << x << "\n";
                }
            }
        }
    }
    std::cout << "Error: Point not found!\n";
    std::cout << "locatePoint() exiting...\n\n";
    return false;
}


std::vector<double> GridManager::getBarycentricCoordinates(int faceID, Vector& vect){
    std::shared_ptr<TriangleFace> triangle = faceArray[faceID];
    if(debug){
        std::cout << "getBaryCentricCoordinates called with faceID: " << faceID << " and vector: "; vect.print(); std::cout << "\n";
        std::cout << "Triangle vertices are at:\n";
        triangle->pointOne->print(); std::cout << ";\n";
        triangle->pointTwo->print(); std::cout << ";\n";
        triangle->pointThree->print(); std::cout << "\n";
    }
    Vector vec = Vector(vect.x, vect.y, 0);
    Vector UA1 = Vector(triangle->pointOne->x, triangle->pointOne->y, 0);
    Vector UA2 = Vector(triangle->pointTwo->x, triangle->pointTwo->y, 0);
    Vector UA3 = Vector(triangle->pointThree->x, triangle->pointThree->y, 0);
    Vector f1 = vec - UA1;
    Vector f2 = vec - UA2;
    Vector f3 = vec - UA3;
    if(debug){
        std::cout << "f1 is: "; f1.print(); std::cout << "\n";
        std::cout << "f2 is: "; f2.print(); std::cout << "\n";
        std::cout << "f3 is: "; f3.print(); std::cout << "\n";
        std::cout << "UA1 is: "; UA1.print(); std::cout << "\n";
        std::cout << "UA2 is: "; UA2.print(); std::cout << "\n";
        std::cout << "UA3 is: "; UA3.print(); std::cout << "\n";
    }
    double triArea = cross(UA1 - UA2, UA1 - UA3).magnitude();
    double a = cross(f2, f3).magnitude() / triArea;
    double b = cross(f1, f3).magnitude() / triArea;
    double c = cross(f1, f2).magnitude() / triArea;
    if(debug){
        std::cout << "A - " << triArea << "\n";
        std::cout << "a1 - " << cross(f2, f3).magnitude() << "\n";
        std::cout << "a2 - " << cross(f1, f3).magnitude() << "\n";
        std::cout << "a3 - " << cross(f1, f2).magnitude() << "\n";
        std::cout << "Returning barycentric coordinates: a - " << a << " b - " << b << " c - " << c << "\n";
    }
    return std::vector<double>{a, b, c};
}

double GridManager::absoluteInterpolateHeight(Vector& xyVec) {
    if(debug){
        std::cout << "absoluteInterpolateHeight() called with vector: ";
        xyVec.print();
        std::cout << "\nLocating point...\n";
    }
    int faceID = locatePoint(xyVec);
    if(debug){
        std::cout << "Point located on face #: " << faceID << "\nInterpolating height...\n";
        std::cout << "absoluteInterpolateHeight() exiting...\n\n";
    }
    return interpolateHeight(faceID, xyVec);
}

double GridManager::interpolateHeight(int faceID, Vector& xyVec) {
    if(debug){
        std::cout << "interpolateHeight() called with faceID: " << faceID << " and vector: ";
        xyVec.print();
        std::cout << "\n";
    }
    
    std::shared_ptr<TriangleFace> triangle = faceArray[faceID];
    
    if(debug){
        std::cout << "Face vrtex heights are: " << triangle->pointOne->z << ", " << triangle->pointTwo->z << ", " << triangle->pointThree->z << ".\n";
        std::cout << "Getting barycentric coordinates...\n\n";
    }
    
    std::vector<double> baryCoord = getBarycentricCoordinates(faceID, xyVec);
    xyVec.z = (baryCoord[0] * triangle->pointOne->z) + (baryCoord[1] * triangle->pointTwo->z) + (baryCoord[2] * triangle->pointThree->z);
    
    if(debug){
        std::cout << "Received barycentric coordinates:\na - " << baryCoord[0] << "\nb - " << baryCoord[1] << "\nc - " << baryCoord[2] << "\n";
    }
    
    if(xyVec.z > 1000){
        std::cout << "Something probably went wrong.\n";
        std::cout << "Vector was "; xyVec.print();
        std::cout << "\n face ID was: " << faceID << "\n";
        std::cout << "surrounding ertex heights are: " << triangle->pointOne->z << " " << triangle->pointTwo->z << " " << triangle->pointThree->z << "\n";
    }
    if(debug){
        std::cout << "interpolateHeight() exiting...\n\n";
    }
    return xyVec.z;
}

Vector GridManager::absoluteInterpolateNormal(Vector& vec){
    if(debug){
        std::cout << "absoluteInterpolateNormal() called with vector: ";
        vec.print();
        std::cout << "\nLocating point...\n";
    }
    int faceID = locatePoint(vec);
    std::cout << "Point located on face #: " << faceID << "\nInterpolating normal...\n";
    std::cout << "absoluteInterpolateNormal() exiting...\n\n";
    return interpolateNormal(faceID, vec);
}

Vector GridManager::interpolateNormal(int faceID, Vector& vec){
    if(debug){
        std::cout << "interpolateNormal() called with faceID: " << faceID << " and vector: ";
        vec.print();
    }
    std::shared_ptr<TriangleFace> triangle = faceArray[faceID];
    if(debug){
        std::cout << "\nGetting barycentric coordinates...\n\n";
    }
    std::vector<double> baryCoord = getBarycentricCoordinates(faceID, vec);
    if(debug){
        std::cout << "Received barycentric coordinates:\na - " << baryCoord[0] << "\nb - " << baryCoord[1] << "\nc - " << baryCoord[2] << "\n";
        std::cout << "Vertex normals for associated triangle are:\n";
        triangle->pointOne->normal.print();
        triangle->pointTwo->normal.print();
        triangle->pointThree->normal.print();
        std::cout << "Interpolated normal is: ";
        ((baryCoord[0] * triangle->pointOne->normal) + (baryCoord[1] * triangle->pointTwo->normal) + (baryCoord[2] * triangle->pointThree->normal)).print();
        std::cout << "\ninterpolateNormal() exiting...\n\n";
    }
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
	int squareWidth = SurfaceRenderer::getCurrentWidth() / squaresPerRow;
	int squareHeight = SurfaceRenderer::getCurrentHeight() / squaresPerColumn;
    
    std::cerr << "Stage 1 passed.\n";
    
    // Fill the vertex array.
    for (int i = 0; i <= squaresPerRow; ++i) {
        std::vector<std::shared_ptr<Vertex>> vertexColumn;
        for (int j = 0; j <= squaresPerColumn; ++j) {
            vertexColumn.push_back(std::shared_ptr<Vertex>(new Vertex(Vector(i * squareWidth, j * squareHeight, SurfaceRenderer::getDepth(i * squareWidth, j * squareHeight)))));
        }
        vertexArray.push_back(vertexColumn);
    }
    
    std::cerr << "Stage 2 passed.\n";
    
    // Fill the square array.
    ID = 0;
    for (int i = 0; i < vertexArray.size() - 1; ++i){
        std::vector<std::shared_ptr<SquareFace>> squareColumn;
        for (int j = 0; j < vertexArray[i].size() - 1; ++j){
            squareColumn.push_back(std::shared_ptr<SquareFace>(new SquareFace(ID, vertexArray[i][j], vertexArray[i+1][j], vertexArray[i][j+1], vertexArray[i+1][j+1])));
            ID += 2;
        }
        squareArray.push_back(squareColumn);
    }
    
    std::cerr << "Stage 3 passed.\n";
    
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
    
    /*
    for(int i = 0; i < squareArray[0].size(); ++i){
        std::cout << "Square ID: " << squareArray[0][i]->ID << "\n";
        squareArray[0][i]->topLeft->print();
        squareArray[0][i]->topRight->print();
        squareArray[0][i]->bottomLeft->print();
        squareArray[0][i]->bottomRight->print();
        std::cout << "\n\n";
    }
    
    std::cout << "HEREHERHERHERHEHREH!\n\n\n\n\n\n\n\n\n";
    std::cout << squareArray[1][0]->ID << "\n";
     */
    
    std::cerr << "Stage 4 passed.\n";
    
    // Associate vertices with faces.
	for (int i = 0; i < squareArray.size(); ++i) {
		for (int j = 0; j < squareArray[i].size(); ++j) {
			SquareFace& square = *squareArray[i][j].get();

			// We move from left to right across each row, associating the left-hand (top-left and bottom-left) vertices appropriately:

			// Every vertex is associated with the faces in its own square.
			square.topLeft->associate(square.top);
			square.bottomLeft->associate(square.top);
			square.bottomLeft->associate(square.bottom);
            square.topRight->associate(square.top);
            square.topRight->associate(square.bottom);
            square.bottomRight->associate(square.bottom);
		}
	}
    
    std::cerr << "Stage 5 passed.\n";
    
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
					if (indexInArray(squareArray.size(), squareArray[i].size(), i - (k-1) + l, j - k)) {
						neighboursAtRadiusK.push_back(squareArray[i - (k-1) + l][j - k]->ID);
						neighboursAtRadiusK.push_back(squareArray[i - (k-1) + l][j - k]->ID + 1);
					}
					// Left column in the box at radius k.
					if (indexInArray(squareArray.size(), squareArray[i].size(), i - k, j - k + l)) {
						neighboursAtRadiusK.push_back(squareArray[i - k][j - k + l]->ID);
						neighboursAtRadiusK.push_back(squareArray[i - k][j - k + l]->ID + 1);
					}
					// Bottom row in the box at radius k.
					if (indexInArray(squareArray.size(), squareArray[i].size(), i + (k-1) - l, j + k)) {
						neighboursAtRadiusK.push_back(squareArray[i + (k-1) - l][j + k]->ID);
						neighboursAtRadiusK.push_back(squareArray[i + (k-1) - l][j + k]->ID + 1);
					}
					// Right column in the box at radius k.
					if (indexInArray(squareArray.size(), squareArray[i].size(), i + k, j + k - l)) {
						neighboursAtRadiusK.push_back(squareArray[i + k][j + k - l]->ID);
						neighboursAtRadiusK.push_back(squareArray[i + k][j + k - l]->ID + 1);
					}
				}
				neighboursList.push_back(neighboursAtRadiusK);
                /*std::cout << "At distance: " << k << " square number: " << squareArray[i][j]->ID << " has neighbours:\n";
                for(auto& x : neighboursAtRadiusK){
                    std::cout << x << ", ";
                }
                std::cout << "\n\n";*/
			}
			squareArray[i][j]->neighbourList = neighboursList;
		}
	}
    
    std::cerr << "Stage 6 passed.\n";

	// Collect our finished triangle faces.
	for (auto& squareColumn : squareArray) {
		for (auto& square : squareColumn) {
			// Use the neighbour list in each square to pass neighbours to contained triangles.
			for (auto& x : square->neighbourList) {
				square->top->neighbourList.push_back(x);
				square->bottom->neighbourList.push_back(x);
			}
            // Since neighbour lists were constructed in square arrays, triangles within the same square must be specifically associated.
            square->top->neighbourList[0].push_back(square->bottom->ID);
            square->bottom->neighbourList[0].push_back(square->top->ID);
			// Finally, add to the face array.
			faceArray.push_back(std::shared_ptr<TriangleFace>(square->top));
			faceArray.push_back(std::shared_ptr<TriangleFace>(square->bottom));
		}
	}
    
    std::cerr << "Stage 7 passed.\n";
    
    // Calculate vertex normals.
    for(auto face : faceArray){
        face->pointOne->calculateNormal();
        face->pointTwo->calculateNormal();
        face->pointThree->calculateNormal();
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


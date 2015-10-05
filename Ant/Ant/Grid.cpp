#include "stdafx.h"
#include "Grid.h"
#include <algorithm>
#include <utility>

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
            vertexColumn.push_back(std::shared_ptr<Vertex>(new Vertex(Vector(i, j, getDepth(i, j)))));
        }
        vertexArray.push_back(vertexColumn);
    }
    
    // Fill the square array.
    for (int i = 0; i < vertexArray.size() - 1; ++i){
        std::vector<std::shared_ptr<SquareFace>> squareColumn;
        for (int j = 0; j < vertexArray[i].size() - 1; ++j){
            squareColumn.push_back(std::shared_ptr<SquareFace>(new SquareFace(ID, vertexArray[i][j], vertexArray[i+1][j], vertexArray[i][j+1], vertexArray[i+1][j+1])));
        }
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
    int 
    squareArray[0][0].topLeft->associate(*squareArray[0][0].top->get());
    squareArray[squareArray.size()-1][squareArr]
    
    
	for (double i = 0; i < sandboxWidth; i += squareWidth) {
		std::vector<PointSquare> squaresColumn;
		for (double j = 0; j < sandboxHeight; j += squareHeight) {
			// Insert into the squarePoints map the ID for the square (incrementing by two) as well as the points in the following order: top left, top right, bottom left, bottom right.
			squaresColumn.push_back(PointSquare(ID, Vector(i, j, getDepth(i, j)), Vector(i + squareWidth, j, getDepth(i + squareWidth, j)),
				Vector(i, j + squareHeight, getDepth(i, j + squareHeight)), Vector(i + squareWidth, j + squareHeight, getDepth(i + squareWidth, j + squareHeight))));
			ID += 2; // Increment ID by two each time as each square will be divided into two triangles.
		}
		squaresArray.push_back(squaresColumn);
	}
	for (int i = 0; i < squaresArray.size(); ++i) {
		for (int j = 0; j < squaresArray[i].size(); ++j) {
			std::map<int, std::vector<int>> neighboursList;
			for (int k = 1; k < std::max(squaresArray.size(), squaresArray[i].size()); ++k) {
				std::vector<int> neighboursAtDistanceK;
				for (int l = 0; l <= k * 2; ++l) {
					if (indexInArray(squaresArray.size(), squaresArray[i].size(), i - k + l, j - k)) {
						neighboursAtDistanceK.push_back(squaresArray[i - k + l][j - k].ID);
						neighboursAtDistanceK.push_back(squaresArray[i - k + l][j - k].ID + 1);
					}
					if (indexInArray(squaresArray.size(), squaresArray[i].size(), i - k, j - k + l)) {
						neighboursAtDistanceK.push_back(squaresArray[i - k][j - k + l].ID);
						neighboursAtDistanceK.push_back(squaresArray[i - k][j - k + l].ID + 1);
					}
					if (indexInArray(squaresArray.size(), squaresArray[i].size(), i + k - l, j + k)) {
						neighboursAtDistanceK.push_back(squaresArray[i + k - l][j + k].ID);
						neighboursAtDistanceK.push_back(squaresArray[i + k - l][j + k].ID + 1);
					}
					if (indexInArray(squaresArray.size(), squaresArray[i].size(), i + k, j + k - l)) {
						neighboursAtDistanceK.push_back(squaresArray[i + k][j + k - l].ID);
						neighboursAtDistanceK.push_back(squaresArray[i + k][j + k - l].ID + 1);
					}
				}
				neighboursList.insert(std::make_pair(k, neighboursAtDistanceK));
			}
			PointSquare& square = squaresArray[i][j];
			faceArray.push_back(TriangleFace(square.ID, square.topLeft, square.bottomLeft, square.bottomRight, neighboursList));
			faceArray.push_back(TriangleFace(square.ID + 1, square.topLeft, square.topRight, square.bottomRight, neighboursList));
		}
	}
};

void TriangleFace::updateNeighbours(int distance, std::vector<int> neighbourIDs) {
	if (!neighbourList.count(distance)) {
		neighbourList.insert(std::make_pair(distance, neighbourIDs));
	}
	else {
		neighbourList[distance] = neighbourIDs;
	}
}



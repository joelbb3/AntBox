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
	int ID = 0;
	std::vector<std::vector<PointSquare>> squaresArray;
	double squareWidth = sandboxWidth / squaresPerRow;
	double squareHeight = sandboxHeight / squaresPerColumn;
	for (double i = 0; i < sandboxWidth; i += squareWidth) {
		std::vector<PointSquare> squaresColumn;
		for (double j = 0; j < sandboxHeight; j += squareHeight) {
			// Insert into the squarePoints map the ID for the square (incrementing by two) as well as the points in the following order: top left, top right, bottom left, bottom right.
			squaresColumn.push_back(PointSquare(ID, Vector(i, j, getDepth(i, j)), Vector(i + sandboxWidth, j, getDepth(i + sandboxWidth, j)),
				Vector(i, j + sandboxHeight, getDepth(i, j + sandboxHeight)), Vector(i + sandboxWidth, j + sandboxHeight, getDepth(i + sandboxWidth, j + sandboxHeight))));
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



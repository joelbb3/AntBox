#pragma once
#include <map>
#include <vector>
#include <fstream>
#include <memory>
#include <cmath>
#include "SurfaceRenderer.h"
#include "Geometry.h"

class Vertex;

class TriangleFace {
    friend class GridManager;
public:
    std::shared_ptr<Vertex> pointOne;
    std::shared_ptr<Vertex> pointTwo;
    std::shared_ptr<Vertex> pointThree;
    Vector normal;
    std::vector<std::vector<int>> neighbourList;
    bool contains(Vector& xyVec);
    const int ID;
    TriangleFace(int ID, std::shared_ptr<Vertex> one, std::shared_ptr<Vertex> two, std::shared_ptr<Vertex> three, std::vector<std::vector<int>> neighbourList);
};


class Vertex : public Vector{
    friend class GridManager;
    static int vertexIDGenerator;
    std::vector<std::weak_ptr<TriangleFace>> associatedFaces;
public:
    const int vertexID;
    Vertex(Vector position);
    Vector normal;
    void associate(std::shared_ptr<TriangleFace> face);
    void calculateNormal();
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
private:
    static const int maxNeighbourRadius; // Critical for performance.
    static const int squaresPerRow;
    static const int squaresPerColumn;
    static std::vector<std::shared_ptr<TriangleFace>> faceArray;
public:
    static bool debug;
    static void test(){
        /*  Print all triangle vertices and normals.
            for(auto face : faceArray){
            std::cout << "Vertex one: "; face->pointOne->print(); std::cout << "\n";
            std::cout << "Vertex two: "; face->pointTwo->print(); std::cout << "\n";
            std::cout << "Vertex three: "; face->pointThree->print(); std::cout << "\n";
            std::cout << "Face normal: ";
            face->normal.print();
            std::cout << "\n\n";
        }*/
        /* Print all vertex normals.*/
        for(auto face : faceArray){
            /*std::cout << "Vertex one (#" << face->pointOne->vertexID << ") is associated with:\n";
            for(auto associatedFace : face->pointOne->associatedFaces){
                std::cout << "Face #" << associatedFace.lock()->ID << ", with normal: "; associatedFace.lock()->normal.print(); std::cout << "\n";
            }*/
            std::cout << "Therefore vertex one has normal: "; face->pointOne->normal.print(); std::cout << "\n";
            /*std::cout << "Vertex two (#" << face->pointTwo->vertexID << ") is associated with:\n";
            for(auto associatedFace : face->pointTwo->associatedFaces){
                std::cout << "Face #" << associatedFace.lock()->ID << ", with normal: "; associatedFace.lock()->normal.print(); std::cout << "\n";
            }
            */std::cout << "Therefore vertex two has normal: "; face->pointTwo->normal.print(); std::cout << "\n";
            /*std::cout << "Vertex three (#" << face->pointThree->vertexID << ") is associated with:\n";
            for(auto associatedFace : face->pointThree->associatedFaces){
                std::cout << "Face #" << associatedFace.lock()->ID << ", with normal: "; associatedFace.lock()->normal.print(); std::cout << "\n";
            }*/
            std::cout << "Therefore vertex three has normal: "; face->pointThree->normal.print(); std::cout << "\n";
        }
        /* Print all associated faces for each vertex.
        for(auto face : faceArray){
            std::cout << "Face #" << face->ID << " has pointOne #" << face->pointOne->vertexID << "\n";
            std::cout << "Vertex #" << face->pointOne->vertexID << " is associated with:\n";
            for(auto assocFace : face->pointOne->associatedFaces){
                std::cout << "Face #" << assocFace.lock()->ID << "\n";
            }
            std::cout << "Face #" << face->ID << " has pointTwo #" << face->pointTwo->vertexID << "\n";
            std::cout << "Vertex #" << face->pointTwo->vertexID << " is associated with:\n";
            for(auto assocFace : face->pointTwo->associatedFaces){
                std::cout << "Face #" << assocFace.lock()->ID << "\n";
            }
            std::cout << "Face #" << face->ID << " has pointThree #" << face->pointThree->vertexID << "\n";
            std::cout << "Vertex #" << face->pointThree->vertexID << " is associated with:\n";
            for(auto assocFace : face->pointThree->associatedFaces){
                std::cout << "Face #" << assocFace.lock()->ID << "\n";
            }
        }*/
         
    }
    static int sandboxWidth;
    static int sandboxHeight;
    static void makeZones();
    static int locatePoint(Vector& vec);
    static std::vector<double> getBarycentricCoordinates(int faceID, Vector& vec);
    static double absoluteInterpolateHeight(Vector& xyVec); // Z-coordinate is modified in-place.
    static double interpolateHeight(int faceID, Vector& xyVec);
    static Vector absoluteInterpolateNormal(Vector& vec);
    static Vector interpolateNormal(int faceID, Vector& vec);
    static std::vector<int> getNeighbours(int faceID, int radius);
};


bool indexInArray(int arrayWidth, int arrayHeight, int i, int j);
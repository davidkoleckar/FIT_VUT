#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

#include "Triangle.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

using namespace std;

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 radiosity;
    uint indicesCount;
};

class Triangle;

class Scene {

public:
    static Scene& getInstance() {
        static Scene instance;
        return instance;
    }
    
private:
    Scene() {}
public:
    Scene(Scene const&)             = delete;
    void operator=(Scene const&)    = delete;

public:
    void loadObjFile(string filename);
    unsigned int getVerticesSize(); 
    void* getVertices(); 
    vector<Vertex>& getVerticesVec();
    unsigned short getIndicesSize(); 
    unsigned short getIndicesCount();
    void* getIndices(); 
    vector<ushort> getIndicesVec();
    vector<Triangle>& getTriangles();

    Triangle& getMaxPatch();
    Triangle& getTriangle(unsigned int index);
    void resetFormFactors();
    void resetVerticesColor();

private:
    vector<Triangle> m_triangles;
    vector<Vertex> m_vertices;
    vector<unsigned short> m_indices;
};

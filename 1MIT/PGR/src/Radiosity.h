#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

#include "Triangle.h"
#include "Scene.h"
#include "Hemicube.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

using namespace std;

class Radiosity {

public:
    Radiosity();
    virtual ~Radiosity();

    // Compute radiosity in max iterations
    void computeRadiosity(int maxIterations);
    void initHemicube(uint size);

private:
    // Calculate form factors for max patch
    void calculateFormFactors(Triangle& maxPatch);
    // Distribute energy from max patch to whole scene
    void distributeEmissionFromMaxPatch(Triangle& maxPatch);
    // For each patch convert its emission to RGBA color
    void convertRadiosityToColor();

private:
    Hemicube m_hemicube;
};

#include "Radiosity.h"


Radiosity::Radiosity() { 
}

Radiosity::~Radiosity() {

}

void Radiosity::initHemicube(uint size)  {
    m_hemicube.init(size);
    // Compute delta factors
    m_hemicube.computeDeltaFactors();
}

/**
 * Compute radiosity in max iterations
 */
void Radiosity::computeRadiosity(int maxIterations) {
    cout << "Computing radiosity in max " << maxIterations << " iterations." << endl;

    for(int i = 0; i < maxIterations; i++) {
        Triangle& maxPatch = Scene::getInstance().getMaxPatch();
        if (glm::length(maxPatch.getEmission()) < 0.0001f) {
            cout << "Stopping radiosity: " << i << endl;
            return;
        }

        calculateFormFactors(maxPatch);
        distributeEmissionFromMaxPatch(maxPatch);
    }
    convertRadiosityToColor();
}

/**
 *  Calculate form factors for max patch
 */
void Radiosity::calculateFormFactors(Triangle& maxPatch) {
    // Reset form factors in all patches
    Scene::getInstance().resetFormFactors();

    // Render hemicube from max patch
    m_hemicube.renderHemicube(maxPatch);

    // Compute form factors
    for (uint i = 0; i < pow(m_hemicube.getRenderSize(), 2); i++) {
        uint index = m_hemicube.getColorIndex(i);
        if (index > 1) {
            Triangle& t = Scene::getInstance().getTriangle(index - 1);
            t.addFormFactor(m_hemicube.getDeltaFormFactor(i));
        }
    }
    // Total number of hemicube render pixels
    int total = m_hemicube.getRenderSize() * m_hemicube.getRenderSize() * 3 / 4;
    for (Triangle& t : Scene::getInstance().getTriangles()) {
        t.setFormFactor(t.getFormFactorCount() / (float)total , 1);
    }
}

/**
 * Distribute energy from max patch to whole scene
 */
void Radiosity::distributeEmissionFromMaxPatch(Triangle& maxPatch) {
    for (Triangle& patch : Scene::getInstance().getTriangles()) {
        // Skip max patch
        if (patch == maxPatch)
            continue;
        // Compute new radiosity for each patch
        glm::vec3 rad = patch.getReflection() * patch.getFormFactor() * maxPatch.getEmission();
        patch.addRadiosity(rad);
        patch.addEmission(rad);
    }
    maxPatch.resetEmission();
}

/**
 * For each patch convert its energy to RGBA color
 */
void Radiosity::convertRadiosityToColor() {
    // Set color of each vertex to 0
    Scene::getInstance().resetVerticesColor();

    for (Triangle& patch : Scene::getInstance().getTriangles()) {
        for (auto index : patch.getTriangleIndices()) {
            Vertex& vertex = Scene::getInstance().getVerticesVec().at(index);
            vertex.color += patch.getColor() / (float) vertex.indicesCount;
            vertex.radiosity += patch.getRadiosity() / (float) vertex.indicesCount;
        }
    }
}

/*
 * File:   Hemicube.h
 * Author: David Koleckar - xkolec07@stud.fit.vutbr.cz
 *         Matus Motlik - xmotli02@stud.fit.vutbr.cz
 */

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "pgr.h"
#include "Triangle.h"
#include "Scene.h"


using namespace std;

struct ppm_image {
    int width;
    int height;
    uint8_t *data;
    size_t size;
};

class Hemicube{

public:

    Hemicube();
    virtual ~Hemicube();

    void init(uint size);

    void computeDeltaFactors();
    void renderHemicube(Triangle maxPatch);
    uint getRenderSize();
    uint getColorIndex(uint i);
    float getDeltaFormFactor(uint i);

private:
    float deltaFactor(int pointX, int pointY);
    void renderColorHemicube();
    void readHemicubeBuffer();
    uint decodeColor(glm::vec3 color);

    void saveImage(string filename);

private:
    uint m_size;
    uint m_renderSize;
    vector<float> m_deltaFactors;
    GLubyte *viewData;

    vector<Vertex> m_vertices;
    vector<ushort> m_indices;

    uint m_tmp;
    vector<uint8_t> _dFactors;

    GLuint m_frameBuf, m_renderBuf, m_renderTexture;
    GLuint m_VS, m_FS, m_prog;
    GLuint m_VBO, m_EBO;
    GLuint m_positionAttrib, m_colorAttrib,m_mvpUniform;

    // Shaders
    const char* m_vertexShader
        = "#version 130\n                                                   \
            in vec3 position;                                               \
            in vec3 color;                                                  \
            uniform mat4 mvp;                                               \
            out vec3 c;                                                     \
            void main() {                                                   \
                gl_Position = mvp*vec4(position,1);                         \
                c = color;                                                  \
            }";
    const char* m_fragmentShader
        = "#version 130\n                                                   \
            in vec3 c;                                                      \
            out vec4 fragColor;                                             \
            void main() {                                                   \
                fragColor = vec4(c,1);                                      \
            }";
};


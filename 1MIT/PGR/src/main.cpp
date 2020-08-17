/*
 * File:   main.cpp
 * Author: David Koleckar - xkolec07@stud.fit.vutbr.cz
 *         Matus Motlik - xmotli02@stud.fit.vutbr.cz
 */

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "pgr.h"
#include "Triangle.h"
#include "Scene.h"
#include "Radiosity.h"

#define HEMICUBE_SIZE 180
#define MAX_ITERATIONS 500

using namespace std;

GLuint VBO, EBO;
int width, height;
bool renderWithRad = false;
float rx = 0.0f, ry = 0.0f, pz = -70.0f;
float wheel = 1.0f;


// Shaders
const char * VSSource
    = "#version 130\n                                                   \
        in vec3 position;                                               \
        in vec3 color;                                                  \
        in vec3 radiosity;                                              \
        uniform int withRadiosity;                                      \
        uniform mat4 mvp;                                               \
        out vec3 c;                                                     \
        out vec3 rad;                                                   \
        flat out int withRad;                                           \
        void main() {                                                   \
            gl_Position = mvp*vec4(position,1);                         \
            c = color;                                                  \
            rad = radiosity;                                            \
            withRad = withRadiosity;                                    \
        }";
const char * FSSource
    = "#version 130\n                                                   \
        in vec3 c;                                                      \
        in vec3 rad;                                                    \
        flat in int withRad;                                            \
        out vec4 fragColor;                                             \
        void main() {                                                   \
            if (withRad > 0) {                                          \
            fragColor = vec4(c*rad*255, 1) + vec4(1) * 0.1;             \
            } else {                                                    \
                fragColor = vec4(c, 1);                                 \
            }                                                           \
        }";

GLuint VS, FS, Prog;

GLuint positionAttrib, colorAttrib, mvpUniform, radAttrib, withRadiosity;


// Scene object
string m_objFile = "";
Radiosity m_radiosity;

/**
 *  Write information about scene to cmd
 */
void writeCMD(){
    if(renderWithRad){
        std::cout << "Render scene with radiosity" << std::endl;
    }
    else{
        std::cout << "Render scene without radiosity" << std::endl;
    }

    std::cout << "Scene file name: " << m_objFile << std::endl;
    std::cout << "Number of triangles in scene: " << Scene::getInstance().getTriangles().size() << std::endl;
    std::cout << "USAGE:" << std::endl 
              << "\tPress 'r' to compute radiosity in scene." << std::endl
              << "\tPress 's' to switch modes 'with/without' radiosity." << std::endl
              << "\tPress 'f' to enable polygon mode GL_FILL." << std::endl
              << "\tPress 'l' to enable polygon mode GL_LINE." << std::endl
              << std::endl;

}

void onInit()
{
    Scene& m_scene = Scene::getInstance();
    m_scene.loadObjFile(m_objFile);

    writeCMD();

    m_radiosity.initHemicube(HEMICUBE_SIZE);

    // Shader
    VS = compileShader(GL_VERTEX_SHADER, VSSource);
    FS = compileShader(GL_FRAGMENT_SHADER, FSSource);
    Prog = linkShader(2, VS, FS);

    positionAttrib = glGetAttribLocation(Prog, "position");
    colorAttrib = glGetAttribLocation(Prog, "color");
    radAttrib = glGetAttribLocation(Prog, "radiosity");
    mvpUniform = glGetUniformLocation(Prog, "mvp");
    withRadiosity = glGetUniformLocation(Prog, "withRadiosity");

    // Copy scene to graphics card
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_scene.getVerticesSize(), m_scene.getVertices(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_scene.getIndicesSize(), m_scene.getIndices(), GL_STATIC_DRAW);
}

void onWindowRedraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glUseProgram(Prog);

    glViewport(0, 0, width, height);

    //MVP
    glm::mat4 projection;
    float aspect = (float)width/(float)height;
    projection = glm::perspective(10.0f*wheel, aspect, 1.0f, 1000.0f);
    glm::mat4 mvp = glm::rotate(
            glm::rotate(
                glm::translate(
                    projection,
                    glm::vec3(0, 0, pz)
                    ),
                glm::radians(ry), glm::vec3(1, 0, 0)
                ),
            glm::radians(rx), glm::vec3(0, 1, 0)
            );

    glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));

    glEnableVertexAttribArray(positionAttrib);
    glEnableVertexAttribArray(colorAttrib);
    glEnableVertexAttribArray(radAttrib);

    if(!renderWithRad){
        glUniform1i(withRadiosity, 0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glVertexAttribPointer(radAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, radiosity));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, Scene::getInstance().getIndicesCount(), GL_UNSIGNED_SHORT, NULL);
    }
    else{
        glUniform1i(withRadiosity, 1);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, Scene::getInstance().getVerticesSize(), Scene::getInstance().getVertices(), GL_STATIC_DRAW);
        glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glVertexAttribPointer(radAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, radiosity));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, Scene::getInstance().getIndicesCount(), GL_UNSIGNED_SHORT, NULL);
    }
    SDL_GL_SwapBuffers();
}

void onWindowResized(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w; height = h;
}

void onKeyDown(SDLKey key, Uint16 /*mod*/)
{
    switch(key) {
        case SDLK_ESCAPE : quit(); return;
        case SDLK_r : {
            // Compute radiosity
            m_radiosity.computeRadiosity(MAX_ITERATIONS);
            renderWithRad = true; 
            break;
        }
        case SDLK_s : {
            renderWithRad = !renderWithRad; 
            break;
        }
        case SDLK_f : {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        }
        case SDLK_l : {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        }
        default : return;
    }
    redraw();
}

void onKeyUp(SDLKey /*key*/, Uint16 /*mod*/)
{
}

void onMouseMove(unsigned /*x*/, unsigned /*y*/, int xrel, int yrel, Uint8 buttons)
{
    if(buttons & SDL_BUTTON_LMASK)
    {
        rx += xrel;
        ry += yrel;
        redraw();
    }
    if(buttons & SDL_BUTTON_RMASK)
    {
        pz += yrel;
        redraw();
    }
}

void onMouseDown(Uint8 button, unsigned /*x*/, unsigned /*y*/)
{
    switch(button) {
        case SDL_BUTTON_WHEELUP : wheel += 1; break;
        case SDL_BUTTON_WHEELDOWN : wheel -= 1;/* if(wheel < 1) wheel = 1; */break;
        default : return;
    };
    redraw();
}

void onMouseUp(Uint8 /*button*/, unsigned /*x*/, unsigned /*y*/)
{
}


// Main
int main(int argc, char ** argv)
{
    if (argc != 2) {
        std::cerr << "Usage: main <scene.obj>" << argc << std::endl;
        return -1;
    }
    m_objFile = argv[1];

    try {
        // Init SDL - only video subsystem will be used
        if(SDL_Init(SDL_INIT_VIDEO) < 0) throw SDL_Exception();

        // Shutdown SDL when program ends
        atexit(SDL_Quit);

        init(800, 600, 24, 16, 0);

        mainLoop();

    } catch(exception & ex) {
        cout << "ERROR : " << ex.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

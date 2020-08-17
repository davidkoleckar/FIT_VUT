/*
 * File:   Hemicube.cpp
 * Author: David Koleckar - xkolec07@stud.fit.vutbr.cz
 *         Matus Motlik - xmotli02@stud.fit.vutbr.cz
 */

#include "Hemicube.h"

Hemicube::Hemicube() : m_size(0), m_renderSize(0), m_vertices(0), m_indices(0) { }

Hemicube::~Hemicube() {
  delete[] viewData;
  
  glDeleteFramebuffers(1, &m_frameBuf);
  glDeleteRenderbuffers(1, &m_renderBuf);
}

void Hemicube::init(uint size)  {
  m_size = size;
  m_renderSize = 2*size;
  m_deltaFactors.resize(pow(m_renderSize,2), 0.0f);

  viewData = new GLubyte[m_renderSize*m_renderSize*3];
  m_tmp = 0;

  vector<Vertex> origVertices = Scene::getInstance().getVerticesVec();

  for (Triangle t : Scene::getInstance().getTriangles()) {
    uint index = (t.getIndex() + 1);// * 11111;
    glm::vec3 color;
    color.x = index % 256;
    color.y = (index >> 8) % 256;
    color.z = (index >> 16) % 256;
    color /= 255.0f;
    vector<ushort> triangleIndices = t.getTriangleIndices();
    for (ushort i : triangleIndices) {
      glm::vec3 origPos = origVertices.at(i).position;
      glm::vec3 origRad = origVertices.at(i).radiosity;
      Vertex v = {origPos , color, origRad, 1};
      // Save triangle vertices to new vector
      m_vertices.push_back(v);
      // Save triangle indices to new vector
      m_indices.push_back(m_vertices.size() - 1);
    }
  } 

  m_VS = compileShader(GL_VERTEX_SHADER, m_vertexShader);
  m_FS = compileShader(GL_FRAGMENT_SHADER, m_fragmentShader);
  m_prog = linkShader(2, m_VS, m_FS);
  
  m_positionAttrib = glGetAttribLocation(m_prog, "position");
  m_colorAttrib = glGetAttribLocation(m_prog, "color");
  m_mvpUniform = glGetUniformLocation(m_prog, "mvp");

  // Copy scene to graphics card
  glGenBuffers(1, &m_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices.front(), GL_STATIC_DRAW);

  glGenBuffers(1, &m_EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(ushort), &m_indices.front(), GL_STATIC_DRAW);

  // Frame buffer
  glGenFramebuffers(1, &m_frameBuf);
  glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuf);
  // Render buffer
  glGenRenderbuffers(1, &m_renderBuf);
  glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuf);

  glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, m_renderSize, m_renderSize);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_renderBuf);

  // Offscreen framebuffer texture target
  glGenTextures(1, &m_renderTexture);
  glBindTexture(GL_TEXTURE_2D, m_renderTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_renderSize, m_renderSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTexture, 0); 

  // Switch to default frame buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

uint Hemicube::getRenderSize() {
  return m_renderSize;
}

uint Hemicube::getColorIndex(uint i) {
  glm::vec3 color = glm::vec3((float)viewData[i*3], (float)viewData[i*3+1], (float)viewData[i*3+2]);
  uint index = decodeColor(color);
  return index;
}

float Hemicube::getDeltaFormFactor(uint i) {
  return m_deltaFactors.at(i);
}

/**
 * Compute all delta form factors in Hemicube
 */
void Hemicube::computeDeltaFactors() {
  // For each point in hemicube compute deltaFactor
  for (int y = 0; y < (int)m_renderSize; y++) {
    for (int x = 0; x < (int)m_renderSize; x++) {
      m_deltaFactors.at(y*m_renderSize + x) = deltaFactor(x,y);
    }
  }
}

void Hemicube::readHemicubeBuffer() {
  glReadPixels(0, 0, m_renderSize, m_renderSize, GL_RGB, GL_UNSIGNED_BYTE, viewData);
}

uint Hemicube::decodeColor(glm::vec3 color){
  return (uint)color.x + ((uint)color.y << 8) + ((uint)color.z << 16);
}

/**
 *Compute delta form factor
 */
float Hemicube::deltaFactor(int pointX, int pointY){
  float x,y,z;

  int _full = (int) m_renderSize;
  int _half = _full >> 1;
  int _1quarter = _full >> 2;
  int _3quarters = _half + _1quarter;
  float N = _1quarter;

  //CENTER
  if(pointX >= _1quarter && pointX < _3quarters &&
     pointY >= _1quarter && pointY < _3quarters)
  {
    x = (((float)pointX - _half));
    y = (((float)pointY - _half));
    z = 1;
  }
  //TOP
  else if(pointX >= _1quarter && pointX < _3quarters &&
          pointY >= 0         && pointY < _1quarter)
  {
    x = (((float)pointX - _half));
    y = ( (float)pointY         );
    z = y;
  }
  //BOTTOM
  else if(pointX >= _1quarter  && pointX < _3quarters &&
          pointY >= _3quarters && pointY < _full)
  {
    x = (((float)pointX - _half));
    y = ((_full - (float)pointY));
    z = y;
  }
  //RIGHT
  else if(pointX >= _3quarters && pointX < _full &&
          pointY >= _1quarter  && pointY < _3quarters)
  {
    x = ((_full - (float)pointX));
    y = (((float)pointY - _half));
    z = x;
  }
  //LEFT
  else if(pointX >= 0         && pointX < _1quarter &&
          pointY >= _1quarter && pointY < _3quarters)
  {
    x = ((float)pointX         ) ;
    y = (((float)pointY - _half));
    z = x;
  }
  else
  {
    return 0.0;
  }

  float temp = (float) x*x + y*y + 1;
  float value = z / (float)(M_PI*temp*temp * N * N);

  return value;
}

/*
 * Render hemicube view
 */
void Hemicube::renderHemicube(Triangle maxPatch) {

  uint _full = m_renderSize;
  uint _half = _full >> 1;
  uint _1quarter = _full >> 2;

  glm::mat4 projection;

  maxPatch.calculateNormal();
  glm::vec3 eye = maxPatch.getCentroid();
  glm::vec3 n = glm::normalize(maxPatch.getNormal());
  glm::vec3 dir = glm::normalize(maxPatch.getCentroid() - maxPatch.getVertex1()); 
  glm::vec3 right = glm::cross(dir, n);

  glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuf);
  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glUseProgram(m_prog);

  for(int i = 1; i <= 5; i++){
    glm::vec3 m_dir;
    glm::vec3 m_up;

    switch(i){
      case 1: //top
        glViewport(_1quarter, _half + _1quarter, _half, _1quarter);
        m_dir = glm::vec3(eye.x + dir.x, eye.y + dir.y, eye.z + dir.z);
        m_up = glm::vec3(-n.x, -n.y, -n.z);
        projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.5f, 1000.0f);
        break;
      case 2: //bottom
        glViewport(_1quarter, 0, _half, _1quarter);
        m_dir = glm::vec3(eye.x - dir.x, eye.y - dir.y, eye.z - dir.z);
        m_up = glm::vec3(n.x, n.y, n.z);
        projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.5f, 1000.0f);
        break;
      case 3: //center
        glViewport(_1quarter, _1quarter, _half, _half);
        m_dir = glm::vec3(eye.x + n.x, eye.y + n.y, eye.z + n.z);
        m_up = glm::vec3(dir.x, dir.y, dir.z);
        projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.5f, 1000.0f);
        break;
      case 4: //right
        glViewport(0, _1quarter, _1quarter, _half);
        m_dir = glm::vec3(eye.x + right.x, eye.y + right.y, eye.z + right.z);
        m_up = glm::vec3(dir.x, dir.y, dir.z);
        projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.5f, 1000.0f);
        break;
      case 5: //left
        glViewport(_half + _1quarter, _1quarter, _1quarter, _half);
        m_dir = glm::vec3(eye.x - right.x, eye.y - right.y, eye.z - right.z);
        m_up = glm::vec3(dir.x, dir.y, dir.z);
        projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.5f, 1000.0f);
        break;
    }

    glm::mat4 view = glm::lookAt(eye, m_dir, m_up);
    glm::mat4 mvp = projection * view;
    glUniformMatrix4fv(m_mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));

    renderColorHemicube();
  }
  readHemicubeBuffer();
  SDL_GL_SwapBuffers();

  // Reset to default frame buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Hemicube::renderColorHemicube() {
  glEnableVertexAttribArray(m_positionAttrib);
  glEnableVertexAttribArray(m_colorAttrib);

  // glShadeModel(GL_FLAT);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glVertexAttribPointer(m_positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
  glVertexAttribPointer(m_colorAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDrawElements(GL_TRIANGLES, Scene::getInstance().getIndicesCount(), GL_UNSIGNED_SHORT, NULL);
}

size_t ppm3_save(ppm_image *img, string filename) {
  ofstream file;
  file.open(filename, std::ofstream::out);

  size_t n = 0;
  file << "P3\n#Hemicube\n" 
       << img->width << " " << img->height 
       << "\n255\n";

  for (int y = 0; y < img->height; y++) {
    for (int x = 0; x < img->width * 3; x++) {
      file << (int)img->data[y*img->height + x] << " ";
      if (x % 3 == 2) 
        file << "\t";
    }
    file << endl;
  }

  file.close();
  return n;
}

size_t ppm2_save(ppm_image *img, string filename) {
  ofstream file;
  file.open(filename, std::ofstream::out);

  size_t n = 0;
  file << "P2\n#Deltafactors\n" 
       << img->width << " " << img->height 
       << "\n255\n";

  for (int y = 0; y < img->height; y++) {
    for (int x = 0; x < img->width; x++) {
      file << (int)img->data[y*img->height + x] << " ";
    }
    file << endl;
  }

  file.close();
  return n;
}

// void Hemicube::saveImage(string filename) {
//   m_tmp++;
//   filename = filename + to_string(m_tmp) + ".txt";
//   cout << "Saving image: " << filename << endl;
//   ppm_image *img =  new ppm_image;
//   img->width = getRenderSize();
//   img->height = getRenderSize();
//   img->size = getRenderSize() * getRenderSize() * 3;
//   img->data = viewData;
//   ppm3_save(img, filename);
//   delete img;
// }


void Hemicube::saveImage(string filename) {
  m_tmp++;
  filename = filename + to_string(m_tmp) + ".txt";
  cout << "Saving image: " << filename << endl;
  ppm_image *img =  new ppm_image;
  img->width = getRenderSize();
  img->height = getRenderSize();
  img->size = getRenderSize() * getRenderSize();
  for (float f : m_deltaFactors) {
    _dFactors.push_back((uint8_t) f * 255);
  }
  img->data = &_dFactors.front();
  ppm2_save(img, filename);
  delete img;
}
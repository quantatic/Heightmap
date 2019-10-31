#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"

class Map {
  Shader shader_;
  int width_;
  int height_;

  // Accessed as data_[(z * width + x) * 3 + {0,1,2}] -> this gives the coords
  // in (x,y,z), packed together
  float* vertex_data_;

  GLuint vao_;
  GLuint ebo_;
  GLuint vbo_;

  void InitVertexData(float startHeight);

 public:
  Map(int width, int height);

  void Render(GLFWwindow* window, float rotation);

  ~Map();
};

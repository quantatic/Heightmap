#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "map.h"
#include "shader.h"

const char* kVertexShader =
    R"(#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inColor;

layout (location = 0) uniform mat4 model;
layout (location = 1) uniform mat4 view;
layout (location = 2) uniform mat4 projection;

layout (location = 0) out vec3 outColor;

void main() {
  outColor = inColor;
  gl_Position = projection * view * model * vec4(aPos, 1.0);
})";

const char* kFragmentShader =
    R"(#version 430 core

layout (location = 0) in vec3 inColor;

layout (location = 0) out vec4 FragColor;

void main() {
  FragColor = vec4(inColor, 1.0f);
})";

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "Heightmap", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return -1;
  }

  glEnable(GL_DEPTH_TEST);

  glViewport(0, 0, 800, 600);

  Map m(25, 25);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  float rotation = 0;

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m.Render(window, rotation);

    glfwSwapBuffers(window);
    glfwPollEvents();
    rotation += 0.5;
  }

  glfwTerminate();
  return 0;
}

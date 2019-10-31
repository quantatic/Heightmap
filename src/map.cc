#include "map.h"

#include <cstdlib>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

const char* kMapVertexShader =
    R"(#version 430 core
layout (location = 0) in vec3 aPos;

layout (location = 0) uniform mat4 model;
layout (location = 1) uniform mat4 view;
layout (location = 2) uniform mat4 projection;

layout (location = 0) out vec3 outPos;

void main() {
  outPos = aPos;
  gl_Position = projection * view * model * vec4(aPos, 1.0);
})";

const char* kMapFragmentShader =
    R"(#version 430 core

layout (location = 0) in vec3 aPos;

layout (location = 0) out vec4 FragColor;

void main() {
  FragColor = vec4(vec3(aPos.y), 1.0f);
})";

void Map::InitVertexData(float startHeight) {
  for (int z = 0; z < height_; z++) {
    for (int x = 0; x < width_; x++) {
      float deltaHeight = ((rand() / (float)RAND_MAX) - 0.5) * 0.35;
      float newX = ((float)x / width_ - 0.5) * 2;
      float newZ = ((float)z / height_ - 0.5) * 2;

      float previousHeightsSummed = 0;
      int foundPreviousHeights = 0;

      if (z > 0) {
        previousHeightsSummed += vertex_data_[((z - 1) * width_ + x) * 3 + 1];
        foundPreviousHeights++;
      }

      if (x > 0) {
        previousHeightsSummed += vertex_data_[(z * width_ + (x - 1)) * 3 + 1];
        foundPreviousHeights++;
      }

      if (foundPreviousHeights == 0) {
        previousHeightsSummed = startHeight;
        foundPreviousHeights++;
      }

      float previousHeightsAverage =
          previousHeightsSummed / foundPreviousHeights;
      float newY = previousHeightsAverage + deltaHeight;

      vertex_data_[(z * width_ + x) * 3 + 0] = newX;
      vertex_data_[(z * width_ + x) * 3 + 1] = newY;
      vertex_data_[(z * width_ + x) * 3 + 2] = newZ;
    }
  }
}

Map::Map(int width, int height)
    : shader_(kMapVertexShader, kMapFragmentShader),
      width_(width),
      height_(height),
      vertex_data_(new float[width * height * 3]) {
  InitVertexData(0.0f);

  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, width * height * 3 * sizeof(float),
               vertex_data_, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  GLuint vertex_indices[(width - 1) * (height - 1) * 2 * 3];
  for (int z = 0; z < height - 1; z++) {
    for (int x = 0; x < width - 1; x++) {
      vertex_indices[(z * (width - 1) + x) * 6 + 0] = z * width + x;
      vertex_indices[(z * (width - 1) + x) * 6 + 1] = z * width + x + 1;
      vertex_indices[(z * (width - 1) + x) * 6 + 2] = (z + 1) * width + x;

      vertex_indices[(z * (width - 1) + x) * 6 + 3] = z * width + x + 1;
      vertex_indices[(z * (width - 1) + x) * 6 + 4] = (z + 1) * width + x;
      vertex_indices[(z * (width - 1) + x) * 6 + 5] = (z + 1) * width + x + 1;
    }
  }

  glGenBuffers(1, &ebo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               (width - 1) * (height - 1) * 2 * 3 * sizeof(GLuint),
               vertex_indices, GL_STATIC_DRAW);

  glBindVertexBuffer(0, vbo_, 0, 3 * sizeof(float));

  glEnableVertexAttribArray(0);
  glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexAttribBinding(0, 0);

  glBindVertexArray(0);
}

void Map::Render(GLFWwindow* window, float rotation) {
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  float aspect = (float)height / width;

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  model =
      glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

  glm::mat4 view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

  glm::mat4 projection =
      glm::perspective(glm::radians(30.0f), aspect, 0.1f, 100.0f);

  shader_.setMatrix4f(0, model);
  shader_.setMatrix4f(1, view);
  shader_.setMatrix4f(2, projection);
  shader_.Use();

  glBindVertexArray(vao_);

  glPointSize(5.0f);
  // glDrawArrays(GL_TRIANGLES, 0, width * height);
  glDrawElements(GL_TRIANGLES, (width - 1) * (height - 1) * 6, GL_UNSIGNED_INT,
                 NULL);
  glBindVertexArray(0);
}

Map::~Map() { delete[] vertex_data_; }

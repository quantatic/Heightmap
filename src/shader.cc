#include "shader.h"
#include <GL/glu.h>

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader::Shader(const GLchar* vertexSource, const GLchar* fragmentSource) {
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

  GLint success;
  GLchar infoLog[512];

  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING::FAILED\n"
              << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  id_ = shaderProgram;
}

void Shader::Use() { glUseProgram(id_); }

void Shader::setBool(GLint location, GLboolean value) {
  glUniform1i(location, value);
}

void Shader::setInt(GLint location, GLint value) {
  glUniform1i(location, value);
}

void Shader::setFloat(GLint location, GLfloat value) {
  glUniform1f(location, value);
}

void Shader::setMatrix4f(GLint location, glm::mat4 value) {
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

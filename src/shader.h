#ifndef SRC_SHADER_H
#define SRC_SHADER_H

#include <GL/glew.h>

#include <glm/glm.hpp>

class Shader {
  GLuint id_;

 public:
  Shader(const GLchar* vertexSource, const GLchar* fragmentSource);

  void Use();

  void setBool(GLint location, GLboolean value);
  void setInt(GLint location, GLint value);
  void setFloat(GLint location, GLfloat value);
  void setMatrix4f(GLint location, glm::mat4 value);
};

#endif  // SRC_SHADER_H

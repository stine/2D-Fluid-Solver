#ifndef __SHADER_H__
#define __SHADER_H__

#include "GL3/gl3.h"
#include <string>
#include <vector>

// Class to encapsulate all logic related to loading/compiling shader programs.
class Shader
{
  GLuint _program;

public:
  // Constructor
  Shader();

  // Destructor
  virtual ~Shader();

  // TODO documentation
  //
  // Arguments:
  //   const char *vs - The GLSW-compatible name of the vertex shader.
  //   const char *vs - The GLSW-compatible name of the geometry shader, or NULL.
  //   const char *vs - The GLSW-compatible name of the fragment shader.
  //
  // Returns:
  //   None
  void compile(const char *vs, const char *gs, const char *fs);

  // TODO documentation
  //
  // Arguments:
  //   None
  // 
  // Returns:
  //   None
  void del();

  // TODO documentation
  //
  // Arguments:
  //   None
  //
  // Returns:
  //   bool - True if the shader program is compiled and ready to be used.
  bool ok() const;

  // TODO documentation
  //
  // Arguments:
  //   None
  //
  // Returns:
  //   GLuint - The handle of the shader program.
  GLuint prog() const;
  
  // Cast operator to GLuint.  Returns an r-value.
  // 
  // Arguments:
  //   None
  //
  // Returns:
  //   GLuint - The handle of the shader program.
  operator const GLuint &() const;

protected:
  // TODO documentation
  GLuint shaderObj(GLenum shaderType, const char *shaderText) const;

  // TODO documentation  
  GLuint shaderProgram(const std::vector<GLuint> &shaderList) const;
};

#endif

#include "GL3/gl3.h"
#include "IFluidRenderer.h"
#include <cstdio>

GLuint IFluidRenderer::shaderObj(GLenum shaderType, const char *shaderText)
{
  // Compile the provided shader text into a shader object.
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderText, NULL);
  glCompileShader(shader);
    
  // Test for failure.  Log an error if so.
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
    
    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
    
    const char *strShaderType = NULL;
    switch(shaderType) {
    case GL_VERTEX_SHADER:   strShaderType = "vertex";   break;
    case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
    case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
    }
        
    fprintf(stderr, "Compilation error in %s shader:\n%s\n", strShaderType, strInfoLog);
    delete [] strInfoLog;
  }
  
  // Return the compiled shader object.
  return shader;
}

GLuint IFluidRenderer::shaderProgram(const std::vector<GLuint> &shaderList)
{
  // Link the provided shader objects into a shader program.
  GLuint program = glCreateProgram();
  for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
    glAttachShader(program, shaderList[iLoop]);
  glLinkProgram(program);
    
  // Test for failure.  Log an error if so.
  GLint status;
  glGetProgramiv (program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        
    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
    fprintf(stderr, "Linker failure: %s\n", strInfoLog);
    delete [] strInfoLog;
  }
    
  // Return the linked shader program.
  return program;
}


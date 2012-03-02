#include "Shader.h"
#include "GL3/gl3.h"
#include <cstdio>
#include <vector>
#include "glsw.h"

using std::vector;

Shader::Shader()
  : _program(0)
{}


Shader::~Shader()
{
  del();
}


void Shader::compile(const char *vs, const char *gs, const char *fs)
{
  vector<GLuint> objects;

  // Clean up any existing shader program.
  del();

  // Compile shader programs.
  objects.push_back(shaderObj(GL_VERTEX_SHADER, vs));
  if (gs) objects.push_back(shaderObj(GL_GEOMETRY_SHADER, gs));    
  objects.push_back(shaderObj(GL_FRAGMENT_SHADER, fs));

  // Link shader program.
  _program = shaderProgram(objects);
}


void Shader::del()
{
  // Clean up the shader program.
  if (glIsProgram(_program))
    glDeleteProgram(_program);
  _program = 0;
}


bool Shader::ok() const
{
  return glIsProgram(_program);
}


GLuint Shader::prog() const
{
  return _program;
}


Shader::operator const GLuint &() const
{
  return _program;
}


GLuint Shader::shaderObj(GLenum shaderType, const char *shaderName) const
{
  glswInit();
  
  // Load shader program text.
  glswSetPath("./", ".glsl");
  const char *shaderText = glswGetShader(shaderName);
  if (!shaderText) {
    fprintf(stderr, "%s\n", glswGetError());
    glswShutdown();
    return 0;
  }

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
        
    fprintf(stderr, "Compilation error in %s shader named \"%s\":\n%s\n",
	    strShaderType, shaderName, strInfoLog);
    delete [] strInfoLog;
  }
  
  // Return the compiled shader object.
  glswShutdown();
  return shader;
}


GLuint Shader::shaderProgram(const std::vector<GLuint> &shaderList) const
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
    fprintf(stderr, "Linker failure: %s\n\n", strInfoLog);
    delete [] strInfoLog;
  }
    
  // Return the linked shader program.
  return program;
}


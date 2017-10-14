#include "pipeline.hpp"
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>

GLuint loadShaderFromSourceCode(Shader::Type type, const char* sourcecode, int length)
{
  GLenum shaderType;
  std::string shaderName;
  switch(type) {
    case Shader::VERTEX:
      shaderType = GL_VERTEX_SHADER;
      shaderName = "Vertex Shader";
      break;
    case Shader::FRAGMENT:
      shaderType = GL_FRAGMENT_SHADER;
      shaderName = "Fragment Shader";
      break;
  }

  GLuint shaderId = glCreateShader(shaderType);

  glShaderSource(shaderId, 1, &sourcecode, &length);
  glCompileShader(shaderId);

  GLint isCompiled = 0;
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
  if(isCompiled == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

    auto errorLog = std::make_unique<GLchar[]>(maxLength);
    glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);

    std::cout << "Error compiling " << shaderName << std::endl
      << &errorLog[0] << std::endl;
    glDeleteShader(shaderId); // Don't leak the shader.
    return 0;
  }

  return shaderId;
}

GLuint loadShaderFromFile(Shader::Type type, const char* filepath)
{
  std::ifstream fstream;
  fstream.open(filepath);

  if (!fstream.is_open())
  {
    std::cout << "Unable to open file '" << filepath << "'" << std::endl;
    return 0;
  }

  std::stringstream sstream;
  std::string line;
  while (std::getline(fstream, line))
    sstream << line << '\n';
  line = sstream.str();

  GLuint shaderId = loadShaderFromSourceCode(type, line.c_str(), line.length());
  if (!shaderId)
    std::cout << "...with filepath '" << filepath << "'"; 

  return shaderId;
} 

GLuint Pipeline::s_activeProgramId = 0;

Pipeline::Pipeline()
  : m_programId(0)
{
}

Pipeline::~Pipeline()
{
  if (m_programId == s_activeProgramId)
  {
    glUseProgram(0);
    s_activeProgramId = 0;
  }
}

void Pipeline::loadFromFile(std::string vertexShaderFilepath, std::string fragmentShaderFilepath)
{
  if (m_programId)
    glDeleteProgram(m_programId);

  m_programId = glCreateProgram();

  GLuint vertexShaderId = loadShaderFromFile(Shader::VERTEX, vertexShaderFilepath.c_str());
  if (!vertexShaderId)
    return;

  GLuint fragmentShaderId = loadShaderFromFile(Shader::FRAGMENT, fragmentShaderFilepath.c_str());
  if (!fragmentShaderId)
    return;

  glAttachShader(m_programId, vertexShaderId);
  glAttachShader(m_programId, fragmentShaderId);
  glLinkProgram(m_programId);
  glDetachShader(m_programId, vertexShaderId);
  glDetachShader(m_programId, fragmentShaderId);
  glDeleteShader(vertexShaderId);
  glDeleteShader(fragmentShaderId);
}

void Pipeline::makeActive()
{
  if (m_programId != s_activeProgramId)
  {
    glUseProgram(m_programId);
    s_activeProgramId = m_programId;
  }
}

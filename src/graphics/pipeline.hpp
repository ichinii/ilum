#ifndef PIPELINE_H
#define PIPELINE_H

#include <GL/glew.h>
#include <string>

namespace Shader {
  enum Type {
    VERTEX,
    FRAGMENT
  };
}

class Pipeline {
public:
  Pipeline();
  ~Pipeline();

  void loadFromFile(std::string vertexShaderFilepath, std::string fragmentShaderFilepath);
  void makeActive();

private:
  GLuint m_programId;
  static GLuint s_activeProgramId;
};

#endif

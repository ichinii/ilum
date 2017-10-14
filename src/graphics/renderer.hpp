#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>

class Pipeline;

class Renderer {
public:
  Renderer();
  ~Renderer();

  void render(float x0, float y0, float x1, float y1, float x2, float y2);
  void render(float* arr, std::size_t size);
  void render(glm::vec2 v0, glm::vec2 v1, glm::vec2 v2);
  void render(glm::vec2 v0, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3);
  void renderCircle(glm::vec2 pos, float radius, int slices = 30);
  void renderRect(glm::vec2 pos, glm::vec2 size);
  void flush();

  std::size_t capacity();
  void setCapacity(std::size_t capacity);

private:
  std::size_t m_capacity;
  std::size_t m_size;
  std::unique_ptr<float[]> m_vertices;;
  GLuint m_vao;
  GLuint m_vbo;
  std::unique_ptr<Pipeline> m_pipeline;
};

#endif

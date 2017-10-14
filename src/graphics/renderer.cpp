#include "renderer.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include "pipeline.hpp"

Renderer::Renderer()
  : m_capacity(0)
  , m_size(0)
{
  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_vbo);

  glBindVertexArray(m_vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0,
    2,
    GL_FLOAT,
    GL_FALSE,
    0,
    static_cast<void*>(nullptr)    
  );

  setCapacity(1024);

  m_pipeline = std::make_unique<Pipeline>();
  m_pipeline->loadFromFile("data/vertex.glsl", "data/fragment.glsl");
}

Renderer::~Renderer()
{
  glDeleteBuffers(1, &m_vbo);
  glDeleteVertexArrays(1, &m_vao);
}

void Renderer::render(float x0, float y0, float x1, float y1, float x2, float y2)
{
  if (m_capacity <= m_size + 3 * 2)
    flush();

  m_vertices[m_size++] = x0;
  m_vertices[m_size++] = y0;
  m_vertices[m_size++] = x1;
  m_vertices[m_size++] = y1;
  m_vertices[m_size++] = x2;
  m_vertices[m_size++] = y2;
}

void Renderer::render(float* arr, std::size_t size)
{
  for (int i = 0; i + 6 < size; i += 6)
    render(arr[i], arr[i+1], arr[i+2], arr[i+3], arr[i+4], arr[i+5]);
}

void Renderer::render(glm::vec2 v0, glm::vec2 v1, glm::vec2 v2)
{
  if (m_capacity <= m_size + 3 * 2)
    flush();

  m_vertices[m_size++] = v0.x;
  m_vertices[m_size++] = v0.y;
  m_vertices[m_size++] = v1.x;
  m_vertices[m_size++] = v1.y;
  m_vertices[m_size++] = v2.x;
  m_vertices[m_size++] = v2.y;
}

void Renderer::render(glm::vec2 v0, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
{
  render(v0, v1, v2);
  render(v2, v3, v0);
}


void Renderer::renderCircle(glm::vec2 pos, float radius, int slices)
{
  float prevX = pos.y + radius * std::cos(0.f);
  float prevY = pos.x + radius * std::sin(0.f);
  for (int i = 1; i < slices + 1; i++)
  {
    float x = pos.x + radius * std::cos(static_cast<float>(i) / slices * 3.14f * 2.f);
    float y = pos.y + radius * std::sin(static_cast<float>(i) / slices * 3.14f * 2.f);
    
    render(pos.x, pos.y, prevX, prevY, x, y);

    prevX = x;
    prevY = y;
  }
}


void Renderer::renderRect(glm::vec2 pos, glm::vec2 size)
{
  glm::vec2 v0(pos);
  glm::vec2 v1(pos.x + size.x, pos.y);
  glm::vec2 v2(pos.x + size.x, pos.y + size.y);
  glm::vec2 v3(pos.x, pos.y + size.y);
  render(v0, v1, v2, v3);
}

void Renderer::flush()
{
  if (m_size == 0)
    return;

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(float), &m_vertices[0], GL_STATIC_DRAW);

  m_pipeline->makeActive();

  glBindVertexArray(m_vao);
  glDrawArrays(GL_TRIANGLES, 0, m_size / 2);
  glBindVertexArray(0);

  m_size = 0;
}

std::size_t Renderer::capacity()
{
  return m_capacity;
}

void Renderer::setCapacity(std::size_t capacity)
{
  if (capacity == 0)
    return;

  if (m_vertices)
    flush();

  m_vertices = std::make_unique<float[]>(capacity);
  m_capacity = capacity;
}

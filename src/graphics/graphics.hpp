#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <glm/glm.hpp>
#include <memory>

class GLFWwindow;
class Renderer;

class Graphics {
public:
  Graphics();
  ~Graphics();

  void clear();
  void display();
  void render();
  void handleWindowEvents();

  Renderer* renderer();
  bool windowShouldClose();
  glm::vec2 size();
  void setSize(glm::vec2 size);

private:
  GLFWwindow* m_window;
  std::unique_ptr<Renderer> m_renderer;
};

#endif

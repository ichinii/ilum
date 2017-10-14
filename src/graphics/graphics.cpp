#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "graphics.hpp"
#include "renderer.hpp"

Graphics::Graphics()
{
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);

  m_window = glfwCreateWindow(800, 600, "ilum", nullptr, nullptr);
  glfwMakeContextCurrent(m_window);

  glewInit();

  glfwSetFramebufferSizeCallback(m_window, [] (GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); 
  });

  glfwSetWindowSizeCallback(m_window, [] (GLFWwindow* window, int width, int height) {
    
  });

  //glEnable(GL_CULL_FACE);
  //glCullFace(GL_BACK);
 
  m_renderer = std::make_unique<Renderer>(); 

}

Graphics::~Graphics()
{
  glfwSetFramebufferSizeCallback(m_window, nullptr);
  glfwSetWindowShouldClose(m_window, true);
  glfwTerminate();
}

void Graphics::clear()
{
  glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::display()
{
  glfwSwapBuffers(m_window);
}


void Graphics::render()
{
  m_renderer->render(glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1));
  m_renderer->flush();
}

void Graphics::handleWindowEvents()
{
  glfwPollEvents();
}

Renderer* Graphics::renderer()
{
  return m_renderer.get();
}

bool Graphics::windowShouldClose()
{
  return glfwWindowShouldClose(m_window);
}

glm::vec2 Graphics::size()
{
  glm::ivec2 size;
  glfwGetWindowSize(m_window, &size.x, &size.y);
  return size;
}

void Graphics::setSize(glm::vec2 size)
{
  glfwSetWindowSize(m_window, size.x, size.y);
}

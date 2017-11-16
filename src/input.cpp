#include "input.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>

std::map<int, int> keyStates;
std::map<int, int> mouseStates;
glm::vec2 cursorPos;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (action == 1)
    keyStates[key] = 1;
  else if (action == 0)
    keyStates[key] = -1; 
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
  cursorPos = glm::vec2(xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
  mouseStates[button] = action == GLFW_PRESS ? -1 : 1;
}



Input::Input()
  : m_window(nullptr)
{
}

Input::~Input()
{
}

bool Input::released(Key key)
{
  return keyStates[keyToGLFWenum(key)] == -1;
}

bool Input::pressed(Key key)
{
  return keyStates[keyToGLFWenum(key)] == 1;
}

bool Input::up(Key key)
{
  return keyStates[keyToGLFWenum(key)] < 0;
}

bool Input::down(Key key)
{
  return keyStates[keyToGLFWenum(key)] > 0;
}

std::chrono::milliseconds Input::duration(Key key)
{
  return std::chrono::milliseconds(std::abs(keyStates[keyToGLFWenum(key)]) - 1);
}

Input::Button Input::button(Key key)
{
  return Button(this, key);
}

void Input::update(std::chrono::milliseconds deltaTime)
{
  for (auto& keyState: keyStates)
    keyState.second += std::min(1, std::max(-1, keyState.second)) * deltaTime.count();
}

GLFWwindow* Input::window()
{
  return m_window;
}

void Input::setWindow(GLFWwindow* window)
{
  m_window = window;
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
}

int Input::keyToGLFWenum(Key key)
{
  static std::map<Key, int> mapping {
    { Key::A, GLFW_KEY_A },
    { Key::Q, GLFW_KEY_Q },
    { Key::W, GLFW_KEY_W },
    { Key::E, GLFW_KEY_E },
    { Key::R, GLFW_KEY_R },
    { Key::T, GLFW_KEY_T },
    { Key::A, GLFW_KEY_A },
    { Key::S, GLFW_KEY_S },
    { Key::D, GLFW_KEY_D },
    { Key::F, GLFW_KEY_F },
    { Key::Y, GLFW_KEY_Y },
    { Key::X, GLFW_KEY_X },
    { Key::SPACE, GLFW_KEY_SPACE },
    { Key::MOUSE_LEFT, GLFW_MOUSE_BUTTON_1 },
    { Key::MOUSE_RIGHT, GLFW_MOUSE_BUTTON_2 },
  };

  return mapping[key];
}


/* class Button */
Input::Button::Button(Input* input, Key key)
  : m_input(input), m_key(key)
{
}

bool Input::Button::released()
{
  return m_input->released(m_key);
}

bool Input::Button::pressed()
{
  return m_input->pressed(m_key);
}

bool Input::Button::up()
{
  return m_input->up(m_key);
}

bool Input::Button::down()
{
  return m_input->down(m_key);
}

std::chrono::milliseconds Input::Button::duration()
{
  return m_input->duration(m_key);
}

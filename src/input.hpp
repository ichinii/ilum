#ifndef INPUT_HPP
#define INPUT_HPP

#include <map>
#include <chrono>

class GLFWwindow;

enum class Key {
  Q, W, E, R, T,
  A, S, D, F,
  Y, X,
	SPACE,
  MOUSE_LEFT,
  MOUSE_RIGHT,
};

class Input {
public:
  Input();
  ~Input();

  class Button;

  bool released(Key key);
  bool pressed(Key key);
  bool up(Key key);
  bool down(Key key);
  std::chrono::milliseconds duration(Key key);
  Button button(Key key);

  void update(std::chrono::milliseconds deltaTime);

  GLFWwindow* window();
  void setWindow(GLFWwindow* window);

private:
  GLFWwindow* m_window;

  int keyToGLFWenum(Key key);
};

class Input::Button {
public:
  Button(Input* input, Key key);

  bool released();
  bool pressed();
  bool up();
  bool down();
  std::chrono::milliseconds duration();

private:
  Input* m_input;
  Key m_key;
};

#endif

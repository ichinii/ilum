#ifndef FIGURE_HPP
#define FIGURE_HPP

#include <chrono>
#include <map>
#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
#include <functional>
#include "input.hpp"
#include "physics.hpp"

class Renderer;
class Controlls;
class ITool;

class Figure : protected Collider {
public:
  Figure();
  ~Figure();

  void update(std::chrono::milliseconds deltaTime);
  void render(Renderer* renderer);

  using ToolUpdateFn = std::function<void(std::chrono::milliseconds, Figure* figure, Input::Button)>;
  void addTool(Input::Button button, ToolUpdateFn update);
  //void addTool(std::unique_ptr<ITool*> tool);

  static Figure createApe(Input* input);

private:
  using BindButton_ToolUpdateFn = decltype( std::bind(
      std::declval<ToolUpdateFn>(),
      std::placeholders::_1,
      std::placeholders::_2,
      std::declval<Input::Button>() ) );

  std::vector<BindButton_ToolUpdateFn> m_toolFns;
};

#endif

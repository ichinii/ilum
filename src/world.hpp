#ifndef WORLD_H
#define WORLD_H

#include <map>
#include <vector>
#include <string>
#include <chrono>
#include <glm/glm.hpp>
#include <Box2D/Box2D.h>
#include <physics.hpp>

class Renderer;

class World : private Collider {
public:
  World();
  ~World();

  void update(std::chrono::milliseconds deltaTime);
  void render(Renderer* renderer);

private:
};

#endif

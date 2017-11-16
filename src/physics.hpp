#ifndef PHYSICS_H
#define PHYSICS_H

#include <Box2D/Box2D.h>
#include <chrono>
#include <map>
#include <glm/glm.hpp>
#include <string>

inline b2World* physics()
{
  static b2World* world = new b2World(b2Vec2(0, -10.f));
  return world;
}

#include <iostream>
inline void updatePhysics(std::chrono::milliseconds deltaTime)
{
  const int velocityIterations = 10;
  const int positionIterations = 8;

  physics()->Step(deltaTime.count() / 1000.f, velocityIterations, positionIterations);
}

class Renderer;

class Collider {
public:
  std::map<std::string, b2Body*> bodies;
  std::map<std::string, b2BodyDef> bodyDefs;
  std::map<std::string, b2FixtureDef> fixtureDefs;
  std::map<std::string, glm::vec3> colors;
  std::map<std::string, b2CircleShape> circleShapes;
  std::map<std::string, b2ChainShape> chainShapes;
  std::map<std::string, b2PolygonShape> polygonShapes;
  std::map<std::string, b2DistanceJointDef> distanceJointDefs;
  
  void render(Renderer* renderer);
};

#endif

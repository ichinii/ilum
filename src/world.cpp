#include "world.hpp"
#include "physics.hpp"
#include <graphics/renderer.hpp>
#include <iostream>

float easeQuadraticInOut (float t) {
  t = 1.f-t;
  float b = 0;
  float c = 1;
  float d = 1;
  t /= d/2;
  if (t < 1) return c/2*t*t + b;
  t--;
  return -c/2 * (t*(t-2) - 1) + b;
}

void box(b2Vec2 bottomLeft, b2Vec2 topRight,
    b2Body** body, b2BodyDef* bodyDef, b2PolygonShape* shape)
{
  b2Vec2 size = b2Vec2((topRight.x - bottomLeft.x) / 2, (topRight.y - bottomLeft.y) / 2);
  bodyDef->position = bottomLeft + b2Vec2(size.x, size.y);
  bodyDef->type = b2_staticBody;
  *body = physics()->CreateBody(bodyDef);
  shape->SetAsBox(size.x, size.y);
  (*body)->CreateFixture(shape, 0.f);
}

void dynamicBox(b2Vec2 bottomLeft, b2Vec2 topRight,
    b2Body** body, b2BodyDef* bodyDef, b2PolygonShape* shape, b2FixtureDef* fixtureDef)
{
  b2Vec2 size = topRight - bottomLeft;
  bodyDef->position.Set(bottomLeft.x + topRight.y / 2, bottomLeft.y + topRight.y / 2);
  bodyDef->type = b2_staticBody;
  *body = physics()->CreateBody(bodyDef);
  shape->SetAsBox(size.x, size.y);
  fixtureDef->shape = shape;
  fixtureDef->density = 1.f;
  fixtureDef->friction = .3f;
  (*body)->CreateFixture(fixtureDef);
}

void qpipe(b2Vec2 pos, b2Vec2 size,
    b2Body** body, b2BodyDef* bodyDef, b2ChainShape* shape)
{
  bodyDef->type = b2_staticBody;
  bodyDef->position = pos;
  *body = physics()->CreateBody(bodyDef);

  const float pi = 3.1415926535897931f;
  const int slices = 16;
  auto rad = [=] (float i) { return pi + i / slices * pi * .5f; };
  b2Vec2 points[slices + 1];
  for (int i = 0; i < slices + 1; i++) {
    b2Vec2 point = b2Vec2((std::cos(rad(i)) + 1) * size.x, (std::sin(rad(i)) + 1) * size.y);
    points[i] = point;
  }
  shape->CreateChain(points, slices + 1);
  (*body)->CreateFixture(shape, 0.f);
}

void interpolation(b2Vec2 pos, b2Vec2 size,
    b2Body** body, b2BodyDef* bodyDef, b2ChainShape* shape, float(*fn) (float))
{
  bodyDef->type = b2_staticBody;
  bodyDef->position = pos;
  *body = physics()->CreateBody(bodyDef);

  const float slices = 16;
  b2Vec2 points[static_cast<int>(slices) + 1];
  for (int i = 0; i < slices + 1; i++)
    points[i] = b2Vec2(i / slices * size.x, fn(i / slices) * size.y);
  shape->CreateChain(points, slices + 1);
  (*body)->CreateFixture(shape, 0.f);
}

World::World()
{
  box(b2Vec2(0, 0), b2Vec2(20, 1), &bodies["2"], &bodyDefs["2"], &polygonShapes["2"]);
  box(b2Vec2(-0.1f, 0), b2Vec2(0, 20), &bodies["0"], &bodyDefs["0"], &polygonShapes["0"]);
  box(b2Vec2(20, 0), b2Vec2(20.1f, 20), &bodies["1"], &bodyDefs["1"], &polygonShapes["1"]);
  box(b2Vec2(0, 12), b2Vec2(20, 12.1f), &bodies["3"], &bodyDefs["3"], &polygonShapes["3"]);

  //box(b2Vec2(10, 1), b2Vec2(12, 2), &bodies["4"], &bodyDefs["4"], &polygonShapes["4"]);
  interpolation(b2Vec2(8, 1), b2Vec2(-2, 1), &bodies["4"], &bodyDefs["4"], &chainShapes["4"],
      easeQuadraticInOut);
  interpolation(b2Vec2(8, 1), b2Vec2(2, 1), &bodies["5"], &bodyDefs["5"], &chainShapes["5"],
      easeQuadraticInOut);
  qpipe(b2Vec2(20, 1), b2Vec2(-4, 4), &bodies["6"], &bodyDefs["6"], &chainShapes["6"]);

  for (auto& [key, body] : bodies)
    colors[key] = glm::vec3(1, 1, 1);
}

World::~World()
{
}

void World::update(std::chrono::milliseconds deltaTime)
{

}

void World::render(Renderer* renderer)
{
  Collider::render(renderer);
}

#include "physics.hpp"
#include <graphics/renderer.hpp>

void Collider::render(Renderer* renderer)
{
  renderer->setColor(glm::vec4(1, 1, 1, 1));

  const auto v = [] (b2Vec2 v) { return glm::vec2(v.x, v.y); };
  const float indicate = .75f;

  for (auto [key, shape] : circleShapes) {
    renderer->setColor(colors[key]);
    renderer->renderCircle( v( bodies[key]->GetPosition() ), shape.m_radius); 

    renderer->setColor(colors[key] * indicate);
    renderer->renderCircle( 
          v( bodies[key]->GetPosition() )
        + shape.m_radius * .5f
        * glm::vec2( std::cos(bodies[key]->GetAngle()), std::sin(bodies[key]->GetAngle()) )
        , shape.m_radius * .5f);
  }

  for (auto [key, shape] : polygonShapes)
  {
    auto body = bodies[key];

    renderer->setColor(colors[key]);
    renderer->render(
        v(body->GetPosition() + shape.GetVertex(0)),
        v(body->GetPosition() + shape.GetVertex(1)),
        v(body->GetPosition() + shape.GetVertex(2)),
        v(body->GetPosition() + shape.GetVertex(3)));
  }

  for (auto& [key, shape] : chainShapes)
  {
    auto body = bodies[key];

    renderer->setColor(colors[key]);
    for (int i = 0; i < shape.GetChildCount(); i++) {
      b2EdgeShape edge;
      shape.GetChildEdge(&edge, i);
      glm::vec2 v0 = v(body->GetPosition()) + glm::vec2(edge.m_vertex0.x, 0);
      glm::vec2 v1 = v(body->GetPosition()) + glm::vec2(edge.m_vertex1.x, 0);
      glm::vec2 v2 = v(body->GetPosition() + edge.m_vertex1);
      glm::vec2 v3 = v(body->GetPosition() + edge.m_vertex0);
      renderer->render(v0, v1, v2, v3);
    }
  }
}

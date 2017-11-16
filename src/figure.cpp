#include "figure.hpp"

void circle(b2Vec2 pos, float radius,
    b2Body** body, b2BodyDef* bodyDef, b2Shape* shape, b2FixtureDef* fixtureDef)
{
  bodyDef->position = pos;
  bodyDef->type = b2_dynamicBody;
  *body = physics()->CreateBody(bodyDef);
  shape->m_radius = radius;
  fixtureDef->shape = shape;
  fixtureDef->density = 1.f;
  fixtureDef->friction = 0.0f;
  (*body)->CreateFixture(fixtureDef);
}

void distanceJoint(b2DistanceJointDef* jointDef, b2Body* b0, b2Body* b1)
{
  jointDef->Initialize(b0, b1, b0->GetPosition(), b1->GetPosition());
  physics()->CreateJoint(jointDef);
}

Figure::Figure()
{
}

Figure::~Figure()
{
  for (auto [key, body]: bodies)
    physics()->DestroyBody(body);
}

void Figure::update(std::chrono::milliseconds deltaTime)
{
  for (auto toolFn : m_toolFns)
    toolFn(deltaTime, this);
}

void Figure::render(Renderer* renderer)
{
  Collider::render(renderer);
}

void Figure::addTool(Input::Button button, ToolUpdateFn update)
{
  m_toolFns.push_back(std::bind(
      update,
      std::placeholders::_1,
      std::placeholders::_2,
      button));
}

Figure Figure::createApe(Input* input)
{
  Figure f;

  circle(b2Vec2(4, 3), .5f, &f.bodies["p"], &f.bodyDefs["p"], &f.circleShapes["p"], &f.fixtureDefs["p"]);
  //circle(b2Vec2(5.3, 3), .2f, &f.bodies["l"], &f.bodyDefs["l"], &f.circleShapes["l"], &f.fixtureDefs["l"]);
  //circle(b2Vec2(2.7, 3), .2f, &f.bodies["r"], &f.bodyDefs["r"], &f.circleShapes["r"], &f.fixtureDefs["r"]);
  //distanceJoint(&f.distanceJointDefs["l"], f.bodies["l"], f.bodies["p"]);
  //distanceJoint(&f.distanceJointDefs["r"], f.bodies["r"], f.bodies["p"]);
  f.colors["p"] = glm::vec3(1, 1, 1);
  //f.colors["l"] = glm::vec3(0, 1, 0);
  //f.colors["r"] = glm::vec3(0, 0, 1);
  
  f.addTool(input->button(Key::A), [] (
      std::chrono::milliseconds deltaTime,
      Figure* f,
      Input::Button button) {
        auto vel = f->bodies["p"]->GetLinearVelocity();

        if (button.down())
          f->bodies["p"]->ApplyForceToCenter(b2Vec2(-20.f + 2.f * std::min(10.f, std::abs(vel.x)), 0), true);
      });

  f.addTool(input->button(Key::D), [] (
      std::chrono::milliseconds deltaTime,
      Figure* f,
      Input::Button button) {
        auto vel = f->bodies["p"]->GetLinearVelocity();

        if (button.down())
          f->bodies["p"]->ApplyForceToCenter(b2Vec2( 20.f - 2.f * std::min(10.f, std::abs(vel.x)), 0), true);
      });

  f.addTool(input->button(Key::SPACE), [] (
      std::chrono::milliseconds deltaTime,
      Figure* f,
      Input::Button button) {
        auto fix = f->bodies["p"]->GetFixtureList();
	
				if (button.down())
					fix->SetRestitution(1.f);
				else
					fix->SetRestitution(0.f);

      });

  return f;
}

#include "graphics/graphics.hpp"
#include "graphics/renderer.hpp"
#include "graphics/pipeline.hpp"
#include "world.hpp"
#include "physics.hpp"
#include "input.hpp"
#include "figure.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char* argv[])
{
  std::cout << "Hello world!" << std::endl;

  std::chrono::steady_clock clock;
  Graphics graphics;
  Input input;
  input.setWindow(graphics.window());
  World world;
  Figure figure = Figure::createApe(&input); // trust rvo or move

  auto clockStart = clock.now();
  auto clockPrev = clockStart;
  float tickSpeed = 100.f;
  int ticks = 0;
  auto clockFpsPrev = clockStart;

  while (!graphics.windowShouldClose()) {
    using namespace std::chrono;
    using namespace std::chrono_literals;
    auto clockNow = clock.now();
    auto deltaNow = duration_cast<milliseconds>(clockNow - clockPrev);
    auto delta = duration(milliseconds(static_cast<int>(
      std::max(1.f, deltaNow.count() / 1000.f * tickSpeed) * 1000.f / tickSpeed
    )));

    // wait for frame
    auto sleepDuration = (delta - (clock.now() - clockPrev));
    if (sleepDuration.count() > 0)
      std::this_thread::sleep_for(sleepDuration);
    clockPrev = clockPrev + delta;
    // begin frame

    input.update(delta);
    graphics.handleWindowEvents();

    updatePhysics(delta);
    world.update(delta);
    figure.update(delta);

    glm::mat4 mvp = glm::ortho<float>(0, 20, 0, 12);
    glUniformMatrix4fv(glGetUniformLocation(graphics.renderer()->m_pipeline->programId(), "mvp"), 1, GL_FALSE, &mvp[0][0]);

    graphics.clear();
    world.render(graphics.renderer());
    figure.render(graphics.renderer());
    graphics.render();
    graphics.display();

    // end frame
    
    ticks++;
    if (ticks % static_cast<int>(tickSpeed) == 0) {
      std::cout
          << "fps: "
          << ticks
          << ", duration (in ms): "
          << (duration_cast<milliseconds>(clockNow - clockFpsPrev).count())
          << std::endl;

      clockFpsPrev = clockNow;
      ticks = 0;
    }
  }

  return 0;
}

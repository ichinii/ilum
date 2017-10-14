#include <iostream>
#include "graphics/graphics.hpp"

int main(int argc, char* argv[])
{
  std::cout << "Hello world!" << std::endl;

  Graphics g;

  while (!g.windowShouldClose()) {
    g.handleWindowEvents();

    g.clear();
    g.render();
    g.display();
  }

  return 0;
}

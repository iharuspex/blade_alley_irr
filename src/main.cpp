#include <iostream>

#include "Game.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char *argv[]) {
  std::cout << "game init attempt...\n";

  if (Game::instance()->init(L"Blade Alley Irr!", WINDOW_WIDTH, WINDOW_HEIGHT,
                             false)) {
    std::cout << "game init success!\n";

    while (Game::instance()->isRunning()) {
      Game::instance()->update();
      Game::instance()->handleEvents();
      Game::instance()->render();
    }
  }

  Game::instance()->clean();

  return 0;
}
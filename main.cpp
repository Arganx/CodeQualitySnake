#include "inc/game.hpp"
#include "tools/inc/visualiser.hpp"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>

int main() {
  Game::Game game;
  uint8_t menuSelection;
  while (true) {
    std::cout << "Snake game\n";
    std::cout << "1) New game\n";
    std::cout << "2) Exit\n";
    std::cin >> menuSelection;
    switch (menuSelection) {
    case '1':
      game.initGame(5, 4);
      tools::Visualiser::visualiseBoard(*game.getBoardPtr());
      while (true) {
        std::this_thread::sleep_for(
            static_cast<std::chrono::milliseconds>(500));
        game.step();
        tools::Visualiser::visualiseBoard(*game.getBoardPtr());
      }
      break;
    case '2':
      std::cout << "Exiting\n";
      return 0;
    default:
      std::cout << "Non existing option. Try again\n";
    }
  }
}
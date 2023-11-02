#include "../inc/visualiser.hpp"
#include <cstdint>
#include <iostream>

namespace tools {
void Visualiser::visualiseBoard(const Game::Board &board) {
  auto boardVector{board.getBoard()};
  for (uint8_t height_index{0}; height_index < board.getHeight();
       ++height_index) {
    for (uint8_t width_index{0}; width_index < board.getWidth();
         ++width_index) {
      std::cout << boardVector[height_index][width_index];
    }
    std::cout << "\n";
  }
}
} // namespace tools
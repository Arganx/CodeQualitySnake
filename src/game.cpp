#include "../inc/game.hpp"

#include <concepts>
#include <cstdint>
#include <iostream>

constexpr char kCellInitValue = '*';

namespace Game {

void Game::printWelcome() const { std::cout << "Welcome" << std::endl; }

void Game::showStatus() const { std::cout << "Score: " << score << std::endl; }

uint8_t Game::getWidth() const { return width; };

uint8_t Game::getHeight() const { return height; };

void Game::setWidth(const uint8_t iWidth) {
  if (iWidth > 0) {
    width = iWidth;
  } else {
    throw std::invalid_argument("Width must be a positive number.");
  }
};

void Game::setHeight(const uint8_t iHeight) {
  if (iHeight > 0) {
    height = iHeight;
  } else {
    throw std::invalid_argument("Height must be a positive number.");
  }
};

void Game::createBoard() {
  board.clear();
  board.reserve(height);
  for (uint8_t heightIndex{0U}; heightIndex < height; ++heightIndex) {
    board.emplace_back(width, kCellInitValue);
  }
}

std::vector<std::vector<uint8_t>> Game::getBoard() const { return board; }

} // namespace Game
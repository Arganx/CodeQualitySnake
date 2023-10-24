#include "../inc/game.hpp"

#include <concepts>
#include <cstdint>
#include <iostream>

namespace Game {
void Game::printWelcome() const { std::cout << "Welcome" << std::endl; }
void Game::showStatus() const {
  std::cout << "Score: " << this->score << std::endl;
}

uint8_t Game::getWidth() const { return this->width; };
uint8_t Game::getHeight() const { return this->height; };

void Game::setWidth(const uint8_t iWidth) {
  if (iWidth > 0) {
    this->width = iWidth;
  } else {
    throw std::invalid_argument("Width must be a positive number.");
  }
};
void Game::setHeight(const uint8_t iHeight) {
  if (iHeight > 0) {
    this->height = iHeight;
  } else {
    throw std::invalid_argument("Height must be a positive number.");
  }
};
} // namespace Game
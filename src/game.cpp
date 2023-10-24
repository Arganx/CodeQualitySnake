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

void Game::setWidth(const uint8_t iWidth) { this->width = iWidth; };
void Game::setHeight(const uint8_t iHeight) { this->height = iHeight; };
} // namespace Game
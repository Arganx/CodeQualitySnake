#include "../inc/game.hpp"

#include <iostream>
#include <memory>

namespace Game {

void Game::printWelcome() const { std::cout << "Welcome" << std::endl; }

void Game::showStatus() const { std::cout << "Score: " << score << std::endl; }

void Game::initGame(uint8_t iWidth, uint8_t iHeight) {
  boardPtr = std::make_unique<Board>(iWidth, iHeight);
}

std::unique_ptr<Board> &Game::getBoardPtr() { return boardPtr; };

Game::Game() = default;

} // namespace Game
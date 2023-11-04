#include "../inc/game.hpp"
#include "../inc/board_position.hpp"
#include "../inc/segment.hpp"

#include "../inc/board_mappings.hpp"
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
namespace Game {

void Game::printWelcome() const { std::cout << "Welcome" << std::endl; }

void Game::showStatus() const { std::cout << "Score: " << score << std::endl; }

void Game::initGame(uint8_t iWidth, uint8_t iHeight) {
  boardPtr = std::make_unique<Board>(iWidth, iHeight);
  snake = std::make_unique<Snake>(
      Segment{BoardPosition{static_cast<uint8_t>(std::floor(iWidth / 2U)),
                            static_cast<uint8_t>(std::floor(iHeight / 2U))}});
}

std::unique_ptr<Board> &Game::getBoardPtr() { return boardPtr; };

Game::Game() = default;

void Game::step() {
  std::cout << "Step" << std::endl;
  cleanSnake();
  // TODO move snake
  drawSnake();
}

void Game::drawSnake() {
  if (snake->getHeadPosition()) {
    boardPtr->drawCharacter(snake->getHeadPosition()->get(),
                            BoardMapping::kSnakeHead);
  }
}

void Game::cleanSnake() {
  for (const auto &segment : snake->getSnakeSegments()) {
    boardPtr->drawCharacter(segment.getPosition(), BoardMapping::kEmptySpace);
  }
}

} // namespace Game
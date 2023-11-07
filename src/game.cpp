#include "../inc/game.hpp"
#include "../inc/board_position.hpp"
#include "../inc/segment.hpp"

#include "../inc/board_mappings.hpp"
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
namespace Game {

void Game::printWelcome() const { std::cout << "Welcome" << std::endl; }

void Game::showStatus() const { std::cout << "Score: " << score << std::endl; }

void Game::initGame(uint8_t iWidth, uint8_t iHeight) {
  boardPtr = std::make_unique<Board>(iWidth, iHeight);
  snakePtr = std::make_unique<Snake>(
      Segment{BoardPosition{static_cast<uint8_t>(std::floor(iWidth / 2U)),
                            static_cast<uint8_t>(std::floor(iHeight / 2U))}});
}

std::unique_ptr<Board> &Game::getBoardPtr() { return boardPtr; };

Game::Game() = default;

void Game::step() {
  std::cout << "Step" << std::endl;
  cleanFullSnake();
  drawFullSnake();
}

void Game::checkIfPointersAreInitialized() const {
  if (snakePtr == nullptr) {
    throw std::invalid_argument("Snake is not initialized");
  }
  if (boardPtr == nullptr) {
    throw std::invalid_argument("Board is not initialized");
  }
}

void Game::drawSnake() {
  checkIfPointersAreInitialized();
  if (snakePtr->getHeadPosition()) {
    boardPtr->drawCharacter(snakePtr->getHeadPosition()->get(),
                            BoardMapping::kSnakeHead);
  }
}

void Game::cleanFullSnake() {
  checkIfPointersAreInitialized();
  for (const auto &segment : snakePtr->getSnakeSegments()) {
    boardPtr->drawCharacter(segment.getPosition(), BoardMapping::kEmptySpace);
  }
}

void Game::drawFullSnake() {
  checkIfPointersAreInitialized();

  if (snakePtr->getHeadPosition()) {
    boardPtr->drawCharacter(snakePtr->getHeadPosition()->get(),
                            BoardMapping::kSnakeHead);
  }
  for (auto segmentIterator = std::next(snakePtr->getSnakeSegments().cbegin());
       segmentIterator < snakePtr->getSnakeSegments().cend();
       ++segmentIterator) {
    boardPtr->drawCharacter(segmentIterator->getPosition(),
                            BoardMapping::kSnakeBody);
  }
}

} // namespace Game
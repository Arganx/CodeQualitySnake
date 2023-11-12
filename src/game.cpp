#include "../inc/game.hpp"
#include "../inc/board_position.hpp"
#include "../inc/segment.hpp"

#include "../inc/board_mappings.hpp"
#include <bits/ranges_algo.h>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
namespace Game {

void Game::showStatus() const { std::cout << "Score: " << score << "\n"; }

void Game::initGame(uint8_t iWidth, uint8_t iHeight) {
  boardPtr = std::make_unique<Board>(iWidth, iHeight);
  snakePtr = std::make_unique<Snake>(
      Segment{BoardPosition{static_cast<uint8_t>(std::floor(iWidth / 2U)),
                            static_cast<uint8_t>(std::floor(iHeight / 2U))}});
  drawFullSnake();
  drawSnack();
}

std::unique_ptr<Board> &Game::getBoardPtr() { return boardPtr; };

Game::Game() = default;

void Game::step() {
  cleanFullSnake();
  moveSnake();
  drawFullSnake();
}

void Game::moveSnake() {
  checkIfPointersAreInitialized();
  if (!snakePtr->getHeadPosition()) {
    throw std::invalid_argument("Snake head not initialized");
  }
  BoardPosition nextPosition{snakePtr->getHeadPosition()->get()};
  bool isPassingBoardBorder = false;
  switch (direction) {
    using enum Direction::Direction;
  case Right:
    nextPosition.incrementX();
    if (nextPosition.getXPosition() >= boardPtr.get()->getWidth()) {
      isPassingBoardBorder = true;
      nextPosition.setXPosition(0U);
    }
    break;
  case Left:
    nextPosition.decrementX();
    if (nextPosition.getXPosition() == 255U) {
      isPassingBoardBorder = true;
      nextPosition.setXPosition(boardPtr.get()->getWidth() -
                                static_cast<uint8_t>(1U));
    }
    break;
  case Down:
    nextPosition.incrementY();
    if (nextPosition.getYPosition() >= boardPtr.get()->getHeight()) {
      isPassingBoardBorder = true;
      nextPosition.setYPosition(0U);
    }
    break;
  case Up:
    nextPosition.decrementY();
    if (nextPosition.getYPosition() == 255U) {
      isPassingBoardBorder = true;
      nextPosition.setYPosition(boardPtr.get()->getHeight() -
                                static_cast<uint8_t>(1U));
    }
    break;
  default:
    throw std::invalid_argument("Unsupported direction");
  }
  snakePtr->move(nextPosition,
                 isPassingBoardBorder); // This position can be outside of the
                                        // current board or with turned unsigned
}

void Game::checkIfSnakeIsInitialized() const {
  if (snakePtr == nullptr) {
    throw std::invalid_argument("Snake is not initialized");
  }
}

void Game::checkIfPointersAreInitialized() const {
  checkIfSnakeIsInitialized();
  if (boardPtr == nullptr) {
    throw std::invalid_argument("Board is not initialized");
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

void Game::drawSnack(const uint8_t iNumberOfSnacks) {
  checkIfPointersAreInitialized();
  auto availablePositions = boardPtr->getAvailablePositions();
  std::vector<BoardPosition> selectedPositions;
  auto gen = std::mt19937{std::random_device{}()};
  std::ranges::sample(availablePositions, std::back_inserter(selectedPositions),
                      iNumberOfSnacks, gen);
  for (const auto &position : selectedPositions) {
    boardPtr->drawCharacter(position, BoardMapping::kSnack);
  }
}

void Game::setDirection(const Direction::Direction iDirection) {
  direction = iDirection;
}

} // namespace Game
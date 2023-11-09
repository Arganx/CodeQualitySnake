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

void Game::printWelcome() const { std::cout << "Welcome" << std::endl; }

void Game::showStatus() const { std::cout << "Score: " << score << std::endl; }

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

} // namespace Game
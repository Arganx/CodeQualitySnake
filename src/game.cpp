#include "../inc/game.hpp"
#include "../inc/board_position.hpp"
#include "../inc/segment.hpp"

#include "../inc/board_mappings.hpp"
#include "../inc/direction.hpp"
#include "../inc/exceptions.hpp"
#include "../inc/limited_uint8_t.hpp"
#include <bits/ranges_algo.h>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
namespace Game {

void Game::showStatus() const { std::cout << "Score: " << score << "\n"; }

uint16_t Game::getScore() const { return score; };

void Game::initGame(uint8_t iWidth, uint8_t iHeight) {
  boardPtr = std::make_unique<Board>(iWidth, iHeight);
  snakePtr = std::make_unique<Snake>(Segment{BoardPosition{
      Limited_uint8_t{static_cast<uint8_t>(std::floor(iWidth / 2U)),
                      static_cast<uint8_t>(iWidth - 1U)},
      Limited_uint8_t{static_cast<uint8_t>(std::floor(iHeight / 2U)),
                      static_cast<uint8_t>(iHeight - 1U)}}});
  drawFullSnake();
  drawSnack();
  gameEnded = false;
  direction = Direction::Direction::Right;
  score = 0;
}

std::unique_ptr<Board> &Game::getBoardPtr() { return boardPtr; };

Game::Game() = default;

bool Game::step() {
  if (gameEnded) {
    return false;
  }
  cleanLastSegmentOfSnake();
  // 0 fail
  // 1 ok
  // 2 snack
  switch (moveSnake()) {
  default:
    [[fallthrough]];
  case 0:
    return false;
  case 1:
    drawMovablePartsOfSnake();
    break;
  case 2:
    drawMovablePartsOfSnake();
    drawSnack();
    break;
  }

  return true;
}

uint8_t Game::moveSnake() {
  checkIfPointersAreInitialized();
  if (!snakePtr->getHeadPosition()) {
    throw SnakeExceptions::PointerNotInitializedException(
        "Snake head not initialized");
  }
  BoardPosition nextPosition{snakePtr->getHeadPosition()->get()};
  switch (direction) {
    using enum Direction::Direction;
  case Right:
    nextPosition.incrementX();
    break;
  case Left:
    nextPosition.decrementX();
    break;
  case Down:
    nextPosition.incrementY();
    break;
  case Up:
    nextPosition.decrementY();
    break;
  default:
    throw SnakeExceptions::UnexpectedSwitchValue("Unsupported direction");
  }

  switch (boardPtr.get()->getBoard()[nextPosition.getYPosition().getValue()]
                                    [nextPosition.getXPosition().getValue()]) {
  case BoardMapping::kSnack:
    ++score;
    snakePtr->move(nextPosition, true);
    return 2;
  case BoardMapping::kSnakeBody:
    gameEnded = true;
    return 0;
  default:
    snakePtr->move(nextPosition);
    return 1;
  }
  return 0;
}

void Game::checkIfSnakeIsInitialized() const {
  if (snakePtr == nullptr) {
    throw SnakeExceptions::PointerNotInitializedException(
        "Snake is not initialized");
  }
}

void Game::checkIfPointersAreInitialized() const {
  checkIfSnakeIsInitialized();
  if (boardPtr == nullptr) {
    throw SnakeExceptions::PointerNotInitializedException(
        "Board is not initialized");
  }
}

void Game::cleanLastSegmentOfSnake() {
  checkIfPointersAreInitialized();
  boardPtr->drawCharacter(snakePtr->getSnakeSegments().back().getPosition(),
                          BoardMapping::kEmptySpace);
}

// There are 3 movable parts: head, 1 segment, last segment. Not all of them
// need to exist.
void Game::drawMovablePartsOfSnake() {
  checkIfPointersAreInitialized();
  // draw head
  if (snakePtr->getHeadPosition()) {
    boardPtr->drawCharacter(snakePtr->getHeadPosition()->get(),
                            BoardMapping::kSnakeHead);
  }
  switch (snakePtr.get()->getSnakeSegments().size()) {
  default:
    // if more than 1 segments then draw last segment
    boardPtr->drawCharacter(snakePtr->getSnakeSegments().back().getPosition(),
                            BoardMapping::kSnakeBody);
    [[fallthrough]];
  case 2:
    // draw 1 segment
    boardPtr->drawCharacter(snakePtr->getSnakeSegments()[1].getPosition(),
                            BoardMapping::kSnakeBody);
    break;
  case 0:
  case 1:
    break;
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
  snackPositions.clear();
  auto availablePositions = boardPtr->getAvailablePositions();
  std::vector<BoardPosition> selectedPositions;
#ifdef TESTING
  auto gen = std::mt19937{42};
#else
  auto gen = std::mt19937{std::random_device{}()};
#endif
  std::ranges::sample(availablePositions, std::back_inserter(selectedPositions),
                      iNumberOfSnacks, gen);
  for (const auto &position : selectedPositions) {
    boardPtr->drawCharacter(position, BoardMapping::kSnack);
    snackPositions.emplace_back(position);
  }
}

void Game::setDirection(const Direction::Direction iDirection) {
  checkIfPointersAreInitialized();
  if (snakePtr.get()->getSnakeSegments().size() > 1) {
    auto snakeHeadXPosition =
        snakePtr->getHeadPosition().value().get().getXPosition();
    auto snakeHeadYPosition =
        snakePtr->getHeadPosition().value().get().getYPosition();
    auto snakeNextSegmentXPosition =
        snakePtr.get()->getSnakeSegments()[1].getPosition().getXPosition();
    auto snakeNextSegmentYPosition =
        snakePtr.get()->getSnakeSegments()[1].getPosition().getYPosition();
    switch (iDirection) {
      using enum Direction::Direction;
    case Right:
      if (snakeNextSegmentXPosition == snakeHeadXPosition + 1U) {
        return;
      }
      break;
    case Left:
      if (snakeNextSegmentXPosition == snakeHeadXPosition - 1U) {
        return;
      }
      break;
    case Down:
      if (snakeNextSegmentYPosition == snakeHeadYPosition + 1U) {
        return;
      }
      break;
    case Up:
      if (snakeNextSegmentYPosition == snakeHeadYPosition - 1U) {
        return;
      }
      break;
    default:
      throw SnakeExceptions::UnexpectedSwitchValue("Unsupported direction");
    }
  }
  direction = iDirection;
}

Snake &Game::getSnake() {
  checkIfSnakeIsInitialized();
  return *snakePtr;
}

const std::vector<BoardPosition> &Game::getSnacksPositions() const {
  return snackPositions;
}

Direction::Direction Game::getDirection() const { return direction; }

} // namespace Game
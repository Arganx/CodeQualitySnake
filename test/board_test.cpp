#include "../inc/board.hpp"
#include "board_mappings.hpp"
#include "board_position.hpp"
#include <bits/ranges_util.h>
#include <cstdint>
#include <limits.h>

#include "gtest/gtest.h"
#include <stdexcept>
namespace {

const uint8_t k0{0U};
const uint8_t k1{1U};
const uint8_t k2{2U};
const uint8_t k10{10U};
const uint8_t k100{100U};

class BoardFixture : public ::testing::Test {
private:
  Game::Board board{k10, k10};

protected:
  Game::Board &getBoard() { return board; };
};

TEST(BoardTest, CanGetWidth) {
  uint8_t kBoardWidth = 20U;
  uint8_t kBoardHeight = 10U;
  Game::Board board{kBoardWidth, kBoardHeight};
  EXPECT_EQ(board.getWidth(), kBoardWidth);
}

TEST(BoardTest, CanGetHeight) {
  uint8_t kBoardWidth = 5U;
  uint8_t kBoardHeight = 50U;
  Game::Board board{kBoardWidth, kBoardHeight};
  EXPECT_EQ(board.getHeight(), kBoardHeight);
}

TEST(BoardTest, ThrowsWhenBoardCreatedWithZeroWidth) {
  try {
    Game::Board board{k0, k10};
    FAIL() << "Should throw std::invalid_argument";
  } catch (const std::invalid_argument &e) {
    EXPECT_STREQ(e.what(), "Width must be a positive number");
  }
}

TEST(BoardTest, ThrowsWhenBoardCreatedWithZeroHeight) {
  try {
    Game::Board board{k10, k0};
    FAIL() << "Should throw std::invalid_argument";
  } catch (const std::invalid_argument &e) {
    EXPECT_STREQ(e.what(), "Height must be a positive number");
  }
}

TEST_F(BoardFixture, TestBoardDimensionsFromFixture) {
  EXPECT_EQ(getBoard().getWidth(), k10);
  EXPECT_EQ(getBoard().getHeight(), k10);
}

TEST_F(BoardFixture, CanChangeWidth) {
  getBoard().setWidth(k100);
  EXPECT_EQ(getBoard().getWidth(), k100);
  EXPECT_EQ(getBoard().getHeight(), k10);
}

TEST_F(BoardFixture, CanChangeHeight) {
  getBoard().setHeight(k100);
  EXPECT_EQ(getBoard().getWidth(), k10);
  EXPECT_EQ(getBoard().getHeight(), k100);
}

TEST_F(BoardFixture, CanChangeWidthAndHeight) {
  getBoard().setHeight(k100);
  getBoard().setWidth(k100);
  EXPECT_EQ(getBoard().getWidth(), k100);
  EXPECT_EQ(getBoard().getHeight(), k100);
}

TEST_F(BoardFixture, ThrowsWhenWidthSetToZero) {
  EXPECT_THROW(getBoard().setWidth(0), std::invalid_argument);
}

TEST_F(BoardFixture, ThrowsWhenHeighthSetToZero) {
  EXPECT_THROW(getBoard().setHeight(0), std::invalid_argument);
}

TEST_F(BoardFixture, CanCreateBoardVectorsSizesCheck) {
  for (const auto &row : getBoard().getBoard()) {
    EXPECT_EQ(row.size(), getBoard().getWidth());
  }
  EXPECT_EQ(getBoard().getBoard().size(), getBoard().getHeight());
}

TEST_F(BoardFixture, CanDrawOnBoard) {
  for (const auto &row : getBoard().getBoard()) {
    for (const auto &cell : row) {
      EXPECT_EQ(cell, BoardMapping::kEmptySpace);
    }
  }

  Game::BoardPosition positionOne{k2, k1};
  Game::BoardPosition positionTwo{k2, k2};

  getBoard().drawCharacter(positionOne, BoardMapping::kSnakeBody);
  getBoard().drawCharacter(positionTwo, BoardMapping::kSnakeHead);

  for (uint8_t heightCounter{0U}; heightCounter < getBoard().getHeight();
       ++heightCounter) {
    for (uint8_t widthCounter{0U}; widthCounter < getBoard().getWidth();
         ++widthCounter) {
      if (heightCounter == k1 && widthCounter == k2) {
        EXPECT_EQ(getBoard().getBoard()[heightCounter][widthCounter],
                  BoardMapping::kSnakeBody);
      } else if (heightCounter == k2 && widthCounter == k2) {
        EXPECT_EQ(getBoard().getBoard()[heightCounter][widthCounter],
                  BoardMapping::kSnakeHead);
      } else {
        EXPECT_EQ(getBoard().getBoard()[heightCounter][widthCounter],
                  BoardMapping::kEmptySpace);
      }
    }
  }
}

TEST_F(BoardFixture, CantDrawOutsideTheBoard) {
  Game::BoardPosition positionOne{k100, k1};
  try {
    getBoard().drawCharacter(positionOne, BoardMapping::kSnakeBody);
    FAIL() << "Should throw std::invalid_argument";
  } catch (const std::invalid_argument &exception) {
    EXPECT_STREQ(exception.what(), "Trying to draw outside of the board. X "
                                   "position bigger than board width.");
  }

  Game::BoardPosition positionTwo{k1, k10};
  try {
    getBoard().drawCharacter(positionTwo, BoardMapping::kSnakeBody);
    FAIL() << "Should throw std::invalid_argument";
  } catch (const std::invalid_argument &exception) {
    EXPECT_STREQ(exception.what(), "Trying to draw outside of the board. Y "
                                   "position bigger than board height.");
  }
}

TEST_F(BoardFixture, ByDefaultAllPositionsAreAvailable) {
  auto availablePositions = getBoard().getAvailablePositions();
  uint8_t counter{k0};
  for (auto heightIndex{k0}; heightIndex < getBoard().getHeight();
       ++heightIndex) {
    for (auto widthIndex{k0}; widthIndex < getBoard().getWidth();
         ++widthIndex) {
      EXPECT_EQ(availablePositions[counter].getXPosition(), widthIndex);
      EXPECT_EQ(availablePositions[counter].getYPosition(), heightIndex);
      ++counter;
    }
  }
}

TEST_F(BoardFixture, DrawingOnTheBoardMakesThePositionsUnavailable) {
  Game::BoardPosition positionOne{k0, k0};
  Game::BoardPosition positionTwo{k2, k0};
  Game::BoardPosition positionThree{k2, k1};
  getBoard().drawCharacter(positionOne, BoardMapping::kSnakeBody);
  getBoard().drawCharacter(positionTwo, BoardMapping::kSnakeHead);
  getBoard().drawCharacter(positionThree, BoardMapping::kSnack);

  auto availablePositions = getBoard().getAvailablePositions();
  for (auto heightIndex{k0}; heightIndex < getBoard().getHeight();
       ++heightIndex) {
    for (auto widthIndex{k0}; widthIndex < getBoard().getWidth();
         ++widthIndex) {
      Game::BoardPosition testedPosition{widthIndex, heightIndex};
      if (testedPosition == positionOne || testedPosition == positionTwo ||
          testedPosition == positionThree) {
        EXPECT_EQ(std::ranges::find(availablePositions, testedPosition),
                  availablePositions.end());
      } else {
        EXPECT_NE(std::ranges::find(availablePositions, testedPosition),
                  availablePositions.end());
      }
    }
  }
}

} // namespace
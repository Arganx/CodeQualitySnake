#include "../inc/board.hpp"
#include <cstdint>
#include <limits.h>

#include "gtest/gtest.h"
#include <stdexcept>
namespace {

const uint8_t k0{0U};
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

} // namespace
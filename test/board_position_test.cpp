#include "../inc/board_position.hpp"
#include <cstdint>

#include "gtest/gtest.h"

namespace {

const uint8_t k0{0U};
const uint8_t k1{1U};
const uint8_t k2{2U};
const uint8_t k3{3U};
const uint8_t k4{4U};
const uint8_t k10{10U};
const uint8_t k5{5U};

class BoardPositionFixture : public ::testing::Test {
private:
  Game::BoardPosition position{k1, k10};

protected:
  Game::BoardPosition &getPosition() { return position; };
};

TEST(BoardPositionTest, DefaultPositionIsZero) {
  Game::BoardPosition position{};
  EXPECT_EQ(position.getXPosition(), k0);
  EXPECT_EQ(position.getYPosition(), k0);
}

TEST_F(BoardPositionFixture, ConstructorPositionIsSaved) {
  EXPECT_EQ(getPosition().getXPosition(), k1);
  EXPECT_EQ(getPosition().getYPosition(), k10);
}

TEST_F(BoardPositionFixture, CanSetYPosition) {
  getPosition().setYPosition(k5);
  EXPECT_EQ(getPosition().getXPosition(), k1);
  EXPECT_EQ(getPosition().getYPosition(), k5);
}

TEST_F(BoardPositionFixture, CanSetXPosition) {
  getPosition().setXPosition(k5);
  EXPECT_EQ(getPosition().getXPosition(), k5);
  EXPECT_EQ(getPosition().getYPosition(), k10);
}

TEST_F(BoardPositionFixture, CanCompareBoardPositions) {
  Game::BoardPosition samePosition{k1, k10};
  Game::BoardPosition differentXPosition{k0, k10};
  Game::BoardPosition differentYPosition{k1, k5};
  Game::BoardPosition differentXAndYPosition{k10, k1};
  EXPECT_EQ(getPosition(), samePosition);
  EXPECT_NE(getPosition(), differentXPosition);
  EXPECT_NE(getPosition(), differentYPosition);
  EXPECT_NE(getPosition(), differentXAndYPosition);
}

TEST_F(BoardPositionFixture, DetectsAdjacentOnXAxis) {
  getPosition().setXPosition(k2);

  Game::BoardPosition samePosition{k2, k10};
  EXPECT_FALSE(getPosition().isAdjacent(samePosition));

  Game::BoardPosition adjacentLeft{k1, k10};
  EXPECT_TRUE(getPosition().isAdjacent(adjacentLeft));

  Game::BoardPosition adjacentRight{k3, k10};
  EXPECT_TRUE(getPosition().isAdjacent(adjacentRight));

  Game::BoardPosition farRight{k4, k10};
  EXPECT_FALSE(getPosition().isAdjacent(farRight));

  Game::BoardPosition farLeft{k0, k10};
  EXPECT_FALSE(getPosition().isAdjacent(farLeft));
}

TEST_F(BoardPositionFixture, DetectsAdjacentOnYAxis) {
  getPosition().setYPosition(k2);

  Game::BoardPosition samePosition{k1, k2};
  EXPECT_FALSE(getPosition().isAdjacent(samePosition));

  Game::BoardPosition adjacentTop{k1, k1};
  EXPECT_TRUE(getPosition().isAdjacent(adjacentTop));

  Game::BoardPosition adjacentDown{k1, k3};
  EXPECT_TRUE(getPosition().isAdjacent(adjacentDown));

  Game::BoardPosition farTop{k1, k0};
  EXPECT_FALSE(getPosition().isAdjacent(farTop));

  Game::BoardPosition farDown{k1, k4};
  EXPECT_FALSE(getPosition().isAdjacent(farDown));
}

TEST_F(BoardPositionFixture, DiagonalAreNotAdjacent) {
  getPosition().setYPosition(k1);

  Game::BoardPosition topRight{k2, k0};
  EXPECT_FALSE(getPosition().isAdjacent(topRight));

  Game::BoardPosition topLeft{k0, k0};
  EXPECT_FALSE(getPosition().isAdjacent(topLeft));

  Game::BoardPosition downRight{k2, k2};
  EXPECT_FALSE(getPosition().isAdjacent(downRight));

  Game::BoardPosition downLeft{k0, k2};
  EXPECT_FALSE(getPosition().isAdjacent(downLeft));
}

} // namespace
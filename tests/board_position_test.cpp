#include "../inc/board_position.hpp"
#include "../inc/limited_uint8_t.hpp"

#include "gtest/gtest.h"

namespace {

const Game::Limited_uint8_t k0{0U, 255U};
const Game::Limited_uint8_t k0Limit5{0U, 5U};
const Game::Limited_uint8_t k0Limit6{0U, 6U};
const Game::Limited_uint8_t k1Limit5{1U, 5U};
const Game::Limited_uint8_t k1{1U, 255U};
const Game::Limited_uint8_t k2{2U, 255U};
const Game::Limited_uint8_t k3{3U, 255U};
const Game::Limited_uint8_t k4{4U, 255U};
const Game::Limited_uint8_t k5{5U, 255U};
const Game::Limited_uint8_t k5Limit5{5U, 5U};
const Game::Limited_uint8_t k10{10U, 255U};
const Game::Limited_uint8_t k255{255U, 255U};

class BoardPositionFixture : public ::testing::Test {
private:
  Game::BoardPosition position{k1, k10};

protected:
  Game::BoardPosition &getPosition() { return position; };
};

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

TEST_F(BoardPositionFixture, IncrementAndDecrementTest) {
  // Test X axis
  getPosition().decrementX();
  EXPECT_EQ(getPosition().getXPosition(), k0);
  getPosition().decrementX();
  EXPECT_EQ(getPosition().getXPosition(), k255);
  getPosition().incrementX();
  EXPECT_EQ(getPosition().getXPosition(), k0);
  getPosition().incrementX();
  getPosition().incrementX();
  EXPECT_EQ(getPosition().getXPosition(), k2);

  // Test Y axis
  getPosition().setYPosition(k0);
  getPosition().decrementY();
  EXPECT_EQ(getPosition().getYPosition(), k255);
  getPosition().incrementY();
  getPosition().incrementY();
  EXPECT_EQ(getPosition().getYPosition(), k1);
  getPosition().decrementY();
  EXPECT_EQ(getPosition().getYPosition(), k0);
}

TEST_F(BoardPositionFixture, DetectsAdjacencyOverLimit) {
  getPosition().setXPosition(k5Limit5);
  getPosition().setYPosition(k5Limit5);

  Game::BoardPosition nonAdjacentPosition{k1Limit5, k5Limit5};
  Game::BoardPosition adjacentPositionOne{k0Limit5, k5Limit5};
  Game::BoardPosition adjacentPositionTwo{k0Limit6, k5Limit5};
  EXPECT_TRUE(adjacentPositionOne.isAdjacent(getPosition()));
  EXPECT_TRUE(getPosition().isAdjacent(adjacentPositionOne));
  EXPECT_TRUE(adjacentPositionTwo.isAdjacent(getPosition()));
  EXPECT_TRUE(getPosition().isAdjacent(adjacentPositionTwo));
  EXPECT_FALSE(nonAdjacentPosition.isAdjacent(getPosition()));
  EXPECT_FALSE(getPosition().isAdjacent(nonAdjacentPosition));
}

} // namespace
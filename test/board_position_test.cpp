#include "../inc/board_position.hpp"
#include <cstdint>

#include "gtest/gtest.h"

namespace {

const uint8_t k0{0U};
const uint8_t k1{1U};
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

} // namespace
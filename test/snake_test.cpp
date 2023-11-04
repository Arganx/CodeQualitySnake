#include "board_position.hpp"
#include "segment.hpp"
#include "snake.hpp"
#include "gtest/gtest.h"

namespace {

class SnakeFixture : public ::testing::Test {
private:
  Game::Snake snake{};

protected:
  Game::Snake &getSnake() { return snake; };
};

TEST(SnakeTest, DefaultSnakeCreation) {
  Game::Snake snake{};
  EXPECT_EQ(snake.getSnakeSegments().size(), 0);
  EXPECT_EQ(snake.getHeadPosition(), std::nullopt);
}

TEST(SnakeTest, CanCreateSnakeWithSegment) {
  Game::BoardPosition position{};
  Game::Segment segment{position};
  Game::Snake snake{segment};
  EXPECT_EQ(snake.getSnakeSegments().size(), 1);
  EXPECT_EQ(snake.getHeadPosition()->get(), position);
}

TEST_F(SnakeFixture, canAddSegments) {
  EXPECT_EQ(getSnake().getSnakeSegments().size(), 0);
  getSnake().addSegment(Game::Segment{Game::BoardPosition{}});
  EXPECT_EQ(getSnake().getSnakeSegments().size(), 1);
  getSnake().addSegment(Game::Segment{Game::BoardPosition{}});
  getSnake().addSegment(Game::Segment{Game::BoardPosition{}});
  EXPECT_EQ(getSnake().getSnakeSegments().size(), 3);
}

} // namespace
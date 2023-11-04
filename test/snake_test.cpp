#include "board_position.hpp"
#include "segment.hpp"
#include "snake.hpp"
#include "gtest/gtest.h"
#include <sys/types.h>

namespace {

constexpr u_int8_t k0{0U};
constexpr u_int8_t k1{1U};
constexpr u_int8_t k3{3U};

class SnakeFixture : public ::testing::Test {
private:
  Game::Snake snake{};

protected:
  Game::Snake &getSnake() { return snake; };
};

TEST(SnakeTest, DefaultSnakeCreation) {
  Game::Snake snake{};
  EXPECT_EQ(snake.getSnakeSegments().size(), k0);
  EXPECT_EQ(snake.getHeadPosition(), std::nullopt);
}

TEST(SnakeTest, CanCreateSnakeWithSegment) {
  Game::BoardPosition position{};
  Game::Segment segment{position};
  Game::Snake snake{segment};
  EXPECT_EQ(snake.getSnakeSegments().size(), k1);
  EXPECT_EQ(snake.getHeadPosition()->get(), position);
}

TEST_F(SnakeFixture, canAddSegments) {
  EXPECT_EQ(getSnake().getSnakeSegments().size(), k0);
  getSnake().addSegment(Game::Segment{Game::BoardPosition{}});
  EXPECT_EQ(getSnake().getSnakeSegments().size(), k1);
  getSnake().addSegment(Game::Segment{Game::BoardPosition{}});
  getSnake().addSegment(Game::Segment{Game::BoardPosition{}});
  EXPECT_EQ(getSnake().getSnakeSegments().size(), k3);
}

} // namespace
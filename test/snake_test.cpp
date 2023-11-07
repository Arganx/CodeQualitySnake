#include "board_position.hpp"
#include "segment.hpp"
#include "snake.hpp"
#include "gtest/gtest.h"
#include <stdexcept>
#include <sys/types.h>

namespace {

constexpr u_int8_t k0{0U};
constexpr u_int8_t k1{1U};
constexpr u_int8_t k2{2U};
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
  Game::Segment segment{Game::BoardPosition{k0, k1}};
  getSnake().addSegment(segment);
  getSnake().addSegment(Game::Segment{Game::BoardPosition{k1, k1}});
  EXPECT_EQ(getSnake().getSnakeSegments().size(), k3);
}

TEST_F(SnakeFixture, CantAddDuplicateSegments) {
  EXPECT_EQ(getSnake().getSnakeSegments().size(), k0);
  getSnake().addSegment(Game::Segment{Game::BoardPosition{}});
  EXPECT_EQ(getSnake().getSnakeSegments().size(), k1);
  getSnake().addSegment(Game::Segment{Game::BoardPosition{}});
  getSnake().addSegment(Game::Segment{Game::BoardPosition{}});
  EXPECT_EQ(getSnake().getSnakeSegments().size(), k1);
}

TEST_F(SnakeFixture, CantMoveToSelf) {
  getSnake().addSegment(Game::Segment{Game::BoardPosition{}});
  try {
    getSnake().move(Game::BoardPosition{});
    FAIL() << "Should have thrown";
  } catch (const std::invalid_argument &exception) {
    EXPECT_STREQ(exception.what(), "Trying to move to non adjacent tile");
  }
}

TEST_F(SnakeFixture, SegmentsMoveCorrectly) {
  Game::BoardPosition nextHeadPosition{k1, k0};
  Game::BoardPosition startingHeadPosition{};
  Game::BoardPosition startingPositionSegmentOne{k0, k1};
  Game::BoardPosition startingPositionSegmentTwo{k0, k2};
  Game::BoardPosition startingPositionSegmentThree{k1, k2};

  getSnake().addSegment(Game::Segment{startingHeadPosition});
  getSnake().addSegment(Game::Segment{startingPositionSegmentOne});
  getSnake().addSegment(Game::Segment{startingPositionSegmentTwo});
  getSnake().addSegment(Game::Segment{std::move(startingPositionSegmentThree)});
  getSnake().move(nextHeadPosition);

  EXPECT_EQ(getSnake().getSnakeSegments()[0].getPosition(), nextHeadPosition);
  EXPECT_EQ(getSnake().getSnakeSegments()[1].getPosition(),
            startingHeadPosition);
  EXPECT_EQ(getSnake().getSnakeSegments()[2].getPosition(),
            startingPositionSegmentOne);
  EXPECT_EQ(getSnake().getSnakeSegments()[3].getPosition(),
            startingPositionSegmentTwo);
}

TEST_F(SnakeFixture, MoveOnEmptySnakeDoesNothing) {
  Game::BoardPosition nextHeadPosition{k1, k0};
  getSnake().move(nextHeadPosition);

  EXPECT_EQ(getSnake().getSnakeSegments().size(), k0);
}

TEST_F(SnakeFixture, CantMoveToNonAdjacentCell) {
  Game::BoardPosition nextHeadPosition{k3, k0};
  Game::BoardPosition startingHeadPosition{};

  getSnake().addSegment(Game::Segment{startingHeadPosition});

  try {
    getSnake().move(nextHeadPosition);
    FAIL() << "Should have thrown";
  } catch (const std::invalid_argument &exception) {
    EXPECT_STREQ(exception.what(), "Trying to move to non adjacent tile");
  }
}

TEST_F(SnakeFixture, CantDo180) {
  Game::BoardPosition nextHeadPosition{k3, k0};
  Game::BoardPosition startingHeadPosition{};
  Game::BoardPosition startingPositionSegmentOne{k0, k1};
  Game::BoardPosition startingPositionSegmentTwo{k0, k2};

  getSnake().addSegment(Game::Segment{startingHeadPosition});
  getSnake().addSegment(Game::Segment{startingPositionSegmentOne});
  getSnake().addSegment(Game::Segment{startingPositionSegmentTwo});

  try {
    getSnake().move(startingPositionSegmentOne);
    FAIL() << "Should have thrown";
  } catch (const std::invalid_argument &exception) {
    EXPECT_STREQ(exception.what(), "Trying to move back into itself");
  }
}

} // namespace
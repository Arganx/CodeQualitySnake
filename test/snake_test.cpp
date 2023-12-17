#include "../inc/limited_uint8_t.hpp"
#include "board_position.hpp"
#include "exceptions.hpp"
#include "segment.hpp"
#include "snake.hpp"
#include "gtest/gtest.h"
#include <sys/types.h>

namespace {

// Simulates board of size 4
const Game::Limited_uint8_t k0Limited{0U, 3U};
const Game::Limited_uint8_t k1Limited{1U, 3U};
const Game::Limited_uint8_t k2Limited{2U, 3U};
const Game::Limited_uint8_t k3Limited{3U, 3U};

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
  Game::BoardPosition position{k0Limited, k0Limited};
  Game::Segment segment{position};
  Game::Snake snake{segment};
  EXPECT_EQ(snake.getSnakeSegments().size(), k1);
  EXPECT_EQ(snake.getHeadPosition()->get(), position);
}

TEST_F(SnakeFixture, canAddSegments) {
  EXPECT_EQ(getSnake().getSnakeSegments().size(), k0);
  getSnake().addSegment(
      Game::Segment{Game::BoardPosition{k0Limited, k0Limited}});
  EXPECT_EQ(getSnake().getSnakeSegments().size(), k1);
  Game::Segment segment{Game::BoardPosition{k0Limited, k1Limited}};
  getSnake().addSegment(segment);
  getSnake().addSegment(
      Game::Segment{Game::BoardPosition{k1Limited, k1Limited}});
  EXPECT_EQ(getSnake().getSnakeSegments().size(), k3);
}

TEST_F(SnakeFixture, CantAddDuplicateSegments) {
  EXPECT_EQ(getSnake().getSnakeSegments().size(), k0);
  getSnake().addSegment(
      Game::Segment{Game::BoardPosition{k0Limited, k0Limited}});
  EXPECT_EQ(getSnake().getSnakeSegments().size(), k1);
  getSnake().addSegment(
      Game::Segment{Game::BoardPosition{k0Limited, k1Limited}});
  getSnake().addSegment(
      Game::Segment{Game::BoardPosition{k0Limited, k0Limited}});
  EXPECT_EQ(getSnake().getSnakeSegments().size(), k2);
}

TEST_F(SnakeFixture, CantMoveToSelf) {
  getSnake().addSegment(
      Game::Segment{Game::BoardPosition{k0Limited, k0Limited}});
  try {
    getSnake().move(Game::BoardPosition{k0Limited, k0Limited});
    FAIL() << "Should have thrown";
  } catch (
      const Game::SnakeExceptions::NonAdjacentMovementException &exception) {
    EXPECT_STREQ(exception.what(), "Trying to move to non adjacent tile");
  }
}

TEST_F(SnakeFixture, SegmentsMoveCorrectly) {
  Game::BoardPosition nextHeadPosition{k1Limited, k0Limited};
  Game::BoardPosition startingHeadPosition{k0Limited, k0Limited};
  Game::BoardPosition startingPositionSegmentOne{k0Limited, k1Limited};
  Game::BoardPosition startingPositionSegmentTwo{k0Limited, k2Limited};
  Game::BoardPosition startingPositionSegmentThree{k1Limited, k2Limited};

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
  Game::BoardPosition nextHeadPosition{k1Limited, k0Limited};
  getSnake().move(nextHeadPosition);

  EXPECT_EQ(getSnake().getSnakeSegments().size(), k0);
}

TEST_F(SnakeFixture, CantMoveToNonAdjacentCell) {
  Game::BoardPosition nextHeadPosition{k3Limited, k0Limited};
  Game::BoardPosition startingHeadPosition{k1Limited, k0Limited};

  getSnake().addSegment(Game::Segment{startingHeadPosition});

  try {
    getSnake().move(nextHeadPosition);
    FAIL() << "Should have thrown";
  } catch (
      const Game::SnakeExceptions::NonAdjacentMovementException &exception) {
    EXPECT_STREQ(exception.what(), "Trying to move to non adjacent tile");
  }
}

TEST_F(SnakeFixture, CanMoveToTheOtherSideOfTheBoarder) {
  Game::BoardPosition nextHeadPosition{k3Limited, k0Limited};
  Game::BoardPosition startingHeadPosition{k0Limited, k0Limited};

  getSnake().addSegment(Game::Segment{startingHeadPosition});

  getSnake().move(nextHeadPosition);
  EXPECT_EQ(getSnake().getSnakeSegments()[0].getPosition(), nextHeadPosition);
}

TEST_F(SnakeFixture, CantDo180) {
  Game::BoardPosition nextHeadPosition{k3Limited, k0Limited};
  Game::BoardPosition startingHeadPosition{k0Limited, k0Limited};
  Game::BoardPosition startingPositionSegmentOne{k0Limited, k1Limited};
  Game::BoardPosition startingPositionSegmentTwo{k0Limited, k2Limited};

  getSnake().addSegment(Game::Segment{startingHeadPosition});
  getSnake().addSegment(Game::Segment{startingPositionSegmentOne});
  getSnake().addSegment(Game::Segment{startingPositionSegmentTwo});

  try {
    getSnake().move(startingPositionSegmentOne);
    FAIL() << "Should have thrown";
  } catch (const Game::SnakeExceptions::MovingIntoItselfException &exception) {
    EXPECT_STREQ(exception.what(), "Trying to move back into itself");
  }
}

TEST_F(SnakeFixture, CantAddSegmentNonAdjacentToTheLastSegment) {
  Game::BoardPosition startingHeadPosition{k0Limited, k0Limited};
  Game::BoardPosition startingPositionSegmentOne{k0Limited, k1Limited};
  Game::BoardPosition startingPositionSegmentTwo{k0Limited, k3Limited};

  getSnake().addSegment(Game::Segment{startingHeadPosition});
  getSnake().addSegment(Game::Segment{startingPositionSegmentOne});
  try {
    getSnake().addSegment(Game::Segment{startingPositionSegmentTwo});
    FAIL() << "Should have thrown";
  } catch (
      const Game::SnakeExceptions::SegmentNonAdjacentException &exception) {
    EXPECT_STREQ(
        exception.what(),
        "Can't add a segment that is not adjacent to the last segment");
  }
}

} // namespace
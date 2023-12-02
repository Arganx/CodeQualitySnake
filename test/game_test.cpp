#include "../inc/exceptions.hpp"
#include "../inc/game.hpp"
#include "board_mappings.hpp"
#include "direction.hpp"
#include <cstdint>

#include "gtest/gtest.h"
namespace {

const uint8_t k0{0U};
const uint8_t k1{1U};
const uint8_t k2{2U};
const uint8_t k3{3U};
const uint8_t k4{4U};
const uint8_t k5{5U};
const uint8_t k6{6U};
const uint8_t k7{7U};
const uint8_t k8{8U};
const uint8_t k9{9U};
const uint8_t k10{10U};
const uint8_t k20{20U};

void gatherCandy(Game::Game &game) {
  game.initGame(k10, k10);

  // Go to eat snack that is at 5, 4
  game.setDirection(Direction::Direction::Left);
  game.step();
  // New candy appears on 5, 6

  // Check that the segment was added
  EXPECT_EQ(game.getBoardPtr().get()->getBoard()[k5][k5],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(game.getBoardPtr().get()->getBoard()[k5][k4],
            BoardMapping::kSnakeHead);
}

void gatherSecondCandyFromPositionOfFirstCandy(Game::Game &game) {
  using enum Direction::Direction;
  game.setDirection(Up);
  game.step();
  game.setDirection(Right);
  game.step();
  game.step();
  game.setDirection(Down);
  game.step();
  // New candy on 2, 1
  EXPECT_EQ(game.getBoardPtr().get()->getBoard()[k4][k5],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(game.getBoardPtr().get()->getBoard()[k4][k6],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(game.getBoardPtr().get()->getBoard()[k5][k6],
            BoardMapping::kSnakeHead);
}

void gatherThirdCandyFromPositionOfSecondCandy(Game::Game &game) {
  game.setDirection(Direction::Direction::Left);
  game.step();
  game.step();
  game.step();
  game.step();
  game.step();
  game.setDirection(Direction::Direction::Up);
  game.step();
  game.step();
  game.step();
  // New candy on 5, 8

  EXPECT_EQ(game.getBoardPtr().get()->getBoard()[k5][k1],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(game.getBoardPtr().get()->getBoard()[k4][k1],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(game.getBoardPtr().get()->getBoard()[k3][k1],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(game.getBoardPtr().get()->getBoard()[k2][k1],
            BoardMapping::kSnakeHead);
}

void gatherFourthCandyFromPositionOfThirdCandy(Game::Game &game) {
  game.setDirection(Direction::Direction::Right);
  game.step();
  game.step();
  game.step();
  game.step();
  game.step();
  game.step();
  game.step();
  game.setDirection(Direction::Direction::Down);
  game.step();
  game.step();
  game.step();

  EXPECT_EQ(game.getBoardPtr().get()->getBoard()[k2][k7],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(game.getBoardPtr().get()->getBoard()[k2][k8],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(game.getBoardPtr().get()->getBoard()[k3][k8],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(game.getBoardPtr().get()->getBoard()[k4][k8],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(game.getBoardPtr().get()->getBoard()[k5][k8],
            BoardMapping::kSnakeHead);
}

void killSnakeWith3Segments(Game::Game &game) {
  using enum Direction::Direction;
  game.setDirection(Left);
  game.step();
  game.setDirection(Up);
  game.step();
  game.setDirection(Right);
  game.step();
}

class GameFixture : public ::testing::Test {
private:
  Game::Game game{};

protected:
  Game::Game &getGame() { return game; };
};

TEST(GameTests, DefaultStatusDisplayed) {
  Game::Game game{};
  testing::internal::CaptureStdout();
  game.showStatus();
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "Score: 0\n");
}

TEST(GameTests, DefaultGameDoesNotHaveABoard) {
  Game::Game game{};
  const auto &ptr = game.getBoardPtr();
  EXPECT_EQ(ptr, nullptr);
}

TEST(GameTests, CanInit) {
  Game::Game game{};
  game.initGame(k10, k20);
  const auto &ptr = game.getBoardPtr();
  EXPECT_NE(ptr, nullptr);
  EXPECT_EQ(ptr->getHeight(), k20);
  EXPECT_EQ(ptr->getWidth(), k10);
}

TEST_F(GameFixture, StepCalledWithoutInitialisation) {
  EXPECT_THROW(getGame().step(),
               Game::SnakeExceptions::PointerNotInitializedException);
}

TEST_F(GameFixture, DeterministicPositionOfSnack) {
  getGame().initGame(k10, k10);
  EXPECT_EQ(getGame().getBoardPtr()->getBoard()[k5][k4], BoardMapping::kSnack);
}

TEST_F(GameFixture, DefaultHeadPosition) {
  getGame().initGame(k10, k10);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k5],
            BoardMapping::kSnakeHead);
}

TEST_F(GameFixture, SnakeCanMoveRight) {
  getGame().initGame(k10, k10);
  getGame().step();
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k5],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k6],
            BoardMapping::kSnakeHead);
}

TEST_F(GameFixture, SnakeCanMoveDown) {
  getGame().initGame(k10, k10);
  getGame().setDirection(Direction::Direction::Down);
  getGame().step();
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k5],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k6][k5],
            BoardMapping::kSnakeHead);
}

TEST_F(GameFixture, SnakeCanMoveUp) {
  getGame().initGame(k10, k10);
  getGame().setDirection(Direction::Direction::Up);
  getGame().step();
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k5],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k5],
            BoardMapping::kSnakeHead);
}

TEST_F(GameFixture, SnakeCanMoveLeft) {
  getGame().initGame(k10, k10);
  // There is a snack on tile [5,4]. Because of that to test moving left without
  // any snacks the snake must first mover right
  getGame().step();
  getGame().setDirection(Direction::Direction::Left);
  getGame().step();
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k6],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k5],
            BoardMapping::kSnakeHead);
}

TEST_F(GameFixture, MapBorderCrossing) {
  getGame().initGame(k4, k4);
  getGame().setDirection(Direction::Direction::Right);
  getGame().step();
  getGame().step();
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k2][k2],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k2][k0],
            BoardMapping::kSnakeHead);

  getGame().setDirection(Direction::Direction::Left);
  getGame().step();
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k2][k0],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k2][k3],
            BoardMapping::kSnakeHead);

  getGame().setDirection(Direction::Direction::Down);
  getGame().step();
  getGame().step();
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k2][k3],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k0][k3],
            BoardMapping::kSnakeHead);

  getGame().setDirection(Direction::Direction::Up);
  getGame().step();
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k0][k3],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k3][k3],
            BoardMapping::kSnakeHead);
}

TEST_F(GameFixture, CantMoveIntoItselfWhileGoingLeft) {
  gatherCandy(getGame());

  // Now the snake has a segment so it can't turn 180, the change to direction
  // should be ignored
  getGame().setDirection(Direction::Direction::Right);
  getGame().step();

  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k5],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k4],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k3],
            BoardMapping::kSnakeHead);
}

TEST_F(GameFixture, CantMoveIntoItselfWhileGoingRight) {
  gatherCandy(getGame());

  // Change the snake orientation so that the segment is on the left of the head
  getGame().setDirection(Direction::Direction::Up);
  getGame().step();
  getGame().setDirection(Direction::Direction::Right);
  getGame().step();

  // Check that the snake is in correct position
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k4],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k5],
            BoardMapping::kSnakeHead);

  // Now the snake has a segment so it can't turn 180, the change to direction
  // should be ignored
  getGame().setDirection(Direction::Direction::Left);
  getGame().step();

  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k4],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k5],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k6],
            BoardMapping::kSnakeHead);
}

TEST_F(GameFixture, CantMoveIntoItselfWhileGoingUp) {
  gatherCandy(getGame());

  // Change the snake orientation so that the segment is below the head
  getGame().setDirection(Direction::Direction::Up);
  getGame().step();

  // Check that the snake is in correct position
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k4],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k4],
            BoardMapping::kSnakeHead);

  // Now the snake has a segment so it can't turn 180, the change to direction
  // should be ignored
  getGame().setDirection(Direction::Direction::Down);
  getGame().step();

  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k4],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k4],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k3][k4],
            BoardMapping::kSnakeHead);
}

TEST_F(GameFixture, CantMoveIntoItselfWhileGoingDown) {
  gatherCandy(getGame());

  // Change the snake orientation so that the segment is above the head
  getGame().setDirection(Direction::Direction::Down);
  getGame().step();

  // Check that the snake is in correct position
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k4],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k6][k4],
            BoardMapping::kSnakeHead);

  // Now the snake has a segment so it can't turn 180, the change to direction
  // should be ignored
  getGame().setDirection(Direction::Direction::Up);
  getGame().step();

  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k4],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k6][k4],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k7][k4],
            BoardMapping::kSnakeHead);
}

TEST_F(GameFixture, CantMoveIntoItselfWhileGoingLeftAtTheMapBoarder) {
  gatherCandy(getGame());

  // Go to the map boarder and pass it
  getGame().step();
  getGame().step();
  getGame().step();
  getGame().step();
  getGame().step();
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k1],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k0],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k9],
            BoardMapping::kSnakeHead);

  // Now the snake has a segment so it can't turn 180, the change to direction
  // should be ignored
  getGame().setDirection(Direction::Direction::Right);
  getGame().step();

  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k0],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k9],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k8],
            BoardMapping::kSnakeHead);
}

TEST_F(GameFixture, CantMoveIntoItselfWhileGoingRightAtTheMapBoarder) {
  gatherCandy(getGame());

  // Go to the right map boarder and pass it
  getGame().setDirection(Direction::Direction::Up);
  getGame().step();
  getGame().setDirection(Direction::Direction::Right);
  getGame().step();

  getGame().step();
  getGame().step();
  getGame().step();
  getGame().step();
  getGame().step();
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k8],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k9],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k0],
            BoardMapping::kSnakeHead);

  // Now the snake has a segment so it can't turn 180, the change to direction
  // should be ignored
  getGame().setDirection(Direction::Direction::Left);
  getGame().step();

  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k9],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k0],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k1],
            BoardMapping::kSnakeHead);
}

TEST_F(GameFixture, CantMoveIntoItselfWhileGoingUpAtTheMapBoarder) {
  gatherCandy(getGame());

  // Go to the upper map boarder and pass it
  getGame().setDirection(Direction::Direction::Up);
  getGame().step();

  getGame().step();
  getGame().step();
  getGame().step();
  getGame().step();
  getGame().step();
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k1][k4],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k0][k4],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k9][k4],
            BoardMapping::kSnakeHead);

  // Now the snake has a segment so it can't turn 180, the change to direction
  // should be ignored
  getGame().setDirection(Direction::Direction::Down);
  getGame().step();

  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k0][k4],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k9][k4],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k8][k4],
            BoardMapping::kSnakeHead);
}

TEST_F(GameFixture, CantMoveIntoItselfWhileGoingDownAtTheMapBoarder) {
  gatherCandy(getGame());

  // Go to the lower map boarder and pass it
  getGame().setDirection(Direction::Direction::Down);
  getGame().step();

  getGame().step();
  getGame().step();
  getGame().step();
  getGame().step();
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k8][k4],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k9][k4],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k0][k4],
            BoardMapping::kSnakeHead);

  // Now the snake has a segment so it can't turn 180, the change to direction
  // should be ignored
  getGame().setDirection(Direction::Direction::Up);
  getGame().step();

  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k9][k4],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k0][k4],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k1][k4],
            BoardMapping::kSnakeHead);
}

TEST_F(GameFixture, CanTurnLeftWithASegment) {
  gatherCandy(getGame());

  getGame().setDirection(Direction::Direction::Up);
  getGame().step();
  getGame().setDirection(Direction::Direction::Left);
  getGame().step();

  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k4],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k4],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k3],
            BoardMapping::kSnakeHead);
}

TEST_F(GameFixture, IfGameEndedThenSnakeWontMove) {
  gatherCandy(getGame());

  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k6],
            BoardMapping::kSnack);
  gatherSecondCandyFromPositionOfFirstCandy(getGame());
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k2][k1],
            BoardMapping::kSnack);
  gatherThirdCandyFromPositionOfSecondCandy(getGame());
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k8],
            BoardMapping::kSnack);
  gatherFourthCandyFromPositionOfThirdCandy(getGame());

  killSnakeWith3Segments(getGame());

  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k8],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k8],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k7],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k7],
            BoardMapping::kSnakeHead);

  // Check no changes after game over
  getGame().step();
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k8],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k8],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k7],
            BoardMapping::kSnakeBody);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k4][k7],
            BoardMapping::kSnakeHead);
}

} // namespace
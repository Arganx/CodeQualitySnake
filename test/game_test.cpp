#include "../inc/game.hpp"
#include "board_mappings.hpp"
#include "direction.hpp"
#include <cstdint>
#include <limits.h>

#include "gtest/gtest.h"
#include <stdexcept>

namespace {

const uint8_t k0{0U};
const uint8_t k2{2U};
const uint8_t k3{3U};
const uint8_t k4{4U};
const uint8_t k5{5U};
const uint8_t k6{6U};
const uint8_t k10{10U};
const uint8_t k20{20U};

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
  EXPECT_THROW(getGame().step(), std::invalid_argument);
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
  getGame().setDirection(Direction::Direction::Left);
  getGame().step();
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k5],
            BoardMapping::kEmptySpace);
  EXPECT_EQ(getGame().getBoardPtr().get()->getBoard()[k5][k4],
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

} // namespace
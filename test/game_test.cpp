#include "../inc/game.hpp"
#include <cstdint>
#include <limits.h>

#include "gtest/gtest.h"
#include <stdexcept>

namespace {

const uint8_t k10{10U};
const uint8_t k20{20U};

class GameFixture : public ::testing::Test {
private:
  Game::Game game{};

protected:
  Game::Game &getGame() { return game; };
};

TEST(GameTests, CanPrintHello) {
  Game::Game game{};
  testing::internal::CaptureStdout();
  game.printWelcome();
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "Welcome\n");
}

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

} // namespace
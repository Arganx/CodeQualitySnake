#include "../inc/game.hpp"
#include <cstdint>
#include <limits.h>

#include "gtest/gtest.h"
namespace {

const uint8_t k10 = static_cast<uint8_t>(10U);
const uint8_t k100 = static_cast<uint8_t>(100U);

class GameFixture : public ::testing::Test {
private:
  Game::Game game{k10, k10};

protected:
  Game::Game &getGame() { return this->game; };
};

TEST(GameTests, CanPrintHello) {
  Game::Game game{static_cast<uint8_t>(2U), static_cast<uint8_t>(2U)};
  testing::internal::CaptureStdout();
  game.printWelcome();
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "Welcome\n");
}

TEST(GameTests, DefaultStatusDisplayed) {
  Game::Game game{static_cast<uint8_t>(2U), static_cast<uint8_t>(2U)};
  testing::internal::CaptureStdout();
  game.showStatus();
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "Score: 0\n");
}

TEST(GameTests, CanGetWidth) {
  uint8_t kGameWidth = 20U;
  uint8_t kGameHeight = 10U;
  Game::Game game{kGameWidth, kGameHeight};
  EXPECT_EQ(game.getWidth(), kGameWidth);
}

TEST(GameTests, CanGetHeight) {
  uint8_t kGameWidth = 5U;
  uint8_t kGameHeight = 50U;
  Game::Game game{kGameWidth, kGameHeight};
  EXPECT_EQ(game.getHeight(), kGameHeight);
}

TEST_F(GameFixture, TestGameDimensionsFromFixture) {
  EXPECT_EQ(getGame().getWidth(), k10);
  EXPECT_EQ(getGame().getHeight(), k10);
}

TEST_F(GameFixture, CanChangeWidth) {
  getGame().setWidth(k100);
  EXPECT_EQ(getGame().getWidth(), k100);
  EXPECT_EQ(getGame().getHeight(), k10);
}

TEST_F(GameFixture, CanChangeHeight) {
  getGame().setHeight(k100);
  EXPECT_EQ(getGame().getWidth(), k10);
  EXPECT_EQ(getGame().getHeight(), k100);
}

TEST_F(GameFixture, CanChangeWidthAndHeight) {
  getGame().setHeight(k100);
  getGame().setWidth(k100);
  EXPECT_EQ(getGame().getWidth(), k100);
  EXPECT_EQ(getGame().getHeight(), k100);
}

} // namespace
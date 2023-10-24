#include "../inc/game.hpp"
#include <cstdint>
#include <limits.h>

#include "gtest/gtest.h"
#include <stdexcept>
namespace {

const uint8_t k0{0U};
const uint8_t k10{10U};
const uint8_t k100{100U};

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

TEST(GameTests, ThrowsWhenGameCreatedWithZeroWidth) {
  try {
    Game::Game game{k0, k10};
    FAIL() << "Should throw std::invalid_argument";
  } catch (const std::invalid_argument &e) {
    EXPECT_STREQ(e.what(), "Width must be a positive number");
  }
}

TEST(GameTests, ThrowsWhenGameCreatedWithZeroHeight) {
  try {
    Game::Game game{k10, k0};
    FAIL() << "Should throw std::invalid_argument";
  } catch (const std::invalid_argument &e) {
    EXPECT_STREQ(e.what(), "Height must be a positive number");
  }
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

TEST_F(GameFixture, ThrowsWhenWidthSetToZero) {
  EXPECT_THROW(getGame().setWidth(0), std::invalid_argument);
}

TEST_F(GameFixture, ThrowsWhenHeighthSetToZero) {
  EXPECT_THROW(getGame().setHeight(0), std::invalid_argument);
}

} // namespace
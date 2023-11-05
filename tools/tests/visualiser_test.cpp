#include "visualiser.hpp"

#include "board.hpp"
#include "gtest/gtest.h"
#include <cstdint>

namespace {

constexpr uint8_t k1{1U};
constexpr uint8_t k2{2U};
constexpr uint8_t k4{4U};

TEST(VisualiserTests, CanVisualise1x1) {
  Game::Board board{k1, k1};
  testing::internal::CaptureStdout();
  tools::Visualiser::visualiseBoard(board);
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "*\n");
}

TEST(VisualiserTests, CanVisualise1x4) {
  Game::Board board{k2, k4};
  testing::internal::CaptureStdout();
  tools::Visualiser::visualiseBoard(board);
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "**\n**\n**\n**\n");
}

} // namespace
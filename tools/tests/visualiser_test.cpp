#include "../../inc/board_mappings.hpp"
#include "../inc/visualiser.hpp"

#include "../../inc/board.hpp"
#include "gtest/gtest.h"
#include <cstdint>

namespace {

constexpr uint8_t k1{1U};
constexpr uint8_t k2{2U};
constexpr uint8_t k4{4U};
constexpr std::string kEscapeString{"\033["};
constexpr char kA{'A'};

TEST(VisualiserTests, CanVisualise1x1) {
  Game::Board board{k1, k1};
  testing::internal::CaptureStdout();
  tools::Visualiser::visualiseBoard(board);
  std::string output = testing::internal::GetCapturedStdout();
  std::stringstream expectedString{};
  expectedString << BoardMapping::kEmptySpace << "\n"
                 << kEscapeString << static_cast<int>(board.getHeight()) << kA;

  EXPECT_EQ(output, expectedString.str());
}

TEST(VisualiserTests, CanVisualise1x4) {
  Game::Board board{k2, k4};
  testing::internal::CaptureStdout();
  tools::Visualiser::visualiseBoard(board);
  std::string output = testing::internal::GetCapturedStdout();
  std::stringstream expectedString{};
  expectedString << BoardMapping::kEmptySpace << BoardMapping::kEmptySpace
                 << "\n"
                 << BoardMapping::kEmptySpace << BoardMapping::kEmptySpace
                 << "\n"
                 << BoardMapping::kEmptySpace << BoardMapping::kEmptySpace
                 << "\n"
                 << BoardMapping::kEmptySpace << BoardMapping::kEmptySpace
                 << "\n"
                 << kEscapeString << static_cast<int>(board.getHeight()) << kA;
  EXPECT_EQ(output, expectedString.str());
}

} // namespace
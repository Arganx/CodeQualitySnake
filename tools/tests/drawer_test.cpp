#include "../inc/drawer.hpp"
#include "gtest/gtest.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cstdint>
#include <memory>
#include <mutex>
#include <vector>

namespace {

constexpr uint16_t kWindowWidth{30U};
constexpr uint16_t kWindowHeight{30U};
constexpr uint16_t kBlockSize{10U};
constexpr uint16_t kSmallBlockSize{2U};
constexpr uint16_t kSpecificBoardPosition{20U};
constexpr uint16_t kMinPixelValue{0U};
constexpr uint16_t kMaxPixelValue{255U};

TEST(Drawer_Tests, CanCreateDrawer) {
  auto window = std::make_shared<sf::RenderWindow>(
      sf::VideoMode(kWindowWidth, kWindowHeight), "Test window");
  tools::Drawer drawer{window};
}

class DrawerFixture : public ::testing::Test {
private:
  std::shared_ptr<sf::RenderWindow> window{std::make_shared<sf::RenderWindow>(
      sf::VideoMode(kWindowWidth, kWindowHeight), "Test window")};
  tools::Drawer drawer{window};

protected:
  tools::Drawer &getDrawer() { return drawer; };
  std::shared_ptr<sf::RenderWindow> &getWindow() { return window; };
};

struct ExpectedValues {
  uint8_t expectedR;
  uint8_t expectedG;
  uint8_t expectedB;
  uint8_t expectedA;
};

void checkImageRectangle(const std::shared_ptr<sf::RenderWindow> iWindow,
                         const uint8_t iStartWidth, const uint8_t iEndWidth,
                         const uint8_t iStartHeight, const uint8_t iEndHeight,
                         const ExpectedValues &&iExpectedValues) {
  for (uint8_t widthPosition{iStartWidth}; widthPosition < iEndWidth;
       ++widthPosition) {
    for (uint8_t heightPosition{iStartHeight}; heightPosition < iEndHeight;
         ++heightPosition) {
      sf::Texture texture;
      texture.create(iWindow->getSize().x, iWindow->getSize().y);
      texture.update(*iWindow);
      sf::Color color =
          texture.copyToImage().getPixel(widthPosition, heightPosition);
      EXPECT_EQ(color.r, iExpectedValues.expectedR);
      EXPECT_EQ(color.g, iExpectedValues.expectedG);
      EXPECT_EQ(color.b, iExpectedValues.expectedB);
      EXPECT_EQ(color.a, iExpectedValues.expectedA);
    }
  }
}

TEST_F(DrawerFixture, CanDrawTiles) {
  auto tiles = std::vector<std::vector<sf::RectangleShape>>();
  tiles.emplace_back();
  tiles[0].emplace_back(sf::Vector2f(kBlockSize, kBlockSize));
  tiles.emplace_back();
  tiles[1].emplace_back(sf::Vector2f(kSmallBlockSize, kSmallBlockSize));
  tiles[1][0].move(kSpecificBoardPosition, kSpecificBoardPosition);
  tiles[1][0].setFillColor(sf::Color(kMaxPixelValue, kMinPixelValue,
                                     kMinPixelValue, kMaxPixelValue));
  getDrawer().drawTiles(tiles);
  // There should be a rectangle of size 10x10 in the upper let corner filled
  // with white color
  checkImageRectangle(getWindow(), kMinPixelValue, kBlockSize, kMinPixelValue,
                      kBlockSize,
                      ExpectedValues(kMaxPixelValue, kMaxPixelValue,
                                     kMaxPixelValue, kMaxPixelValue));
  // A red rectangle of size 2x2 should be at position 20 20
  checkImageRectangle(getWindow(), kSpecificBoardPosition,
                      kSpecificBoardPosition + kSmallBlockSize,
                      kSpecificBoardPosition,
                      kSpecificBoardPosition + kSmallBlockSize,
                      ExpectedValues(kMaxPixelValue, kMinPixelValue,
                                     kMinPixelValue, kMaxPixelValue));
  // All of the other pixels should be black
  for (uint8_t widthPosition{kBlockSize}; widthPosition < kWindowWidth;
       ++widthPosition) {
    for (uint8_t heightPosition{kBlockSize}; heightPosition < kWindowHeight;
         ++heightPosition) {
      if ((widthPosition >= kSpecificBoardPosition &&
           widthPosition < kSpecificBoardPosition + kSmallBlockSize) ||
          (heightPosition >= kSpecificBoardPosition &&
           heightPosition < kSpecificBoardPosition + kSmallBlockSize)) {
        continue;
      }
      sf::Texture texture;
      texture.create(getWindow()->getSize().x, getWindow()->getSize().y);
      texture.update(*getWindow());
      sf::Color color =
          texture.copyToImage().getPixel(widthPosition, heightPosition);
      EXPECT_EQ(color.r, kMinPixelValue);
      EXPECT_EQ(color.g, kMinPixelValue);
      EXPECT_EQ(color.b, kMinPixelValue);
      EXPECT_EQ(color.a, kMinPixelValue);
    }
  }
}

TEST_F(DrawerFixture, CanDrawBlocks) {
  auto blocks = std::vector<sf::RectangleShape>();
  blocks.emplace_back(sf::Vector2f(kSmallBlockSize, kBlockSize));
  std::mutex mutex;
  getDrawer().drawBlocks(mutex, blocks);
  // There should be a rectangle of size 2x10 in the upper let corner filled
  // with white color
  checkImageRectangle(getWindow(), kMinPixelValue, kSmallBlockSize,
                      kMinPixelValue, kBlockSize,
                      ExpectedValues(kMaxPixelValue, kMaxPixelValue,
                                     kMaxPixelValue, kMaxPixelValue));
  // All of the other pixels should be black
  checkImageRectangle(getWindow(), kSmallBlockSize, kWindowWidth, kBlockSize,
                      kWindowHeight,
                      ExpectedValues(kMinPixelValue, kMinPixelValue,
                                     kMinPixelValue, kMinPixelValue));
}

} // namespace
#include "../inc/options_manager.hpp"
#include "../inc/candies.hpp"
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <string_view>

namespace tools {
constexpr std::string kPlayerNameMarker{"Player name: "};
constexpr std::string kBoardWidthMarker{"Board width: "};
constexpr std::string kBoardHeightMarker{"Board height: "};
constexpr std::string kCandyMarker{"Candy: "};
constexpr std::string kSnakeColorMarker{"Snake color: "};
constexpr char kDefaultPlayerName[]{"Player"};
constexpr uint8_t kDefaultBoardWidth{10U};
constexpr uint8_t kDefaultBoardHeight{10U};

void OptionsManager::processLine(const std::string_view &iLine) {
  if (iLine.starts_with(kPlayerNameMarker)) {
    playerName = iLine.substr(kPlayerNameMarker.length(), iLine.length());
  }
  if (iLine.starts_with(kBoardWidthMarker)) {
    auto width{std::stoi(
        iLine.substr(kBoardWidthMarker.length(), iLine.length()).data())};
    if (width > 255) {
      width = 255U;
    }
    boardWidth = static_cast<uint8_t>(width);
  }
  if (iLine.starts_with(kBoardHeightMarker)) {
    auto height{std::stoi(
        iLine.substr(kBoardHeightMarker.length(), iLine.length()).data())};
    if (height > 255) {
      height = 255U;
    }
    boardHeight = static_cast<uint8_t>(height);
  }

  if (iLine.starts_with(kCandyMarker)) {
    auto candy{
        std::stoi(iLine.substr(kCandyMarker.length(), iLine.length()).data())};
    if (candy > 255) {
      candy = 255U;
    }
    selectedCandy.setCurrentCandy(static_cast<Candies>(candy));
    acceptedCandy = selectedCandy.getCurrentCandy();
  }

  if (iLine.starts_with(kSnakeColorMarker)) {
    auto snakeColor{std::stoi(
        iLine.substr(kSnakeColorMarker.length(), iLine.length()).data())};
    if (snakeColor > 255) {
      snakeColor = 255U;
    }
    selectedSnakeColor.setCurrentColor(static_cast<Colors>(snakeColor));
    acceptedColor = selectedSnakeColor.getCurrentColor();
  }
}

OptionsManager::OptionsManager(const std::string &iOptionsFilePath)
    : optionsFilePath(iOptionsFilePath) {
  if (std::filesystem::exists(iOptionsFilePath)) {
    configFile.open(optionsFilePath, std::ios::in);
    if (configFile.is_open()) {
      std::string line;
      while (std::getline(configFile, line)) {
        processLine(line);
      }
      configFile.close();
    } else { // Failed to open file
      std::cout << "Failed to load options from file\n";
    }
  } else {
    playerName = kDefaultPlayerName;
    boardWidth = kDefaultBoardWidth;
    boardHeight = kDefaultBoardHeight;
    fillTheFile();
  }
}

void OptionsManager::setPlayerName(const std::string_view &iNewName) {
  playerName = iNewName;
}

const std::string &OptionsManager::getPlayerName() const { return playerName; }

void OptionsManager::fillTheFile() {
  configFile.open(optionsFilePath, std::ios::out | std::ios::trunc);
  if (configFile.is_open()) {
    configFile << kPlayerNameMarker << playerName << "\n";
    configFile << kBoardWidthMarker << static_cast<uint16_t>(boardWidth)
               << "\n";
    configFile << kBoardHeightMarker << static_cast<uint16_t>(boardHeight)
               << "\n";
    configFile << kCandyMarker << static_cast<uint16_t>(acceptedCandy) << "\n";
    configFile << kSnakeColorMarker << static_cast<uint16_t>(acceptedColor)
               << "\n";
    configFile.close();
  } else {
    std::cout << "Failed to save board width to the options file\n";
  }
}

void OptionsManager::setBoardWidth(uint8_t iNewWidth) {
  boardWidth = iNewWidth;
}
void OptionsManager::setBoardHeight(uint8_t iNewHeight) {
  boardHeight = iNewHeight;
}
uint8_t OptionsManager::getBoardWidth() const { return boardWidth; }
uint8_t OptionsManager::getBoardHeight() const { return boardHeight; }

std::string OptionsManager::getCandy() const {
  switch (selectedCandy.getCurrentCandy()) {
    using enum tools::Candies;
  case Apple:
    return "apple";
  case Banana:
    return "banana";
  case Blue_berry:
    return "blue_berry";
  case Cherry:
    return "cherry";
  case Lemon:
    return "lemon";
  case Watermelon:
    return "watermelon";
  }
  return "apple";
}

void OptionsManager::acceptNewCandy() {
  acceptedCandy = selectedCandy.getCurrentCandy();
}

void OptionsManager::revertCandy() {
  selectedCandy.setCurrentCandy(acceptedCandy);
}
std::string OptionsManager::getAcceptedCandy() const {
  switch (acceptedCandy) {
    using enum tools::Candies;
  case Apple:
    return "apple";
  case Banana:
    return "banana";
  case Blue_berry:
    return "blue_berry";
  case Cherry:
    return "cherry";
  case Lemon:
    return "lemon";
  case Watermelon:
    return "watermelon";
  }
  return "apple";
}

void OptionsManager::nextCandy() { selectedCandy.getNextCandy(); }
void OptionsManager::previousCandy() { selectedCandy.getPreviousCandy(); }

std::string OptionsManager::getSnakeColor() const {
  switch (selectedSnakeColor.getCurrentColor()) {
    using enum tools::Colors;
  case Blue:
    return "blue";
  case Green:
    return "green";
  case Red:
    return "red";
  case Yellow:
    return "yellow";
  }
  return "blue";
}
void OptionsManager::nextSnakeColor() { selectedSnakeColor.getNextColor(); }
void OptionsManager::previousSnakeColor() {
  selectedSnakeColor.getPreviousColor();
}
void OptionsManager::acceptNewSnakeColor() {
  acceptedColor = selectedSnakeColor.getCurrentColor();
}
void OptionsManager::revertSnakeColor() {
  selectedSnakeColor.setCurrentColor(acceptedColor);
}
Colors OptionsManager::getAcceptedSnakeColor() const { return acceptedColor; }

std::string OptionsManager::getTextureBodyHorizontal() const {
  switch (acceptedColor) {
    using enum tools::Colors;
  case Blue:
    return "body_horizontal";
  case Green:
    return "body_horizontal_green";
  case Red:
    return "body_horizontal_red";
  case Yellow:
    return "body_horizontal_yellow";
  }
  return "body_horizontal";
}
std::string OptionsManager::getTextureBodyTopRight() const {
  switch (acceptedColor) {
    using enum tools::Colors;
  case Blue:
    return "body_topright";
  case Green:
    return "body_topright_green";
  case Red:
    return "body_topright_red";
  case Yellow:
    return "body_topright_yellow";
  }
  return "body_topright";
}
std::string OptionsManager::getTextureBodyBottomRight() const {
  switch (acceptedColor) {
    using enum tools::Colors;
  case Blue:
    return "body_bottomright";
  case Green:
    return "body_bottomright_green";
  case Red:
    return "body_bottomright_red";
  case Yellow:
    return "body_bottomright_yellow";
  }
  return "body_bottomright";
}
std::string OptionsManager::getTextureBodyTopLeft() const {
  switch (acceptedColor) {
    using enum tools::Colors;
  case Blue:
    return "body_topleft";
  case Green:
    return "body_topleft_green";
  case Red:
    return "body_topleft_red";
  case Yellow:
    return "body_topleft_yellow";
  }
  return "body_topleft";
}
std::string OptionsManager::getTextureBodyBottomLeft() const {
  switch (acceptedColor) {
    using enum tools::Colors;
  case Blue:
    return "body_bottomleft";
  case Green:
    return "body_bottomleft_green";
  case Red:
    return "body_bottomleft_red";
  case Yellow:
    return "body_bottomleft_yellow";
  }
  return "body_bottomleft";
}
std::string OptionsManager::getTextureBodyVertical() const {
  switch (acceptedColor) {
    using enum tools::Colors;
  case Blue:
    return "body_vertical";
  case Green:
    return "body_vertical_green";
  case Red:
    return "body_vertical_red";
  case Yellow:
    return "body_vertical_yellow";
  }
  return "body_vertical";
}

std::string OptionsManager::getTextureTailLeft() const {
  switch (acceptedColor) {
    using enum tools::Colors;
  case Blue:
    return "tail_left";
  case Green:
    return "tail_left_green";
  case Red:
    return "tail_left_red";
  case Yellow:
    return "tail_left_yellow";
  }
  return "tail_left";
}
std::string OptionsManager::getTextureTailRight() const {
  switch (acceptedColor) {
    using enum tools::Colors;
  case Blue:
    return "tail_right";
  case Green:
    return "tail_right_green";
  case Red:
    return "tail_right_red";
  case Yellow:
    return "tail_right_yellow";
  }
  return "tail_right";
}
std::string OptionsManager::getTextureTailUp() const {
  switch (acceptedColor) {
    using enum tools::Colors;
  case Blue:
    return "tail_up";
  case Green:
    return "tail_up_green";
  case Red:
    return "tail_up_red";
  case Yellow:
    return "tail_up_yellow";
  }
  return "tail_up";
}
std::string OptionsManager::getTextureTailDown() const {
  switch (acceptedColor) {
    using enum tools::Colors;
  case Blue:
    return "tail_down";
  case Green:
    return "tail_down_green";
  case Red:
    return "tail_down_red";
  case Yellow:
    return "tail_down_yellow";
  }
  return "tail_down";
}

std::string OptionsManager::getTextureHeadLeft() const {
  switch (acceptedColor) {
    using enum tools::Colors;
  case Blue:
    return "head_left";
  case Green:
    return "head_left_green";
  case Red:
    return "head_left_red";
  case Yellow:
    return "head_left_yellow";
  }
  return "head_left";
}
std::string OptionsManager::getTextureHeadRight() const {
  switch (acceptedColor) {
    using enum tools::Colors;
  case Blue:
    return "head_right";
  case Green:
    return "head_right_green";
  case Red:
    return "head_right_red";
  case Yellow:
    return "head_right_yellow";
  }
  return "head_right";
}
std::string OptionsManager::getTextureHeadUp() const {
  switch (acceptedColor) {
    using enum tools::Colors;
  case Blue:
    return "head_up";
  case Green:
    return "head_up_green";
  case Red:
    return "head_up_red";
  case Yellow:
    return "head_up_yellow";
  }
  return "head_up";
}
std::string OptionsManager::getTextureHeadDown() const {
  switch (acceptedColor) {
    using enum tools::Colors;
  case Blue:
    return "head_down";
  case Green:
    return "head_down_green";
  case Red:
    return "head_down_red";
  case Yellow:
    return "head_down_yellow";
  }
  return "head_down";
}

} // namespace tools
#include "../inc/options_manager.hpp"
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
    configFile.open(optionsFilePath, std::ios::out | std::ios::trunc);
    fillTheFile();
  }
}

void OptionsManager::setPlayerName(const std::string_view &iNewName) {
  configFile.open(optionsFilePath, std::ios::out | std::ios::trunc);
  playerName = iNewName;
  fillTheFile();
}

const std::string &OptionsManager::getPlayerName() const { return playerName; }

void OptionsManager::fillTheFile() {
  if (configFile.is_open()) {
    configFile << kPlayerNameMarker << playerName << "\n";
    configFile << kBoardWidthMarker << static_cast<uint16_t>(boardWidth)
               << "\n";
    configFile << kBoardHeightMarker << static_cast<uint16_t>(boardHeight)
               << "\n";
    configFile.close();
  } else {
    std::cout << "Failed to save board width to the options file\n";
  }
}

void OptionsManager::setBoardWidth(uint8_t iNewWidth) {
  configFile.open(optionsFilePath, std::ios::out | std::ios::trunc);
  boardWidth = iNewWidth;
  fillTheFile();
}
void OptionsManager::setBoardHeight(uint8_t iNewHeight) {
  configFile.open(optionsFilePath, std::ios::out | std::ios::trunc);
  boardHeight = iNewHeight;
  fillTheFile();
}
uint8_t OptionsManager::getBoardWidth() const { return boardWidth; }
uint8_t OptionsManager::getBoardHeight() const { return boardHeight; }

} // namespace tools
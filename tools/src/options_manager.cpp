#include "../inc/options_manager.hpp"
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>

namespace tools {
constexpr std::string kPlayerNameMarker{"Player name: "};
constexpr char kDefaultPlayerName[]{"Player"};

OptionsManager::OptionsManager(const std::string &iOptionsFilePath)
    : optionsFilePath(iOptionsFilePath) {
  if (std::filesystem::exists(iOptionsFilePath)) {
    configFile.open(optionsFilePath, std::ios::in);
    if (configFile.is_open()) {
      std::string line;
      while (std::getline(configFile, line)) {
        if (line.starts_with(kPlayerNameMarker)) {
          playerName = line.substr(kPlayerNameMarker.length(), line.length());
        }
      }
      configFile.close();
    } else { // Failed to open file
      std::cout << "Failed to load options from file\n";
    }
  } else {
    configFile.open(optionsFilePath, std::ios::out | std::ios::trunc);
    if (configFile.is_open()) {
      configFile << kPlayerNameMarker << kDefaultPlayerName;
      configFile.close();
    } else {
      std::cout << "Failed create options file\n";
    }
    playerName = kDefaultPlayerName;
  }
}

void OptionsManager::setPlayerName(const std::string &iNewName) {
  configFile.open(optionsFilePath, std::ios::out | std::ios::trunc);
  if (configFile.is_open()) {
    configFile << kPlayerNameMarker << iNewName;
    configFile.close();
  } else {
    std::cout << "Failed to save player name to the options file\n";
  }
  playerName = iNewName;
}

const std::string &OptionsManager::getPlayerName() const { return playerName; }

} // namespace tools
#ifndef OPTIONS_MANAGER_GUARD
#define OPTIONS_MANAGER_GUARD

#include "candies.hpp"
#include <cstdint>
#include <fstream>
#include <string>
#include <sys/types.h>

namespace tools {
class OptionsManager {
private:
  std::string playerName;
  uint8_t boardWidth;
  uint8_t boardHeight;
  std::fstream configFile;
  std::string optionsFilePath;
  void processLine(const std::string_view &iLine);
  CandyCounter selectedCandy{Candies::Apple};
  Candies acceptedCandy{Candies::Apple};

public:
  explicit OptionsManager(const std::string &iOptionsFilePath);
  void setPlayerName(const std::string_view &iNewName);
  void setBoardWidth(uint8_t iNewWidth);
  void setBoardHeight(uint8_t iNewHeight);
  const std::string &getPlayerName() const;
  uint8_t getBoardWidth() const;
  uint8_t getBoardHeight() const;
  std::string getCandy() const;
  void nextCandy();
  void previousCandy();
  void fillTheFile();
  void acceptNewCandy();
  void revertCandy();
  std::string getAcceptedCandy() const;
};
} // namespace tools

#endif
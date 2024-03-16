#ifndef OPTIONS_MANAGER_GUARD
#define OPTIONS_MANAGER_GUARD

#include "candies.hpp"
#include "colors.hpp"
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
  ColorCounter selectedSnakeColor{Colors::Blue};
  Colors acceptedColor{Colors::Blue};

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
  std::string getSnakeColor() const;
  void nextSnakeColor();
  void previousSnakeColor();
  void acceptNewSnakeColor();
  void revertSnakeColor();
  Colors getAcceptedSnakeColor() const;
  std::string getTextureBodyHorizontal() const;
  std::string getTextureBodyTopRight() const;
  std::string getTextureBodyBottomRight() const;
  std::string getTextureBodyTopLeft() const;
  std::string getTextureBodyBottomLeft() const;
  std::string getTextureBodyVertical() const;
  std::string getTextureTailLeft() const;
  std::string getTextureTailRight() const;
  std::string getTextureTailUp() const;
  std::string getTextureTailDown() const;
  std::string getTextureHeadLeft() const;
  std::string getTextureHeadRight() const;
  std::string getTextureHeadUp() const;
  std::string getTextureHeadDown() const;
};
} // namespace tools

#endif
#ifndef OPTIONS_MANAGER_GUARD
#define OPTIONS_MANAGER_GUARD

#include <fstream>
#include <string>

namespace tools {
class OptionsManager {
private:
  std::string playerName;
  std::fstream configFile;
  std::string optionsFilePath;

public:
  explicit OptionsManager(const std::string &iOptionsFilePath);
  void setPlayerName(const std::string &iNewName);
  const std::string &getPlayerName() const;
};
} // namespace tools

#endif
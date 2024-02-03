#ifndef DATABASE_MANAGER_GUARD
#define DATABASE_MANAGER_GUARD
#include "../../SQL-Lite/sqlite3.h"
#include <array>
#include <memory>
#include <string>

namespace tools {
class DatabaseManager {
private:
  std::unique_ptr<sqlite3, decltype(&sqlite3_close)> database_ptr;
  std::array<std::string, 5> bestScores;

public:
  explicit DatabaseManager(const std::string &iDatabasePath);
  std::array<std::pair<std::string, std::string>, 5> getBestScores() const;
  void insertScore(const std::string &iName, const uint32_t &iScore);
  void cleanScores() const;
};
} // namespace tools

#endif
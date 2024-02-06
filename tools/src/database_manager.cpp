#include "../inc/database_manager.hpp"
#include "../../SQL-Lite/sqlite3.h"
#include "../inc/config/variable_config.hpp"
#include "../inc/exceptions.hpp"
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <utility>

namespace {
int callback(void *data, int argc, char **argv, char **azColName) {
  static_cast<void>(azColName);
  static uint8_t current_record{0U};
  auto &scores =
      *static_cast<std::array<std::pair<std::string, std::string>, 5> *>(data);
  if (argc != 2) {
    throw tools::exceptions::IncorrectFormatException(
        "Expected 2 values for each row:");
  }
  scores[current_record] =
      std::make_pair(std::string(argv[0]), std::string(argv[1]));
  ++current_record;
  if (current_record >= config::kNumberOfDisplayedHighScores) {
    current_record = 0U;
  }
  return 0;
}
} // namespace

namespace tools {
DatabaseManager::DatabaseManager(const std::string &iDatabasePath)
    : database_ptr(nullptr, &sqlite3_close) {
  bool databaseExisted = std::ifstream(iDatabasePath).good();
  sqlite3 *dbRawPtr = nullptr;
  int exit = sqlite3_open(iDatabasePath.c_str(), &dbRawPtr);
  if (exit != SQLITE_OK) {
    throw tools::exceptions::DatabaseException(
        ("Cannot open database: " + iDatabasePath).c_str());
  }
  database_ptr.reset(dbRawPtr);
  if (!databaseExisted) {
    std::string create_table_query("CREATE TABLE HighScores("
                                   "Name TEXT NOT NULL,"
                                   "Score INT NOT NULL);");

    exit = sqlite3_exec(database_ptr.get(), create_table_query.c_str(), nullptr,
                        nullptr, nullptr);

    if (exit != SQLITE_OK) {
      throw tools::exceptions::DatabaseException(
          "Failed to created table HighScores");
    } else
      std::cout << "Database created successfully" << std::endl;
  }
}
std::array<std::pair<std::string, std::string>,
           config::kNumberOfDisplayedHighScores>
DatabaseManager::getBestScores() const {
  char *errMsg = nullptr;
  std::array<std::pair<std::string, std::string>,
             config::kNumberOfDisplayedHighScores>
      scores;
  std::string query =
      "SELECT Name, Score FROM HighScores ORDER BY Score DESC LIMIT 5;";

  if (auto exit = static_cast<uint8_t>(sqlite3_exec(
          database_ptr.get(), query.c_str(), callback, &scores, &errMsg));
      exit != SQLITE_OK) {
    sqlite3_free(errMsg);
    std::cout << "Failed to get scores from the database\n";
  }

  return scores;
}

void DatabaseManager::insertScore(const std::string &iName,
                                  const uint32_t &iScore) const {
  char *errMsg = nullptr;
  std::string query = std::format(
      "INSERT INTO HighScores (Name, Score) VALUES ('{}', {});", iName, iScore);
  int exit = sqlite3_exec(database_ptr.get(), query.c_str(), nullptr, nullptr,
                          &errMsg);

  if (exit != SQLITE_OK) {
    sqlite3_free(errMsg);
    std::cout << "Failed to insert score into the database\n";
  }
}

void DatabaseManager::cleanScores() const {
  std::string query = std::format("DELETE FROM HighScores");
  if (sqlite3_exec(database_ptr.get(), query.c_str(), nullptr, nullptr,
                   nullptr) != SQLITE_OK) {
    std::cout << "Failed to clean the database\n";
  }
}

} // namespace tools
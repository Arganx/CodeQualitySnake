#include "../inc/database_manager.hpp"
#include "../../SQL-Lite/sqlite3.h"
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
    throw std::runtime_error(
        "Expected 2 values for each row:"); // TODO create new exception
  }
  scores[current_record] =
      std::make_pair(std::string(argv[0]), std::string(argv[1]));
  ++current_record;
  if (current_record > 4) {
    current_record = 0U;
  }
  return 0;
}
} // namespace

namespace tools {
DatabaseManager::DatabaseManager(const std::string &iDatabasePath)
    : database_ptr(nullptr, sqlite3_close) {
  bool databaseExisted = std::ifstream(iDatabasePath).good();
  sqlite3 *dbRawPtr = nullptr;
  int exit = sqlite3_open(iDatabasePath.c_str(), &dbRawPtr);
  if (exit != SQLITE_OK) {
    throw std::runtime_error("Cannot open database: " +
                             iDatabasePath); // TODO create new exception
  }
  database_ptr.reset(dbRawPtr);
  if (!databaseExisted) {
    std::string create_table_query("CREATE TABLE HighScores("
                                   "Name TEXT NOT NULL,"
                                   "Score INT NOT NULL);");

    exit = sqlite3_exec(database_ptr.get(), create_table_query.c_str(), nullptr,
                        nullptr, nullptr);

    if (exit != SQLITE_OK) {
      throw std::runtime_error(
          "Failed to created table HighScores"); // TODO create new exception
    } else
      std::cout << "Database created successfully" << std::endl;
  }
}
std::array<std::pair<std::string, std::string>, 5>
DatabaseManager::getBestScores() const {
  char *errMsg = nullptr;
  std::array<std::pair<std::string, std::string>, 5> scores;
  std::string query =
      "SELECT Name, Score FROM HighScores ORDER BY Score DESC LIMIT 5;";

  auto exit = static_cast<uint8_t>(sqlite3_exec(
      database_ptr.get(), query.c_str(), callback, &scores, &errMsg));

  if (exit != SQLITE_OK) {
    sqlite3_free(errMsg);
    throw std::runtime_error("Failed to get scores");
  }

  return scores;
}

void DatabaseManager::insertScore(const std::string &iName,
                                  const uint32_t &iScore) {
  char *errMsg = nullptr;
  std::string query = std::format(
      "INSERT INTO HighScores (Name, Score) VALUES ('{}', {});", iName, iScore);
  int exit = sqlite3_exec(database_ptr.get(), query.c_str(), nullptr, nullptr,
                          &errMsg);

  if (exit != SQLITE_OK) {
    sqlite3_free(errMsg);
    throw std::runtime_error(
        "Failed to insert score"); // TODO create new exception should it throw?
  }
}

void DatabaseManager::cleanScores() const {
  std::string query = std::format("DELETE FROM HighScores");
  if (sqlite3_exec(database_ptr.get(), query.c_str(), nullptr, nullptr,
                   nullptr) != SQLITE_OK) {
    throw std::runtime_error(
        "Failed to clean db"); // TODO create new exception should it throw?
  }
}

} // namespace tools
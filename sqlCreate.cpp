#include "SQL-Lite/sqlite3.h"

#include <iostream>

int main() {
  sqlite3 *DB;
  int exit = 0;
  exit = sqlite3_open("highscores.db", &DB);

  if (exit) {
    std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
    return (-1);
  } else
    std::cout << "Opened Database Successfully!" << std::endl;

  std::string sql("CREATE TABLE HighScores("
                  "Name TEXT NOT NULL,"
                  "Score INT NOT NULL);");

  exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, NULL);

  if (exit != SQLITE_OK) {
  } else
    std::cout << "Hight scores created successfully" << std::endl;

  sqlite3_close(DB);

  return (0);
}
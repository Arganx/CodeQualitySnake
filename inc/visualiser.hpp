#ifndef Visualiser_Guard
#define Visualiser_Guard
#include "board.hpp"

namespace tools {

class Visualiser {
public:
  static void visualiseBoard(const Game::Board &board);
};
} // namespace tools

#endif
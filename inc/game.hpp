#ifndef Game_Guard
#define Game_Guard
#include "board.hpp"
#include "direction.hpp"
#include "snake.hpp"
#include <atomic>
#include <cstdint>
#include <memory>
namespace Game {

/**
 * The main class of the Snake game. Contains everything necessary to run the
 * game.
 */
class Game {
private:
  uint16_t score{0U};
  std::unique_ptr<Board> boardPtr;
  std::unique_ptr<Snake> snakePtr;
  std::atomic<Direction::Direction> direction{Direction::Direction::Right};
  bool gameEnded = false;

  void drawFullSnake();
  void drawMovablePartsOfSnake();
  void cleanFullSnake();
  void cleanLastSegmentOfSnake();
  void checkIfSnakeIsInitialized() const;
  void checkIfPointersAreInitialized() const;
  void drawSnack(const uint8_t iNumberOfSnacks = 1);
  uint8_t moveSnake();

public:
  void setDirection(const Direction::Direction iDirection);

  Snake &getSnake();

  /**
   * Prints the current value of the status variable to the standard output.
   *
   * @return void.
   */
  void showStatus() const;

  /**
   *
   * Initializes the components of the Game class.
   *
   * @param iWidth The width that the game board is supposed to have.
   * @param iHeight The height that the game board is supposed to have.
   *
   * @return void.
   */
  void initGame(uint8_t iWidth, uint8_t iHeight);

  bool step();

  /**
   *
   * Getter for the board.
   *
   * @return std::unique_ptr<Board> - Pointer to the board. The pointer can be
   * null.
   */
  std::unique_ptr<Board> &getBoardPtr();

  /**
   * Main game constructor.
   *
   * Creates a game object and sets the score member variable.
   *
   */
  explicit Game();
};
} // namespace Game
#endif
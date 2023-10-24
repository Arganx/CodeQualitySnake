#ifndef Game_Guard
#define Game_Guard
#include <concepts>
#include <cstdint>
#include <stdexcept>
namespace Game {
/**
 * The main class of the Snake game. Contains everything necessary to run the
 * game.
 */
class Game {
private:
  uint16_t score = 0U;
  uint8_t width;
  uint8_t height;

public:
  /**
   * Prints welcome to the standard output.
   *
   * @return void.
   */
  void printWelcome() const;
  /**
   * Prints the current value of the status variable to the standard output.
   *
   * @return void.
   */
  void showStatus() const;
  /**
   * Gets the board width.
   *
   * @return The board width.
   */
  uint8_t getWidth() const;
  /**
   * Gets the board height.
   *
   * @return The board height.
   */
  uint8_t getHeight() const;
  /**
   * Sets the board width.
   *
   * Changes the value of the class member width, to the one provided.
   *
   * @param iWidth The new width that the game board is supposed to have.
   * @return void.
   * @throws std::invalid_argument if iWidth is not a positive number.
   */
  void setWidth(const uint8_t iWidth);
  /**
   * Sets the board height.
   *
   * Changes the value of the class member height, to the one provided.
   *
   * @param iHeight The new height that the game board is supposed to have.
   * @return void.
   * @throws std::invalid_argument if iHeight is not a positive number.
   */
  void setHeight(const uint8_t iHeight);
  /**
   * Main game constructor.
   *
   * Creates a game object and sets the width, height and score member
   * variables.
   *
   * @param iWidth The width that the game board is supposed to have.
   * @param iHeight The height that the game board is supposed to have.
   */
  explicit Game(std::same_as<uint8_t> auto iWidth,
                std::same_as<uint8_t> auto iHeight)
      : width{iWidth}, height{iHeight} {
    if (iWidth <= 0) {
      throw std::invalid_argument("Width must be a positive number");
    } else if (iHeight <= 0) {
      throw std::invalid_argument("Height must be a positive number");
    }
  };
};
} // namespace Game
#endif
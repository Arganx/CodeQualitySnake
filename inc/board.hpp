#ifndef Board_Guard
#define Board_Guard

#include "board_position.hpp"
#include <cstdint>
#include <stdexcept>
#include <vector>
namespace Game {

/**
 * Wrapper around board implemented on 2D vector.
 * Contains all of the functions necessary to handle board related operations.
 */
class Board {
private:
  uint8_t width;
  uint8_t height;
  std::vector<std::vector<uint8_t>> boardSpace;

  /**
   * Fills in the boardSpace with the '*' signs. The dimensions of the vectors
   * are taken from the width and height variables.
   *
   * @return void.
   */
  void createBoard();

public:
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
   * @param iWidth The new width that the board is supposed to have.
   * @return void.
   * @throws std::invalid_argument if iWidth is not a positive number.
   */
  void setWidth(const uint8_t iWidth);

  /**
   * Sets the board height.
   *
   * Changes the value of the class member height, to the one provided.
   *
   * @param iHeight The new height that the board is supposed to have.
   * @return void.
   * @throws std::invalid_argument if iHeight is not a positive number.
   */
  void setHeight(const uint8_t iHeight);

  std::vector<std::vector<uint8_t>> getBoard() const;

  void drawCharacter(const BoardPosition &iPosition, uint8_t iCharacter);

  std::vector<BoardPosition> getAvailablePositions() const;

  /**
   * Main Board constructor.
   *
   * Creates a board object and sets the width and height member
   * variables.
   *
   * @param iWidth The width that the board is supposed to have.
   * @param iHeight The height that the board is supposed to have.
   */
  explicit Board(std::same_as<uint8_t> auto iWidth,
                 std::same_as<uint8_t> auto iHeight)
      : width{iWidth}, height{iHeight} {
    if (iWidth <= 0) {
      throw std::invalid_argument("Width must be a positive number");
    } else if (iHeight <= 0) {
      throw std::invalid_argument("Height must be a positive number");
    }
    createBoard();
  };
};

} // namespace Game
#endif
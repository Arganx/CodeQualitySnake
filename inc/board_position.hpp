#ifndef Board_Position_Guard
#define Board_Position_Guard

#include <cstdint>
#include <variant>

namespace Game {
class BoardPosition {
private:
  uint8_t xPosition = 0U;
  uint8_t yPosition = 0U;

public:
  /**
   * Default constructor, initializes x_position and y_position to 0
   */
  BoardPosition();
  /**
   * Parameterized constructor, allows the user to specify the initial values of
   * x_position and y_position
   * @param iXPosition the position on the x axis
   * @param iYPosition the position on the y axis
   */
  BoardPosition(uint8_t iXPosition, uint8_t iYPosition);
  uint8_t getXPosition() const;
  uint8_t getYPosition() const;
  void setXPosition(const uint8_t iPosition);
  void setYPosition(const uint8_t iPosition);
  bool operator==(const BoardPosition &iPosition) const;
};

} // namespace Game
#endif
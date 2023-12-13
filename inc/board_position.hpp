#ifndef Board_Position_Guard
#define Board_Position_Guard

#include "limited_uint8_t.hpp"

namespace Game {
class BoardPosition {
private:
  Limited_uint8_t xPosition;
  Limited_uint8_t yPosition;

public:
  /**
   * Parameterized constructor, allows the user to specify the initial values of
   * x_position and y_position
   * @param iXPosition the position on the x axis
   * @param iYPosition the position on the y axis
   */
  BoardPosition(Limited_uint8_t iXPosition, Limited_uint8_t iYPosition);
  Limited_uint8_t getXPosition() const;
  Limited_uint8_t getYPosition() const;
  void setXPosition(const Limited_uint8_t iPosition);
  void setYPosition(const Limited_uint8_t iPosition);
  bool operator==(const BoardPosition &iPosition) const;
  bool isAdjacent(const BoardPosition &iPosition) const;
  void incrementX();
  void decrementX();
  void incrementY();
  void decrementY();
};

} // namespace Game
#endif
#include "../inc/board_position.hpp"

namespace Game {

BoardPosition::BoardPosition(Limited_uint8_t iXPosition,
                             Limited_uint8_t iYPosition)
    : xPosition{iXPosition}, yPosition{iYPosition} {};
Limited_uint8_t BoardPosition::getXPosition() const { return xPosition; }
Limited_uint8_t BoardPosition::getYPosition() const { return yPosition; };
void BoardPosition::setXPosition(const Limited_uint8_t iPosition) {
  xPosition = iPosition;
};
void BoardPosition::setYPosition(const Limited_uint8_t iPosition) {
  yPosition = iPosition;
};

bool BoardPosition::operator==(const BoardPosition &iPosition) const {
  if (iPosition.xPosition == xPosition && iPosition.yPosition == yPosition) {
    return true;
  }
  return false;
}

bool BoardPosition::isAdjacent(const BoardPosition &iPosition) const {
  if (iPosition.yPosition == yPosition &&
      (iPosition.xPosition + 1U == xPosition ||
       iPosition.xPosition == xPosition + 1U)) {
    return true;
  }
  if (iPosition.xPosition == xPosition &&
      (iPosition.yPosition + 1U == yPosition ||
       iPosition.yPosition == yPosition + 1U)) {
    return true;
  }
  return false;
}

void BoardPosition::incrementX() { ++xPosition; };
void BoardPosition::decrementX() { --xPosition; };
void BoardPosition::incrementY() { ++yPosition; };
void BoardPosition::decrementY() { --yPosition; };

} // namespace Game
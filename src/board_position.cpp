#include "../inc/board_position.hpp"

namespace Game {

BoardPosition::BoardPosition() = default;

BoardPosition::BoardPosition(uint8_t iXPosition, uint8_t iYPosition)
    : xPosition{iXPosition}, yPosition{iYPosition} {};
uint8_t BoardPosition::getXPosition() const { return xPosition; }
uint8_t BoardPosition::getYPosition() const { return yPosition; };
void BoardPosition::setXPosition(const uint8_t iPosition) {
  xPosition = iPosition;
};
void BoardPosition::setYPosition(const uint8_t iPosition) {
  yPosition = iPosition;
};
} // namespace Game
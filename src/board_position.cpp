#include "../inc/board_position.hpp"
#include <cmath>
#include <cstdint>
#include <utility>

namespace {
constexpr uint8_t k0{0U};
constexpr uint8_t k1{1U};
} // namespace

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

bool BoardPosition::operator==(const BoardPosition &iPosition) const {
  if (iPosition.xPosition == xPosition && iPosition.yPosition == yPosition) {
    return true;
  }
  return false;
}

bool BoardPosition::isAdjacent(const BoardPosition &iPosition) const {
  auto xDiff = std::abs(iPosition.xPosition - xPosition);
  auto yDiff = std::abs(iPosition.yPosition - yPosition);
  if ((xDiff == k1 && yDiff == k0) || (xDiff == k0 && yDiff == k1)) {
    return true;
  }
  return false;
}

void BoardPosition::incrementX() { ++xPosition; };
void BoardPosition::decrementX() { --xPosition; };
void BoardPosition::incrementY() { ++yPosition; };
void BoardPosition::decrementY() { --yPosition; };

} // namespace Game
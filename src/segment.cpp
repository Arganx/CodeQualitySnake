#include "../inc/segment.hpp"

namespace Game {
Segment::Segment(const BoardPosition &iPosition)
    : segmentPosition{iPosition} {};

Segment::Segment(BoardPosition &&iPosition)
    : segmentPosition{std::move(iPosition)} {};
const BoardPosition &Segment::getPosition() const { return segmentPosition; }

void Segment::goToPosition(const BoardPosition &iPosition) {
  segmentPosition = iPosition;
}

bool Segment::operator==(const Segment &iSegment) const {
  if (segmentPosition == iSegment.getPosition()) {
    return true;
  }
  return false;
}
} // namespace Game
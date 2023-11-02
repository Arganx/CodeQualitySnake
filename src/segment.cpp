#include "../inc/segment.hpp"

namespace Game {
Segment::Segment(const BoardPosition &iPosition)
    : segmentPosition{iPosition} {};
const BoardPosition &Segment::getPosition() const { return segmentPosition; }
} // namespace Game
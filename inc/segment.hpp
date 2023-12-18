#ifndef Segment_Guard
#define Segment_Guard

#include "board_position.hpp"

namespace Game {

class Segment {
private:
  BoardPosition segmentPosition;

public:
  explicit Segment(const BoardPosition &iPosition);
  explicit Segment(BoardPosition &&iPosition);
  const BoardPosition &getPosition() const;
  bool operator==(const Segment &iSegment) const;
};
} // namespace Game
#endif
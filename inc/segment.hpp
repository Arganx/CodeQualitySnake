#ifndef Segment_Guard
#define Segment_Guard

#include "board_position.hpp"
#include <memory>

namespace Game {

class Segment {
private:
  BoardPosition segmentPosition;

public:
  // void goToNextPosition();
  explicit Segment(const BoardPosition &iPosition);
  const BoardPosition &getPosition() const;
};
} // namespace Game
#endif
#ifndef Snake_Guard
#define Snake_Guard

#include "board_position.hpp"
#include "segment.hpp"
#include <vector>
namespace Game {
class Snake {
private:
  std::vector<Segment> segments;

public:
  void addSegment(const Segment &iSegment);
  void clear();
  const BoardPosition &getHeadPosition() const;
  Snake();
  explicit Snake(const Segment &iSegment);
  const std::vector<Segment> &getSnakeSegments() const;
};
} // namespace Game

#endif
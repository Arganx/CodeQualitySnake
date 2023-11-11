#ifndef Snake_Guard
#define Snake_Guard

#include "board_position.hpp"
#include "segment.hpp"
#include <deque>
#include <functional>
#include <optional>
namespace Game {
class Snake {
private:
  std::deque<Segment> segments;
  bool isDoing180(const BoardPosition &iNextHeadPosition) const;
  void checkIfAdjacentToLastSegment(const Segment &iSegment) const;

public:
  void addSegment(const Segment &iSegment);
  void addSegment(Segment &&iSegment);
  std::optional<std::reference_wrapper<const BoardPosition>>
  getHeadPosition() const;
  Snake();
  explicit Snake(const Segment &iSegment);
  const std::deque<Segment> &getSnakeSegments() const;
  void move(const BoardPosition &iNextHeadPosition,
            bool iIsPassingBoardBoarder = false);
};
} // namespace Game

#endif
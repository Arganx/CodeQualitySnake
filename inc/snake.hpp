#ifndef Snake_Guard
#define Snake_Guard

#include "board_position.hpp"
#include "segment.hpp"
#include <functional>
#include <optional>
#include <vector>
namespace Game {
class Snake {
private:
  std::vector<Segment> segments;
  bool isDoing180(const BoardPosition &iNextHeadPosition) const;

public:
  void addSegment(const Segment &iSegment);
  void addSegment(Segment &&iSegment);
  std::optional<std::reference_wrapper<const BoardPosition>>
  getHeadPosition() const;
  Snake();
  explicit Snake(const Segment &iSegment);
  const std::vector<Segment> &getSnakeSegments() const;
  void move(const BoardPosition &iNextHeadPosition);
};
} // namespace Game

#endif
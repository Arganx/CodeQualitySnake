#include "../inc/snake.hpp"
#include "../inc/segment.hpp"
#include <optional>

namespace Game {
Snake::Snake() = default;

Snake::Snake(const Segment &iSegment) : segments{iSegment} {};

void Snake::addSegment(const Segment &iSegment) {
  segments.emplace_back(iSegment);
}

std::optional<std::reference_wrapper<const BoardPosition>>
Snake::getHeadPosition() const {
  if (!segments.empty()) {
    return segments[0].getPosition();
  }
  return std::nullopt;
}

const std::vector<Segment> &Snake::getSnakeSegments() const { return segments; }

} // namespace Game
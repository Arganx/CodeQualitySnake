#include "../inc/snake.hpp"
#include "../inc/segment.hpp"

namespace Game {
Snake::Snake() = default;

Snake::Snake(const Segment &iSegment) : segments{iSegment} {};

void Snake::addSegment(const Segment &iSegment) {
  segments.emplace_back(iSegment);
}

void Snake::clear() { segments.clear(); }

const BoardPosition &Snake::getHeadPosition() const {
  return segments[0].getPosition();
}

const std::vector<Segment> &Snake::getSnakeSegments() const { return segments; }

} // namespace Game
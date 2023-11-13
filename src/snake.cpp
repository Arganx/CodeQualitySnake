#include "../inc/snake.hpp"
#include "../inc/segment.hpp"
#include <algorithm>
#include <bits/ranges_util.h>
#include <iterator>
#include <optional>
#include <stdexcept>

namespace Game {
Snake::Snake() = default;

Snake::Snake(const Segment &iSegment) : segments{iSegment} {};

void Snake::addSegment(const Segment &iSegment) {
  checkIfAdjacentToLastSegment(iSegment);
  if (std::ranges::find(segments, iSegment) == segments.end()) {
    segments.emplace_back(iSegment);
  }
}

void Snake::addSegment(Segment &&iSegment) {
  checkIfAdjacentToLastSegment(iSegment);
  if (std::ranges::find(segments, iSegment) == segments.end()) {
    segments.emplace_back(std::move(iSegment));
  }
}

std::optional<std::reference_wrapper<const BoardPosition>>
Snake::getHeadPosition() const {
  if (!segments.empty()) {
    return segments[0].getPosition();
  }
  return std::nullopt;
}

const std::deque<Segment> &Snake::getSnakeSegments() const { return segments; }

void Snake::move(const BoardPosition &iNextHeadPosition,
                 bool iIsPassingBoardBoarder, bool isSnackEaten) {
  if (segments.empty()) {
    return;
  }
  if (!iIsPassingBoardBoarder &&
      !iNextHeadPosition.isAdjacent(segments[0].getPosition())) {
    throw std::invalid_argument(
        "Trying to move to non adjacent tile"); // TODO
                                                // change
                                                // exception
                                                // type
  }
  if (segments.size() > 1 && isDoing180(iNextHeadPosition)) {
    throw std::invalid_argument("Trying to move back into itself");
  }
  if (!isSnackEaten) {
    segments.pop_back();
  }
  segments.emplace_front(iNextHeadPosition);
}

bool Snake::isDoing180(const BoardPosition &iNextHeadPosition) const {
  if (segments.size() > 1 && iNextHeadPosition == segments[1].getPosition()) {
    return true;
  }
  return false;
}

void Snake::checkIfAdjacentToLastSegment(const Segment &iSegment) const {
  if (!segments.empty() &&
      !iSegment.getPosition().isAdjacent(segments.back().getPosition())) {
    throw std::invalid_argument(
        "Can't add a segment that is not adjacent to the last segment");
  }
}

} // namespace Game
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
  if (std::ranges::find(segments, iSegment) == segments.end()) {
    segments.emplace_back(iSegment);
  }
}

void Snake::addSegment(Segment &&iSegment) {
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

const std::vector<Segment> &Snake::getSnakeSegments() const { return segments; }

void Snake::move(const BoardPosition &iNextHeadPosition) {
  if (segments.empty()) {
    return;
  }
  if (!iNextHeadPosition.isAdjacent(segments[0].getPosition())) {
    throw std::invalid_argument(
        "Trying to move to non adjacent tile"); // TODO
                                                // change
                                                // exception
                                                // type
  }
  if (segments.size() > 1) {
    if (isDoing180(iNextHeadPosition)) {
      throw std::invalid_argument("Trying to move back into itself");
    }
    for (auto iterator{--segments.end()}; iterator > segments.begin();
         --iterator) {
      iterator->goToPosition(std::prev(iterator)->getPosition());
    }
  }
  segments[0].goToPosition(iNextHeadPosition);
}

bool Snake::isDoing180(const BoardPosition &iNextHeadPosition) const {
  if (segments.size() > 1) {
    if (iNextHeadPosition == segments[1].getPosition()) {
      return true;
    }
  }
  return false;
}

} // namespace Game
#include "../inc/colors.hpp"
#include <cstdint>

namespace tools {

ColorCounter::ColorCounter(const Colors &iStartingValue)
    : currentColor{iStartingValue} {}
Colors ColorCounter::getNextColor() {
  currentColor =
      static_cast<Colors>((static_cast<uint8_t>(currentColor) + 1U) %
                          (static_cast<uint8_t>(Colors::Yellow) + 1U));
  return currentColor;
}

Colors ColorCounter::getPreviousColor() {
  currentColor =
      static_cast<Colors>((static_cast<uint8_t>(currentColor) +
                           static_cast<uint8_t>(Colors::Yellow)) %
                          (static_cast<uint8_t>(Colors::Yellow) + 1U));
  return currentColor;
}

Colors ColorCounter::getCurrentColor() const { return currentColor; }

void ColorCounter::setCurrentColor(const Colors &iNewColor) {
  currentColor = iNewColor;
}

} // namespace tools
#include "../inc/candies.hpp"
#include <cstdint>

namespace tools {

CandyCounter::CandyCounter(const Candies &iStartingValue)
    : currentCandy{iStartingValue} {}
Candies CandyCounter::getNextCandy() {
  currentCandy =
      static_cast<Candies>((static_cast<uint8_t>(currentCandy) + 1U) %
                           (static_cast<uint8_t>(Candies::Watermelon) + 1U));
  return currentCandy;
}

Candies CandyCounter::getPreviousCandy() {
  currentCandy =
      static_cast<Candies>((static_cast<uint8_t>(currentCandy) +
                            static_cast<uint8_t>(Candies::Watermelon)) %
                           (static_cast<uint8_t>(Candies::Watermelon) + 1U));
  return currentCandy;
}

Candies CandyCounter::getCurrentCandy() const { return currentCandy; }

void CandyCounter::setCurrentCandy(const Candies &iNewCandy) {
  currentCandy = iNewCandy;
}

} // namespace tools
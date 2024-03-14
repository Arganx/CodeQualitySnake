#include "../inc/candies.hpp"
#include <cstdint>
#include <iostream>

namespace tools {

CandyCounter::CandyCounter(const Candies &iStartingValue)
    : currentCandy{iStartingValue} {}
Candies CandyCounter::getNextCandy() {
  std::cout << "Start: " << int(currentCandy) << std::endl;
  currentCandy =
      static_cast<Candies>((static_cast<uint8_t>(currentCandy) + 1U) %
                           (static_cast<uint8_t>(Candies::Watermelon) + 1U));
  std::cout << "Next" << std::endl;
  std::cout << "End: " << int(currentCandy) << std::endl;
  return currentCandy;
}

Candies CandyCounter::getPreviousCandy() {
  std::cout << "Start: " << int(currentCandy) << std::endl;
  currentCandy =
      static_cast<Candies>((static_cast<uint8_t>(currentCandy) +
                            static_cast<uint8_t>(Candies::Watermelon)) %
                           (static_cast<uint8_t>(Candies::Watermelon) + 1U));
  std::cout << "Previous" << std::endl;
  std::cout << "End: " << int(currentCandy) << std::endl;
  return currentCandy;
}

Candies CandyCounter::getCurrentCandy() const { return currentCandy; }

void CandyCounter::setCurrentCandy(const Candies &iNewCandy) {
  currentCandy = iNewCandy;
}

} // namespace tools
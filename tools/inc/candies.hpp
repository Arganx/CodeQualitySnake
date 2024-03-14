#ifndef CANDIES_GUARD
#define CANDIES_GUARD

namespace tools {

enum class Candies {
  Apple,
  Banana,
  Blue_berry,
  Cherry,
  Lemon,
  Watermelon,
};

class CandyCounter {
public:
  explicit CandyCounter(const Candies &iStartingValue);
  Candies getNextCandy();

  Candies getPreviousCandy();

  Candies getCurrentCandy() const;

  void setCurrentCandy(const Candies &iNewCandy);

private:
  Candies currentCandy;
};
} // namespace tools
#endif
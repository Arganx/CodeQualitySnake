#ifndef COLORS_GUARD
#define COLORS_GUARD

namespace tools {

enum class Colors { Blue, Green, Red, Yellow };

class ColorCounter {
public:
  explicit ColorCounter(const Colors &iStartingValue);
  Colors getNextColor();

  Colors getPreviousColor();

  Colors getCurrentColor() const;

  void setCurrentColor(const Colors &iNewColor);

private:
  Colors currentColor;
};
} // namespace tools
#endif
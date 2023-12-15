#ifndef Limited_uint8_t_Guard
#define Limited_uint8_t_Guard
#include <compare>
#include <cstdint>
namespace Game {

class Limited_uint8_t {
private:
  uint8_t value;
  uint8_t maxValue;

public:
  explicit Limited_uint8_t(uint8_t iValue, uint8_t iMaxValue = 255U);
  bool operator==(const Limited_uint8_t &iOther) const;
  bool operator==(const uint8_t iUint8) const;
  std::strong_ordering operator<=>(const uint8_t iUint8) const;
  std::strong_ordering operator<=>(const Limited_uint8_t &iOther) const;
  Limited_uint8_t &operator++();
  Limited_uint8_t operator++(int);
  Limited_uint8_t operator+(const uint8_t iUint) const;
  Limited_uint8_t operator-(const uint8_t iUint) const;
  Limited_uint8_t operator+(const Limited_uint8_t &iOther)
      const; // Keeps the limit of the first element
  Limited_uint8_t operator-(const Limited_uint8_t &iOther) const;
  Limited_uint8_t &operator--();
  Limited_uint8_t operator--(int);
  uint8_t getValue() const;
};
} // namespace Game
#endif
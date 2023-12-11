#ifndef Limited_uint8_t_Guard
#define Limited_uint8_t_Guard
#include <cstdint>
namespace Game {

class Limited_uint8_t {
private:
  uint8_t value;
  uint16_t maxPlusOne;

public:
  explicit Limited_uint8_t(uint8_t iValue, uint8_t iMaxValue = 255U);
  bool operator==(const Limited_uint8_t &iOther) const;
  bool operator==(const uint8_t iUint8) const;
  Limited_uint8_t &operator++();
  Limited_uint8_t operator++(int);
  Limited_uint8_t &operator--();
  Limited_uint8_t operator--(int);
  uint8_t getValue() const;
};
} // namespace Game
#endif
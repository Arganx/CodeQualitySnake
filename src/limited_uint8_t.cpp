#include "../inc/limited_uint8_t.hpp"
#include <cstdint>

namespace Game {
Limited_uint8_t::Limited_uint8_t(uint8_t iValue, uint8_t iMaxValue)
    : value{iValue}, maxPlusOne{static_cast<uint16_t>(iMaxValue + 1U)} {
  value = static_cast<uint8_t>(value % maxPlusOne);
}

bool Limited_uint8_t::operator==(const Limited_uint8_t &iOther) const {
  return iOther.getValue() == value;
}

bool Limited_uint8_t::operator==(const uint8_t iUint8) const {
  return iUint8 == value;
}

Limited_uint8_t &Limited_uint8_t::operator++() {
  value = static_cast<uint8_t>((value + 1U) % maxPlusOne);
  return *this;
}

Limited_uint8_t Limited_uint8_t::operator++(int) {
  Limited_uint8_t oldValue = *this;
  operator++();
  return oldValue;
}

Limited_uint8_t &Limited_uint8_t::operator--() {
  --value;
  if (value > maxPlusOne - 1U) {
    value = static_cast<uint8_t>(maxPlusOne - 1U);
  }
  return *this;
}

Limited_uint8_t Limited_uint8_t::operator--(int) {
  Limited_uint8_t oldValue = *this;
  operator--();
  return oldValue;
}

uint8_t Limited_uint8_t::getValue() const { return value; }

} // namespace Game
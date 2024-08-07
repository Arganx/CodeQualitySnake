#include "../inc/limited_uint8_t.hpp"
#include <cstdint>

namespace Game {

Limited_uint8_t::Limited_uint8_t(uint8_t iValue, uint8_t iMaxValue)
    : value{iValue}, maxValue{iMaxValue} {
  value = static_cast<uint8_t>(value % (maxValue + 1U));
}

bool Limited_uint8_t::operator==(const Limited_uint8_t &iOther) const {
  return iOther.getValue() == value;
}

bool Limited_uint8_t::operator==(const uint8_t iUint8) const {
  return iUint8 == value;
}

std::strong_ordering Limited_uint8_t::operator<=>(const uint8_t iUint8) const {
  return value <=> iUint8;
}

std::strong_ordering
Limited_uint8_t::operator<=>(const Limited_uint8_t &iOther) const {
  return value <=> iOther.getValue();
}

Limited_uint8_t &Limited_uint8_t::operator++() {
  value = static_cast<uint8_t>((value + 1U) % (maxValue + 1U));
  return *this;
}

Limited_uint8_t Limited_uint8_t::operator++(int) {
  Limited_uint8_t oldValue = *this;
  operator++();
  return oldValue;
}

Limited_uint8_t &Limited_uint8_t::operator--() {
  --value;
  if (value > maxValue) {
    value = maxValue;
  }
  return *this;
}

Limited_uint8_t Limited_uint8_t::operator--(int) {
  Limited_uint8_t oldValue = *this;
  operator--();
  return oldValue;
}

Limited_uint8_t Limited_uint8_t::operator+(const uint8_t iUint) const {
  uint16_t resultValue{static_cast<uint16_t>(iUint + value)};
  while (resultValue > maxValue) {
    resultValue -= static_cast<uint16_t>(maxValue + 1U);
  }
  Limited_uint8_t result(static_cast<uint8_t>(resultValue), maxValue);
  return result;
}

Limited_uint8_t Limited_uint8_t::operator-(const uint8_t iUint) const {
  auto resultValue{static_cast<int32_t>(value - iUint)};
  while (resultValue < 0) {
    resultValue += static_cast<uint32_t>(maxValue + 1U);
  }
  Limited_uint8_t result(static_cast<uint8_t>(resultValue), maxValue);
  return result;
}

Limited_uint8_t
Limited_uint8_t::operator-(const Limited_uint8_t &iOther) const {
  auto resultValue{static_cast<int32_t>(value - iOther.getValue())};
  while (resultValue < 0) {
    resultValue += static_cast<uint32_t>(maxValue + 1U);
  }
  Limited_uint8_t result(static_cast<uint8_t>(resultValue), maxValue);
  return result;
}

Limited_uint8_t
Limited_uint8_t::operator+(const Limited_uint8_t &iOther) const {
  uint16_t resultValue{static_cast<uint16_t>(iOther.getValue() + value)};
  while (resultValue > maxValue) {
    resultValue -= static_cast<uint16_t>(maxValue + 1U);
  }
  Limited_uint8_t result(static_cast<uint8_t>(resultValue), maxValue);
  return result;
}

uint8_t Limited_uint8_t::getValue() const { return value; }

} // namespace Game
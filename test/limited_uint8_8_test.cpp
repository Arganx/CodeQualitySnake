#include "../inc/limited_uint8_t.hpp"
#include "gtest/gtest.h"

namespace {
const uint8_t k0{0U};
const uint8_t k1{1U};
const uint8_t k2{2U};
const uint8_t k3{3U};
const uint8_t k49{49U};
const uint8_t k144{144U};
const uint8_t k145{145U};
const uint8_t k196{196U};
const uint8_t k200{200U};
const uint8_t k201{201U};
const uint8_t k204{204U};
const uint8_t k250{250U};
const uint8_t k254{254U};
const uint8_t k255{255U};
const uint16_t k256{256U};

class LimitedFixture : public ::testing::Test {
private:
  Game::Limited_uint8_t limitedOne{k200, k255};
  Game::Limited_uint8_t limitedTwo{k201, k255};

protected:
  Game::Limited_uint8_t &getLimitedOne() { return limitedOne; };
  Game::Limited_uint8_t &getLimitedTwo() { return limitedTwo; };
};
} // namespace

TEST(LimitedUint8tTest, CanCreateLimited) {
  Game::Limited_uint8_t limited(k2);
  EXPECT_EQ(limited.getValue(), k2);
}

TEST(LimitedUint8tTest, CanCreateLimited255) {
  Game::Limited_uint8_t limited(k255);
  EXPECT_EQ(limited.getValue(), k255);
}

TEST(LimitedUint8tTest, Limited257Is0) {
  Game::Limited_uint8_t limited(static_cast<uint8_t>(k256));
  EXPECT_EQ(limited.getValue(), k0);
}

TEST(LimitedUint8tTest, LimitTo2UnderLimit) {
  Game::Limited_uint8_t limited(k2, k3);
  EXPECT_EQ(limited.getValue(), k2);
}

TEST(LimitedUint8tTest, LimitTo2OverLimit) {
  Game::Limited_uint8_t limited(k3, k2);
  EXPECT_EQ(limited.getValue(), k0);
}

TEST(LimitedUint8tTest, LimitTo200OverLimit) {
  Game::Limited_uint8_t limited(k250, k200);
  EXPECT_EQ(limited, k49);
}

TEST(LimitedUint8tTest, CanPreIncrement) {
  Game::Limited_uint8_t limited(k1);
  ++limited;
  EXPECT_EQ(limited.getValue(), k2);
}

TEST(LimitedUint8tTest, PreIncrementOverLimit) {
  Game::Limited_uint8_t limited(k2, k2);
  ++limited;
  EXPECT_EQ(limited.getValue(), k0);
}

TEST(LimitedUint8tTest, PreIncrementOnTypeLimit) {
  Game::Limited_uint8_t limited(k255);
  ++limited;
  EXPECT_EQ(limited, k0);
}

TEST(LimitedUint8tTest, PostIncrement) {
  Game::Limited_uint8_t limited(k1);
  EXPECT_EQ(k1, limited++);
  EXPECT_EQ(k2, limited);
}

TEST(LimitedUint8tTest, PreDecrement) {
  Game::Limited_uint8_t limited(k1);
  EXPECT_EQ(k0, --limited);
}

TEST(LimitedUint8tTest, PostDecrement) {
  Game::Limited_uint8_t limited(k1);
  EXPECT_EQ(k1, limited--);
  EXPECT_EQ(k0, limited);
}

TEST(LimitedUint8tTest, PreDecrementOverLimit) {
  Game::Limited_uint8_t limited(k0, k2);
  EXPECT_EQ(k2, --limited);
}

TEST(LimitedUint8tTest, LimitComparison) {
  Game::Limited_uint8_t limitedOne(k0, k2);
  Game::Limited_uint8_t limitedTwo(k0, k2);
  Game::Limited_uint8_t limitedThree(k0, k49);
  Game::Limited_uint8_t limitedFour(k2, k2);
  EXPECT_EQ(limitedOne, limitedTwo);
  EXPECT_EQ(limitedOne, limitedThree);
  EXPECT_NE(limitedOne, limitedFour);
  EXPECT_EQ(limitedTwo, limitedThree);
  EXPECT_NE(limitedTwo, limitedFour);
  EXPECT_NE(limitedThree, limitedFour);
}

TEST(LimitedUint8tTest, AddUnderLimit) {
  Game::Limited_uint8_t limitedOne(k0, k3);
  auto result = limitedOne + k2;
  EXPECT_EQ(result, k2);
}

TEST(LimitedUint8tTest, AddOverLimit) {
  Game::Limited_uint8_t limitedOne(k0, k2);
  auto result = limitedOne + k3;
  EXPECT_EQ(result, k0);
}

TEST(LimitedUint8tTest, AddOverLimitTwice) {
  Game::Limited_uint8_t limitedOne(k200, k250);
  auto result = limitedOne + k255;
  EXPECT_EQ(result, k204);
}

TEST(LimitedUint8tTest, AddOverLimit254) {
  Game::Limited_uint8_t limitedOne(k200, k254);
  auto result = limitedOne + k200;
  EXPECT_EQ(result, k145);
}

TEST(LimitedUint8tTest, AddOverLimit255) {
  Game::Limited_uint8_t limitedOne(k200, k255);
  auto result = limitedOne + k200;
  EXPECT_EQ(result, k144);
}

TEST(LimitedUint8tTest, AddUnderLimitClass) {
  Game::Limited_uint8_t limitedOne(k0, k3);
  Game::Limited_uint8_t limitedTwo(k2, k255);
  EXPECT_EQ(limitedOne + limitedTwo, k2);
}

TEST(LimitedUint8tTest, AddOverLimitClass) {
  Game::Limited_uint8_t limitedOne(k0, k2);
  Game::Limited_uint8_t limitedTwo(k3, k255);
  EXPECT_EQ(limitedOne + limitedTwo, k0);
}

TEST(LimitedUint8tTest, AddOverLimitTwiceClass) {
  Game::Limited_uint8_t limitedOne(k200, k250);
  Game::Limited_uint8_t limitedTwo(k255, k255);
  EXPECT_EQ(limitedOne + limitedTwo, k204);
}

TEST(LimitedUint8tTest, AddOverLimit254Class) {
  Game::Limited_uint8_t limitedOne(k200, k254);
  Game::Limited_uint8_t limitedTwo(k200, k255);
  EXPECT_EQ(limitedOne + limitedTwo, k145);
}

TEST(LimitedUint8tTest, AddOverLimit255Class) {
  Game::Limited_uint8_t limitedOne(k200, k255);
  Game::Limited_uint8_t limitedTwo(k200, k255);
  EXPECT_EQ(limitedOne + limitedTwo, k144);
}

TEST(LimitedUint8tTest, SubtractUnderLimit) {
  Game::Limited_uint8_t limitedOne(k2, k3);
  auto result = limitedOne - k2;
  EXPECT_EQ(result, k0);
}

TEST(LimitedUint8tTest, SubtractOverLimit) {
  Game::Limited_uint8_t limitedOne(k0, k2);
  auto result = limitedOne - k3;
  EXPECT_EQ(result, k0);
}

TEST(LimitedUint8tTest, SubtractOverLimitTwice) {
  Game::Limited_uint8_t limitedOne(k200, k250);
  auto result = limitedOne - k255;
  EXPECT_EQ(result, k196);
}

TEST(LimitedUint8tTest, SubtractOverLimit254) {
  Game::Limited_uint8_t limitedOne(k200, k254);
  auto result = limitedOne - k201;
  EXPECT_EQ(result, k254);
}

TEST(LimitedUint8tTest, SubtractOverLimit255) {
  Game::Limited_uint8_t limitedOne(k200, k255);
  auto result = limitedOne - k201;
  EXPECT_EQ(result, k255);
}

TEST(LimitedUint8tTest, SubtractUnderLimitClass) {
  Game::Limited_uint8_t limitedOne(k2, k3);
  Game::Limited_uint8_t limitedTwo(k2, k3);
  EXPECT_EQ(limitedOne - limitedTwo, k0);
}

TEST(LimitedUint8tTest, SubtractOverLimitClass) {
  Game::Limited_uint8_t limitedOne(k0, k2);
  Game::Limited_uint8_t limitedTwo(k3, k3);
  EXPECT_EQ(limitedOne - limitedTwo, k0);
}

TEST(LimitedUint8tTest, SubtractOverLimitTwiceClass) {
  Game::Limited_uint8_t limitedOne(k200, k250);
  Game::Limited_uint8_t limitedTwo(k255, k255);
  EXPECT_EQ(limitedOne - limitedTwo, k196);
}

TEST(LimitedUint8tTest, SubtractOverLimit254Class) {
  Game::Limited_uint8_t limitedOne(k200, k254);
  Game::Limited_uint8_t limitedTwo(k201, k255);
  EXPECT_EQ(limitedOne - limitedTwo, k254);
}

TEST(LimitedUint8tTest, SubtractOverLimit255Class) {
  Game::Limited_uint8_t limitedOne(k200, k255);
  Game::Limited_uint8_t limitedTwo(k201, k255);
  EXPECT_EQ(limitedOne - limitedTwo, k255);
}

TEST_F(LimitedFixture, ComparisonSmaller) {
  EXPECT_TRUE(getLimitedOne() < getLimitedTwo());
  EXPECT_TRUE(getLimitedOne() < k204);
  EXPECT_FALSE(getLimitedTwo() < getLimitedOne());
  EXPECT_FALSE(k204 < getLimitedOne());
  EXPECT_FALSE(getLimitedOne() < k200);
  EXPECT_FALSE(getLimitedOne() < getLimitedOne());
}

TEST_F(LimitedFixture, ComparisonBigger) {
  EXPECT_FALSE(getLimitedOne() > getLimitedTwo());
  EXPECT_FALSE(getLimitedOne() > k204);
  EXPECT_TRUE(getLimitedTwo() > getLimitedOne());
  EXPECT_TRUE(k204 > getLimitedOne());
  EXPECT_FALSE(getLimitedOne() > k200);
  EXPECT_FALSE(getLimitedOne() > getLimitedOne());
}

TEST_F(LimitedFixture, ComparisonSmallerOrEqual) {
  EXPECT_TRUE(getLimitedOne() <= getLimitedTwo());
  EXPECT_TRUE(getLimitedOne() <= k204);
  EXPECT_FALSE(getLimitedTwo() <= getLimitedOne());
  EXPECT_FALSE(k204 <= getLimitedOne());
  EXPECT_TRUE(getLimitedOne() <= k200);
  EXPECT_TRUE(getLimitedOne() <= getLimitedOne());
}

TEST_F(LimitedFixture, ComparisonBiggerOrEqual) {
  EXPECT_FALSE(getLimitedOne() >= getLimitedTwo());
  EXPECT_FALSE(getLimitedOne() >= k204);
  EXPECT_TRUE(getLimitedTwo() >= getLimitedOne());
  EXPECT_TRUE(k204 >= getLimitedOne());
  EXPECT_TRUE(getLimitedOne() >= k200);
  EXPECT_TRUE(getLimitedOne() >= getLimitedOne());
}
#include "../inc/texture_loader.hpp"
#include "gtest/gtest.h"
#include <filesystem>

namespace {

constexpr std::string texturePath = "test-images";
constexpr std::string notExistingPath = "NotExisting";
constexpr std::string noImagesPath = "inc";

TEST(TextureLoader_Tests, CanLoadTrueTextures) {
  tools::TextureLoader loader{texturePath};
  std::map<std::string, sf::Texture, std::less<>> textureMap;
  loader.loadTextures(textureMap);
  EXPECT_TRUE(textureMap.contains("apple"));
  EXPECT_TRUE(textureMap.contains("body_topleft"));
  EXPECT_TRUE(textureMap.contains("snake_skin_texture"));
}

TEST(TextureLoader_Tests, ThrowsWhenNotExistingPathIsUsed) {
  tools::TextureLoader loader{notExistingPath};
  std::map<std::string, sf::Texture, std::less<>> textureMap;
  EXPECT_THROW(loader.loadTextures(textureMap),
               std::filesystem::filesystem_error);
}

TEST(TextureLoader_Tests, LoadingWhenNoTexturesResultsInEmptyMap) {
  tools::TextureLoader loader{noImagesPath};
  std::map<std::string, sf::Texture, std::less<>> textureMap;
  loader.loadTextures(textureMap);
  EXPECT_EQ(textureMap.size(), 0);
}

} // namespace
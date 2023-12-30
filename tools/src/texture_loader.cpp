#include "../inc/texture_loader.hpp"
#include <filesystem>
#include <iostream>

namespace tools {
TextureLoader::TextureLoader(const std::string &iTexturePath)
    : texturePath{iTexturePath} {}

void TextureLoader::loadTextures(
    std::map<std::string, sf::Texture, std::less<>> &oTexturesMap) const {
  std::cout << "Loading Textures\n";
  for (const auto &file : std::filesystem::directory_iterator(texturePath)) {
    if (auto fileExtension = file.path().string().substr(
            file.path().string().find_last_of(".") + 1);
        fileExtension != "png" && fileExtension != "jpg") {
      continue;
    }
    auto startPosition = file.path().string().find_last_of("/") + 1U;
    auto endPosition = file.path().string().find_last_of(".");
    auto name =
        file.path().string().substr(startPosition, endPosition - startPosition);
    oTexturesMap.try_emplace(name);
    oTexturesMap[name].loadFromFile(file.path());
  }
  std::cout << "Textures Loaded\n";
}
} // namespace tools
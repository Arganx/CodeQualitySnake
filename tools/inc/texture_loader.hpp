#ifndef TEXTURE_LOADER_GUARD
#define TEXTURE_LOADER_GUARD

#include "SFML/Graphics/Texture.hpp"
#include <map>
#include <string>

namespace tools {
class TextureLoader {
  std::string texturePath;

public:
  explicit TextureLoader(const std::string &iTexturePath);
  void loadTextures(
      std::map<std::string, sf::Texture, std::less<>> &oTexturesMap) const;
};
} // namespace tools

#endif
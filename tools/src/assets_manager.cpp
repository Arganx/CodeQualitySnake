#include "../inc/assets_manager.hpp"
#include "../inc/exceptions.hpp"
#include <memory>

namespace tools {
AssetsManager::AssetsManager(const std::string &iFontPath,
                             const std::string &iTexturesPath)
    : textureLoader{iTexturesPath} {
  if (!font->loadFromFile(iFontPath)) {
    throw tools::exceptions::FontNotFoundException("Failed to load font");
  }
  textureLoader.loadTextures(*textures);
}

std::shared_ptr<sf::Font> AssetsManager::getFont() const { return font; }
std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
AssetsManager::getTextures() const {
  return textures;
}
} // namespace tools
#ifndef ASSET_MANAGER_GUARD
#define ASSET_MANAGER_GUARD

#include "texture_loader.hpp"
#include <SFML/Graphics/Font.hpp>
#include <memory>
namespace tools {
class AssetsManager {
private:
  std::shared_ptr<sf::Font> font{std::make_shared<sf::Font>()};
  std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>> textures{
      std::make_shared<std::map<std::string, sf::Texture, std::less<>>>()};
  TextureLoader textureLoader;

public:
  AssetsManager(const std::string &iFontPath, const std::string &iTexturesPath);
  std::shared_ptr<sf::Font> getFont() const;
  std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
  getTextures() const;
};
} // namespace tools

#endif
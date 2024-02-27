#ifndef TEXT_BOX_GUARD
#define TEXT_BOX_GUARD
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <memory>

namespace tools {

class TextBox : public sf::RectangleShape {
private:
  sf::Text text;
  std::shared_ptr<sf::Font> fontPtr;

public:
  explicit TextBox(std::shared_ptr<sf::Font> iFontPtr);

  void setString(const std::string &str);

  std::string getString() const;

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
  void addCharacter(const uint32_t &iCharacter);
  void resize(const sf::Vector2u &iNewWindowSize);
};
} // namespace tools
#endif
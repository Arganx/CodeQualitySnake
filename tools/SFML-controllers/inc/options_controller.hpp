#ifndef OPTIONS_CONTROLLER_GUARD
#define OPTIONS_CONTROLLER_GUARD

#include "../../inc/screen_selector.hpp"
#include "../../inc/text_box.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cstdint>
#include <map>
#include <memory>

namespace controllers {
enum class OptionsButton { Ok, Cancel, None };
class OptionController {
private:
  std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
      textureMapPtr;
  std::shared_ptr<sf::Font> fontPtr;
  sf::Sprite backgroundObject;
  sf::Sprite okButtonSprite;
  sf::Text okText;
  sf::Sprite cancelButtonSprite;
  OptionsButton pressedButton{OptionsButton::None};
  sf::Text cancelText;
  tools::TextBox nameBox;
  uint8_t selectedTextBox{0U};
  bool mouseButtonPressed{false};
  void resizeBackground(const sf::Vector2u &iNewWindowSize);
  void resizeOkButton(const sf::Vector2u &iNewWindowSize);
  void resizeCancelButton(const sf::Vector2u &iNewWindowSize);
  void resizeOkText();
  void resizeCancelText();
  void handleEvent(sf::RenderWindow &iWindow, const sf::Event &iEvent,
                   tools::ScreenSelector &ioSelector);
  void handleCancel(tools::ScreenSelector &ioSelector) const;
  void handleOk(tools::ScreenSelector &ioSelector) const;

public:
  explicit OptionController(
      const sf::Vector2u &iWindowSize,
      std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
          iTextureMap,
      const std::shared_ptr<sf::Font> &iFont);
  void call(sf::RenderWindow &iWindow, tools::ScreenSelector &ioSelector);
  void resize(const sf::Vector2u &iNewWindowSize);
};
} // namespace controllers
#endif
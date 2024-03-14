#ifndef OPTIONS_CONTROLLER_GUARD
#define OPTIONS_CONTROLLER_GUARD
#include "../../inc/options_manager.hpp"
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
enum class OptionsButton { Ok, Cancel, None, PreviousCandy, NextCandy };
class OptionController {
private:
  std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
      textureMapPtr;
  std::shared_ptr<sf::Font> fontPtr;
  sf::Sprite backgroundObject;
  sf::Sprite backgroundBoard;
  sf::Sprite okButtonSprite;
  sf::Text okText;
  sf::Sprite cancelButtonSprite;
  OptionsButton pressedButton{OptionsButton::None};
  sf::Text cancelText;
  tools::TextBox nameBox;
  tools::TextBox widthBox;
  tools::TextBox heightBox;
  sf::Text playerNameText;
  sf::Text boardDimensionsText;
  sf::Text candyText;
  uint8_t selectedTextBox{0U};
  sf::Sprite candyLeft;
  sf::Sprite candyRight;
  sf::Sprite candyExample;
  bool mouseButtonPressed{false};
  void resizeBackground(const sf::Vector2u &iNewWindowSize);
  void resizeOkButton(const sf::Vector2u &iNewWindowSize);
  void resizeCancelButton(const sf::Vector2u &iNewWindowSize);
  void resizeOkText();
  void resizePlayerNameText(const sf::Vector2u &iNewWindowSize);
  void resizeCancelText();
  void resizeBackgroundBoard(const sf::Vector2u &iNewWindowSize);
  void resizeBoardDimensionsText(const sf::Vector2u &iNewWindowSize);
  void resizeCandyText(const sf::Vector2u &iNewWindowSize);
  void handleEvent(sf::RenderWindow &iWindow, const sf::Event &iEvent,
                   tools::ScreenSelector &ioSelector,
                   tools::OptionsManager &iOptionsManager);
  void handleCancel(tools::ScreenSelector &ioSelector,
                    tools::OptionsManager &iOptionsManager) const;
  void handleOk(tools::ScreenSelector &ioSelector,
                tools::OptionsManager &iOptionsManager) const;
  void resizeCandyLeft(const sf::Vector2u &iNewWindowSize);
  void resizeCandyRight(const sf::Vector2u &iNewWindowSize);
  void resizeCandyExample(const sf::Vector2u &iNewWindowSize);

public:
  explicit OptionController(
      const sf::Vector2u &iWindowSize,
      std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
          iTextureMap,
      const std::shared_ptr<sf::Font> &iFont);
  void call(sf::RenderWindow &iWindow, tools::ScreenSelector &ioSelector,
            tools::OptionsManager &iOptionsManager);
  void resize(const sf::Vector2u &iNewWindowSize);
  void refreshValues(const tools::OptionsManager &iOptionsManager);
};
} // namespace controllers
#endif
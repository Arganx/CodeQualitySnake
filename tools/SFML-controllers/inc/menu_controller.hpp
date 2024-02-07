#ifndef MENU_GUARD
#define MENU_GUARD

#include "../../inc/screen_selector.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <cstdint>
#include <memory>
#include <vector>
namespace controllers {
enum class Button { New_Game, High_Scores, Options, Exit, None };
class MenuController {
private:
  std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
      textureMapPtr;
  std::shared_ptr<sf::Font> fontPtr;
  std::vector<sf::Sprite> buttonsSprites;
  std::vector<sf::Text> buttonTexts;
  sf::Sprite backgroundSprite;
  bool mouseButtonPressed{false};
  Button pressedButton{Button::None};
  void createButton(const sf::Vector2u &iWindowSize,
                    const float iXButtonPosition, float &ioYButtonPosition,
                    const float iButtonYSize);
  void createTexts(const std::string &iLabel, const sf::Sprite &iButton,
                   uint8_t iFontSize = 13);
  void resizeButton(sf::Sprite &ioButton, const sf::Vector2u &iWindowSize,
                    const float iXButtonPosition, float &ioYButtonPosition,
                    const float iButtonYSize) const;
  void resizeBackground(const sf::Vector2u &iNewWindowSize);
  void resizeText(sf::Text &ioButtonText, const sf::Sprite &iButton) const;
  void handleEvent(sf::RenderWindow &iWindow, const sf::Event &iEvent,
                   tools::ScreenSelector &ioSelector);

public:
  MenuController(
      std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
          iTextureMap,
      const std::shared_ptr<sf::Font> &iFont,
      const std::vector<std::string> &iButtonLabels,
      const sf::Vector2u &iWindowSize);
  void call(sf::RenderWindow &iWindow, tools::ScreenSelector &ioSelector);
  void resize(const sf::Vector2u &iNewWindowSize);
};
} // namespace controllers
#endif
#ifndef MENU_GUARD
#define MENU_GUARD

#include "../../inc/screen_selector.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
namespace tools {
enum class Button { New_Game, High_Scores, Options, Exit, None };
class Menu {
private:
  std::vector<sf::Sprite> buttonsSprites;
  std::vector<sf::Text> buttonTexts;
  sf::Texture buttonTexture;
  sf::Texture backgroundTexture;
  sf::Sprite backgroundSprite;
  sf::Font buttonFont;
  bool mouseButtonPressed{false};
  Button pressedButton{Button::None};
  void createButton(const sf::Vector2u &iWindowSize,
                    const float iXButtonPosition, float &ioYButtonPosition,
                    const float iButtonYSize);
  void createTexts(const std::string &iLabel, const sf::Sprite &iButton,
                   uint8_t iFontSize = 13);
  void loadMenuAssets(const std::string &iButtonTexturePath,
                      const std::string &iBackgroundTexturePath,
                      const std::string &iButtonFontPath);
  void resizeButton(sf::Sprite &ioButton, const sf::Vector2u &iWindowSize,
                    const float iXButtonPosition, float &ioYButtonPosition,
                    const float iButtonYSize) const;
  void resizeBackground(const sf::Vector2u &iNewWindowSize);
  void resizeText(sf::Text &ioButtonText, const sf::Sprite &iButton) const;
  void handleEvent(sf::RenderWindow &iWindow, const sf::Event &iEvent,
                   ScreenSelector &ioSelector);
  void resize(const sf::Vector2u &iNewWindowSize);

public:
  Menu(const std::vector<std::string> &iButtonLabels,
       const std::string &iButtonTexturePath,
       const std::string &iBackgroundTexturePath,
       const std::string &iButtonFontPath, const sf::Vector2u &iWindowSize);
  void call(sf::RenderWindow &iWindow, ScreenSelector &ioSelector);
};
} // namespace tools
#endif
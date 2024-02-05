#ifndef HIGH_SCORE_GUARD
#define HIGH_SCORE_GUARD
#include "../../inc/database_manager.hpp"
#include "../../inc/screen_selector.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <map>
#include <memory>
#include <string>
namespace controllers {
class HighScoreController {
private:
  std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
      textureMapPtr;
  std::vector<sf::Text> highScoreTexts;
  sf::Sprite backgroundSprite;
  sf::Sprite highScoreBoardSprite;
  sf::Sprite returnButtonSprite;
  std::shared_ptr<sf::Font> fontPtr;
  sf::Text buttonText;
  bool mouseButtonPressed{false};
  void handleEvent(sf::RenderWindow &iWindow, const sf::Event &iEvent,
                   tools::ScreenSelector &ioSelector);
  void resizeBackground(const sf::Vector2u &iNewWindowSize);
  void resizeButton(const sf::Vector2u &iNewWindowSize);
  void resizeButtonText();
  void resizeScoreTexts(const sf::Vector2u &iNewWindowSize);
  void resizeHighScoreBoard(const sf::Vector2u &iNewWindowSize);

public:
  explicit HighScoreController(
      const sf::Vector2u &iWindowSize,
      std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
          iTextureMap,
      const std::shared_ptr<sf::Font> &iFont);
  void updateHighScores(const tools::DatabaseManager &iDatabaseManager);
  void call(sf::RenderWindow &iWindow, tools::ScreenSelector &ioSelector);
  void resize(const sf::Vector2u &iNewWindowSize);
};
} // namespace controllers

#endif
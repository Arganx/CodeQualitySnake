#include "SFML/Graphics/RenderWindow.hpp"
#include "tools/SFML-controllers/inc/high_score_controller.hpp"
#include "tools/SFML-controllers/inc/menu_controller.hpp"
#include "tools/SFML-controllers/inc/new_game_controller.hpp"
#include "tools/inc/assets_manager.hpp"
#include "tools/inc/database_manager.hpp"
#include "tools/inc/screen_selector.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <memory>

constexpr std::string kTexturePath = "textures";

int main() {
  tools::AssetsManager assetsManager{"./fonts/SnakeChan.ttf", kTexturePath};
  tools::DatabaseManager databaseManager{"highscores.db"};

  auto window =
      std::make_shared<sf::RenderWindow>(sf::VideoMode(300, 300), "Snake Game");
  window->setFramerateLimit(30);

  tools::ScreenSelector selector;
  std::vector<std::string> labels{"New game", "High scores", "Options", "Exit"};
  controllers::MenuController menu{assetsManager.getTextures(),
                                   assetsManager.getFont(), labels,
                                   window->getSize()};
  controllers::NewGameController controller{5, 4, window,
                                            assetsManager.getTextures()};
  controllers::HighScoreController highScoresController{
      window->getSize(), assetsManager.getTextures(), assetsManager.getFont()};

  while (window->isOpen()) {
    switch (selector.getSelectedOption()) {
    default:
      if (selector.isFirstPass()) {
        menu.resize(window->getSize());
        selector.setFirstPass(false);
      }
      menu.call(*window, selector);
      break;
    case tools::SelectorOptions::HighScores:
      if (selector.isFirstPass()) {
        highScoresController.resize(window->getSize());
        highScoresController.updateHighScores(databaseManager);
        selector.setFirstPass(false);
      }
      highScoresController.call(*window, selector);
      break;
    case tools::SelectorOptions::Options:
      window->close();
      break;
    case tools::SelectorOptions::Game:
      if (selector.isFirstPass()) {
        controller.reset(5, 4);
        controller.startGame(selector, databaseManager);
        controller.resize(window->getSize());
        selector.setFirstPass(false);
      }
      controller.call(*window);
      break;
    }
  }
}
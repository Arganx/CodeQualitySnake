#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "tools/SFML-controllers/inc/menu_controller.hpp"
#include "tools/SFML-controllers/inc/new_game_controller.hpp"
#include "tools/inc/database_manager.hpp"
#include "tools/inc/screen_selector.hpp"
#include "tools/inc/texture_loader.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <memory>

const std::string texturePath = "textures";

int main() {
  tools::DatabaseManager databaseManager{"highscores.db"};
  auto textureMap =
      std::make_shared<std::map<std::string, sf::Texture, std::less<>>>();
  tools::TextureLoader textureLoader(texturePath);
  textureLoader.loadTextures(*textureMap);

  auto window =
      std::make_shared<sf::RenderWindow>(sf::VideoMode(300, 300), "Snake Game");
  window->setFramerateLimit(30);

  tools::ScreenSelector selector;
  std::vector<std::string> labels{"New game", "High scores", "Options", "Exit"};
  controllers::MenuController menu{labels, "./textures/menu_button.png",
                                   "./textures/menu_background.png",
                                   "./fonts/SnakeChan.ttf", window->getSize()};
  controllers::NewGameController controller{5, 4, window, textureMap};

  while (window->isOpen()) {
    switch (selector.getSelectedOption()) {
    default:
      if (selector.isFirstPass()) {
        menu.resize(window->getSize());
        selector.setFirstPass(false);
        auto scores = databaseManager.getBestScores();
        for (auto const &score : scores) {
          std::cout << score.first << " with a score of: " << score.second
                    << std::endl;
        }
      }
      menu.call(*window, selector);
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
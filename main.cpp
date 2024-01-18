#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "tools/SFML-tools/inc/menu.hpp"
#include "tools/SFML-tools/inc/new_game_controller.hpp"
#include "tools/inc/screen_selector.hpp"
#include "tools/inc/texture_loader.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <map>
#include <memory>

const std::string texturePath = "textures";

int main() {
  auto textureMap =
      std::make_shared<std::map<std::string, sf::Texture, std::less<>>>();
  tools::TextureLoader textureLoader(texturePath);
  textureLoader.loadTextures(*textureMap);

  auto window =
      std::make_shared<sf::RenderWindow>(sf::VideoMode(300, 300), "Snake Game");
  window->setFramerateLimit(30);

  tools::ScreenSelector selector;
  std::vector<std::string> labels{"New game", "High scores", "Options", "Exit"};
  tools::Menu menu{labels, "./textures/menu_button.png",
                   "./images/menu_background.png", "./fonts/SnakeChan.ttf",
                   window->getSize()};
  controllers::NewGameController controller{5, 4, window, textureMap};

  while (window->isOpen()) {
    switch (selector.getSelectedOption()) {
    default:
      menu.call(*window, selector);
      break;
    case tools::SelectorOptions::Options:
      window->close();
      break;
    case tools::SelectorOptions::Game:
      if (selector.isFirstPass()) {
        controller.reset(5, 4);
        controller.startGame(selector);
        selector.setFirstPass(false);
      }
      controller.call();
      break;
    }
  }
}
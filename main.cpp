#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "tools/SFML-tools/inc/new_game_controller.hpp"
#include "tools/inc/drawer.hpp"
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

  tools::Drawer drawer(window);
  controllers::NewGameController controller{5, 4, window, textureMap};

  while (window->isOpen()) {
    controller.call();
  }
}
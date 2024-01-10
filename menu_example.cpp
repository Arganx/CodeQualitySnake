#include "tools/SFML-tools/inc/menu.hpp"
#include "tools/inc/screen_selector.hpp"
#include "tools/inc/selector_options.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

int main() {
  sf::RenderWindow window(sf::VideoMode(400, 400), "Game menu");
  window.setFramerateLimit(30);
  std::vector<std::string> labels{"New game", "High scores", "Options", "Exit"};
  tools::Menu menu{labels, "./textures/menu_button.png",
                   "./images/menu_background.png", "./fonts/SnakeChan.ttf",
                   window.getSize()};
  tools::ScreenSelector selector;
  while (window.isOpen()) {
    switch (selector.getSelectedOption()) {
    default:
      menu.call(window, selector);
      break;
    case tools::SelectorOptions::Options:
      std::cout << "Not Implemented\n";
      window.close();
      break;
    case tools::SelectorOptions::Game:
      window.close();
      break;
    }
  }

  return 0;
}
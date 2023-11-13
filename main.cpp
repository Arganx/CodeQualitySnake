#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "direction.hpp"
#include "inc/game.hpp"
#include "tools/inc/visualiser.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>

void mainGameThread(std::stop_token stop_token, Game::Game &game) {
  while (true) {
    if (stop_token.stop_requested()) {
      return;
    }
    std::this_thread::sleep_for(static_cast<std::chrono::milliseconds>(500));
    if (!game.step()) {
      return;
    }
    tools::Visualiser::visualiseBoard(*game.getBoardPtr());
  }
}

void handleKey(const sf::Keyboard::Key &keyCode, Game::Game &game,
               std::jthread &gameThread) {

  if (keyCode == sf::Keyboard::Left) {
    game.setDirection(Direction::Direction::Left);
  }
  if (keyCode == sf::Keyboard::Right) {
    game.setDirection(Direction::Direction::Right);
  }
  if (keyCode == sf::Keyboard::Up) {
    game.setDirection(Direction::Direction::Up);
  }
  if (keyCode == sf::Keyboard::Down) {
    game.setDirection(Direction::Direction::Down);
  }
  if (keyCode == sf::Keyboard::Escape) {
    gameThread.request_stop();
    game.showStatus();
  }
}

int main() {
  Game::Game game;
  game.initGame(5, 4);
  sf::RenderWindow window(sf::VideoMode(200, 200), "Snake Game");
  std::stop_source stop_source;
  std::jthread gameThread(mainGameThread, std::ref(game));
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      if (event.type == sf::Event::KeyPressed) {
        handleKey(event.key.code, game, gameThread);
      }
    }

    window.clear();
    window.display();
  }
}
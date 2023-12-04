#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "board.hpp"
#include "direction.hpp"
#include "inc/game.hpp"
#include "tools/inc/visualiser.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <memory>
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

std::vector<std::vector<sf::RectangleShape>>
createTiles(const Game::Board &iBoard, const sf::Vector2u &iResolution,
            const sf::Texture &texture) {
  // TODO think what to do with the offset
  auto offsetX = static_cast<uint16_t>(
      std::round(0.0f * static_cast<float>(iResolution.x)));
  auto offsetY = static_cast<uint16_t>(
      std::round(0.0f * static_cast<float>(iResolution.y)));
  auto sizeX =
      static_cast<uint16_t>((iResolution.x - 2 * offsetX) / iBoard.getWidth());
  auto sizeY =
      static_cast<uint16_t>((iResolution.y - 2 * offsetY) / iBoard.getHeight());
  std::vector<std::vector<sf::RectangleShape>> sfmlTiles;
  sfmlTiles.reserve(iBoard.getHeight());
  for (uint8_t column{0U}; column < iBoard.getHeight(); ++column) {
    sfmlTiles.emplace_back();
    sfmlTiles[column].reserve(iBoard.getWidth());
    for (uint8_t row{0U}; row < iBoard.getWidth(); ++row) {
      sfmlTiles[column].emplace_back(sf::Vector2f(sizeX, sizeY));
      sfmlTiles[column][row].setPosition(
          static_cast<float>(offsetX + row * sizeX),
          static_cast<float>(offsetY + column * sizeY));
      sfmlTiles[column][row].setTexture(&texture);
    }
  }
  return sfmlTiles;
}

void drawTiles(const std::vector<std::vector<sf::RectangleShape>> &iTiles,
               sf::RenderWindow &iWindow) {
  for (const auto &column : iTiles) {
    for (const auto &tile : column) {
      iWindow.draw(tile);
    }
  }
}

int main() {
  Game::Game game;
  game.initGame(5, 4); // From here the boardPtr is initialized
  sf::RenderWindow window(sf::VideoMode(100, 100), "Snake Game");
  sf::Texture grassTexture;
  grassTexture.loadFromFile("Textures/Grass_04.png");
  auto tiles = createTiles(*game.getBoardPtr(), window.getSize(), grassTexture);

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
    drawTiles(tiles, window);
    window.display();
  }
}
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
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <utility>

void updateBlocksPositions(std::mutex &snakeBlockMutex,
                           std::vector<sf::RectangleShape> &snakeBlocks,
                           Game::Game &game,
                           const std::pair<uint16_t, uint16_t> &tileSizes) {
  std::scoped_lock lock(snakeBlockMutex);
  for (uint16_t blockIndex{0U}; blockIndex < snakeBlocks.size(); ++blockIndex) {
    snakeBlocks[blockIndex].setPosition(game.getSnake()
                                                .getSnakeSegments()[blockIndex]
                                                .getPosition()
                                                .getXPosition() *
                                            tileSizes.first,
                                        game.getSnake()
                                                .getSnakeSegments()[blockIndex]
                                                .getPosition()
                                                .getYPosition() *
                                            tileSizes.second);
  }
}

void mainGameThread(std::stop_token stop_token, Game::Game &game,
                    std::vector<sf::RectangleShape> &snakeBlocks,
                    const std::pair<uint16_t, uint16_t> &tileSizes,
                    std::mutex &snakeBlockMutex) {
  while (true) {
    if (stop_token.stop_requested()) {
      return;
    }
    std::this_thread::sleep_for(static_cast<std::chrono::milliseconds>(500));
    if (!game.step()) {
      return;
    }
    // TODO make sure the sizes are the same and insert the blocks if they are
    // not.
    updateBlocksPositions(snakeBlockMutex, snakeBlocks, game, tileSizes);
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
            const std::vector<sf::Texture> &iTextures,
            std::pair<uint16_t, uint16_t> &oTileSizes) {
  if (iTextures.size() < 1U) {
    throw std::invalid_argument(
        "There must be at least one texture for the tiles");
  }
  // TODO think what to do with the offset
  auto offsetX = static_cast<uint16_t>(
      std::round(0.0f * static_cast<float>(iResolution.x)));
  auto offsetY = static_cast<uint16_t>(
      std::round(0.0f * static_cast<float>(iResolution.y)));
  oTileSizes.first =
      static_cast<uint16_t>((iResolution.x - 2U * offsetX) / iBoard.getWidth());
  oTileSizes.second = static_cast<uint16_t>((iResolution.y - 2U * offsetY) /
                                            iBoard.getHeight());
  std::vector<std::vector<sf::RectangleShape>> sfmlTiles;
  sfmlTiles.reserve(iBoard.getHeight());
  for (uint8_t column{0U}; column < iBoard.getHeight(); ++column) {
    sfmlTiles.emplace_back();
    sfmlTiles[column].reserve(iBoard.getWidth());
    for (uint8_t row{0U}; row < iBoard.getWidth(); ++row) {
      sfmlTiles[column].emplace_back(
          sf::Vector2f(oTileSizes.first, oTileSizes.second));
      sfmlTiles[column][row].setPosition(
          static_cast<float>(offsetX + row * oTileSizes.first),
          static_cast<float>(offsetY + column * oTileSizes.second));

      sfmlTiles[column][row].setTexture(
          &iTextures[(column + row) % iTextures.size()]);
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

void drawSnakeBlocks(std::mutex &iSnakeBlockMutex, sf::RenderWindow &iWindow,
                     const std::vector<sf::RectangleShape> &iSnakeBlocks) {
  std::scoped_lock lock(iSnakeBlockMutex);
  for (const auto &block : iSnakeBlocks) {
    iWindow.draw(block);
  }
}

int main() {
  Game::Game game;
  game.initGame(5, 4); // From here the boardPtr is initialized

  sf::RenderWindow window(sf::VideoMode(100, 100), "Snake Game");

  sf::Texture snakeHeadTexture;
  snakeHeadTexture.loadFromFile("Textures/head_up.png");
  std::vector<sf::Texture> textures;
  textures.emplace_back();
  textures.emplace_back();
  textures[0].loadFromFile("Textures/Light_Green.png");
  textures[1].loadFromFile("Textures/Dark_Green.png");

  std::pair<uint16_t, uint16_t> tileSizes;
  auto tiles =
      createTiles(*game.getBoardPtr(), window.getSize(), textures, tileSizes);

  std::vector<sf::RectangleShape> snakeBlocks;
  snakeBlocks.emplace_back(sf::Vector2f(tileSizes.first, tileSizes.second));
  snakeBlocks[0].setTexture(&snakeHeadTexture);

  std::mutex snakeBlockMutex;

  std::stop_source stop_source;
  std::jthread gameThread(mainGameThread, std::ref(game), std::ref(snakeBlocks),
                          std::ref(tileSizes), std::ref(snakeBlockMutex));
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
    drawSnakeBlocks(snakeBlockMutex, window, snakeBlocks);
    window.display();
  }
}
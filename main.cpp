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
#include <cstdint>
#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <utility>

const std::string texturePath = "Textures";

struct Mutexes {
  std::mutex snakeBlockMutex{};
  std::mutex candyBlocksMutex{};
  std::mutex directionMutex{};
};

void updateBlocksPositions(std::mutex &snakeBlockMutex,
                           std::vector<sf::RectangleShape> &snakeBlocks,
                           Game::Game &game,
                           const std::pair<uint16_t, uint16_t> &tileSizes) {
  std::scoped_lock lock(snakeBlockMutex);
  for (uint16_t blockIndex{0U}; blockIndex < snakeBlocks.size(); ++blockIndex) {
    snakeBlocks[blockIndex].setPosition(game.getSnake()
                                                .getSnakeSegments()[blockIndex]
                                                .getPosition()
                                                .getXPosition()
                                                .getValue() *
                                            tileSizes.first,
                                        game.getSnake()
                                                .getSnakeSegments()[blockIndex]
                                                .getPosition()
                                                .getYPosition()
                                                .getValue() *
                                            tileSizes.second);
  }
}

void updateCandy(std::mutex &candyBlocksMutex,
                 std::vector<sf::RectangleShape> &candyBlocks,
                 const Game::Game &game,
                 const std::pair<uint16_t, uint16_t> &tileSizes,
                 std::map<std::string, sf::Texture, std::less<>> &textures) {
  std::scoped_lock lock(candyBlocksMutex);
  while (candyBlocks.size() < game.getSnacksPositions().size()) {
    candyBlocks.emplace_back(sf::Vector2f(tileSizes.first, tileSizes.second));
    candyBlocks.back().setTexture(&textures["apple"]);
  }
  while (candyBlocks.size() > game.getSnacksPositions().size()) {
    candyBlocks.pop_back();
  }
  if (candyBlocks.size() != game.getSnacksPositions().size()) {
    throw std::invalid_argument(
        "Candy blocks don't much the number of candies in the game");
  }
  for (uint16_t blockIndex{0U}; blockIndex < candyBlocks.size(); ++blockIndex) {
    candyBlocks[blockIndex].setPosition(
        game.getSnacksPositions()[blockIndex].getXPosition().getValue() *
            tileSizes.first,
        game.getSnacksPositions()[blockIndex].getYPosition().getValue() *
            tileSizes.second);
  }
}

void setHeadTexture(std::vector<sf::RectangleShape> &snakeBlocks,
                    std::map<std::string, sf::Texture, std::less<>> &textures,
                    const Direction::Direction stepDirection) {
  if (snakeBlocks.empty()) {
    return;
  }
  switch (stepDirection) {
    using enum Direction::Direction;
  case Up:
    snakeBlocks[0].setTexture(&textures["head_up"]);
    break;
  case Down:
    snakeBlocks[0].setTexture(&textures["head_down"]);
    break;
  case Left:
    snakeBlocks[0].setTexture(&textures["head_left"]);
    break;
  case Right:
    snakeBlocks[0].setTexture(&textures["head_right"]);
    break;
  default:
    break;
  }
}

void setTailTexture(std::vector<sf::RectangleShape> &snakeBlocks,
                    std::map<std::string, sf::Texture, std::less<>> &textures,
                    Game::Game &game) {
  if (game.getSnake().getSnakeSegments().size() != snakeBlocks.size()) {
    throw std::domain_error("Mismatch between snakeBlocks and snake segments");
  }
  if (snakeBlocks.size() < 2U) {
    return;
  }

  auto tailX =
      game.getSnake().getSnakeSegments().back().getPosition().getXPosition();
  auto tailY =
      game.getSnake().getSnakeSegments().back().getPosition().getYPosition();
  auto beforeLastX = (game.getSnake().getSnakeSegments().end() - 2)
                         ->getPosition()
                         .getXPosition();
  auto beforeLastY = (game.getSnake().getSnakeSegments().end() - 2)
                         ->getPosition()
                         .getYPosition();

  if (tailX + 1U == beforeLastX) {
    snakeBlocks.back().setTexture(&textures["tail_left"]);
    return;
  }
  if (tailX - 1U == beforeLastX) {
    snakeBlocks.back().setTexture(&textures["tail_right"]);
    return;
  }
  if (tailY + 1U == beforeLastY) {
    snakeBlocks.back().setTexture(&textures["tail_up"]);
    return;
  }
  if (tailY - 1U == beforeLastY) {
    snakeBlocks.back().setTexture(&textures["tail_down"]);
    return;
  }
  return;
}

void setSegmentsTextures(
    std::vector<sf::RectangleShape> &snakeBlocks,
    std::map<std::string, sf::Texture, std::less<>> &textures,
    Game::Game &game) {
  if (snakeBlocks.size() < 3U) {
    return;
  }
  // TODO make sure vectors have the same size
  for (uint16_t segmentNumber{1U}; segmentNumber < snakeBlocks.size() - 1;
       ++segmentNumber) {
    auto previousX{game.getSnake()
                       .getSnakeSegments()[segmentNumber - 1]
                       .getPosition()
                       .getXPosition()};
    auto previousY{game.getSnake()
                       .getSnakeSegments()[segmentNumber - 1]
                       .getPosition()
                       .getYPosition()};
    auto nextX{game.getSnake()
                   .getSnakeSegments()[segmentNumber + 1]
                   .getPosition()
                   .getXPosition()};
    auto nextY{game.getSnake()
                   .getSnakeSegments()[segmentNumber + 1]
                   .getPosition()
                   .getYPosition()};
    auto currentX{game.getSnake()
                      .getSnakeSegments()[segmentNumber]
                      .getPosition()
                      .getXPosition()};
    auto currentY{game.getSnake()
                      .getSnakeSegments()[segmentNumber]
                      .getPosition()
                      .getYPosition()};

    if (currentX + 1U == nextX) // Exit on the right
    {
      if (currentY - 1U == previousY) // Entrance on top
      {
        snakeBlocks[segmentNumber].setTexture(&textures["body_topright"]);
      } else if (currentY + 1U == previousY) // Entrance on bottom
      {
        snakeBlocks[segmentNumber].setTexture(&textures["body_bottomright"]);
      } else {
        snakeBlocks[segmentNumber].setTexture(&textures["body_horizontal"]);
      }
    } else if (currentX - 1U == nextX) // Exit on the left
    {
      if (currentY - 1U == previousY) // Entrance on top
      {
        snakeBlocks[segmentNumber].setTexture(&textures["body_topleft"]);
      } else if (currentY + 1U == previousY) // Entrance on bottom
      {
        snakeBlocks[segmentNumber].setTexture(&textures["body_bottomleft"]);
      } else {
        snakeBlocks[segmentNumber].setTexture(&textures["body_horizontal"]);
      }
    } else if (currentY - 1U == nextY) // Exit on top
    {
      if (currentX - 1U == previousX) // Entrance on the left
      {
        snakeBlocks[segmentNumber].setTexture(&textures["body_topleft"]);
      } else if (currentX + 1U == previousX) {
        snakeBlocks[segmentNumber].setTexture(&textures["body_topright"]);
      } else {
        snakeBlocks[segmentNumber].setTexture(&textures["body_vertical"]);
      }
    } else if (currentY + 1U == nextY) // Exit on the bottom
    {
      if (currentX - 1U == previousX) // Entrance on the left
      {
        snakeBlocks[segmentNumber].setTexture(&textures["body_bottomleft"]);
      } else if (currentX + 1U == previousX) {
        snakeBlocks[segmentNumber].setTexture(&textures["body_bottomright"]);
      } else {
        snakeBlocks[segmentNumber].setTexture(&textures["body_vertical"]);
      }
    } else {
      snakeBlocks[segmentNumber].setTexture(&textures["apple"]);
    }
  }
}

void setSnakeTextures(std::vector<sf::RectangleShape> &snakeBlocks,
                      std::map<std::string, sf::Texture, std::less<>> &textures,
                      Game::Game &game,
                      const Direction::Direction stepDirection) {
  setHeadTexture(snakeBlocks, textures, stepDirection);
  setTailTexture(snakeBlocks, textures, game);
  setSegmentsTextures(snakeBlocks, textures, game);
}

void mainGameThread(std::stop_token stop_token, Game::Game &game,
                    std::vector<sf::RectangleShape> &snakeBlocks,
                    std::vector<sf::RectangleShape> &candiesBlocks,
                    const std::pair<uint16_t, uint16_t> &tileSizes,
                    Mutexes &mutexes,
                    std::map<std::string, sf::Texture, std::less<>> &textures) {
  while (true) {
    if (stop_token.stop_requested()) {
      return;
    }
    std::this_thread::sleep_for(static_cast<std::chrono::milliseconds>(400));
    // TODO add mutex
    Direction::Direction stepDirection = game.getDirection();
    if (!game.step()) {
      return;
    }
    // TODO close mutex
    while (game.getSnake().getSnakeSegments().size() > snakeBlocks.size()) {
      snakeBlocks.emplace_back(sf::Vector2f(tileSizes.first, tileSizes.second));
    }
    if (game.getSnake().getSnakeSegments().size() != snakeBlocks.size()) {
      throw std::domain_error(
          "Length of snake segments does not match the graphics");
    }
    updateBlocksPositions(mutexes.snakeBlockMutex, snakeBlocks, game,
                          tileSizes);
    setSnakeTextures(snakeBlocks, textures, game, stepDirection);
    updateCandy(mutexes.candyBlocksMutex, candiesBlocks, game, tileSizes,
                textures);
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
            std::map<std::string, sf::Texture, std::less<>> &iTextureMap,
            std::pair<uint16_t, uint16_t> &oTileSizes) {
  oTileSizes.first = static_cast<uint16_t>(iResolution.x / iBoard.getWidth());
  oTileSizes.second = static_cast<uint16_t>(iResolution.y / iBoard.getHeight());
  std::vector<std::vector<sf::RectangleShape>> sfmlTiles;
  sfmlTiles.reserve(iBoard.getHeight());
  for (uint8_t column{0U}; column < iBoard.getHeight(); ++column) {
    sfmlTiles.emplace_back();
    sfmlTiles[column].reserve(iBoard.getWidth());
    for (uint8_t row{0U}; row < iBoard.getWidth(); ++row) {
      sfmlTiles[column].emplace_back(
          sf::Vector2f(oTileSizes.first, oTileSizes.second));
      sfmlTiles[column][row].setPosition(
          static_cast<float>(row * oTileSizes.first),
          static_cast<float>(column * oTileSizes.second));
      // TODO make sure the Textures are there
      if ((column + row) % 2U == 0) {
        sfmlTiles[column][row].setTexture(&iTextureMap["Light_Green"]);
      } else {
        sfmlTiles[column][row].setTexture(&iTextureMap["Dark_Green"]);
      }
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

void drawBlocks(std::mutex &iMutex, sf::RenderWindow &iWindow,
                const std::vector<sf::RectangleShape> &iBlocks) {
  std::scoped_lock lock(iMutex);
  for (const auto &block : iBlocks) {
    iWindow.draw(block);
  }
}

void loadTextures(
    std::map<std::string, sf::Texture, std::less<>> &texturesMap) {
  std::cout << "Loading Textures\n";
  for (const auto &file : std::filesystem::directory_iterator(texturePath)) {
    auto startPosition = file.path().string().find("/") + 1U;
    auto endPosition = file.path().string().find(".");
    auto name =
        file.path().string().substr(startPosition, endPosition - startPosition);
    texturesMap.try_emplace(name);
    texturesMap[name].loadFromFile(file.path());
  }
  std::cout << "Textures Loaded\n";
}

int main() {
  Game::Game game;
  game.initGame(5, 4); // From here the boardPtr is initialized

  std::map<std::string, sf::Texture, std::less<>> textureMap;
  loadTextures(textureMap);

  sf::RenderWindow window(sf::VideoMode(300, 300), "Snake Game");

  std::pair<uint16_t, uint16_t> tileSizes;
  auto tiles =
      createTiles(*game.getBoardPtr(), window.getSize(), textureMap, tileSizes);

  std::vector<sf::RectangleShape> snakeBlocks;
  snakeBlocks.emplace_back(sf::Vector2f(tileSizes.first, tileSizes.second));
  std::vector<sf::RectangleShape> candyBlocks;
  candyBlocks.emplace_back(sf::Vector2f(tileSizes.first, tileSizes.second));
  candyBlocks.back().setTexture(&textureMap["apple"]);

  Mutexes mutexes;

  updateBlocksPositions(mutexes.snakeBlockMutex, snakeBlocks, game, tileSizes);
  setSnakeTextures(snakeBlocks, textureMap, game, game.getDirection());
  updateCandy(mutexes.candyBlocksMutex, candyBlocks, game, tileSizes,
              textureMap);

  std::stop_source stop_source;
  std::jthread gameThread(mainGameThread, std::ref(game), std::ref(snakeBlocks),
                          std::ref(candyBlocks), std::ref(tileSizes),
                          std::ref(mutexes), std::ref(textureMap));
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
    drawBlocks(mutexes.snakeBlockMutex, window, snakeBlocks);
    drawBlocks(mutexes.candyBlocksMutex, window, candyBlocks);
    window.display();
  }
}
#include "../inc/new_game_controller.hpp"
#include "../../inc/exceptions.hpp"
#include "../../inc/screen_selector.hpp"
#include "../../inc/visualiser.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "exceptions.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <map>
#include <memory>
#include <thread>
namespace controllers {
NewGameController::NewGameController(
    uint8_t iGameWidth, uint8_t iGameHeight,
    std::shared_ptr<sf::RenderWindow> iWindowPtr,
    std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
        iTextureMap)
    : windowPtr{iWindowPtr}, drawer{iWindowPtr}, textureMapPtr{iTextureMap} {
  game.initGame(iGameWidth, iGameHeight);
  createTiles();
  snakeBlocks.emplace_back(
      sf::Vector2f(getTileSize().first, getTileSize().second));
  candyBlocks.emplace_back(
      sf::Vector2f(getTileSize().first, getTileSize().second));
  candyBlocks.back().setTexture(&(*textureMapPtr)["apple"]);

  updateBlocksPositions();
  setSnakeTextures(game.getDirection());
  updateCandy();
}
void NewGameController::startGame(tools::ScreenSelector &iSelector) {
  gameThread =
      std::make_unique<std::jthread>(&NewGameController::mainGameThread, this,
                                     stopSrc.get_token(), std::ref(iSelector));
}

Game::Game &NewGameController::getGame() { return game; }

const std::vector<std::vector<sf::RectangleShape>> &
NewGameController::getTiles() const {
  return boardTiles;
}

tools::Mutexes &NewGameController::getMutexes() { return mutexes; }

std::vector<sf::RectangleShape> &NewGameController::getCandyBlocks() {
  return candyBlocks;
}

std::pair<uint16_t, uint16_t> NewGameController::getTileSize() const {
  if (boardTiles.empty()) {
    return std::pair<uint16_t, uint16_t>{0, 0};
  }
  if (boardTiles.front().empty()) {
    return std::pair<uint16_t, uint16_t>{0, 0};
  }
  return std::pair<uint16_t, uint16_t>{boardTiles.front().front().getSize().x,
                                       boardTiles.front().front().getSize().y};
}

void NewGameController::createTiles() {
  if (game.getBoardPtr() == nullptr) {
    throw Game::SnakeExceptions::PointerNotInitializedException(
        "Board not initialized");
  }
  auto tileSizeX = static_cast<uint16_t>(windowPtr->getSize().x /
                                         game.getBoardPtr()->getWidth());
  auto tileSizeY = static_cast<uint16_t>(windowPtr->getSize().y /
                                         game.getBoardPtr()->getHeight());
  boardTiles.reserve(game.getBoardPtr()->getHeight());
  for (uint8_t column{0U}; column < game.getBoardPtr()->getHeight(); ++column) {
    boardTiles.emplace_back();
    boardTiles[column].reserve(game.getBoardPtr()->getWidth());
    for (uint8_t row{0U}; row < game.getBoardPtr()->getWidth(); ++row) {
      boardTiles[column].emplace_back(sf::Vector2f(tileSizeX, tileSizeY));
      boardTiles[column][row].setPosition(
          static_cast<float>(row * tileSizeX),
          static_cast<float>(column * tileSizeY));
      if (!(*textureMapPtr).contains("Light_Green") ||
          !(*textureMapPtr).contains("Dark_Green")) {
        throw tools::TextureNotFoundException(
            "Light_Green or Dark_Green textures not found");
      }
      if ((column + row) % 2U == 0) {
        boardTiles[column][row].setTexture(&(*textureMapPtr)["Light_Green"]);
      } else {
        boardTiles[column][row].setTexture(&(*textureMapPtr)["Dark_Green"]);
      }
    }
  }
}
std::vector<sf::RectangleShape> &NewGameController::getSnakeBlocks() {
  return snakeBlocks;
}

void NewGameController::mainGameThread(std::stop_token stopToken,
                                       tools::ScreenSelector &iSelector) {
  while (true) {
    if (stopToken.stop_requested()) {
      return;
    }
    std::this_thread::sleep_for(static_cast<std::chrono::milliseconds>(400));
    Direction::Direction stepDirection;
    if (gameStep(stepDirection)) {
      iSelector.setFirstPass(true);
      iSelector.setSelectedOption(tools::SelectorOptions::MainMenu);
      game.showStatus();
      return;
    }
    while (game.getSnake().getSnakeSegments().size() > snakeBlocks.size()) {
      snakeBlocks.emplace_back(
          sf::Vector2f(getTileSize().first, getTileSize().second));
    }
    if (game.getSnake().getSnakeSegments().size() != snakeBlocks.size()) {
      throw std::domain_error(
          "Length of snake segments does not match the graphics");
    }
    updateBlocksPositions();
    setSnakeTextures(stepDirection);
    updateCandy();
    tools::Visualiser::visualiseBoard(*game.getBoardPtr());
  }
}

void NewGameController::updateBlocksPositions() {
  std::scoped_lock lock(mutexes.snakeBlockMutex);
  for (uint16_t blockIndex{0U}; blockIndex < snakeBlocks.size(); ++blockIndex) {
    snakeBlocks[blockIndex].setPosition(game.getSnake()
                                                .getSnakeSegments()[blockIndex]
                                                .getPosition()
                                                .getXPosition()
                                                .getValue() *
                                            getTileSize().first,
                                        game.getSnake()
                                                .getSnakeSegments()[blockIndex]
                                                .getPosition()
                                                .getYPosition()
                                                .getValue() *
                                            getTileSize().second);
  }
}

void NewGameController::updateCandy() {
  std::scoped_lock lock(mutexes.candyBlocksMutex);
  while (candyBlocks.size() < game.getSnacksPositions().size()) {
    candyBlocks.emplace_back(
        sf::Vector2f(getTileSize().first, getTileSize().second));
    candyBlocks.back().setTexture(&(*textureMapPtr)["apple"]);
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
            getTileSize().first,
        game.getSnacksPositions()[blockIndex].getYPosition().getValue() *
            getTileSize().second);
  }
}

void NewGameController::setHeadTexture(
    const Direction::Direction stepDirection) {
  if (snakeBlocks.empty()) {
    return;
  }
  switch (stepDirection) {
    using enum Direction::Direction;
  case Up:
    snakeBlocks[0].setTexture(&(*textureMapPtr)["head_up"]);
    break;
  case Down:
    snakeBlocks[0].setTexture(&(*textureMapPtr)["head_down"]);
    break;
  case Left:
    snakeBlocks[0].setTexture(&(*textureMapPtr)["head_left"]);
    break;
  case Right:
    snakeBlocks[0].setTexture(&(*textureMapPtr)["head_right"]);
    break;
  default:
    break;
  }
}

void NewGameController::setTailTexture() {
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
    snakeBlocks.back().setTexture(&(*textureMapPtr)["tail_left"]);
    return;
  }
  if (tailX - 1U == beforeLastX) {
    snakeBlocks.back().setTexture(&(*textureMapPtr)["tail_right"]);
    return;
  }
  if (tailY + 1U == beforeLastY) {
    snakeBlocks.back().setTexture(&(*textureMapPtr)["tail_up"]);
    return;
  }
  if (tailY - 1U == beforeLastY) {
    snakeBlocks.back().setTexture(&(*textureMapPtr)["tail_down"]);
    return;
  }
  return;
}

void NewGameController::setSegmentsTextures() {
  if (snakeBlocks.size() < 3U) {
    return;
  }
  if (snakeBlocks.size() != game.getSnake().getSnakeSegments().size()) {
    throw std::domain_error(
        "Snake block size does not match snake segments size");
  }
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
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)["body_topright"]);
      } else if (currentY + 1U == previousY) // Entrance on bottom
      {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)["body_bottomright"]);
      } else {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)["body_horizontal"]);
      }
    } else if (currentX - 1U == nextX) // Exit on the left
    {
      if (currentY - 1U == previousY) // Entrance on top
      {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)["body_topleft"]);
      } else if (currentY + 1U == previousY) // Entrance on bottom
      {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)["body_bottomleft"]);
      } else {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)["body_horizontal"]);
      }
    } else if (currentY - 1U == nextY) // Exit on top
    {
      if (currentX - 1U == previousX) // Entrance on the left
      {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)["body_topleft"]);
      } else if (currentX + 1U == previousX) {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)["body_topright"]);
      } else {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)["body_vertical"]);
      }
    } else if (currentY + 1U == nextY) // Exit on the bottom
    {
      if (currentX - 1U == previousX) // Entrance on the left
      {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)["body_bottomleft"]);
      } else if (currentX + 1U == previousX) {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)["body_bottomright"]);
      } else {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)["body_vertical"]);
      }
    } else {
      snakeBlocks[segmentNumber].setTexture(&(*textureMapPtr)["apple"]);
    }
  }
}

void NewGameController::setSnakeTextures(
    const Direction::Direction stepDirection) {
  setHeadTexture(stepDirection);
  setTailTexture();
  setSegmentsTextures();
}

bool NewGameController::gameStep(Direction::Direction &oStepDirection) {
  std::scoped_lock lock(mutexes.directionMutex);
  oStepDirection = game.getDirection();
  if (!game.step()) {
    return true;
  }
  return false;
}

void NewGameController::handleKey(const sf::Keyboard::Key &keyCode) {
  std::scoped_lock lock(mutexes.directionMutex);

  if (keyCode == sf::Keyboard::Left) {
    game.setDirection(Direction::Direction::Left);
  } else if (keyCode == sf::Keyboard::Right) {
    game.setDirection(Direction::Direction::Right);
  } else if (keyCode == sf::Keyboard::Up) {
    game.setDirection(Direction::Direction::Up);
  } else if (keyCode == sf::Keyboard::Down) {
    game.setDirection(Direction::Direction::Down);
  } else if (keyCode == sf::Keyboard::Escape) {
    stopSrc.request_stop();
    game.showStatus();
  }
}

void NewGameController::call() {
  sf::Event event;
  while (windowPtr->pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      stopSrc.request_stop();
      windowPtr->close();
    }

    if (event.type == sf::Event::KeyPressed) {
      handleKey(event.key.code);
    }
  }
  windowPtr->clear();
  drawer.drawTiles(boardTiles);
  drawer.drawBlocks(mutexes.snakeBlockMutex, snakeBlocks);
  drawer.drawBlocks(mutexes.candyBlocksMutex, candyBlocks);
  windowPtr->display();
}

} // namespace controllers
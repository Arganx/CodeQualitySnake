#include "../inc/new_game_controller.hpp"
#include "../../../inc/exceptions.hpp"
#include "../../inc/config/texture_config.hpp"
#include "../../inc/database_manager.hpp"
#include "../../inc/exceptions.hpp"
#include "../../inc/options_manager.hpp"
#include "../../inc/screen_selector.hpp"
#include "../../inc/visualiser.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <cstdint>
#include <map>
#include <memory>
#include <stdexcept>
#include <thread>
namespace controllers {
NewGameController::NewGameController(
    const tools::OptionsManager &iOptionsManager,
    std::shared_ptr<sf::RenderWindow> iWindowPtr,
    std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
        iTextureMap)
    : windowPtr{iWindowPtr}, drawer{iWindowPtr}, textureMapPtr{iTextureMap} {
  reset(iOptionsManager);
}
void NewGameController::startGame(
    tools::ScreenSelector &iSelector, tools::DatabaseManager &iDatabaseManager,
    const tools::OptionsManager &iOptionsManager) {
  gameThread = std::make_unique<std::jthread>(
      &NewGameController::mainGameThread, this, stopSrc.get_token(),
      std::ref(iSelector), std::ref(iDatabaseManager),
      std::ref(iOptionsManager));
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
      if (!(*textureMapPtr).contains(config::kGridTileTextureOne) ||
          !(*textureMapPtr).contains(config::kGridTileTextureTwo)) {
        throw tools::exceptions::TextureNotFoundException(
            "Grid textures not found");
      }
      if ((column + row) % 2U == 0) {
        boardTiles[column][row].setTexture(
            &(*textureMapPtr)[config::kGridTileTextureOne]);
      } else {
        boardTiles[column][row].setTexture(
            &(*textureMapPtr)[config::kGridTileTextureTwo]);
      }
    }
  }
}
std::vector<sf::RectangleShape> &NewGameController::getSnakeBlocks() {
  return snakeBlocks;
}

void NewGameController::mainGameThread(
    std::stop_token stopToken, tools::ScreenSelector &iSelector,
    const tools::DatabaseManager &iDatabaseManager,
    const tools::OptionsManager &iOptionsManager) {
  while (true) {
    if (stopToken.stop_requested()) {
      return;
    }
    std::this_thread::sleep_for(static_cast<std::chrono::milliseconds>(400));
    Direction::Direction stepDirection;
    if (gameStep(stepDirection)) {
      iSelector.setFirstPass(true);
      iSelector.setSelectedOption(tools::SelectorOptions::MainMenu);
      iDatabaseManager.insertScore(iOptionsManager.getPlayerName(),
                                   game.getScore());
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
    setSnakeTextures(stepDirection, iOptionsManager);
    updateCandy(iOptionsManager);
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

void NewGameController::updateCandy(
    const tools::OptionsManager &iOptionsManager) {
  std::scoped_lock lock(mutexes.candyBlocksMutex);
  while (candyBlocks.size() < game.getSnacksPositions().size()) {
    candyBlocks.emplace_back(
        sf::Vector2f(getTileSize().first, getTileSize().second));
    candyBlocks.back().setTexture(
        &(*textureMapPtr)[iOptionsManager.getAcceptedCandy()]);
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
    const Direction::Direction stepDirection,
    const tools::OptionsManager &iOptionsManager) {
  if (snakeBlocks.empty()) {
    return;
  }
  switch (stepDirection) {
    using enum Direction::Direction;
  case Up:
    snakeBlocks[0].setTexture(
        &(*textureMapPtr)[iOptionsManager.getTextureHeadUp()]);
    break;
  case Down:
    snakeBlocks[0].setTexture(
        &(*textureMapPtr)[iOptionsManager.getTextureHeadDown()]);
    break;
  case Left:
    snakeBlocks[0].setTexture(
        &(*textureMapPtr)[iOptionsManager.getTextureHeadLeft()]);
    break;
  case Right:
    snakeBlocks[0].setTexture(
        &(*textureMapPtr)[iOptionsManager.getTextureHeadRight()]);
    break;
  default:
    break;
  }
}

void NewGameController::setTailTexture(
    const tools::OptionsManager &iOptionsManager) {
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
    snakeBlocks.back().setTexture(
        &(*textureMapPtr)[iOptionsManager.getTextureTailLeft()]);
    return;
  }
  if (tailX - 1U == beforeLastX) {
    snakeBlocks.back().setTexture(
        &(*textureMapPtr)[iOptionsManager.getTextureTailRight()]);
    return;
  }
  if (tailY + 1U == beforeLastY) {
    snakeBlocks.back().setTexture(
        &(*textureMapPtr)[iOptionsManager.getTextureTailUp()]);
    return;
  }
  if (tailY - 1U == beforeLastY) {
    snakeBlocks.back().setTexture(
        &(*textureMapPtr)[iOptionsManager.getTextureTailDown()]);
    return;
  }
  return;
}

void NewGameController::setSegmentsTextures(
    const tools::OptionsManager &iOptionsManager) {
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
            &(*textureMapPtr)[iOptionsManager.getTextureBodyTopRight()]);
      } else if (currentY + 1U == previousY) // Entrance on bottom
      {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)[iOptionsManager.getTextureBodyBottomRight()]);
      } else {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)[iOptionsManager.getTextureBodyHorizontal()]);
      }
    } else if (currentX - 1U == nextX) // Exit on the left
    {
      if (currentY - 1U == previousY) // Entrance on top
      {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)[iOptionsManager.getTextureBodyTopLeft()]);
      } else if (currentY + 1U == previousY) // Entrance on bottom
      {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)[iOptionsManager.getTextureBodyBottomLeft()]);
      } else {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)[iOptionsManager.getTextureBodyHorizontal()]);
      }
    } else if (currentY - 1U == nextY) // Exit on top
    {
      if (currentX - 1U == previousX) // Entrance on the left
      {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)[iOptionsManager.getTextureBodyTopLeft()]);
      } else if (currentX + 1U == previousX) {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)[iOptionsManager.getTextureBodyTopRight()]);
      } else {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)[iOptionsManager.getTextureBodyVertical()]);
      }
    } else if (currentY + 1U == nextY) // Exit on the bottom
    {
      if (currentX - 1U == previousX) // Entrance on the left
      {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)[iOptionsManager.getTextureBodyBottomLeft()]);
      } else if (currentX + 1U == previousX) {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)[iOptionsManager.getTextureBodyBottomRight()]);
      } else {
        snakeBlocks[segmentNumber].setTexture(
            &(*textureMapPtr)[iOptionsManager.getTextureBodyVertical()]);
      }
    } else {
      snakeBlocks[segmentNumber].setTexture(
          &(*textureMapPtr)[iOptionsManager.getAcceptedCandy()]);
    }
  }
}

void NewGameController::setSnakeTextures(
    const Direction::Direction stepDirection,
    const tools::OptionsManager &iOptionsManager) {
  setHeadTexture(stepDirection, iOptionsManager);
  setTailTexture(iOptionsManager);
  setSegmentsTextures(iOptionsManager);
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

void NewGameController::resize(const sf::Vector2u &iNewWindowSize,
                               const tools::OptionsManager &iOptionsManager) {
  // Resize board tiles
  if (boardTiles.size() != game.getBoardPtr()->getHeight()) {
    throw tools::exceptions::ExpectedSizeMismatchException(
        "Number of tiles in SFML, does not match the "
        "board height");
  }
  if (boardTiles[0].size() != game.getBoardPtr()->getWidth()) {
    throw tools::exceptions::ExpectedSizeMismatchException(
        "Number of tiles in SFML, does not match the "
        "board width");
  }
  auto newTileHeight = static_cast<float>(iNewWindowSize.y) /
                       static_cast<float>(boardTiles.size());
  auto newTileWidth = static_cast<float>(iNewWindowSize.x) /
                      static_cast<float>(boardTiles[0].size());
  for (uint8_t columnNumber{0U}; columnNumber < boardTiles.size();
       ++columnNumber) {
    for (uint8_t rowNumber{0U}; rowNumber < boardTiles[0].size(); ++rowNumber) {
      boardTiles[columnNumber][rowNumber].setSize(
          sf::Vector2f(newTileWidth, newTileHeight));
      boardTiles[columnNumber][rowNumber].setPosition(
          rowNumber * newTileWidth, columnNumber * newTileHeight);
    }
  }
  // Resize snake blocks
  for (auto &block : snakeBlocks) {
    block.setSize(sf::Vector2f(newTileWidth, newTileHeight));
  }
  updateBlocksPositions();
  // Resize candies
  for (auto &candy : candyBlocks) {
    candy.setSize(sf::Vector2f(newTileWidth, newTileHeight));
  }
  updateCandy(iOptionsManager);
}

void NewGameController::call(sf::RenderWindow &iWindow,
                             const tools::OptionsManager &iOptionsManager) {
  sf::Event event;
  while (windowPtr->pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      stopSrc.request_stop();
      windowPtr->close();
    }

    if (event.type == sf::Event::KeyPressed) {
      handleKey(event.key.code);
    }
    if (event.type == sf::Event::Resized) {
      sf::FloatRect visibleArea(0, 0, static_cast<float>(event.size.width),
                                static_cast<float>(event.size.height));
      iWindow.setView(sf::View(visibleArea));
      resize(iWindow.getSize(), iOptionsManager);
    }
  }
  windowPtr->clear();
  drawer.drawTiles(boardTiles);
  drawer.drawBlocks(mutexes.snakeBlockMutex, snakeBlocks);
  drawer.drawBlocks(mutexes.candyBlocksMutex, candyBlocks);
  windowPtr->display();
}

void NewGameController::reset(const tools::OptionsManager &iOptionsManager) {
  boardTiles.clear();
  snakeBlocks.clear();
  candyBlocks.clear();
  game.initGame(iOptionsManager.getBoardWidth(),
                iOptionsManager.getBoardHeight());
  createTiles();
  snakeBlocks.emplace_back(
      sf::Vector2f(getTileSize().first, getTileSize().second));
  candyBlocks.emplace_back(
      sf::Vector2f(getTileSize().first, getTileSize().second));
  candyBlocks.back().setTexture(
      &(*textureMapPtr)[iOptionsManager.getAcceptedCandy()]);

  updateBlocksPositions();
  setSnakeTextures(game.getDirection(), iOptionsManager);
  updateCandy(iOptionsManager);
}

} // namespace controllers
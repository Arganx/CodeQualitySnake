#include "../inc/new_game_controller.hpp"
#include "../../inc/exceptions.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "exceptions.hpp"
#include <SFML/Window/Window.hpp>
#include <map>

namespace controllers {
NewGameController::NewGameController(
    uint8_t iGameWidth, uint8_t iGameHeight,
    std::shared_ptr<sf::Window> iWindowPtr,
    std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
        iTextureMap)
    : windowPtr{iWindowPtr}, textureMapPtr{iTextureMap} {
  game.initGame(iGameWidth, iGameHeight);
  createTiles();
  snakeBlocks.emplace_back(
      sf::Vector2f(getTileSize().first, getTileSize().second));
  candyBlocks.emplace_back(
      sf::Vector2f(getTileSize().first, getTileSize().second));
  candyBlocks.back().setTexture(&(*textureMapPtr)["apple"]);
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

} // namespace controllers
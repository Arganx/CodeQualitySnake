#ifndef NEW_GAME_CONTROLLER_GUARD
#define NEW_GAME_CONTROLLER_GUARD

#include "../../inc/mutexes.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "game.hpp"
#include <cstdint>
#include <map>
#include <memory>
namespace controllers {

class NewGameController {
private:
  Game::Game game;
  std::vector<std::vector<sf::RectangleShape>> boardTiles;
  std::vector<sf::RectangleShape> snakeBlocks;
  std::shared_ptr<sf::Window> windowPtr;
  std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
      textureMapPtr;
  std::vector<sf::RectangleShape> candyBlocks;
  void createTiles();
  tools::Mutexes mutexes;

public:
  NewGameController(
      uint8_t iGameWidth, uint8_t iGameHeight,
      std::shared_ptr<sf::Window> iWindowPtr,
      std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
          iTextureMap);
  Game::Game &getGame();
  const std::vector<std::vector<sf::RectangleShape>> &getTiles() const;

  std::pair<uint16_t, uint16_t> getTileSize() const;
  std::vector<sf::RectangleShape> &getSnakeBlocks();
  std::vector<sf::RectangleShape> &getCandyBlocks();
  tools::Mutexes &getMutexes();
};
} // namespace controllers
#endif
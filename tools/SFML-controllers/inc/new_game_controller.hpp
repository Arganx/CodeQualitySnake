#ifndef NEW_GAME_CONTROLLER_GUARD
#define NEW_GAME_CONTROLLER_GUARD

#include "../../../inc/game.hpp"
#include "../../inc/database_manager.hpp"
#include "../../inc/drawer.hpp"
#include "../../inc/mutexes.hpp"
#include "../../inc/options_manager.hpp"
#include "../../inc/screen_selector.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <cstdint>
#include <map>
#include <memory>
#include <stop_token>
#include <thread>
namespace controllers {

class NewGameController {
private:
  Game::Game game;
  std::stop_source stopSrc;
  std::vector<std::vector<sf::RectangleShape>> boardTiles;
  std::vector<sf::RectangleShape> snakeBlocks;
  std::shared_ptr<sf::RenderWindow> windowPtr;
  tools::Drawer drawer;
  std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
      textureMapPtr;
  std::vector<sf::RectangleShape> candyBlocks;
  void createTiles();
  tools::Mutexes mutexes;
  std::unique_ptr<std::jthread> gameThread;
  void setHeadTexture(const Direction::Direction stepDirection);
  void setTailTexture();
  void setSegmentsTextures();
  bool gameStep(Direction::Direction &oStepDirection);
  void mainGameThread(std::stop_token stopToken,
                      tools::ScreenSelector &iSelector,
                      const tools::DatabaseManager &iDatabaseManager,
                      const tools::OptionsManager &iOptionsManager);
  void updateBlocksPositions();
  void updateCandy();
  void setSnakeTextures(const Direction::Direction stepDirection);
  std::pair<uint16_t, uint16_t> getTileSize() const;
  void handleKey(const sf::Keyboard::Key &keyCode);

public:
  NewGameController(
      uint8_t iGameWidth, uint8_t iGameHeight,
      std::shared_ptr<sf::RenderWindow> iWindowPtr,
      std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
          iTextureMap);
  Game::Game &getGame();
  const std::vector<std::vector<sf::RectangleShape>> &getTiles() const;

  std::vector<sf::RectangleShape> &getSnakeBlocks();
  std::vector<sf::RectangleShape> &getCandyBlocks();
  tools::Mutexes &getMutexes();
  void call(sf::RenderWindow &iWindow);
  void startGame(tools::ScreenSelector &iSelector,
                 tools::DatabaseManager &iDatabaseManager,
                 const tools::OptionsManager &iOptionsManager);
  void reset(uint8_t iGameWidth, uint8_t iGameHeight);
  void resize(const sf::Vector2u &iNewWindowSize);
};
} // namespace controllers
#endif
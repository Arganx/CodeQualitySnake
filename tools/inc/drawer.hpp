#ifndef DRAWER_GUARD
#define DRAWER_GUARD
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <memory>
#include <mutex>

namespace tools {

class Drawer {
  std::unique_ptr<sf::RenderWindow> window;

public:
  explicit Drawer(sf::RenderWindow &iWindow);
  void drawTiles(const std::vector<std::vector<sf::RectangleShape>> &iTiles);
  void drawBlocks(std::mutex &iMutex,
                  const std::vector<sf::RectangleShape> &iBlocks);
};
} // namespace tools
#endif
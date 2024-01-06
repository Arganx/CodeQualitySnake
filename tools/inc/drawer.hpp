#ifndef DRAWER_GUARD
#define DRAWER_GUARD
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <memory>
#include <mutex>

namespace tools {

class Drawer {
  std::shared_ptr<sf::RenderWindow> window;

public:
  explicit Drawer(std::shared_ptr<sf::RenderWindow> iWindow);
  void
  drawTiles(const std::vector<std::vector<sf::RectangleShape>> &iTiles) const;
  void drawBlocks(std::mutex &iMutex,
                  const std::vector<sf::RectangleShape> &iBlocks) const;
};
} // namespace tools
#endif
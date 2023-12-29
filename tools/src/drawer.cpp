#include "../inc/drawer.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

namespace tools {
Drawer::Drawer(sf::RenderWindow &iWindow) : window(&iWindow) {}

void Drawer::drawTiles(
    const std::vector<std::vector<sf::RectangleShape>> &iTiles) {
  for (const auto &column : iTiles) {
    for (const auto &tile : column) {
      window->draw(tile);
    }
  }
}

void Drawer::drawBlocks(std::mutex &iMutex,
                        const std::vector<sf::RectangleShape> &iBlocks) {
  std::scoped_lock lock(iMutex);
  for (const auto &block : iBlocks) {
    window->draw(block);
  }
}
} // namespace tools
#include "../inc/drawer.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

namespace tools {
Drawer::Drawer(std::shared_ptr<sf::RenderWindow> iWindow) : window(iWindow) {}

void Drawer::drawTiles(
    const std::vector<std::vector<sf::RectangleShape>> &iTiles) const {
  for (const auto &column : iTiles) {
    for (const auto &tile : column) {
      window->draw(tile);
    }
  }
}

void Drawer::drawBlocks(std::mutex &iMutex,
                        const std::vector<sf::RectangleShape> &iBlocks) const {
  std::scoped_lock lock(iMutex);
  for (const auto &block : iBlocks) {
    window->draw(block);
  }
}
} // namespace tools
#include "../inc/board.hpp"
#include <stdexcept>

constexpr char kCellInitValue = '*';

namespace Game {

uint8_t Board::getWidth() const { return width; };

uint8_t Board::getHeight() const { return height; };

void Board::setWidth(const uint8_t iWidth) {
  if (iWidth > 0) {
    width = iWidth;
  } else {
    throw std::invalid_argument("Width must be a positive number.");
  }
};

void Board::setHeight(const uint8_t iHeight) {
  if (iHeight > 0) {
    height = iHeight;
  } else {
    throw std::invalid_argument("Height must be a positive number.");
  }
};

void Board::createBoard() {
  boardSpace.clear();
  boardSpace.reserve(height);
  for (uint8_t heightIndex{0U}; heightIndex < height; ++heightIndex) {
    boardSpace.emplace_back(width, kCellInitValue);
  }
}

std::vector<std::vector<uint8_t>> Board::getBoard() const { return boardSpace; }

} // namespace Game
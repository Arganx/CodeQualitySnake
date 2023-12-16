#include "../inc/board.hpp"
#include "../inc/board_mappings.hpp"
#include "../inc/limited_uint8_t.hpp"
#include <cstdint>
#include <stdexcept>

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
    boardSpace.emplace_back(width, BoardMapping::kEmptySpace);
  }
}

std::vector<std::vector<uint8_t>> Board::getBoard() const { return boardSpace; }

void Board::checkIfBoardInitializedWithCorrectSize() const {
  if (boardSpace.size() != height || boardSpace.empty()) {
    throw std::invalid_argument(
        "Board height does match the expectation. Possibly not initialized");
  }
  if (boardSpace.front().size() != width || boardSpace.front().empty()) {
    throw std::invalid_argument(
        "Board width does match the expectation. Possibly not initialized");
  }
  return;
}

void Board::drawCharacter(const BoardPosition &iPosition, uint8_t iCharacter) {
  checkIfBoardInitializedWithCorrectSize();
  if (iPosition.getXPosition() >= width) {
    throw std::invalid_argument("Trying to draw outside of the board. X "
                                "position bigger than board width.");
  }
  if (iPosition.getYPosition() >= height) {
    throw std::invalid_argument("Trying to draw outside of the board. Y "
                                "position bigger than board height.");
  }
  boardSpace[iPosition.getYPosition().getValue()]
            [iPosition.getXPosition().getValue()] = iCharacter;
}

std::vector<BoardPosition> Board::getAvailablePositions() const {
  checkIfBoardInitializedWithCorrectSize();
  std::vector<BoardPosition> availablePositions;
  for (auto heightIndex{static_cast<uint8_t>(0U)}; heightIndex < height;
       ++heightIndex) {
    for (auto widthIndex{static_cast<uint8_t>(0U)}; widthIndex < width;
         ++widthIndex) {
      if (boardSpace[heightIndex][widthIndex] == BoardMapping::kEmptySpace) {
        availablePositions.emplace_back(Limited_uint8_t(widthIndex, width),
                                        Limited_uint8_t(heightIndex, height));
      }
    }
  }

  return availablePositions;
}

} // namespace Game
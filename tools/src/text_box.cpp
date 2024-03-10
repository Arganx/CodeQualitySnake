#include "../inc/text_box.hpp"
#include <SFML/Graphics/Color.hpp>
#include <cstdint>

namespace tools {

TextBox::TextBox(std::shared_ptr<sf::Font> iFontPtr) : fontPtr{iFontPtr} {
  setFillColor(sf::Color::White);
  setSize(sf::Vector2f(200, 50));
  setOutlineColor(sf::Color::Black);
  setOutlineThickness(2);
  text.setFont(*fontPtr);
  text.setFillColor(sf::Color::Black);
  text.setCharacterSize(24);
  setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);
}

void TextBox::setString(const std::string &iText) { text.setString(iText); }

std::string TextBox::getString() const { return text.getString(); }

void TextBox::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(static_cast<sf::RectangleShape>(*this), states);
  target.draw(text, states);
}

void TextBox::addCharacter(const uint32_t &iCharacter) {
  std::string nameText = getString();
  if (iCharacter == 8U && !nameText.empty()) // ASCII backspace character
  {
    nameText.pop_back();
  } else {
    nameText += static_cast<char>(iCharacter);
  }
  if (static_cast<uint8_t>(nameText[0]) == 8U) {
    nameText = nameText.substr(1, nameText.length());
  }
  setString(nameText);
  if (text.getGlobalBounds().left + text.getGlobalBounds().width >
      getGlobalBounds().left + getGlobalBounds().width) {
    nameText.pop_back();
  }
  setString(nameText);
  text.setPosition(getPosition());
  text.setOrigin(text.getLocalBounds().width / 2,
                 text.getLocalBounds().height / 2);
}

void TextBox::resizePlayerName(const sf::Vector2u &iNewWindowSize) {
  setSize(sf::Vector2f(static_cast<float>(iNewWindowSize.x * 0.3),
                       static_cast<float>(iNewWindowSize.y * 0.05)));
  setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);
  setPosition(static_cast<float>(iNewWindowSize.x * 0.6),
              static_cast<float>(iNewWindowSize.y * 0.05));
  if (auto newFontSize = getSize().y - 10; newFontSize > 0) {
    text.setCharacterSize(static_cast<uint32_t>(newFontSize));
  }
  text.setOrigin(text.getLocalBounds().width / 2,
                 text.getLocalBounds().height / 2);
  text.setPosition(getPosition());
}

void TextBox::resizeWidth(const sf::Vector2u &iNewWindowSize) {
  setSize(sf::Vector2f(static_cast<float>(iNewWindowSize.x * 0.1),
                       static_cast<float>(iNewWindowSize.y * 0.05)));
  setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);
  setPosition(static_cast<float>(iNewWindowSize.x * 0.5),
              static_cast<float>(iNewWindowSize.y * 0.12));
  if (auto newFontSize = getSize().y - 10; newFontSize > 0) {
    text.setCharacterSize(static_cast<uint32_t>(newFontSize));
  }
  text.setOrigin(text.getLocalBounds().width / 2,
                 text.getLocalBounds().height / 2);
  text.setPosition(getPosition());
}

void TextBox::resizeHeight(const sf::Vector2u &iNewWindowSize) {
  setSize(sf::Vector2f(static_cast<float>(iNewWindowSize.x * 0.1),
                       static_cast<float>(iNewWindowSize.y * 0.05)));
  setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);
  setPosition(static_cast<float>(iNewWindowSize.x * 0.7),
              static_cast<float>(iNewWindowSize.y * 0.12));
  if (auto newFontSize = getSize().y - 10; newFontSize > 0) {
    text.setCharacterSize(static_cast<uint32_t>(newFontSize));
  }
  text.setOrigin(text.getLocalBounds().width / 2,
                 text.getLocalBounds().height / 2);
  text.setPosition(getPosition());
}

} // namespace tools
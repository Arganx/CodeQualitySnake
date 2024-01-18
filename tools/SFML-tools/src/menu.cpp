#include "../inc/menu.hpp"
#include "../../inc/exceptions.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <cstdint>

namespace {
void calculateButtonPosition(const sf::Vector2u &iWindowSize,
                             float &oXButtonPosition, float &oYButtonPosition) {
  oXButtonPosition = static_cast<float>(iWindowSize.x) * 0.125F;
  oYButtonPosition = static_cast<float>(iWindowSize.y) *
                     0.125F; // 12,5% from the top is taken as empty space
}

void calculateYButtonSizeRatio(const uint8_t iNumberOfButtons,
                               float &oButtonSizeRatio) {
  oButtonSizeRatio =
      0.7F / (static_cast<float>(iNumberOfButtons * 2U) -
              1); // Default y size of a file as % of window height
}
} // namespace

namespace tools {

void Menu::createButton(const sf::Vector2u &iWindowSize,
                        const float iXButtonPosition, float &ioYButtonPosition,
                        const float iButtonYSize) {
  buttonsSprites.emplace_back();
  buttonsSprites.back().setTexture(buttonTexture);
  buttonsSprites.back().setOrigin(
      buttonsSprites.back().getLocalBounds().width / 2,
      buttonsSprites.back().getLocalBounds().height / 2);

  resizeButton(buttonsSprites.back(), iWindowSize, iXButtonPosition,
               ioYButtonPosition, iButtonYSize);
}
// TODO this should be done in main
void Menu::loadMenuAssets(const std::string &iButtonTexturePath,
                          const std::string &iBackgroundTexturePath,
                          const std::string &iButtonFontPath) {
  if (!buttonTexture.loadFromFile(iButtonTexturePath)) {
    throw TextureNotFoundException("Failed to load button texture");
  }
  if (!backgroundTexture.loadFromFile(iBackgroundTexturePath)) {
    throw TextureNotFoundException("Failed to load background texture");
  }
  if (!buttonFont.loadFromFile(iButtonFontPath)) {
    throw FontNotFoundException("Failed to load menu font");
  }
}

void Menu::resizeButton(sf::Sprite &ioButton, const sf::Vector2u &iWindowSize,
                        const float iXButtonPosition, float &ioYButtonPosition,
                        const float iButtonYSize) const {
  ioButton.setScale(static_cast<float>(iWindowSize.x) * 0.35F /
                        static_cast<float>(buttonTexture.getSize().x),
                    static_cast<float>(iWindowSize.y) * iButtonYSize * 1.3F /
                        static_cast<float>(buttonTexture.getSize().y));

  ioButton.setPosition(iXButtonPosition + ioButton.getGlobalBounds().width / 2,
                       ioYButtonPosition +
                           ioButton.getGlobalBounds().height / 2);
  ioYButtonPosition = ioYButtonPosition +
                      static_cast<float>(iWindowSize.y) * iButtonYSize * 2.0F;
}

void Menu::resizeBackground(const sf::Vector2u &iNewWindowSize) {
  backgroundSprite.setScale(
      static_cast<float>(iNewWindowSize.x) /
          static_cast<float>(backgroundTexture.getSize().x),
      static_cast<float>(iNewWindowSize.y) /
          static_cast<float>(backgroundTexture.getSize().y));
}

void Menu::resizeText(sf::Text &ioButtonText, const sf::Sprite &iButton) const {
  ioButtonText.setCharacterSize(static_cast<unsigned int>(
      0.3F * iButton.getGlobalBounds().height)); // TODO change the resizing
  ioButtonText.setOrigin(ioButtonText.getLocalBounds().width / 2,
                         ioButtonText.getLocalBounds().height / 2);
  ioButtonText.setPosition(iButton.getPosition());
}

void Menu::createTexts(const std::string &iLabel, const sf::Sprite &iButton,
                       uint8_t iFontSize) {
  buttonTexts.emplace_back(iLabel, buttonFont, iFontSize);
  buttonTexts.back().setFillColor(sf::Color::Black);
  resizeText(buttonTexts.back(), iButton);
}

Menu::Menu(const std::vector<std::string> &iButtonLabels,
           const std::string &iButtonTexturePath,
           const std::string &iBackgroundTexturePath,
           const std::string &iButtonFontPath,
           const sf::Vector2u &iWindowSize) {
  loadMenuAssets(iButtonTexturePath, iBackgroundTexturePath, iButtonFontPath);

  float xButtonPosition;
  float yButtonPosition;
  calculateButtonPosition(iWindowSize, xButtonPosition, yButtonPosition);

  float buttonYSize;
  calculateYButtonSizeRatio(static_cast<uint8_t>(iButtonLabels.size()),
                            buttonYSize);

  for (const auto &label : iButtonLabels) {
    createButton(iWindowSize, xButtonPosition, yButtonPosition, buttonYSize);
    createTexts(label, buttonsSprites.back());
  }
  backgroundSprite.setTexture(backgroundTexture);
  resizeBackground(iWindowSize);
};

void Menu::resize(const sf::Vector2u &iNewWindowSize) {
  float xButtonPosition;
  float yButtonPosition;
  calculateButtonPosition(iNewWindowSize, xButtonPosition, yButtonPosition);
  float buttonYSize;
  calculateYButtonSizeRatio(static_cast<uint8_t>(buttonsSprites.size()),
                            buttonYSize);

  resizeBackground(iNewWindowSize);

  for (auto &sprite : buttonsSprites) {
    resizeButton(sprite, iNewWindowSize, xButtonPosition, yButtonPosition,
                 buttonYSize);
  }
  for (uint8_t iterator{0U}; iterator < buttonsSprites.size(); ++iterator) {
    resizeText(buttonTexts[iterator], buttonsSprites[iterator]);
  }
}

void Menu::handleEvent(sf::RenderWindow &iWindow, const sf::Event &iEvent,
                       ScreenSelector &ioSelector) {
  if (iEvent.type == sf::Event::Closed) {
    iWindow.close();
  } else if (iEvent.type == sf::Event::MouseButtonPressed) {
    if (iEvent.mouseButton.button == sf::Mouse::Left && !mouseButtonPressed) {
      mouseButtonPressed = true;
      sf::Vector2i mousePos = sf::Mouse::getPosition(iWindow);
      if (buttonsSprites[0].getGlobalBounds().contains(
              static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
        buttonTexts[0].setFillColor(sf::Color::Red);
        pressedButton = Button::New_Game;
      } else if (buttonsSprites[3].getGlobalBounds().contains(
                     static_cast<float>(mousePos.x),
                     static_cast<float>(mousePos.y))) {
        buttonTexts[3].setFillColor(sf::Color::Red);
        pressedButton = Button::Exit;
      } else if (buttonsSprites[2].getGlobalBounds().contains(
                     static_cast<float>(mousePos.x),
                     static_cast<float>(mousePos.y))) {
        buttonTexts[2].setFillColor(sf::Color::Red);
        pressedButton = Button::Options;
      }
    }
  } else if (iEvent.type == sf::Event::MouseButtonReleased) {
    if (iEvent.mouseButton.button == sf::Mouse::Left) {
      sf::Vector2i mousePos = sf::Mouse::getPosition(iWindow);
      for (auto &buttonText : buttonTexts) {
        buttonText.setFillColor(sf::Color::Black);
      }
      mouseButtonPressed = false;
      if (pressedButton == Button::New_Game &&
          buttonsSprites[0].getGlobalBounds().contains(
              static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
        ioSelector.setSelectedOption(SelectorOptions::Game);
        ioSelector.setFirstPass(true);
      } else if (pressedButton == Button::Exit &&
                 buttonsSprites[3].getGlobalBounds().contains(
                     static_cast<float>(mousePos.x),
                     static_cast<float>(mousePos.y))) {
        iWindow.close();
      } else if (pressedButton == Button::Options &&
                 buttonsSprites[2].getGlobalBounds().contains(
                     static_cast<float>(mousePos.x),
                     static_cast<float>(mousePos.y))) {
        ioSelector.setSelectedOption(SelectorOptions::Options);
        ioSelector.setFirstPass(true);
      }
      pressedButton = Button::None;
    }
  } else if (iEvent.type == sf::Event::Resized) {
    sf::FloatRect visibleArea(0, 0, static_cast<float>(iEvent.size.width),
                              static_cast<float>(iEvent.size.height));
    iWindow.setView(sf::View(visibleArea));
    resize(iWindow.getSize());
  }
}

void Menu::call(sf::RenderWindow &iWindow, ScreenSelector &ioSelector) {
  sf::Event event;
  while (iWindow.pollEvent(event)) {
    handleEvent(iWindow, event, ioSelector);
  }
  iWindow.clear();
  iWindow.draw(backgroundSprite);
  for (const auto &button : buttonsSprites) {
    iWindow.draw(button);
  }
  for (const auto &buttonText : buttonTexts) {
    iWindow.draw(buttonText);
  }
  iWindow.display();
}

} // namespace tools
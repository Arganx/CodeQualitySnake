#include "../inc/menu_controller.hpp"
#include "../../inc/config/texture_config.hpp"
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

namespace controllers {

MenuController::MenuController(
    std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
        iTextureMap,
    const std::shared_ptr<sf::Font> &iFont,
    const std::vector<std::string> &iButtonLabels,
    const sf::Vector2u &iWindowSize)
    : textureMapPtr{iTextureMap}, fontPtr{iFont} {

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
  backgroundSprite.setTexture((*textureMapPtr)[config::kMenuBackgroundName]);
  resizeBackground(iWindowSize);
};

void MenuController::createButton(const sf::Vector2u &iWindowSize,
                                  const float iXButtonPosition,
                                  float &ioYButtonPosition,
                                  const float iButtonYSize) {
  buttonsSprites.emplace_back();
  buttonsSprites.back().setTexture(
      (*textureMapPtr)[config::kButtonTextureName]);
  buttonsSprites.back().setOrigin(
      buttonsSprites.back().getLocalBounds().width / 2,
      buttonsSprites.back().getLocalBounds().height / 2);

  resizeButton(buttonsSprites.back(), iWindowSize, iXButtonPosition,
               ioYButtonPosition, iButtonYSize);
}

void MenuController::resizeButton(sf::Sprite &ioButton,
                                  const sf::Vector2u &iWindowSize,
                                  const float iXButtonPosition,
                                  float &ioYButtonPosition,
                                  const float iButtonYSize) const {
  if (ioButton.getTexture() == nullptr) {
    throw tools::exceptions::TextureNotSetException(
        "Button texture not set in the menu");
  }
  ioButton.setScale(static_cast<float>(iWindowSize.x) * 0.35F /
                        static_cast<float>(ioButton.getTexture()->getSize().x),
                    static_cast<float>(iWindowSize.y) * iButtonYSize * 1.3F /
                        static_cast<float>(ioButton.getTexture()->getSize().y));

  ioButton.setPosition(iXButtonPosition + ioButton.getGlobalBounds().width / 2,
                       ioYButtonPosition +
                           ioButton.getGlobalBounds().height / 2);
  ioYButtonPosition = ioYButtonPosition +
                      static_cast<float>(iWindowSize.y) * iButtonYSize * 2.0F;
}

void MenuController::resizeBackground(const sf::Vector2u &iNewWindowSize) {
  if (backgroundSprite.getTexture() == nullptr) {
    throw tools::exceptions::TextureNotSetException(
        "Background texture not set in the menu");
  }
  backgroundSprite.setScale(
      static_cast<float>(iNewWindowSize.x) /
          static_cast<float>(backgroundSprite.getTexture()->getSize().x),
      static_cast<float>(iNewWindowSize.y) /
          static_cast<float>(backgroundSprite.getTexture()->getSize().y));
}

void MenuController::resizeText(sf::Text &ioButtonText,
                                const sf::Sprite &iButton) const {
  ioButtonText.setCharacterSize(
      static_cast<unsigned int>(0.3F * iButton.getGlobalBounds().height));
  ioButtonText.setOrigin(ioButtonText.getLocalBounds().width / 2,
                         ioButtonText.getLocalBounds().height / 2);
  ioButtonText.setPosition(iButton.getPosition());
}

void MenuController::createTexts(const std::string &iLabel,
                                 const sf::Sprite &iButton, uint8_t iFontSize) {
  buttonTexts.emplace_back(iLabel, *fontPtr, iFontSize);
  buttonTexts.back().setFillColor(sf::Color::Black);
  resizeText(buttonTexts.back(), iButton);
}

void MenuController::resize(const sf::Vector2u &iNewWindowSize) {
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

void MenuController::handleEvent(sf::RenderWindow &iWindow,
                                 const sf::Event &iEvent,
                                 tools::ScreenSelector &ioSelector) {
  if (iEvent.type == sf::Event::Closed) {
    iWindow.close();
  } else if (iEvent.type == sf::Event::MouseButtonPressed) {
    if (iEvent.mouseButton.button == sf::Mouse::Left && !mouseButtonPressed) {
      handleMouseButtonPressed(iWindow);
    }
  } else if (iEvent.type == sf::Event::MouseButtonReleased) {
    if (iEvent.mouseButton.button == sf::Mouse::Left) {
      handleMouseButtonReleased(iWindow, ioSelector);
    }
  } else if (iEvent.type == sf::Event::Resized) {
    sf::FloatRect visibleArea(0, 0, static_cast<float>(iEvent.size.width),
                              static_cast<float>(iEvent.size.height));
    iWindow.setView(sf::View(visibleArea));
    resize(iWindow.getSize());
  }
}

void MenuController::handleMouseButtonPressed(const sf::RenderWindow &iWindow) {
  using enum controllers::Button;
  mouseButtonPressed = true;
  sf::Vector2i mousePos = sf::Mouse::getPosition(iWindow);
  if (buttonsSprites[0].getGlobalBounds().contains(
          static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
    buttonTexts[0].setFillColor(sf::Color::Red);
    pressedButton = New_Game;
  } else if (buttonsSprites[1].getGlobalBounds().contains(
                 static_cast<float>(mousePos.x),
                 static_cast<float>(mousePos.y))) {
    buttonTexts[1].setFillColor(sf::Color::Red);
    pressedButton = High_Scores;
  } else if (buttonsSprites[3].getGlobalBounds().contains(
                 static_cast<float>(mousePos.x),
                 static_cast<float>(mousePos.y))) {
    buttonTexts[3].setFillColor(sf::Color::Red);
    pressedButton = Exit;
  } else if (buttonsSprites[2].getGlobalBounds().contains(
                 static_cast<float>(mousePos.x),
                 static_cast<float>(mousePos.y))) {
    buttonTexts[2].setFillColor(sf::Color::Red);
    pressedButton = Options;
  }
}

void MenuController::handleMouseButtonReleased(
    sf::RenderWindow &iWindow, tools::ScreenSelector &ioSelector) {
  sf::Vector2i mousePos = sf::Mouse::getPosition(iWindow);
  for (auto &buttonText : buttonTexts) {
    buttonText.setFillColor(sf::Color::Black);
  }
  mouseButtonPressed = false;
  if (pressedButton == Button::New_Game &&
      buttonsSprites[0].getGlobalBounds().contains(
          static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
    ioSelector.setSelectedOption(tools::SelectorOptions::Game);
    ioSelector.setFirstPass(true);
  } else if (pressedButton == Button::High_Scores &&
             buttonsSprites[1].getGlobalBounds().contains(
                 static_cast<float>(mousePos.x),
                 static_cast<float>(mousePos.y))) {
    ioSelector.setSelectedOption(tools::SelectorOptions::HighScores);
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
    ioSelector.setSelectedOption(tools::SelectorOptions::Options);
    ioSelector.setFirstPass(true);
  }
  pressedButton = Button::None;
}

void MenuController::call(sf::RenderWindow &iWindow,
                          tools::ScreenSelector &ioSelector) {
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

} // namespace controllers
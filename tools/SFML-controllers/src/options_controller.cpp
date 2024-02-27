#include "../inc/options_controller.hpp"
#include "../../inc/config/texture_config.hpp"
#include "../../inc/exceptions.hpp"
#include <SFML/Window/Event.hpp>
#include <cstdint>
#include <iostream>

namespace controllers {
OptionController::OptionController(
    const sf::Vector2u &iWindowSize,
    std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
        iTextureMap,
    const std::shared_ptr<sf::Font> &iFont)
    : textureMapPtr{iTextureMap}, fontPtr{iFont},
      okText{"Accept", *fontPtr, 17}, cancelText{"Cancel", *fontPtr, 17},
      nameBox{fontPtr} {
  // Setup background object
  if (!textureMapPtr->contains(config::kOptionsBackgroundName)) {
    throw tools::exceptions::TextureNotFoundException(
        "Options background texture not found");
  }
  backgroundObject.setTexture((*textureMapPtr)[config::kOptionsBackgroundName]);

  // Setup buttons
  if (!textureMapPtr->contains(config::kButtonTextureName)) {
    throw tools::exceptions::TextureNotFoundException(
        "Button texture not found");
  }
  okButtonSprite.setTexture((*textureMapPtr)[config::kButtonTextureName]);
  cancelButtonSprite.setTexture((*textureMapPtr)[config::kButtonTextureName]);
  okButtonSprite.setOrigin(okButtonSprite.getLocalBounds().width / 2,
                           okButtonSprite.getLocalBounds().height / 2);
  cancelButtonSprite.setOrigin(cancelButtonSprite.getLocalBounds().width / 2,
                               cancelButtonSprite.getLocalBounds().height / 2);

  // Setup texts
  okText.setFillColor(sf::Color::Black);
  cancelText.setFillColor(sf::Color::Black);

  resize(iWindowSize);
}

void OptionController::resizeOkText() {
  okText.setCharacterSize(static_cast<unsigned int>(
      0.35F * okButtonSprite.getGlobalBounds().height));
  okText.setOrigin(okText.getLocalBounds().width / 2,
                   okText.getLocalBounds().height / 2);
  okText.setPosition(okButtonSprite.getPosition());
}

void OptionController::resizeCancelText() {
  cancelText.setCharacterSize(static_cast<unsigned int>(
      0.35F * cancelButtonSprite.getGlobalBounds().height));
  cancelText.setOrigin(cancelText.getLocalBounds().width / 2,
                       cancelText.getLocalBounds().height / 2);
  cancelText.setPosition(cancelButtonSprite.getPosition());
}

void OptionController::resizeBackground(const sf::Vector2u &iNewWindowSize) {
  if (backgroundObject.getTexture() == nullptr) {
    throw tools::exceptions::TextureNotSetException(
        "Background texture not assigned");
  }
  backgroundObject.setScale(
      static_cast<float>(iNewWindowSize.x) /
          static_cast<float>(backgroundObject.getTexture()->getSize().x),
      static_cast<float>(iNewWindowSize.y) /
          static_cast<float>(backgroundObject.getTexture()->getSize().y));
}
void OptionController::resizeOkButton(const sf::Vector2u &iNewWindowSize) {
  if (okButtonSprite.getTexture() == nullptr) {
    throw tools::exceptions::TextureNotSetException(
        "Ok button texture not assigned");
  }
  okButtonSprite.setScale(
      static_cast<float>(iNewWindowSize.x) * 0.2F /
          static_cast<float>(okButtonSprite.getTexture()->getSize().x),
      static_cast<float>(iNewWindowSize.y) * 0.1F /
          static_cast<float>(okButtonSprite.getTexture()->getSize().y));
  okButtonSprite.setPosition(static_cast<float>(iNewWindowSize.x) * 0.75F,
                             static_cast<float>(iNewWindowSize.y) * 0.9F);
}

void OptionController::resizeCancelButton(const sf::Vector2u &iNewWindowSize) {
  if (cancelButtonSprite.getTexture() == nullptr) {
    throw tools::exceptions::TextureNotSetException(
        "Cancel button texture not assigned");
  }
  cancelButtonSprite.setScale(
      static_cast<float>(iNewWindowSize.x) * 0.2F /
          static_cast<float>(cancelButtonSprite.getTexture()->getSize().x),
      static_cast<float>(iNewWindowSize.y) * 0.1F /
          static_cast<float>(cancelButtonSprite.getTexture()->getSize().y));
  cancelButtonSprite.setPosition(static_cast<float>(iNewWindowSize.x) * 0.25F,
                                 static_cast<float>(iNewWindowSize.y) * 0.9F);
}

void OptionController::resize(const sf::Vector2u &iNewWindowSize) {
  resizeBackground(iNewWindowSize);
  resizeOkButton(iNewWindowSize);
  resizeCancelButton(iNewWindowSize);
  resizeOkText();
  resizeCancelText();
  nameBox.resize(iNewWindowSize);
}

void OptionController::handleCancel(tools::ScreenSelector &ioSelector) const {
  ioSelector.setSelectedOption(tools::SelectorOptions::MainMenu);
  ioSelector.setFirstPass(true);
}
void OptionController::handleOk(tools::ScreenSelector &ioSelector) const {
  ioSelector.setSelectedOption(tools::SelectorOptions::MainMenu);
  ioSelector.setFirstPass(true);
}

void OptionController::handleEvent(sf::RenderWindow &iWindow,
                                   const sf::Event &iEvent,
                                   tools::ScreenSelector &ioSelector) {
  if (iEvent.type == sf::Event::Closed) {
    iWindow.close();
  } else if (iEvent.type == sf::Event::MouseButtonPressed) {
    if (iEvent.mouseButton.button == sf::Mouse::Left && !mouseButtonPressed) {
      mouseButtonPressed = true;
      sf::Vector2i mousePos = sf::Mouse::getPosition(iWindow);
      if (cancelButtonSprite.getGlobalBounds().contains(
              static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {

        cancelText.setFillColor(sf::Color::Red);
        pressedButton = OptionsButton::Cancel;
      } else if (okButtonSprite.getGlobalBounds().contains(
                     static_cast<float>(mousePos.x),
                     static_cast<float>(mousePos.y))) {

        okText.setFillColor(sf::Color::Red);
        pressedButton = OptionsButton::Ok;
      } else if (nameBox.getGlobalBounds().contains(
                     static_cast<float>(mousePos.x),
                     static_cast<float>(mousePos.y))) {
        selectedTextBox = static_cast<uint8_t>(1U);
      }
    }
  } else if (iEvent.type == sf::Event::MouseButtonReleased) {
    if (iEvent.mouseButton.button == sf::Mouse::Left) {
      sf::Vector2i mousePos = sf::Mouse::getPosition(iWindow);
      okText.setFillColor(sf::Color::Black);
      cancelText.setFillColor(sf::Color::Black);
      mouseButtonPressed = false;
      if (cancelButtonSprite.getGlobalBounds().contains(
              static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
        handleCancel(ioSelector);
      } else if (okButtonSprite.getGlobalBounds().contains(
                     static_cast<float>(mousePos.x),
                     static_cast<float>(mousePos.y))) {
        handleOk(ioSelector);
      }
    }
  } else if (iEvent.type == sf::Event::TextEntered) {
    if (selectedTextBox == 1U) // name box
    {
      nameBox.addCharacter(iEvent.text.unicode);
    }
  } else if (iEvent.type == sf::Event::Resized) {
    sf::FloatRect visibleArea(0, 0, static_cast<float>(iEvent.size.width),
                              static_cast<float>(iEvent.size.height));
    iWindow.setView(sf::View(visibleArea));
    resize(iWindow.getSize());
  }
}

void OptionController::call(sf::RenderWindow &iWindow,
                            tools::ScreenSelector &ioSelector) {
  sf::Event event;
  while (iWindow.pollEvent(event)) {
    handleEvent(iWindow, event, ioSelector);
  }
  iWindow.clear();
  iWindow.draw(backgroundObject);
  iWindow.draw(nameBox);
  iWindow.draw(okButtonSprite);
  iWindow.draw(cancelButtonSprite);
  iWindow.draw(okText);
  iWindow.draw(cancelText);
  iWindow.display();
}
} // namespace controllers
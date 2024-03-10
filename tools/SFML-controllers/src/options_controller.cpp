#include "../inc/options_controller.hpp"
#include "../../inc/config/texture_config.hpp"
#include "../../inc/exceptions.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <cstdint>
#include <string>

namespace controllers {
OptionController::OptionController(
    const sf::Vector2u &iWindowSize,
    std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
        iTextureMap,
    const std::shared_ptr<sf::Font> &iFont)
    : textureMapPtr{iTextureMap}, fontPtr{iFont},
      okText{"Accept", *fontPtr, 17}, cancelText{"Cancel", *fontPtr, 17},
      nameBox{fontPtr}, widthBox(fontPtr), heightBox(fontPtr),
      playerNameText{"Player name:", *fontPtr, 17},
      boardDimensionsText{"Board (X, Y):", *fontPtr, 17} {
  // Setup background object
  if (!textureMapPtr->contains(config::kOptionsBackgroundName)) {
    throw tools::exceptions::TextureNotFoundException(
        "Options background texture not found");
  }
  backgroundObject.setTexture((*textureMapPtr)[config::kOptionsBackgroundName]);

  // Setup background board
  if (!textureMapPtr->contains(config::kOptionBoard)) {
    throw tools::exceptions::TextureNotFoundException(
        "Texture for options background board not found");
  }
  backgroundBoard.setTexture((*textureMapPtr)[config::kOptionBoard]);
  // Set transparency
  sf::Color color = backgroundBoard.getColor();
  color.a = static_cast<uint8_t>(255.0F * 0.8F);
  backgroundBoard.setColor(color);

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

  backgroundBoard.setOrigin(backgroundBoard.getLocalBounds().width / 2,
                            backgroundBoard.getLocalBounds().height / 2);

  // Setup texts
  okText.setFillColor(sf::Color::Black);
  cancelText.setFillColor(sf::Color::Black);
  playerNameText.setFillColor(sf::Color::Black);
  boardDimensionsText.setFillColor(sf::Color::Black);

  resize(iWindowSize);
}

void OptionController::resizePlayerNameText(
    const sf::Vector2u &iNewWindowSize) {
  playerNameText.setCharacterSize(static_cast<unsigned int>(
      0.35F * okButtonSprite.getGlobalBounds().height));
  playerNameText.setOrigin(playerNameText.getLocalBounds().width / 2,
                           playerNameText.getLocalBounds().height / 2);
  playerNameText.setPosition(static_cast<float>(iNewWindowSize.x * 0.3),
                             static_cast<float>(iNewWindowSize.y * 0.05));
}

void OptionController::resizeBoardDimensionsText(
    const sf::Vector2u &iNewWindowSize) {
  boardDimensionsText.setCharacterSize(static_cast<unsigned int>(
      0.35F * okButtonSprite.getGlobalBounds().height));
  boardDimensionsText.setOrigin(boardDimensionsText.getLocalBounds().width / 2,
                                boardDimensionsText.getLocalBounds().height /
                                    2);
  boardDimensionsText.setPosition(static_cast<float>(iNewWindowSize.x * 0.3),
                                  static_cast<float>(iNewWindowSize.y * 0.12));
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

void OptionController::resizeBackgroundBoard(
    const sf::Vector2u &iNewWindowSize) {
  backgroundBoard.setPosition(static_cast<float>(iNewWindowSize.x) / 2,
                              static_cast<float>(iNewWindowSize.y) * 0.4F);
  backgroundBoard.setScale(
      static_cast<float>(iNewWindowSize.x) * 0.9F /
          static_cast<float>(backgroundBoard.getTexture()->getSize().x),
      static_cast<float>(iNewWindowSize.y) * 0.8F /
          static_cast<float>(backgroundBoard.getTexture()->getSize().y));
}

void OptionController::resize(const sf::Vector2u &iNewWindowSize) {
  resizeBackground(iNewWindowSize);
  resizeBackgroundBoard(iNewWindowSize);
  resizeOkButton(iNewWindowSize);
  resizeCancelButton(iNewWindowSize);
  resizeOkText();
  resizeCancelText();
  nameBox.resizePlayerName(iNewWindowSize);
  widthBox.resizeWidth(iNewWindowSize);
  heightBox.resizeHeight(iNewWindowSize);
  resizePlayerNameText(iNewWindowSize);
  resizeBoardDimensionsText(iNewWindowSize);
}

void OptionController::handleCancel(tools::ScreenSelector &ioSelector) const {
  ioSelector.setSelectedOption(tools::SelectorOptions::MainMenu);
  ioSelector.setFirstPass(true);
}
void OptionController::handleOk(tools::ScreenSelector &ioSelector,
                                tools::OptionsManager &iOptionsManager) const {
  ioSelector.setSelectedOption(tools::SelectorOptions::MainMenu);
  ioSelector.setFirstPass(true);
  iOptionsManager.setPlayerName(nameBox.getString());
  iOptionsManager.setBoardWidth(
      static_cast<uint8_t>(std::stoi(widthBox.getString())));
  iOptionsManager.setBoardHeight(
      static_cast<uint8_t>(std::stoi(heightBox.getString())));
}

void OptionController::handleEvent(sf::RenderWindow &iWindow,
                                   const sf::Event &iEvent,
                                   tools::ScreenSelector &ioSelector,
                                   tools::OptionsManager &iOptionsManager) {
  if (iEvent.type == sf::Event::Closed) {
    iWindow.close();
  } else if (iEvent.type == sf::Event::MouseButtonPressed) {
    if (iEvent.mouseButton.button == sf::Mouse::Left && !mouseButtonPressed) {
      mouseButtonPressed = true;
      selectedTextBox = static_cast<uint8_t>(0U);
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
      } else if (widthBox.getGlobalBounds().contains(
                     static_cast<float>(mousePos.x),
                     static_cast<float>(mousePos.y))) {
        selectedTextBox = static_cast<uint8_t>(2U);
      } else if (heightBox.getGlobalBounds().contains(
                     static_cast<float>(mousePos.x),
                     static_cast<float>(mousePos.y))) {
        selectedTextBox = static_cast<uint8_t>(3U);
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
        handleOk(ioSelector, iOptionsManager);
      }
    }
  } else if (iEvent.type == sf::Event::TextEntered) {
    if (selectedTextBox == 1U) // name box
    {
      nameBox.addCharacter(iEvent.text.unicode);
    } else if (selectedTextBox == 2U &&
               ((47U < iEvent.text.unicode && iEvent.text.unicode < 58U) ||
                iEvent.text.unicode ==
                    8)) // board width ASCII for 0 and 9 and backspace
    {
      widthBox.addCharacter(iEvent.text.unicode);
    } else if (selectedTextBox == 3U &&
               ((47U < iEvent.text.unicode && iEvent.text.unicode < 58U) ||
                iEvent.text.unicode ==
                    8)) // board height ASCII for 0 and 9 and backspace
    {
      heightBox.addCharacter(iEvent.text.unicode);
    }
  } else if (iEvent.type == sf::Event::Resized) {
    sf::FloatRect visibleArea(0, 0, static_cast<float>(iEvent.size.width),
                              static_cast<float>(iEvent.size.height));
    iWindow.setView(sf::View(visibleArea));
    resize(iWindow.getSize());
  }
}

void OptionController::call(sf::RenderWindow &iWindow,
                            tools::ScreenSelector &ioSelector,
                            tools::OptionsManager &iOptionsManager) {
  sf::Event event;
  while (iWindow.pollEvent(event)) {
    handleEvent(iWindow, event, ioSelector, iOptionsManager);
  }
  iWindow.clear();
  iWindow.draw(backgroundObject);
  iWindow.draw(backgroundBoard);
  iWindow.draw(nameBox);
  iWindow.draw(widthBox);
  iWindow.draw(heightBox);
  iWindow.draw(okButtonSprite);
  iWindow.draw(cancelButtonSprite);
  iWindow.draw(okText);
  iWindow.draw(cancelText);
  iWindow.draw(playerNameText);
  iWindow.draw(boardDimensionsText);
  iWindow.display();
}

void OptionController::refreshValues(
    const tools::OptionsManager &iOptionsManager) {
  nameBox.setString(iOptionsManager.getPlayerName());
  widthBox.setString(std::to_string(iOptionsManager.getBoardWidth()));
  heightBox.setString(std::to_string(iOptionsManager.getBoardHeight()));
}
} // namespace controllers
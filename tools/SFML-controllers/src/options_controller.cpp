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
      boardDimensionsText{"Board (X, Y):", *fontPtr, 17},
      candyText{"Candy: ", *fontPtr, 17},
      snakeColorText{"Snake color:", *fontPtr, 17} {
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

  if (!textureMapPtr->contains(config::kArrowLeft)) {
    throw tools::exceptions::TextureNotFoundException(
        "Arrow left texture not found");
  }
  candyLeft.setTexture((*textureMapPtr)[config::kArrowLeft]);
  candyLeft.setOrigin(candyLeft.getLocalBounds().width / 2,
                      candyLeft.getLocalBounds().height / 2);

  snakeColorLeft.setTexture((*textureMapPtr)[config::kArrowLeft]);
  snakeColorLeft.setOrigin(snakeColorLeft.getLocalBounds().width / 2,
                           snakeColorLeft.getLocalBounds().height / 2);

  if (!textureMapPtr->contains(config::kArrowRight)) {
    throw tools::exceptions::TextureNotFoundException(
        "Arrow right texture not found");
  }
  candyRight.setTexture((*textureMapPtr)[config::kArrowRight]);
  candyRight.setOrigin(candyRight.getLocalBounds().width / 2,
                       candyRight.getLocalBounds().height / 2);

  snakeColorRight.setTexture((*textureMapPtr)[config::kArrowRight]);
  snakeColorRight.setOrigin(snakeColorRight.getLocalBounds().width / 2,
                            snakeColorRight.getLocalBounds().height / 2);

  backgroundBoard.setOrigin(backgroundBoard.getLocalBounds().width / 2,
                            backgroundBoard.getLocalBounds().height / 2);

  // Setup texts
  okText.setFillColor(sf::Color::Black);
  cancelText.setFillColor(sf::Color::Black);
  playerNameText.setFillColor(sf::Color::Black);
  boardDimensionsText.setFillColor(sf::Color::Black);
  candyText.setFillColor(sf::Color::Black);
  snakeColorText.setFillColor(sf::Color::Black);

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

void OptionController::resizeCandyLeft(const sf::Vector2u &iNewWindowSize) {
  candyLeft.setScale(
      static_cast<float>(iNewWindowSize.x) * 0.05F /
          static_cast<float>(candyLeft.getTexture()->getSize().x),
      static_cast<float>(iNewWindowSize.y) * 0.1F /
          static_cast<float>(candyLeft.getTexture()->getSize().y));
  candyLeft.setPosition(widthBox.getPosition().x,
                        widthBox.getPosition().y +
                            static_cast<float>(iNewWindowSize.y) * 0.1F);
}

void OptionController::resizeCandyRight(const sf::Vector2u &iNewWindowSize) {
  candyRight.setScale(
      static_cast<float>(iNewWindowSize.x) * 0.05F /
          static_cast<float>(candyRight.getTexture()->getSize().x),
      static_cast<float>(iNewWindowSize.y) * 0.1F /
          static_cast<float>(candyRight.getTexture()->getSize().y));
  candyRight.setPosition(heightBox.getPosition().x,
                         heightBox.getPosition().y +
                             static_cast<float>(iNewWindowSize.y) * 0.1F);
}

void OptionController::resizeCandyText(const sf::Vector2u &iNewWindowSize) {
  candyText.setCharacterSize(static_cast<unsigned int>(
      0.35F * cancelButtonSprite.getGlobalBounds().height));
  candyText.setOrigin(candyText.getLocalBounds().width / 2,
                      candyText.getLocalBounds().height / 2);
  candyText.setPosition(static_cast<float>(iNewWindowSize.x * 0.3),
                        static_cast<float>(iNewWindowSize.y * 0.21));
}

void OptionController::resizeCandyExample(const sf::Vector2u &iNewWindowSize) {
  if (candyExample.getTexture() != nullptr) {
    candyExample.setScale(
        static_cast<float>(iNewWindowSize.x) * 0.05F /
            static_cast<float>(candyExample.getTexture()->getSize().x),
        static_cast<float>(iNewWindowSize.y) * 0.1F /
            static_cast<float>(candyExample.getTexture()->getSize().y));
    candyExample.setPosition(static_cast<float>(iNewWindowSize.x * 0.57),
                             static_cast<float>(iNewWindowSize.y * 0.18));
  }
}

void OptionController::resizeSnakeColorLeft(
    const sf::Vector2u &iNewWindowSize) {
  snakeColorLeft.setScale(
      static_cast<float>(iNewWindowSize.x) * 0.05F /
          static_cast<float>(snakeColorLeft.getTexture()->getSize().x),
      static_cast<float>(iNewWindowSize.y) * 0.1F /
          static_cast<float>(snakeColorLeft.getTexture()->getSize().y));
  snakeColorLeft.setPosition(widthBox.getPosition().x,
                             widthBox.getPosition().y +
                                 static_cast<float>(iNewWindowSize.y) * 0.22F);
}
void OptionController::resizeSnakeColorRight(
    const sf::Vector2u &iNewWindowSize) {
  snakeColorRight.setScale(
      static_cast<float>(iNewWindowSize.x) * 0.05F /
          static_cast<float>(snakeColorRight.getTexture()->getSize().x),
      static_cast<float>(iNewWindowSize.y) * 0.1F /
          static_cast<float>(snakeColorRight.getTexture()->getSize().y));
  snakeColorRight.setPosition(heightBox.getPosition().x,
                              heightBox.getPosition().y +
                                  static_cast<float>(iNewWindowSize.y) * 0.22F);
}

void OptionController::resizeSnakeColorText(
    const sf::Vector2u &iNewWindowSize) {
  snakeColorText.setCharacterSize(static_cast<unsigned int>(
      0.35F * cancelButtonSprite.getGlobalBounds().height));
  snakeColorText.setOrigin(snakeColorText.getLocalBounds().width / 2,
                           snakeColorText.getLocalBounds().height / 2);
  snakeColorText.setPosition(static_cast<float>(iNewWindowSize.x * 0.3),
                             static_cast<float>(iNewWindowSize.y * 0.33));
}

void OptionController::resizeSnakeColorExample(
    const sf::Vector2u &iNewWindowSize) {
  if (snakeColorExample.getTexture() != nullptr) {
    snakeColorExample.setScale(
        static_cast<float>(iNewWindowSize.x) * 0.05F /
            static_cast<float>(snakeColorExample.getTexture()->getSize().x),
        static_cast<float>(iNewWindowSize.y) * 0.1F /
            static_cast<float>(snakeColorExample.getTexture()->getSize().y));
    snakeColorExample.setPosition(static_cast<float>(iNewWindowSize.x * 0.57),
                                  static_cast<float>(iNewWindowSize.y * 0.29));
  }
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
  resizeCandyText(iNewWindowSize);
  resizeCandyLeft(iNewWindowSize);
  resizeCandyRight(iNewWindowSize);
  resizeCandyExample(iNewWindowSize);
  resizeSnakeColorText(iNewWindowSize);
  resizeSnakeColorLeft(iNewWindowSize);
  resizeSnakeColorRight(iNewWindowSize);
  resizeSnakeColorExample(iNewWindowSize);
}

void OptionController::handleCancel(
    tools::ScreenSelector &ioSelector,
    tools::OptionsManager &iOptionsManager) const {
  iOptionsManager.revertCandy();
  iOptionsManager.revertSnakeColor();
  ioSelector.setSelectedOption(tools::SelectorOptions::MainMenu);
  ioSelector.setFirstPass(true);
}
void OptionController::handleOk(tools::ScreenSelector &ioSelector,
                                tools::OptionsManager &iOptionsManager) const {
  ioSelector.setSelectedOption(tools::SelectorOptions::MainMenu);
  ioSelector.setFirstPass(true);
  iOptionsManager.setPlayerName(nameBox.getString());
  auto newWidth{std::stoi(widthBox.getString())};
  if (newWidth > 255) {
    newWidth = 255;
  } else if (newWidth < 3) {
    newWidth = 3U;
  }
  auto newHeight{std::stoi(heightBox.getString())};
  if (newHeight > 255) {
    newHeight = 255;
  } else if (newHeight < 3) {
    newHeight = 3U;
  }
  iOptionsManager.setBoardWidth(static_cast<uint8_t>(newWidth));
  iOptionsManager.setBoardHeight(static_cast<uint8_t>(newHeight));
  iOptionsManager.acceptNewCandy();
  iOptionsManager.acceptNewSnakeColor();
  iOptionsManager.fillTheFile();
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
      } else if (candyLeft.getGlobalBounds().contains(
                     static_cast<float>(mousePos.x),
                     static_cast<float>(mousePos.y))) {
        pressedButton = OptionsButton::PreviousCandy;

      } else if (candyRight.getGlobalBounds().contains(
                     static_cast<float>(mousePos.x),
                     static_cast<float>(mousePos.y))) {
        pressedButton = OptionsButton::NextCandy;
      } else if (snakeColorLeft.getGlobalBounds().contains(
                     static_cast<float>(mousePos.x),
                     static_cast<float>(mousePos.y))) {
        pressedButton = OptionsButton::PreviousSnakeColor;
      } else if (snakeColorRight.getGlobalBounds().contains(
                     static_cast<float>(mousePos.x),
                     static_cast<float>(mousePos.y))) {
        pressedButton = OptionsButton::NextSnakeColor;
      }
    }
  } else if (iEvent.type == sf::Event::MouseButtonReleased) {
    if (iEvent.mouseButton.button == sf::Mouse::Left) {
      sf::Vector2i mousePos = sf::Mouse::getPosition(iWindow);
      okText.setFillColor(sf::Color::Black);
      cancelText.setFillColor(sf::Color::Black);
      mouseButtonPressed = false;
      if (pressedButton == OptionsButton::Cancel &&
          cancelButtonSprite.getGlobalBounds().contains(
              static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
        handleCancel(ioSelector, iOptionsManager);
      } else if (pressedButton == OptionsButton::Ok &&
                 okButtonSprite.getGlobalBounds().contains(
                     static_cast<float>(mousePos.x),
                     static_cast<float>(mousePos.y))) {
        handleOk(ioSelector, iOptionsManager);
      } else if (pressedButton == OptionsButton::PreviousCandy &&
                 candyLeft.getGlobalBounds().contains(
                     static_cast<float>(mousePos.x),
                     static_cast<float>(mousePos.y))) {
        iOptionsManager.previousCandy();
        refreshValues(iOptionsManager);
        resizeCandyExample(iWindow.getSize());
      } else if (pressedButton == OptionsButton::NextCandy &&
                 candyRight.getGlobalBounds().contains(
                     static_cast<float>(mousePos.x),
                     static_cast<float>(mousePos.y))) {
        iOptionsManager.nextCandy();
        refreshValues(iOptionsManager);
        resizeCandyExample(iWindow.getSize());
      } else if (pressedButton == OptionsButton::PreviousSnakeColor &&
                 snakeColorLeft.getGlobalBounds().contains(
                     static_cast<float>(mousePos.x),
                     static_cast<float>(mousePos.y))) {
        iOptionsManager.previousSnakeColor();
        snakeColorExample.setTexture(
            (*textureMapPtr)[iOptionsManager.getSnakeColor()]);
        resizeSnakeColorExample(iWindow.getSize());
      } else if (pressedButton == OptionsButton::NextSnakeColor &&
                 snakeColorRight.getGlobalBounds().contains(
                     static_cast<float>(mousePos.x),
                     static_cast<float>(mousePos.y))) {
        iOptionsManager.nextSnakeColor();
        snakeColorExample.setTexture(
            (*textureMapPtr)[iOptionsManager.getSnakeColor()]);
        resizeSnakeColorExample(iWindow.getSize());
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
  iWindow.draw(candyText);
  iWindow.draw(candyLeft);
  iWindow.draw(candyRight);
  iWindow.draw(candyExample);
  iWindow.draw(snakeColorText);
  iWindow.draw(snakeColorLeft);
  iWindow.draw(snakeColorRight);
  iWindow.draw(snakeColorExample);
  iWindow.display();
}

void OptionController::refreshValues(
    const tools::OptionsManager &iOptionsManager) {
  nameBox.setString(iOptionsManager.getPlayerName());
  widthBox.setString(std::to_string(iOptionsManager.getBoardWidth()));
  heightBox.setString(std::to_string(iOptionsManager.getBoardHeight()));
  if (!textureMapPtr->contains(iOptionsManager.getCandy())) {
    throw tools::exceptions::TextureNotFoundException(
        "Candy texture not found");
  }
  candyExample.setTexture((*textureMapPtr)[iOptionsManager.getCandy()]);
  snakeColorExample.setTexture(
      (*textureMapPtr)[iOptionsManager.getSnakeColor()]);
}
} // namespace controllers
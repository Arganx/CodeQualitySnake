#include "../inc/high_score_controller.hpp"
#include "../../inc/config/texture_config.hpp"
#include "../../inc/config/variable_config.hpp"
#include "../../inc/exceptions.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <cstdint>
#include <format>
#include <string>

namespace {

void setScoreTextsOrigin(sf::Text &iText) {
  iText.setOrigin(iText.getLocalBounds().width / 2,
                  iText.getLocalBounds().height / 2);
}
} // namespace

namespace controllers {
HighScoreController::HighScoreController(
    const sf::Vector2u &iWindowSize,
    std::shared_ptr<std::map<std::string, sf::Texture, std::less<>>>
        iTextureMap,
    const std::shared_ptr<sf::Font> &iFont)
    : textureMapPtr{iTextureMap}, fontPtr{iFont},
      buttonText("Main menu", *fontPtr, 17) {
  highScoreTexts.reserve(config::kNumberOfDisplayedHighScores);
  if (!(*textureMapPtr).contains(config::kHighScoreBackgroundName)) {
    throw tools::exceptions::TextureNotFoundException(
        "Background texture of high scores not found");
  }
  if (!(*textureMapPtr).contains(config::kButtonTextureName)) {
    throw tools::exceptions::TextureNotFoundException(
        "Menu button texture not found");
  }
  if (!(*textureMapPtr).contains(config::kBoardTexture)) {
    throw tools::exceptions::TextureNotFoundException(
        "High score board texture not found");
  }
  backgroundSprite.setTexture(
      (*textureMapPtr)[config::kHighScoreBackgroundName]);
  returnButtonSprite.setTexture((*textureMapPtr)[config::kButtonTextureName]);
  highScoreBoardSprite.setTexture((*textureMapPtr)[config::kBoardTexture]);
  returnButtonSprite.setOrigin(returnButtonSprite.getLocalBounds().width / 2,
                               returnButtonSprite.getLocalBounds().height / 2);
  buttonText.setOrigin(buttonText.getLocalBounds().width / 2,
                       buttonText.getLocalBounds().height / 2);
  highScoreBoardSprite.setOrigin(
      highScoreBoardSprite.getLocalBounds().width / 2,
      highScoreBoardSprite.getLocalBounds().height / 2);
  buttonText.setFillColor(sf::Color::Black);
  // Set transparency
  sf::Color color = returnButtonSprite.getColor();
  color.a = static_cast<uint8_t>(255.0F * 0.85F);
  returnButtonSprite.setColor(color);

  color = highScoreBoardSprite.getColor();
  color.a = static_cast<uint8_t>(255.0F * 0.85F);
  highScoreBoardSprite.setColor(color);

  resizeButton(iWindowSize);
  buttonText.setPosition(returnButtonSprite.getPosition());
  resizeBackground(iWindowSize);

  // Create empty texts
  for (uint8_t textIndex{0U}; textIndex < config::kNumberOfDisplayedHighScores;
       ++textIndex) {
    highScoreTexts.emplace_back("", *fontPtr, 13);
    highScoreTexts[textIndex].setFillColor(sf::Color::Black);
  }
  resizeScoreTexts(iWindowSize);

  resizeHighScoreBoard(iWindowSize);
}

void HighScoreController::resizeHighScoreBoard(
    const sf::Vector2u &iNewWindowSize) {
  highScoreBoardSprite.setPosition(static_cast<float>(iNewWindowSize.x) / 2.F,
                                   static_cast<float>(iNewWindowSize.y) / 2.5F);
  highScoreBoardSprite.setScale(
      static_cast<float>(iNewWindowSize.x) * 0.7F /
          static_cast<float>(highScoreBoardSprite.getTexture()->getSize().x),
      static_cast<float>(iNewWindowSize.y) * 0.75F /
          static_cast<float>(highScoreBoardSprite.getTexture()->getSize().y));
}

void HighScoreController::resizeScoreTexts(const sf::Vector2u &iNewWindowSize) {
  auto currentTextYPosition = static_cast<float>(iNewWindowSize.y) * 0.2F;
  for (auto &highScoreText : highScoreTexts) {
    highScoreText.setCharacterSize(static_cast<unsigned int>(
        0.35F * returnButtonSprite.getGlobalBounds().height));
    highScoreText.setPosition(static_cast<float>(iNewWindowSize.x) / 2.0F,
                              currentTextYPosition);
    currentTextYPosition += static_cast<float>(iNewWindowSize.y) * 0.1F;
    setScoreTextsOrigin(highScoreText);
  }
}

void HighScoreController::resizeButtonText() {
  buttonText.setCharacterSize(static_cast<unsigned int>(
      0.35F * returnButtonSprite.getGlobalBounds().height));
  buttonText.setOrigin(buttonText.getLocalBounds().width / 2,
                       buttonText.getLocalBounds().height / 2);
  buttonText.setPosition(returnButtonSprite.getPosition());
}

void HighScoreController::resizeButton(const sf::Vector2u &iNewWindowSize) {
  if (returnButtonSprite.getTexture() == nullptr) {
    throw tools::exceptions::TextureNotSetException(
        "Background texture not assigned");
  }
  returnButtonSprite.setScale(
      static_cast<float>(iNewWindowSize.x) * 0.50F /
          static_cast<float>(returnButtonSprite.getTexture()->getSize().x),
      static_cast<float>(iNewWindowSize.y) * 0.15F /
          static_cast<float>(returnButtonSprite.getTexture()->getSize().y));
  returnButtonSprite.setPosition(static_cast<float>(iNewWindowSize.x) * 0.5F,
                                 static_cast<float>(iNewWindowSize.y) * 0.9F);
}

void HighScoreController::resizeBackground(const sf::Vector2u &iNewWindowSize) {
  if (backgroundSprite.getTexture() == nullptr) {
    throw tools::exceptions::TextureNotSetException(
        "Background texture not assigned");
  }
  backgroundSprite.setScale(
      static_cast<float>(iNewWindowSize.x) /
          static_cast<float>(backgroundSprite.getTexture()->getSize().x),
      static_cast<float>(iNewWindowSize.y) /
          static_cast<float>(backgroundSprite.getTexture()->getSize().y));
}

void HighScoreController::handleEvent(sf::RenderWindow &iWindow,
                                      const sf::Event &iEvent,
                                      tools::ScreenSelector &ioSelector) {
  if (iEvent.type == sf::Event::Closed) {
    iWindow.close();
  } else if (iEvent.type == sf::Event::MouseButtonPressed) {
    if (iEvent.mouseButton.button == sf::Mouse::Left && !mouseButtonPressed) {
      mouseButtonPressed = true;
      sf::Vector2i mousePos = sf::Mouse::getPosition(iWindow);
      if (returnButtonSprite.getGlobalBounds().contains(
              static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
        buttonText.setFillColor(sf::Color::Red);
      }
    }
  } else if (iEvent.type == sf::Event::MouseButtonReleased) {
    if (iEvent.mouseButton.button == sf::Mouse::Left) {
      sf::Vector2i mousePos = sf::Mouse::getPosition(iWindow);
      buttonText.setFillColor(sf::Color::Black);
      mouseButtonPressed = false;
      if (returnButtonSprite.getGlobalBounds().contains(
              static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
        ioSelector.setSelectedOption(tools::SelectorOptions::MainMenu);
        ioSelector.setFirstPass(true);
      }
    }
  } else if (iEvent.type == sf::Event::Resized) {
    sf::FloatRect visibleArea(0, 0, static_cast<float>(iEvent.size.width),
                              static_cast<float>(iEvent.size.height));
    iWindow.setView(sf::View(visibleArea));
    resize(iWindow.getSize());
  }
}

void HighScoreController::call(sf::RenderWindow &iWindow,
                               tools::ScreenSelector &ioSelector) {
  sf::Event event;
  while (iWindow.pollEvent(event)) {
    handleEvent(iWindow, event, ioSelector);
  }
  iWindow.clear();
  iWindow.draw(backgroundSprite);
  iWindow.draw(highScoreBoardSprite);
  for (const auto &score : highScoreTexts) {
    iWindow.draw(score);
  }
  iWindow.draw(returnButtonSprite);
  iWindow.draw(buttonText);
  iWindow.display();
}
void HighScoreController::resize(const sf::Vector2u &iNewWindowSize) {
  resizeBackground(iNewWindowSize);
  resizeButton(iNewWindowSize);
  resizeButtonText();
  resizeScoreTexts(iNewWindowSize);
  resizeHighScoreBoard(iNewWindowSize);
}

void HighScoreController::updateHighScores(
    const tools::DatabaseManager &iDatabaseManager) {
  auto scores = iDatabaseManager.getBestScores();
  for (uint8_t scoreIndex{0U};
       scoreIndex < config::kNumberOfDisplayedHighScores; ++scoreIndex) {

    highScoreTexts[scoreIndex].setString(
        std::format("#{}  {}:  {}", std::to_string(scoreIndex + 1),
                    scores[scoreIndex].first, scores[scoreIndex].second));
    setScoreTextsOrigin(highScoreTexts[scoreIndex]);
  }
}
} // namespace controllers
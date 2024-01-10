#include "../inc/screen_selector.hpp"

namespace tools {
SelectorOptions ScreenSelector::getSelectedOption() const {
  return selectedOption;
}

void ScreenSelector::setSelectedOption(SelectorOptions iOption) {
  switch (iOption) {
    using enum tools::SelectorOptions;
  case MainMenu:
  case Options:
  case Game:
    selectedOption = iOption;
    break;
  default:
    selectedOption = MainMenu;
  }
}
} // namespace tools
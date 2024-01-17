#ifndef SCREEN_SELECTOR_GUARD
#define SCREEN_SELECTOR_GUARD

#include "selector_options.hpp"
namespace tools {
class ScreenSelector {
private:
  SelectorOptions selectedOption{SelectorOptions::MainMenu};
  bool firstPass{false};

public:
  SelectorOptions getSelectedOption() const;
  void setSelectedOption(SelectorOptions iOption);
  bool isFirstPass() const;
  void setFirstPass(bool iPassValue);
};
} // namespace tools

#endif
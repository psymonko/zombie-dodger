#pragma once
#include "uiscreen.h"

class PauseMenu : public UIScreen {
 public:
  PauseMenu(class Game* game);
  ~PauseMenu();

  void ProcessInput(const struct InputState& state) override;
};

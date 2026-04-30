#pragma once
#include "uiscreen.h"

class GameOverMenu : public UIScreen {
 public:
  GameOverMenu(class Game* game);
  ~GameOverMenu();

  void ProcessInput(const struct InputState& state) override;
};

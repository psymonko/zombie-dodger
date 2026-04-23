#include "pausemenu.h"
#include "game.h"
#include "inputsystem.h"
#include "dialogbox.h"

PauseMenu::PauseMenu(Game* game)
  :UIScreen(game)
{
  mGame->SetState(Game::EPaused);
  SetTitle("PAUSED");
  AddButton("ResumeButton", [this]() {
    Close();
  });
  AddButton("QuitButton", [this]() {
    new DialogBox(mGame, "Do you want to quit?",
       [this]() {
	 mGame->SetState(Game::EQuit);
       });
  });
}

PauseMenu::~PauseMenu() {
  mGame->SetState(Game::EGameplay);
}

void PauseMenu::ProcessInput(const struct InputState& state) {
  UIScreen::ProcessInput(state);
  
  if (state.Keyboard.GetKeyState(SDL_SCANCODE_ESCAPE) == EPressed) {
    Close();
  }
}

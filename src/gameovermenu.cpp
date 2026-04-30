#include "gameovermenu.h"
#include "game.h"
#include "dialogbox.h"
#include "inputsystem.h"

GameOverMenu::GameOverMenu(Game* game)
  :UIScreen(game)
{
  mGame->SetState(Game::EPaused);
  SetTitle("GAME OVER!");
  AddButton("Restart", [this]() {
    mGame->RestartGame();
    Close();
  });
  AddButton("Quit Game", [this]() {
    new DialogBox(mGame, "Do you want to quit?",
       [this]() {
	 mGame->SetState(Game::EQuit);
       });
  });
}

GameOverMenu::~GameOverMenu() {
  mGame->SetState(Game::EGameplay);
}

void GameOverMenu::ProcessInput(const struct InputState& state) {
  UIScreen::ProcessInput(state);

  if (state.Keyboard.GetKeyState(SDL_SCANCODE_ESCAPE) == EPressed) {
    Close();
  }
}

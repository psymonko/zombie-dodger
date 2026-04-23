#include "dialogbox.h"
#include "game.h"
#include <string>

DialogBox::DialogBox(Game* game, const std::string& text,
		     std::function<void()> onOK)
  :UIScreen(game)
{
  mBGPos = Vector2(0.0f, 0.0f);
  mTitlePos = Vector2(0.0f, 100.0f);
  mNextButtonPos = Vector2(0.0f, 0.0f);

  mBackground = mGame->GetTexture("assets/DialogBG.png");
  SetTitle(text, Vector3::Zero, 30);
  
  AddButton("OK", [onOK]() {
    onOK();
  });
  AddButton("Cancel", [this]() {
    Close();
  });
}

DialogBox::~DialogBox() {
  
}

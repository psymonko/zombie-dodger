#include "uiscreen.h"
#include "game.h"
#include "shader.h"
#include "inputsystem.h"
#include "texture.h"
#include "font.h"
#include "Math.h"

UIScreen::UIScreen(Game* game)
  :mGame(game)
  ,mTitle(nullptr)
  ,mBackground(nullptr)
  ,mTitlePos(0.0f, 300.0f)
  ,mNextButtonPos(0.0f, 200.0f)
  ,mBGPos(0.0f, 250.0f)
  ,mState(EActive)
{
  game->PushUI(this);
  mFont = mGame->GetFont("assets/Carlito-Regular.ttf");
  mButtonOn = mGame->GetTexture("assets/ButtonYellow.png");
  mButtonOff = mGame->GetTexture("assets/ButtonBlue.png");
}

UIScreen::~UIScreen() {
  
}

void UIScreen::Update(float deltaTime) {

}

void UIScreen::Draw(Shader* shader) {
  if (mBackground) {
    DrawTexture(shader, mBackground, mBGPos);
  }
  if (mTitle) {
    DrawTexture(shader, mTitle, mTitlePos);
  }

  for (auto b : mButtons) {
    Texture* tex = b->GetHighlighted() ? mButtonOn : mButtonOff;
    DrawTexture(shader, tex, b->GetPosition());
    DrawTexture(shader, b->GetNameTex(), b->GetPosition());
  }
}

void UIScreen::ProcessInput(const struct InputState& state) {
  if (!mButtons.empty()) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    // convert to (0, 0) center coordinates
    Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
    mousePos.x -= 1024.0f * 0.5f;
    mousePos.y = 768 * 0.5f - mousePos.y;

    for (auto b : mButtons) {
      if (b->ContainsPoint(mousePos)) {
	b->SetHighlighted(true);
      } else {
	b->SetHighlighted(false);
      }
    }
  }

  if (state.Mouse.GetButtonValue(SDL_BUTTON_LEFT) == true) {
    for (auto b : mButtons) {
      if (b->GetHighlighted()) {
	b->OnClick();
	break;
      }
    }
  }
}

void UIScreen::HandleKeyPress(int) {

}

void UIScreen::Close() {
  mState = EClosing;
}

void UIScreen::AddButton(const std::string& name,
			 std::function<void()> onClick)
{
  Vector2 dims(static_cast<float>(mButtonOn->GetWidth()),
	       static_cast<float>(mButtonOn->GetHeight()));
  Button* b = new Button(name, mFont, onClick, mNextButtonPos, dims);
  mButtons.emplace_back(b);

  mNextButtonPos.y -= mButtonOff->GetHeight() + 20.0f;
}

void UIScreen::SetTitle(const std::string& text,
			const Vector3& color,
			int pointSize)
{
  if (mTitle) {
    mTitle->Unload();
    delete mTitle;
    mTitle = nullptr;
  }
  mTitle = mFont->RenderText(text, color, pointSize);
}

void UIScreen::DrawTexture(class Shader* shader, class Texture* texture,
			   const Vector2& offset,
			   float scale)
{
  // scale the quad by width/height of the texture
  Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(texture->GetWidth()) * scale,
					  static_cast<float>(texture->GetHeight()) * scale,
					  1.0f);
  Matrix4 transMat = Matrix4::CreateTranslation(Vector3(offset.x, offset.y, 0.0f));
  Matrix4 world = scaleMat * transMat;
  shader->SetMatrixUniform("uWorldTransform", world);
  texture->SetActive();
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

Button::Button(const std::string& name, Font* font,
	       std::function<void()> onClick,
	       const Vector2& pos, const Vector2& dims)
  :mOnClick(onClick)
  ,mNameTex(nullptr)
  ,mFont(font)
  ,mPosition(pos)
  ,mDimensions(dims)
  ,mHighlighted(false)
{
  SetName(name);
}

Button::~Button() {
  if (mNameTex) {
    mNameTex->Unload();
    delete mNameTex;
  }
}

void Button::SetName(const std::string& name) {
  mName = name;
  if (mNameTex) {
    mNameTex->Unload();
    delete mNameTex;
    mNameTex = nullptr;
  }
  mNameTex = mFont->RenderText(mName);
}

bool Button::ContainsPoint(const Vector2& pt) const {
  bool no = pt.x < (mPosition.x - mDimensions.x / 2.0f) ||
      pt.x > (mPosition.x + mDimensions.x / 2.0f) ||
      pt.y < (mPosition.y - mDimensions.y / 2.0f) ||
      pt.y > (mPosition.y + mDimensions.y / 2.0f);
  return !no;
}

void Button::OnClick() {
  if (mOnClick) {
    mOnClick();
  }
}

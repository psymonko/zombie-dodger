#pragma once
#include "Math.h"
#include <functional>
#include <string>

class Button {
public:
  Button(const std::string& name, class Font* font,
	 std::function<void()> onClick,
	 const Vector2& pos, const Vector2& dims);
  ~Button();
  
  void SetName(const std::string& name);

  class Texture* GetNameTex() { return mNameTex; }
  const Vector2& GetPosition() const { return mPosition; }
  void SetHighlighted(bool sel) { mHighlighted = sel; }
  bool GetHighlighted() const { return mHighlighted; }
  
  bool ContainsPoint(const Vector2& pt) const;
  void OnClick();

private:
  std::function<void()> mOnClick;
  std::string mName;
  class Texture* mNameTex;
  class Font* mFont;
  Vector2 mPosition;
  Vector2 mDimensions;
  bool mHighlighted;
};

class UIScreen {
 public:
  UIScreen(class Game* game);
  virtual ~UIScreen();
  virtual void Update(float deltaTime);
  virtual void Draw(class Shader* shader);
  virtual void ProcessInput(const struct InputState& state);
  virtual void HandleKeyPress(int key);
  
  // tracks if ui is active or closing
  enum UIState { EActive, EClosing };
  void Close();
  UIState GetState() const { return mState; }
  void SetTitle(const std::string& text,
		const Vector3& color = Color::White,
		int pointSize = 48);

  void AddButton(const std::string& name, std::function<void()> onClick);
 protected:
  // helper to draw a texture
  void DrawTexture(class Shader* shader, class Texture* texture,
		   const Vector2& offset = Vector2::Zero,
		   float scale = 1.0f);
  class Game* mGame;

  class Font* mFont;
  class Texture* mTitle;
  class Texture* mBackground;
  class Texture* mButtonOn;
  class Texture* mButtonOff;
  
  Vector2 mTitlePos;
  Vector2 mNextButtonPos;
  Vector2 mBGPos;

  // state
  UIState mState;

  std::vector<Button*> mButtons;
};

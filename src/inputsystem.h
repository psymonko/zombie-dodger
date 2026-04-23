#pragma once
#include "Math.h"
#include <SDL2/SDL.h>

enum ButtonState {
  ENone,
  EPressed,
  EHeld,
  EReleased
};

class KeyboardState {
 public:
  friend class InputSystem;
  
  bool GetKeyValue(SDL_Scancode keyCode) const;
  ButtonState GetKeyState(SDL_Scancode keyCode) const;

 private:
  const Uint8* mCurrState;
  Uint8 mPrevState[SDL_NUM_SCANCODES];
};

class MouseState {
public:
  friend class InputSystem;

  const Vector2& GetPosition() const { return mMousePos; }
  const Vector2& GetScrollWheel() const { return mScrollWheel; }
  bool IsRelative() const { return mIsRelative; }

  bool GetButtonValue(int button) const;
  ButtonState GetButtonState(int button) const;

private:
  Vector2 mMousePos;
  Vector2 mScrollWheel;
  Uint32 mCurrButtons;
  Uint32 mPrevButtons;
  // in relative mouse mode?
  bool mIsRelative;
};

struct InputState {
  KeyboardState Keyboard;
  MouseState Mouse;
};

class InputSystem {
 public:
  bool Initialize();
  void Shutdown();

  // called before sdl poll events loop to save previous states and prepare mouse input
  void PrepareForUpdate();
  // called after to get state
  void Update();
  // called to process a specific sdl event in input system
  void ProcessEvent(union SDL_Event& event);

  const InputState& GetState() const { return mState; }

  void SetRelativeMouseMode(bool value);
 private:
  InputState mState;
};

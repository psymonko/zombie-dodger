#include "inputsystem.h"
#include <SDL2/SDL.h>

bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const {
  return mCurrState[keyCode] == 1;
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const {
  if (mPrevState[keyCode] == 0) {
    if (mCurrState[keyCode] == 0) {
      return ENone;
    } else {
      return EPressed;
    }
  } else {
    if (mCurrState[keyCode] == 0) {
      return EReleased;
    } else {
      return EHeld;
    }
  }
}

bool MouseState::GetButtonValue(int button) const {
  return (SDL_BUTTON(button) & mCurrButtons);
}

ButtonState MouseState::GetButtonState(int button) const {
  int mask = SDL_BUTTON(button);
  if ((mask & mPrevButtons) == 0) {
    if ((mask & mCurrButtons) == 0) {
      return ENone;
    } else {
      return EPressed;
    }
  } else {
    if ((mask & mCurrButtons) == 0) {
      return EReleased;
    } else {
      return EHeld;
    }
  }
}

bool InputSystem::Initialize() {
  mState.Keyboard.mCurrState = SDL_GetKeyboardState(NULL);
  memset(mState.Keyboard.mPrevState, 0, SDL_NUM_SCANCODES);

  mState.Mouse.mCurrButtons = 0;
  mState.Mouse.mPrevButtons = 0;

  return true;
}

void InputSystem::PrepareForUpdate() {
  memcpy(mState.Keyboard.mPrevState,
	 mState.Keyboard.mCurrState,
	 SDL_NUM_SCANCODES);
  mState.Mouse.mPrevButtons = mState.Mouse.mCurrButtons;
  mState.Mouse.mIsRelative = false;
  mState.Mouse.mScrollWheel = Vector2::Zero;
}

void InputSystem::Update() {
  int x = 0, y = 0;
  if (mState.Mouse.mIsRelative) {
    mState.Mouse.mCurrButtons = SDL_GetRelativeMouseState(&x, &y);
  } else {
    mState.Mouse.mCurrButtons = SDL_GetMouseState(&x, &y);
  }

  // convert to opengl coordinates, screen width and height hardcoded
  x = x - 1024.0f / 2;
  y = 768 / 2 - y;
  
  mState.Mouse.mMousePos.x = static_cast<float>(x);
  mState.Mouse.mMousePos.y = static_cast<float>(y);
}

void InputSystem::ProcessEvent(SDL_Event& event) {
  switch (event.type) {
  case SDL_MOUSEWHEEL:
    mState.Mouse.mScrollWheel = Vector2(
		     static_cast<float>(event.wheel.x),
		     static_cast<float>(event.wheel.y));
    break;
  default:
    break;
  }
}

void InputSystem::SetRelativeMouseMode(bool value) {
  SDL_bool set = value ? SDL_TRUE : SDL_FALSE;
  SDL_SetRelativeMouseMode(set);

  mState.Mouse.mIsRelative = value;
}

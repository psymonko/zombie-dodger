#include "inputcomponent.h"
#include "actor.h"
#include "inputsystem.h"

InputComponent::InputComponent(Actor* owner)
  :MoveComponent(owner)
  ,mForwardKey(0)
  ,mBackKey(0)
  ,mRightKey(0)
  ,mLeftKey(0)
{

}

void InputComponent::ProcessInput(const struct InputState& state) {
  float forwardSpeed = 0.0f;
  if (state.Keyboard.GetKeyValue(SDL_Scancode(mForwardKey))) {
    forwardSpeed += mMaxForwardSpeed;
  }
  if (state.Keyboard.GetKeyValue(SDL_Scancode(mBackKey))) {
    forwardSpeed -= mMaxForwardSpeed;
  }
  SetForwardSpeed(forwardSpeed);
  
  float rightSpeed = 0.0f;
  if (state.Keyboard.GetKeyValue(SDL_Scancode(mRightKey))) {
    rightSpeed += mMaxRightSpeed;
  }
  if (state.Keyboard.GetKeyValue(SDL_Scancode(mLeftKey))) {
    rightSpeed -= mMaxRightSpeed;
  }
  SetRightSpeed(rightSpeed);
}

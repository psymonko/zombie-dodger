#pragma once
#include "movecomponent.h"

class InputComponent : public MoveComponent {
 public:
  InputComponent(class Actor* owner);

  void ProcessInput(const struct InputState& state);

  float GetMaxForward() const { return mMaxForwardSpeed; }
  float GetMaxRight() const { return mMaxRightSpeed; }
  int GetForwardKey() const { return mForwardKey; }
  int GetBackKey() const { return mBackKey; }
  int GetRightKey() const { return mRightKey; }
  int GetLeftKey() const { return mLeftKey; }

  void SetMaxForwardSpeed(float speed) { mMaxForwardSpeed = speed; }
  void SetMaxRightSpeed(float speed) { mMaxRightSpeed = speed; }
  void SetForwardKey(int key) { mForwardKey = key; }
  void SetBackKey(int key) { mBackKey = key; }
  void SetRightKey(int key) { mRightKey = key; }
  void SetLeftKey(int key) { mLeftKey = key; }

 private:
  float mMaxForwardSpeed;
  float mMaxRightSpeed;
  int mForwardKey;
  int mBackKey;
  int mRightKey;
  int mLeftKey;
};

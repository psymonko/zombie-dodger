#pragma once
#include "component.h"

class MoveComponent : public Component {
 public:
  // lower update order so it updates first
  MoveComponent(class Actor* owner, int updateOrder = 10);
  void Update(float deltaTime) override;


  float GetForwardSpeed() const { return mForwardSpeed; }
  void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
  float GetRightSpeed() const { return mRightSpeed; } 
  void SetRightSpeed(float speed) { mRightSpeed = speed; }
  float GetDefaultSpeed() const { return mDefaultSpeed; }
  void SetDefaultSpeed(float speed) { mDefaultSpeed = speed; }

 private:
  float mDefaultSpeed;
  float mForwardSpeed;
  float mRightSpeed;
};

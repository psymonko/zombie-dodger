#include "movecomponent.h"
#include "actor.h"
#include "Math.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
  :Component(owner, updateOrder)
  ,mRightSpeed(0.0f)
  ,mForwardSpeed(0.0f)
  ,mDefaultSpeed(300.0f)
{
  
}

void MoveComponent::Update(float deltaTime) {
  if (!Math::NearZero(mForwardSpeed) || !Math::NearZero(mRightSpeed)) {
    Vector2 pos = mOwner->GetPosition();
    Vector2 move =
      mOwner->GetForward() * mForwardSpeed +
      mOwner->GetRight() * mRightSpeed;
    if (!Math::NearZero(move.Length())) {
      move.Normalize();
    }

    pos += move * mDefaultSpeed * deltaTime;
    mOwner->SetPosition(pos);
  }
}

#include "component.h"
#include "actor.h"
#include <cstdint>

Component::Component(Actor* owner, int updateOrder)
  :mOwner(owner)
  ,mUpdateOrder(updateOrder)
{
  mOwner->AddComponent(this);
}

Component::~Component() {
  mOwner->RemoveComponent(this);
}

void Component::Update(float deltaTime)
{
}

void Component::ProcessInput(const struct InputState& state) {
  
}

void Component::OnUpdateWorldTransform() {

}

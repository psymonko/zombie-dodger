#pragma once
#include <cstdint>
#include "Math.h"

class Component {
 public:
  // the lower update order, the earlier the component updates
  Component(class Actor* owner, int updateOrder = 100);
  virtual ~Component();
  virtual void Update(float deltaTime);
  virtual void ProcessInput(const struct InputState& state);
  // whenever the transform of the owner updates
  virtual void OnUpdateWorldTransform();

  int GetUpdateOrder() const { return mUpdateOrder; }
 protected:
  class Actor* mOwner;
  int mUpdateOrder;
};

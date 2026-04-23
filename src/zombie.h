#pragma once
#include "actor.h"

class Zombie : public Actor {
 public:
  Zombie(class Game* game);
  ~Zombie();
  
  void UpdateActor(float deltaTime) override;
  void ActorInput(const struct InputState& state) override;

  class CircleComponent* GetCircle() { return mCircle; }
private:
  class CircleComponent* mCircle;
  class MoveComponent* mMoveComponent;
};

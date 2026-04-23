#pragma once
#include "actor.h"

class Player : public Actor {
public:
  Player(class Game* game);

  void UpdateActor(float deltaTime) override;
  void ActorInput(const struct InputState& state) override;

private:
  class CircleComponent* mCircle;
  
};




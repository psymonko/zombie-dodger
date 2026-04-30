#pragma once
#include "actor.h"
#include <random>

class Spawner : public Actor {
 public:
  Spawner(class Game* game);

  void UpdateActor(float deltaTime) override;
  void ActorInput(const struct InputState& state) override;
  void SetIncrement(float increment) { mIncrement = increment; }

 private:
  float mTimer;
  float mIncrement;
  float mCurrentTime;
  std::random_device rd;
};

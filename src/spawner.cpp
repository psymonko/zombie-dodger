#include "spawner.h"
#include "game.h"
#include "zombie.h"
#include <SDL2/SDL.h>
#include <random>

Spawner::Spawner(Game* game)
  :Actor(game)
  ,mTimer(0.05f)
  ,mIncrement(0.3f)
  ,mCurrentTime(0.0f)
{
}

void Spawner::UpdateActor(float deltaTime) {
  mCurrentTime += mIncrement * deltaTime;
  mIncrement += 0.01f * deltaTime;
  if (mCurrentTime >= mTimer) {
    mCurrentTime = 0.0f;
    
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-500.0f, 500.0f);
    float randX = dis(gen);

    Vector2 pos = Vector2(randX, -390.0f);
    
    Zombie* zomb = new Zombie(GetGame());
    zomb->SetPosition(pos);
    GetGame()->AddZombie(zomb);
  }
}

void Spawner::ActorInput(const struct InputState& state) {

}

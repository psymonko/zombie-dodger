#include "zombie.h"
#include "game.h"
#include "spritecomponent.h"
#include "movecomponent.h"
#include "circlecomponent.h"
#include "Math.h"

Zombie::Zombie(Game* game)
  :Actor(game)
{
  SpriteComponent* sc = new SpriteComponent(this);
  sc->SetTexture(game->GetTexture("assets/zombie.png"));

  //Vector2 startPos(0.0f, -390.0f);
  Vector2 startPos(0.0f, -390.0f);
  SetPosition(startPos);
  
  mMoveComponent = new MoveComponent(this);
  mMoveComponent->SetForwardSpeed(100.0f);

  mCircle = new CircleComponent(this);
  mCircle->SetRadius(30.0f);

  game->AddZombie(this);
}

Zombie::~Zombie() {
  GetGame()->RemoveZombie(this);
}

void Zombie::UpdateActor(float deltaTime) {
  if (mMoveComponent->GetForwardSpeed() > 0 && GetPosition().y > 384) {
    SetState(EDead);
    GetGame()->RemoveZombie(this);
  }
}

void Zombie::ActorInput(const struct InputState& state) {
  
}

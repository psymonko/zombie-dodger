#include "player.h"
#include "spritecomponent.h"
#include "game.h"
#include "inputsystem.h"
#include "inputcomponent.h"
#include "circlecomponent.h"
#include "zombie.h"
#include "gameovermenu.h"
#include "pausemenu.h"

Player::Player(Game* game)
  :Actor(game)
{
  SpriteComponent* sc = new SpriteComponent(this, 150);
  sc->SetTexture(game->GetTexture("assets/ship.png"));

  InputComponent* ic = new InputComponent(this);
  ic->SetForwardKey(SDL_SCANCODE_W);
  ic->SetBackKey(SDL_SCANCODE_S);
  ic->SetRightKey(SDL_SCANCODE_D);
  ic->SetLeftKey(SDL_SCANCODE_A);
  ic->SetMaxForwardSpeed(300.0f);
  ic->SetMaxRightSpeed(300.0f);

  mCircle = new CircleComponent(this);
  mCircle->SetRadius(30.0f);

  Vector2 startPos = Vector2(100, 100);

  SetPosition(startPos);
}

void Player::UpdateActor(float deltaTime) {
  for (auto zombie : GetGame()->GetZombies()) {
    if (Intersect(*mCircle, *(zombie->GetCircle()))) {
      //SetState(EDead);
      new GameOverMenu(GetGame());
      break;
    }
  }
}

void Player::ActorInput(const struct InputState& state) {
  
}

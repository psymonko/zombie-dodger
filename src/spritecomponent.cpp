#include "spritecomponent.h"
#include "actor.h"
#include "Math.h"
#include "texture.h"
#include "game.h"
#include "shader.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
  :Component(owner)
  ,mDrawOrder(drawOrder)
{
  mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
  mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::SetTexture(Texture* texture) {
  mTexture = texture;
  mTexWidth = texture->GetWidth();
  mTexHeight = texture->GetHeight();
}

void SpriteComponent::Draw(Shader* shader) {
  if (mTexture) {
    // i want to scale this quad by the width and height of the texture
    Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(mTexWidth),
					    static_cast<float>(mTexHeight),
					    1.0f);
    Matrix4 world = scaleMat * mOwner->GetWorldTransform();
    // all sprites use same shader and vertices
    shader->SetMatrixUniform("uWorldTransform", world);
    mTexture->SetActive();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  }
}

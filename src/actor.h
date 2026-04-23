#pragma once
#include "Math.h"
#include <cstdint>
#include <vector>

class Actor {
 public:
  enum State {
    EActive,
    EPaused,
    EDead
  };
  
  Actor(class Game* game);
  virtual ~Actor();

  // called from game each frame, all actors updated
  void Update(float deltaTime);
  // update all components on the actor
  void UpdateComponents(float deltaTime);
  virtual void UpdateActor(float deltaTime);

  void ProcessInput(const struct InputState& state);
  // any actor specific input code
  virtual void ActorInput(const struct InputState& state);


  // getters/setters
  const Vector2& GetPosition() const { return mPosition; }
  const Matrix4& GetWorldTransform() const { return mWorldTransform; }
  float GetScale() const { return mScale; }
  float GetRotation() const { return mRotation; }
  State GetState() const { return mState; }
  
  void SetPosition(const Vector2& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
  void ComputeWorldTransform();
  void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }
  void SetRotation(float rotation) { mRotation = rotation; mRecomputeWorldTransform = true; }
  void SetState(State state) { mState = state; }

  Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), Math::Sin(mRotation)); }
  Vector2 GetRight() const { return Vector2(Math::Sin(mRotation), Math::Cos(mRotation)); }
  
  class Game* GetGame() { return mGame; }

  void AddComponent(class Component* component);
  void RemoveComponent(class Component* component);
  
 private:
  State mState;
  class Game* mGame;
  std::vector<class Component*> mComponents;

  // transform
  Matrix4 mWorldTransform;
  Vector2 mPosition;
  float mScale;
  float mRotation;
  bool mRecomputeWorldTransform;
};

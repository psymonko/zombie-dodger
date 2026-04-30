#pragma once
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <unordered_map>
#include <string>

class Game {
 public:
  Game();
  bool Initialize();
  void RunLoop();
  void Shutdown();

  void AddActor(class Actor* actor);
  void RemoveActor(class Actor* actor);

  void AddSprite(class SpriteComponent* sprite);
  void RemoveSprite(class SpriteComponent* sprite);

  enum GameState {
    EGameplay,
    EPaused,
    EQuit
  };

  GameState GetState() const { return mGameState; }
  void SetState(GameState state) { mGameState = state; }

  class Texture* GetTexture(const std::string& fileName);
  class Font* GetFont(const std::string& fileName);

  const std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }
  void PushUI(class UIScreen* screen);

  // game specific code
  void AddZombie(class Zombie* zombie);
  void RemoveZombie(class Zombie* zombie);
  std::vector<class Zombie*>& GetZombies() { return mZombies; }
  
  void RestartGame();
  
 private:
  void ProcessInput();
  void UpdateGame();
  void GenerateOutput();
  void LoadData();
  void UnloadData();
  bool LoadShaders();
  void CreateSpriteVerts();


  class Shader* mSpriteShader;
  class VertexArray* mSpriteVerts;
  class InputSystem* mInputSystem;
  
  std::vector<class Actor*> mActors;
  std::vector<class SpriteComponent*> mSprites;
  std::vector<class Actor*> mPendingActors;
  std::vector<class UIScreen*> mUIStack;
  std::unordered_map<std::string, class Texture*> mTextures;
  std::unordered_map<std::string, class Font*> mFonts;

  SDL_Window* mWindow;
  SDL_GLContext mContext;
  Uint32 mTicksCount;
  bool mUpdatingActors;
  bool mGameOverShown;
  GameState mGameState;
  
  // game specific code
  class Player* mPlayer;
  class Zombie* mZombie;
  class Spawner* mSpawner;
  std::vector<class Zombie*> mZombies;
};

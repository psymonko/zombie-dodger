#include "game.h"
#include "actor.h"
#include "spritecomponent.h"
#include "shader.h"
#include "vertexarray.h"
#include "texture.h"
#include "player.h"
#include "inputsystem.h"
#include "zombie.h"
#include "font.h"
#include "uiscreen.h"
#include "pausemenu.h"
#include "spawner.h"
#include <algorithm>
#include <iostream>

Game::Game()
  :mWindow(nullptr)
  ,mSpriteShader(nullptr)
  ,mGameState(EGameplay)
  ,mUpdatingActors(false)
{
}

bool Game::Initialize() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  // request a color buffer with 8 bits per rgba channel
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  // enable double buffering
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  // hardware acceleration
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

  mWindow = SDL_CreateWindow("alilsum", 100, 100, 1024, 768, SDL_WINDOW_OPENGL);
  if (!mWindow) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }

  mInputSystem = new InputSystem();
  if (!mInputSystem->Initialize()) {
    SDL_Log("Failed to initialize input system");
    return false;
  }

  mContext = SDL_GL_CreateContext(mWindow);
  if (!mContext) {
    SDL_Log("Failed to initialize context: %s", SDL_GetError());
    return false;
  }

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    SDL_Log("Failed to initialize GLEW. %d", glGetError()); 
    return false;
  }
  // clear benign error after glew initialization on some platforms
  glGetError();

  if (!LoadShaders()) {
    SDL_Log("Failed to load shaders");
    return false;
  }

  if (TTF_Init() != 0) {
    SDL_Log("Failed to initialize SDL_ttf");
    return false;
  }
  
  CreateSpriteVerts();
  LoadData();

  mTicksCount = SDL_GetTicks();

  return true;
}

void Game::AddSprite(SpriteComponent* sprite) {
  // insert sprite right before next sprite with a higher draw order
  // lower draw orders should be drawn first
  int myDrawOrder = sprite->GetDrawOrder();
  auto iter = mSprites.begin();
  for (;
       iter != mSprites.end();
       ++iter)
    {
      if (myDrawOrder < (*iter)->GetDrawOrder()) {
	break;
      }
    }
  // will insert before position of iterator
  mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite) {
  auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
  mSprites.erase(iter);
}

void Game::AddActor(Actor* actor) {
  if (mUpdatingActors) {
    mPendingActors.emplace_back(actor);
  } else {
    mActors.emplace_back(actor);
  }
}

void Game::RemoveActor(Actor* actor) {
  // if in pending actors
  auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);

  if (iter != mPendingActors.end()) {
    // put it at the end of vector then pop it off (erase copies avoided)
    std::iter_swap(iter, mPendingActors.end() - 1);
    mPendingActors.pop_back();
  }

  // in normal actors
  iter = std::find(mActors.begin(), mActors.end(), actor);
  if (iter != mActors.end()) {
    std::iter_swap(iter, mActors.end() - 1);
    mActors.pop_back();
  }
}

void Game::AddZombie(Zombie* zombie) {
  mZombies.emplace_back(zombie);
}

void Game::RemoveZombie(Zombie* zombie) {
  auto iter = std::find(mZombies.begin(), mZombies.end(), zombie);
  if (iter != mZombies.end()) {
    mZombies.erase(iter);
  }
}


void Game::LoadData() {
  mPlayer = new Player(this);
  mZombie = new Zombie(this);
  mSpawner = new Spawner(this);
}

Texture* Game::GetTexture(const std::string& fileName) {
  Texture* tex = nullptr;
  auto iter = mTextures.find(fileName);
  if (iter != mTextures.end()) {
    tex = iter->second;
  } else {
    tex = new Texture();
    if (tex->Load(fileName)) {
      mTextures.emplace(fileName, tex);
    } else {
      delete tex;
      tex = nullptr;
    }
  }

  return tex;
}

void Game::CreateSpriteVerts() {
  float vertices[] = {
    -0.5f,  0.5f, 0.f, 0.f, 0.f, // top left
    0.5f,  0.5f, 0.f, 1.f, 0.f, // top right
    0.5f, -0.5f, 0.f, 1.f, 1.f, // bottom right
    -0.5f, -0.5f, 0.f, 0.f, 1.f  // bottom left
  };

  unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
  };

  mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

bool Game::LoadShaders() {
  mSpriteShader = new Shader();
  if (!mSpriteShader->Load("shaders/shader.vert", "shaders/shader.frag")) {
    return false;
  }

  mSpriteShader->SetActive();
  // set the view projection matrix
  Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1024.f, 768.f);
  mSpriteShader->SetMatrixUniform("uViewProj", viewProj);
  return true;
}

void Game::PushUI(UIScreen* screen) {
  mUIStack.emplace_back(screen);
}

Font* Game::GetFont(const std::string& fileName) {
  auto iter = mFonts.find(fileName);
  if (iter != mFonts.end()) {
    return iter->second;
  } else {
    Font* font = new Font(this);
    if (font->Load(fileName)) {
      mFonts.emplace(fileName, font);
    } else {
      font->Unload();
      delete font;
      font = nullptr;
    }
    return font;
  }
}

void Game::ProcessInput() {
  // save previous states
  mInputSystem->PrepareForUpdate();
  
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      mGameState = EQuit;
      break;
    case SDL_MOUSEWHEEL:
      mInputSystem->ProcessEvent(event);
      break;
    default:
      break;
    }
  }

  // set new states in input system
  mInputSystem->Update();
  const InputState& state = mInputSystem->GetState();

  if (mGameState == EGameplay) {
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_ESCAPE) == EPressed) {
      new PauseMenu(this);
    }
    
    mUpdatingActors = true;
    for (auto actor : mActors) {
      actor->ProcessInput(state);
    }
    mUpdatingActors = false;
  } else if (!mUIStack.empty()) {
    mUIStack.back()->ProcessInput(state);
  }
}
void Game::UpdateGame() {
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
    ;

  float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
  if (deltaTime > 0.05) {
    deltaTime = 0.05f;
  }
  mTicksCount = SDL_GetTicks();

  if (mGameState == EGameplay) {
    mUpdatingActors = true;
    for (auto actor : mActors) {
      actor->Update(deltaTime);
    }
    mUpdatingActors = false;

    for (auto pending : mPendingActors) {
      pending->ComputeWorldTransform();
      mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    std::vector<Actor*> deadActors;
    for (auto actor : mActors) {
      if (actor->GetState() == Actor::EDead) {
	deadActors.emplace_back(actor);
      }
    }

    for (auto actor : deadActors) {
      delete actor;
    }
  }

  for (auto ui : mUIStack) {
    if (ui->GetState() == UIScreen::EActive) {
      ui->Update(deltaTime);
    }
  }

  auto iter = mUIStack.begin();
  while (iter != mUIStack.end()) {
    if ((*iter)->GetState() == UIScreen::EClosing) {
      delete *iter;
      iter = mUIStack.erase(iter);
    } else {
      ++iter;
    }
  }
}

void Game::GenerateOutput() {
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.2f, 0.6f, 0.4f, 1.0f);
  
  // enable alpha blending on color buffer
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  mSpriteShader->SetActive();
  mSpriteVerts->SetActive();

  for (auto sprite : mSprites) {
    sprite->Draw(mSpriteShader);
  }

  for (auto ui : mUIStack) {
    ui->Draw(mSpriteShader);
  }
  
  // swap buffers
  SDL_GL_SwapWindow(mWindow);
}

void Game::RunLoop() {
  while (mGameState != EQuit) {
    ProcessInput();
    UpdateGame();
    GenerateOutput();
  }
}

void Game::Shutdown() {
  SDL_GL_DeleteContext(mContext);
  SDL_DestroyWindow(mWindow);
  SDL_Quit();
}

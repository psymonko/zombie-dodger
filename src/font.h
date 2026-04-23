#pragma once
#include "Math.h"
#include <string>
#include <unordered_map>
#include <SDL2/SDL_ttf.h>

class Font {
 public:
  Font(class Game* game);
  ~Font();
  bool Load(const std::string& fileName);
  void Unload();
  // given string and this font, draw to a texture
  class Texture* RenderText(const std::string& text,
			    const Vector3& color = Color::White,
			    int pointSize = 30);

 private:
  std::unordered_map<int, TTF_Font*> mFontData;
};

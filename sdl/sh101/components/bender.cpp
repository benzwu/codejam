#include "bender.h"

#include <SDL/SDL.h>

Bender::Bender(float * value, SDL_Surface * image, SDL_Surface * background, int x, int y)
  : value(value), Component(x, y, 0, 0, image, background) {}

void Bender::setValue(int x, int y, int mouseEvent)
{
  /*return;
  if (mouseEvent == SDL_MOUSEBUTTONUP) {
    *this->value = 0.5f;
  } else {
    float v = float(x - this->bbox.at(0).first - this->image->w / 2) / (this->bbox.at(1).first - this->image->w);
    v = v < 0 ? 0 : v > 1 ? 1 : v;
    *this->value = v;
  }*/
}

void Bender::render()
{
  /*return;
  SDL_Rect pos;
  pos.x = this->bbox.at(0).first + int(*this->value * (this->bbox.at(1).first - this->image->w - 1));
  pos.y = this->bbox.at(0).second + this->bbox.at(1).second / 2 - this->image->h / 2;
  Component::render(&pos);*/
}

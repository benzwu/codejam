#include "handle.h"

#include <SDL/SDL.h>

Handle::Handle(int * value, SDL_Surface * image, SDL_Surface * background, int x, int y)
  : value(value), modes(3), Component(x, y, 0, 0, image, background)
{
  this->locations[0] = .1f;
  this->locations[1] = .5f;
  this->locations[2] = .9f;
}

void Handle::setValue(int x, int y, int mouseEvent)
{
  return;
  float v = float(x - this->hitArea.at(0).first - this->image->w / 2) / (this->hitArea.at(1).first - this->image->w);
  v = v < 0 ? 0 : v > 1 ? 1 : v;
  *this->value = int(v * (this->modes - 1) + 0.5f);
}

void Handle::render()
{
  return;
  SDL_Rect pos;
  pos.x = this->hitArea.at(0).first + int(this->locations[*this->value] * (this->hitArea.at(1).first - this->image->w - 1));
  pos.y = this->hitArea.at(0).second + this->hitArea.at(1).second / 2 - this->image->h / 2;
  Component::render(&pos);
}

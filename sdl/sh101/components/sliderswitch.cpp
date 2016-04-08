#include "sliderswitch.h"

#include <SDL/SDL.h>

SliderSwitch::SliderSwitch(int * value, SDL_Surface * image, SDL_Surface * background, int x, int y, int height, int steps)
  : value(value), steps(steps), Component(x, y, 12, height, image, background)
{
  this->positions[0] = .65f;
  this->positions[1] = .5f;
  this->positions[2] = .35f;
  this->hitArea.push_back(point(x, y));
  this->hitArea.push_back(point(x + this->bbox->w, y));
  this->hitArea.push_back(point(x + this->bbox->w, y + height));
  this->hitArea.push_back(point(x, y + height));
  this->render();
}

void SliderSwitch::setValue(int x, int y, int mouseEvent)
{
  float v = float(y - this->bbox->y - this->image->h / 2) / (this->bbox->h - this->image->h);
  v = v < 0 ? 0 : v > 1 ? 1 : v;
  *this->value = int(v * (this->steps - 1) + 0.5f);
  this->render();
}

void SliderSwitch::render()
{
  SDL_Rect pos = { this->bbox->x, this->bbox->y };
  pos.x += this->bbox->w / 2 - this->image->w / 2;
  pos.y += int((1 - this->positions[*this->value]) * (this->bbox->h - this->image->h));
  Component::render(&pos);
}

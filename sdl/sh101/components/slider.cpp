#include "slider.h"

#include <SDL/SDL.h>

Slider::Slider(float * value, SDL_Surface * image, SDL_Surface * background, int x, int y, int height)
  : value(value), Component(x, y, 12, height, image, background)
{
  this->hitArea.push_back(point(x, y));
  this->hitArea.push_back(point(x + this->bbox->w, y));
  this->hitArea.push_back(point(x + this->bbox->w, y + height));
  this->hitArea.push_back(point(x, y + height));
  this->render();
}

void Slider::setValue(int x, int y, int mouseEvent)
{
  float v = 1 - float(y - this->bbox->y - this->image->h / 2) / (this->bbox->h - this->image->h);
  v = v < 0 ? 0 : v > 1 ? 1 : v;
  *this->value = v;
  this->render();
}

void Slider::render() {
  SDL_Rect pos = { this->bbox->x, this->bbox->y };
  pos.x += this->bbox->w / 2 - this->image->w / 2;
  pos.y += int((1 - *this->value) * (this->bbox->h - this->image->h));
  Component::render(&pos);
}

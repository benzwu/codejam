#include "light.h"

#include <SDL/SDL.h>

Light::Light(int * value, SDL_Surface * images[], SDL_Surface * background, int x, int y, int onValue)
  : value(value), images(images), onValue(onValue), Component(x, y, 29, 29, images[0], background)
{
  this->render();
}

void Light::render()
{
  this->image = this->images[*this->value == this->onValue];
  SDL_Rect pos = {this->bbox->x, this->bbox->y};
  Component::render(&pos);
}

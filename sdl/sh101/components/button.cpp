#include "button.h"

#include <SDL/SDL.h>

Button::Button(int * value, SDL_Surface * images[], SDL_Surface * background, int x, int y, int onValue)
  : value(value), images(images), onValue(onValue), Component(x, y, 20, 12, images[0], background)
{
  this->hitArea.push_back(point(x, y));
  this->hitArea.push_back(point(x + this->bbox->w, y));
  this->hitArea.push_back(point(x + this->bbox->w, y + this->bbox->h));
  this->hitArea.push_back(point(x, y + this->bbox->h));
  this->render();
}

void Button::setValue(int x, int y, int mouseEvent)
{
  if (mouseEvent == SDL_MOUSEBUTTONDOWN) {
    *this->value = *this->value == this->onValue ? 0 : this->onValue;
    this->image = this->images[1];
  }
  if (mouseEvent == SDL_MOUSEBUTTONUP) {
    this->image = this->images[0];
  }
  this->render();
}

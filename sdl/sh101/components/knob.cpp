#include "knob.h"

#include <SDL/SDL.h>
#include <cmath>

#define M_PI 3.14159265358979323846

Knob::Knob(float * value, SDL_Surface * images[], SDL_Surface * background, int x, int y)
  : value(value), images(images), frames(21), Component(x, y, 29, 29, images[0], background)
{
  int frame = int(*this->value * (this->frames - 1));
  this->image = this->images[frame];
  this->hitArea.push_back(point(x, y));
  this->hitArea.push_back(point(x + this->bbox->w, y));
  this->hitArea.push_back(point(x + this->bbox->w, y + this->bbox->h));
  this->hitArea.push_back(point(x, y + this->bbox->h));
  this->render();
}

void Knob::setValue(int x, int y, int mouseEvent)
{
  int xx = this->bbox->x + this->bbox->w / 2 - x;
  int yy = this->bbox->y + this->bbox->h / 2 - y;
  float angle = atan2(float(yy), float(xx)) + float(M_PI / 2);
  angle = float(angle > 0 ? angle / M_PI / 2 : (1 - angle / - M_PI) / 2 + 0.5);
  angle = (angle - .5f) / .8f + .5f;
  angle = angle < 0 ? 0 : angle > 1 ? 1 : angle;

  int frame = int(angle * (this->frames - 1));
  this->image = this->images[frame];
  *this->value = angle;

  this->render();
}

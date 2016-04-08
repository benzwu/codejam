#include "knobswitch.h"

#include <SDL/SDL.h>
#include <cmath>

#define M_PI 3.14159265358979323846

KnobSwitch::KnobSwitch(int * value, SDL_Surface * images[], SDL_Surface * background, int x, int y)
  : value(value), images(images), modes(4), Component(x, y, 29, 29, images[0], background)
{
  this->angles[0] = 13;
  this->angles[1] = 11;
  this->angles[2] = 9;
  this->angles[3] = 7;
  this->image = this->images[this->angles[*this->value]];
  this->hitArea.push_back(point(x, y));
  this->hitArea.push_back(point(x + this->bbox->w, y));
  this->hitArea.push_back(point(x + this->bbox->w, y + this->bbox->h));
  this->hitArea.push_back(point(x, y + this->bbox->h));
  this->render();
}

void KnobSwitch::setValue(int x, int y, int mouseEvent)
{
  int xx = this->bbox->x + this->bbox->w / 2 - x;
  int yy = this->bbox->y + this->bbox->h / 2 - y;
  float angle = atan2(float(yy), float(xx)) + float(M_PI / 2);
  angle = float(angle > 0 ? angle / M_PI / 2 : (1 - angle / - M_PI) / 2 + 0.5);
  angle = (angle - .5f) / .3f + .5f;
  angle = angle < 0 ? 0 : angle > 1 ? 1 : angle;
  int val = int(this->modes - 1 - angle * (this->modes - 1) + 0.5f); 

  int frame = this->angles[val];
  this->image = this->images[frame];
  *this->value = val;
  this->render();
}

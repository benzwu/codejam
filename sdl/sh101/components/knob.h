#ifndef KNOB_H
#define KNOB_H

#include "component.h"

class Knob : public Component
{
public:
  Knob(float * value, SDL_Surface * images[], SDL_Surface * background, int x, int y);
  void setValue(int x, int y, int mouseEvent);
  float * value;
  SDL_Surface ** images;
  const int frames;
};

#endif

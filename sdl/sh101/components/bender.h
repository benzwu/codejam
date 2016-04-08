#ifndef BENDER_H
#define BENDER_H

#include "component.h"

class Bender : public Component
{
public:
  Bender(float * value, SDL_Surface * image, SDL_Surface * background, int x, int y);
  void setValue(int x, int y, int mouseEvent);
  void render();
  float * value;
};

#endif

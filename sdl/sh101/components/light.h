#ifndef LIGHT_H
#define LIGHT_H

#include "component.h"

class Light : public Component
{
public:
  Light(int * value, SDL_Surface * images[], SDL_Surface * background, int x, int y, int onValue = 1);
  void render();
  void setValue(int x, int y, int mouseEvent) {}
  SDL_Surface ** images;
  int * value;
  int onValue;
};

#endif

#ifndef HANDLE_H
#define HANDLE_H

#include "component.h"

class Handle : public Component
{
public:
  Handle(int * value, SDL_Surface * image, SDL_Surface * background, int x, int y);
  void setValue(int x, int y, int mouseEvent);
  void render();
  int * value;
  int modes;
  float locations[3];
};

#endif

#ifndef BUTTON_H
#define BUTTON_H

#include "component.h"

class Button : public Component
{
public:
  Button(int * value, SDL_Surface * images[], SDL_Surface * background, int x, int y, int onValue = 1);
  void setValue(int x, int y, int mouseEvent);
  SDL_Surface ** images;
  int * value;
  int onValue;
};

#endif

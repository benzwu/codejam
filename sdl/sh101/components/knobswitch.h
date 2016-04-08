#ifndef KNOBSWITCH_H
#define KNOBSWITCH_H

#include "component.h"

class KnobSwitch : public Component
{
public:
  KnobSwitch(int * value, SDL_Surface * images[], SDL_Surface * background, int x, int y);
  void setValue(int x, int y, int mouseEvent);
  int * value;
  SDL_Surface ** images;
  int angles[4];
  int modes;
};

#endif

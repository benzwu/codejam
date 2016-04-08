#ifndef SLIDERSWITCH_H
#define SLIDERSWITCH_H

#include "component.h"

class SliderSwitch : public Component
{
public:
  SliderSwitch(int * value, SDL_Surface * image, SDL_Surface * background, int x, int y, int height, int steps);
  void setValue(int x, int y, int mouseEvent);
  void render();
  int * value;
  float positions[3];
  int steps;
};

#endif

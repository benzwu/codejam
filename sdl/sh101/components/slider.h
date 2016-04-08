#ifndef SLIDER_H
#define SLIDER_H

#include "component.h"

class Slider : public Component
{
public:
  Slider(float * value, SDL_Surface * image, SDL_Surface * background, int x, int y, int height);
  void setValue(int x, int y, int mouseEvent);
  void render();
  float * value;
};

#endif

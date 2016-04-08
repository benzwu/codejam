#include "component.h"

#include <SDL/SDL.h>

Component::Component(int x, int y, int w, int h, SDL_Surface * image, SDL_Surface * background)
  : image(image), background(background)
{
  this->bbox = new SDL_Rect;
  this->bbox->x = x;
  this->bbox->y = y;
  this->bbox->w = w;
  this->bbox->h = h;
}

Component::~Component()
{
  delete this->bbox;
}

bool Component::intersects(int x, int y)
{
  bool inside = false;
  for (unsigned int i = 0, j = this->hitArea.size() - 1; i < this->hitArea.size(); j = i++) {
    int x1 = this->hitArea.at(i).first;
    int y1 = this->hitArea.at(i).second;
    int x2 = this->hitArea.at(j).first;
    int y2 = this->hitArea.at(j).second;
    if (((y1 <= y) && (y < y2)) || ((y2 <= y) && (y < y1)))
      if (x < (x2 - x1) * (y - y1) / (y2 - y1) + x1)
        inside = !inside;
  }
  return inside;
}

void Component::render(bool renderBackground)
{
  this->render(this->bbox, renderBackground);
}

void Component::render(SDL_Rect * pos, bool renderBackground)
{
  SDL_Surface * screen = SDL_GetVideoSurface();
  if (renderBackground) {
    SDL_BlitSurface(this->background, this->bbox, screen, this->bbox);
  }
  SDL_BlitSurface(this->image, NULL, screen, pos);
  SDL_Flip(screen);
}

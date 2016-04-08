#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <vector>

struct SDL_Surface;
struct SDL_Rect;

class Component
{
public:
  virtual ~Component();
  bool intersects(int x, int y);
  virtual void render(bool renderBackground = true);
  void render(SDL_Rect * pos, bool renderBackground = true);
  virtual void setValue(int x, int y, int mouseEvent) = 0;

protected:
  typedef std::pair<int, int> point;
  Component(int x, int y, int w, int h, SDL_Surface * image, SDL_Surface * background);
  SDL_Rect * bbox;
  SDL_Surface * image;
  SDL_Surface * background;
  std::vector<point> hitArea;
};

#endif

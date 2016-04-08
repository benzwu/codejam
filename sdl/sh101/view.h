#ifndef VIEW_H
#define VIEW_H

#include <vector>

class SH101;
class Component;
struct SDL_Surface;

class View
{
public:
  View(SH101 * sh101);
  ~View();
  int pick(int x, int y) const;
  std::vector<Component *> components;

private:
  enum Color { ORANGE, GREEN, YELLOW };
  SDL_Surface * getSprite(int x, int y, int width, int height, SDL_Surface * sprites);
  SDL_Surface * screen;
  SDL_Surface * background;
  SDL_Surface * knob[21];
  SDL_Surface * sliders[3];
  SDL_Surface * button[2];
  SDL_Surface * light[2];
  SDL_Surface * handle;
  SDL_Surface * bender;
  SDL_Surface * pianoKeys[7][2];
  SH101 * sh101;
};

#endif //VIEW_H

#ifndef XBLAGGER_H
#define XBLAGGER_H

struct SDL_Renderer;
struct SDL_Window;
class Level;

class XBlagger
{
public:
  XBlagger();
  ~XBlagger();

private:
    void readKeys();
    void start();

    SDL_Renderer* renderer;
    SDL_Window* window;

    Level* level;

    bool run;
};

#endif

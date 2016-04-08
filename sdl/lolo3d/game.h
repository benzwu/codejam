#ifndef GAME_H
#define GAME_H

#include <glm/glm.hpp>

using glm::mat4;

struct SDL_Window;
class Level;

class Game
{
public:
	Game(SDL_Window * window);
    ~Game();
    void run();

private:
	SDL_Window * window;
	mat4 projectionMatrix;
	Level * level;
};

#endif
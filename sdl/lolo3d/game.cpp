#include "game.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "level.h"
#include <glm/gtc/matrix_transform.hpp>

Game::Game(SDL_Window * window) : window(window)
{
	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	projectionMatrix = glm::perspective(40.0f, float(width) / height, 0.1f, 100.0f);

	level = new Level("data/lolo1.map");

	run();
}

Game::~Game()
{
	delete level;
}

void Game::run()
{
    bool run = true;
    SDL_Event event;
	GLint polyMode[2];

    do {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                SDL_Keycode sym = event.key.keysym.sym;

                if (sym == SDLK_w) {
					glGetIntegerv(GL_POLYGON_MODE, polyMode);
                    glPolygonMode(GL_FRONT_AND_BACK, polyMode[0] == GL_FILL ? GL_LINE : GL_FILL);
                }
                if (sym == SDLK_q)
                    run = false;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		level->render(projectionMatrix);

		SDL_GL_SwapWindow(window);
        SDL_Delay(1000 / 30);
    } while (run);
}

#include <SDL2/SDL.h>
#include "glsl_utils.h"
#include "game.h"

int main(int argc, char * argv[])
{
	int width = 800;
	int height = 600;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window * window = SDL_CreateWindow("Lolo 3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	glewInit();
	GLSLUtils::installShaders();

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);

	Game game(window);

	SDL_GL_DeleteContext(glContext);
	SDL_Quit();
	return 0;
}
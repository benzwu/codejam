#include "xblagger.h"

#include "datamanager.h"
#include <SDL.h>
#include <SDL_mixer.h>

XBlagger::XBlagger()
{
<<<<<<< HEAD:cpp/xblagger/xblagger.cpp
    int scale = 3;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(320 * scale, 200 * scale, SDL_WINDOW_OPENGL, &window, &renderer);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024);
=======
	float scale = 2;
	int width = int(320 * scale);
	int height = int(200 * scale);

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_OPENGL, &window, &renderer);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
>>>>>>> d23ad25db0a227419939857de80694b7e4e1dbd8:sdl/xblagger/xblagger.cpp

    DataManager::getInstance().init(renderer);
    level = DataManager::getInstance().getLevel("the bank");

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
<<<<<<< HEAD:cpp/xblagger/xblagger.cpp
    SDL_RenderSetScale(renderer, scale, scale);
=======
	SDL_RenderSetScale(renderer, scale, scale);
>>>>>>> d23ad25db0a227419939857de80694b7e4e1dbd8:sdl/xblagger/xblagger.cpp

    start();
}

XBlagger::~XBlagger()
{
    delete level;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void XBlagger::readKeys()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_LEFT:
                level->moveBlagger(Object::Direction::LEFT);
                break;
            case SDLK_RIGHT:
                level->moveBlagger(Object::Direction::RIGHT);
                break;
            case SDLK_q:
                run = false;
                break;
            }
        }
        if (event.type == SDL_KEYUP)
            level->stopBlagger();
    }
}

void XBlagger::start()
{
    run = true;
    while (run) {
        readKeys();
    
        SDL_RenderClear(renderer);
        level->render(); // TODO should be consistent. pass the renderer here
        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / 60);
    }
}

int main(int argc, char* argv[])
{
    XBlagger();
    return 0;
}

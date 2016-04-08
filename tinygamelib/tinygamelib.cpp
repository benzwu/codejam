#include "tinygamelib.h"

#include <SDL.h>
#include <SDL_mixer.h>

#include "types.h"

TinyGameLibrary::TinyGameLibrary()
{
    spriteSheet = NULL;
}

TinyGameLibrary::~TinyGameLibrary()
{
    for (auto& it : this->sounds) {
        Mix_Chunk* sample = it.second;
        Mix_FreeChunk(sample);
    }
    
    SDL_DestroyTexture(spriteSheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();
}

void TinyGameLibrary::init(int width, int height, float scale)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_OPENGL, &window, &renderer)) {
        fprintf(stderr, "Unable to create SDL window or renderer: %s\n", SDL_GetError());
        exit(1);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024)) {
        fprintf(stderr, "Unable to initialize SDL audio: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderSetScale(renderer, scale, scale);
}

void TinyGameLibrary::readKeys()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    break;
                case SDLK_RIGHT:
                    break;
                case SDLK_q:
                    run = false;
                    break;
            }
        }
        if (event.type == SDL_KEYUP) {
        }
    }
}

void TinyGameLibrary::mainLoop()
{
    run = true;

    while (run)
    {   
        readKeys();
        SDL_RenderClear(renderer);
        drawLevel();
        //drawSprite("blagger", 50, 50);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
}

void TinyGameLibrary::drawObject(int x, int y, const TGL_Object& object)
{
    TGL_SpriteSheetMap map = object.spriteSheetMap;

    SDL_Rect srcRect = { map.x, map.y, map.width, map.height };
    SDL_Rect dstRect = { x, y, map.width, map.height };

    int frame = 0;
    srcRect.x += frame * map.width;
    
    SDL_RenderCopy(renderer, spriteSheet, &srcRect, &dstRect);
}

void TinyGameLibrary::setSpriteSheet(const string& filename)
{
    if (spriteSheet) SDL_DestroyTexture(spriteSheet);

    SDL_Surface* temp = SDL_LoadBMP(filename.c_str());
    SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, 255, 0, 255));
    spriteSheet = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
}

void TinyGameLibrary::playSound(const TGL_Sound& sound)
{
    Mix_PlayChannel(0, sound.data, 0);
}

TGL_Sound TinyGameLibrary::setSound(const string& filename, bool loop)
{
    TGL_Sound sound;
    sound.data = Mix_LoadWAV(filename.c_str());
    sound.loop = loop;
    
    return sound;
}

void TinyGameLibrary::drawLevel()
{
    for (int i = 0; i < level.data.size(); i++) {
        for (int j = 0; j < level.data.at(i).size(); j++) {
            char tile = level.data.at(i).at(j);
            if (tile == 0) continue;

            int x = j * tileSize;
            int y = i * tileSize;
            SDL_Rect srcRect = { 0, 71, tileSize, tileSize };
            SDL_Rect dstRect = { x, y, tileSize, tileSize };

            SDL_RenderCopy(renderer, tileSet, &srcRect, &dstRect);
        }
    }
}

void TinyGameLibrary::setLevel(const string& name, const vector<vector<char>>& data)
{
    level.name = name;
    level.data = data;
}
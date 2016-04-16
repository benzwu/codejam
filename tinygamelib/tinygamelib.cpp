#include "tinygamelib.h"

#include <SDL.h>
#include <SDL_mixer.h>

TinyGameLibrary::TinyGameLibrary()
{
    spriteSheet = NULL;
}

TinyGameLibrary::~TinyGameLibrary()
{
    /*for (auto& it : this->sounds) {
        Mix_Chunk* sampleB = it.second;
        Mix_FreeChunk(sample);
    }*/
    
    SDL_DestroyTexture(spriteSheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();
}

void TinyGameLibrary::init(int width, int height, int scale)
{
    width *= scale;
    height *= scale;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_OPENGL, &window, &renderer)) {
        fprintf(stderr, "Unable to create SDL window or renderer: %s\n", SDL_GetError());
        exit(1);
    }

    /*if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024)) {
        fprintf(stderr, "Unable to initialize SDL audio: %s\n", SDL_GetError());
        exit(1);
    }*/

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderSetScale(renderer, (float)scale, (float)scale);
}

void TinyGameLibrary::readKeys()
{
    TGL_Object& player = levels.at(levelNum).layer1.at(playerId);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym)
            {
                case SDLK_q:
                    run = false;
                    break;
                case SDLK_LEFT:
                    player.direction = TGL_Direction::LEFT;
                    player.state = TGL_State::WALKING;
                    break;
                case SDLK_RIGHT:
                    player.direction = TGL_Direction::RIGHT;
                    player.state = TGL_State::WALKING;
                    break;
                case SDLK_UP:
                    player.direction = TGL_Direction::UP;
                    player.state = TGL_State::WALKING;
                    break;
                case SDLK_DOWN:
                    player.direction = TGL_Direction::DOWN;
                    player.state = TGL_State::WALKING;
                    break;
            }
        }
        if (event.type == SDL_KEYUP) player.state = TGL_State::STOPPED;
    }
}

bool TinyGameLibrary::hitTest(TGL_Object& object, int deltaX, int deltaY)
{
    return false;

    TGL_SpriteSheetCoord& coord = objectDefinitions.at(object.definitionId).spriteSheetCoord;
    int x = (object.x + coord.width / 2) / tileWidth + deltaX;
    int y = (object.y + coord.height / 2) / tileHeight + deltaY;
    
    int width = levels.at(levelNum).layer0.size();
    int height = levels.at(levelNum).layer0.at(0).size();
    if (x < 0 || x >= width) return true;
    if (y < 0 || y >= height) return true;

    TGL_Id id = levels.at(levelNum).layer0.at(y).at(x);
    TGL_ObjectType type = objectDefinitions.at(id).type;
    return type == TGL_ObjectType::BLOCK;
}

#include <algorithm>
#include <iostream>

void TinyGameLibrary::moveObjects()
{
    int speed = 1;

    if (playerId != -1) {
        TGL_Object& player = levels.at(levelNum).layer1.at(playerId);
        TGL_SpriteSheetCoord& coord = objectDefinitions.at(player.definitionId).spriteSheetCoord;

        int x = (player.x + coord.width / 2 - tileWidth / 2) / tileWidth;

        if (player.state == TGL_State::WALKING) {
            if (player.direction == TGL_Direction::LEFT) x--;
            if (player.direction == TGL_Direction::RIGHT) x++;
        }

        if (player.direction == TGL_Direction::LEFT) player.x = max(player.x - speed, x * tileWidth);
        if (player.direction == TGL_Direction::RIGHT) player.x = min(player.x + speed, x * tileWidth);
    }
}

void TinyGameLibrary::mainLoop()
{
    run = true;
    while (run) {
        SDL_RenderClear(renderer);
        readKeys();
        moveObjects();
        drawLevel();
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 30);
    }
}

void TinyGameLibrary::drawSprite(TGL_Id id, int x, int y)
{
    TGL_ObjectDefinition& objectDefinition = objectDefinitions.at(id);
    TGL_SpriteSheetCoord& coord = objectDefinition.spriteSheetCoord;

    SDL_Rect srcRect = { coord.x, coord.y, coord.width, coord.height };
    SDL_Rect dstRect = { x, y, coord.width, coord.height };

    srcRect.x += coord.frame * coord.width;
    coord.frame = (coord.frame + 1) % coord.totalFrames;
    
    SDL_RenderCopy(renderer, spriteSheet, &srcRect, &dstRect);
}

void TinyGameLibrary::setSpriteSheet(string filename)
{
    SDL_DestroyTexture(spriteSheet);
    SDL_Surface* temp = SDL_LoadBMP(filename.c_str());
    SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, 255, 0, 255));
    spriteSheet = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
}

void TinyGameLibrary::setObjectDefinitions(vector<TGL_ObjectDefinition> objectDefinitionList)
{
    for (auto& object : objectDefinitionList) {
        TGL_Id id = object.id;
        objectDefinitions[id] = object;
    }
}

TGL_Sound TinyGameLibrary::loadSound(string filename, int channel)
{
    TGL_Sound sound;
    sound.channel = channel;
    sound.loop = false;
    sound.data = Mix_LoadWAV(filename.c_str());
    return sound;
}

void TinyGameLibrary::playSound(TGL_Sound& sound)
{
    int loop = sound.loop ? -1 : 0;
    Mix_PlayChannel(sound.channel, sound.data, loop);
}

void TinyGameLibrary::drawLevel()
{
    TGL_Level& level = levels.at(levelNum);

    for (unsigned int i = 0; i < level.layer0.size(); i++) {
        for (unsigned int j = 0; j < level.layer0.at(i).size(); j++) {
            TGL_Id id = level.layer0.at(i).at(j);
            int x = j * tileWidth;
            int y = i * tileHeight;

            drawSprite(id, x, y);
        }
    }

    for (unsigned int i = 0; i < level.layer1.size(); i++) {
        TGL_Object& object = level.layer1.at(i);

        drawSprite(object.definitionId, object.x, object.y);
    }
}

void TinyGameLibrary::setLevels(const vector<TGL_Level>& levelList, int tw, int th)
{
    levels = levelList;
    tileWidth = tw;
    tileHeight = th;

    for (unsigned int i = 0; i < levels.size(); i++) {
        TGL_Level& level = levels.at(i);
        for (unsigned int j = 0; j < level.layer1.size(); j++) {
            TGL_Object& object = level.layer1.at(j);
            TGL_Id id = object.definitionId;
            TGL_SpriteSheetCoord& coord = objectDefinitions.at(id).spriteSheetCoord;
            int x = object.x * tileWidth + tileWidth / 2 - coord.width / 2;
            int y = object.y * tileHeight + tileHeight / 2 - coord.height / 2;

            object.x = x;
            object.y = y;
        }
    }

    setLevel(0);
}

void TinyGameLibrary::setLevel(int num)
{
    levelNum = num;
    playerId = -1;

    TGL_Level& level = levels.at(levelNum);
    for (unsigned int i = 0; i < level.layer1.size(); i++) {
        TGL_Id id = level.layer1.at(i).definitionId;
        TGL_ObjectDefinition& objDef = objectDefinitions.at(id);
        
        if (objDef.type == TGL_ObjectType::PLAYER) {
            playerId = i;
            break;
        }
    }
}
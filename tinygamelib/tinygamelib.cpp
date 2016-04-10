#include "tinygamelib.h"

#include <SDL.h>
#include <SDL_mixer.h>

TinyGameLibrary::TinyGameLibrary()
{
    playerDirection = TGL_Direction::STOP;
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
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym)
            {
                case SDLK_q:
                    run = false;
                    break;
                case SDLK_LEFT:
                    playerDirection = TGL_Direction::LEFT;
                    break;
                case SDLK_RIGHT:
                    playerDirection = TGL_Direction::RIGHT;
                    break;
                case SDLK_UP:
                    playerDirection = TGL_Direction::UP;
                    break;
                case SDLK_DOWN:
                    playerDirection = TGL_Direction::DOWN;
                    break;
            }
        }
        if (event.type == SDL_KEYUP) playerDirection = TGL_Direction::STOP;
    }
}

bool TinyGameLibrary::hitTest(TGL_Position& position, int deltaX, int deltaY)
{
    TGL_SpriteSheetCoord& coord = objects.at(position.objectId).spriteSheetCoord;
    int x = (position.x + coord.width / 2) / tileWidth + deltaX;
    int y = (position.y + coord.height / 2) / tileHeight + deltaY;
    
    int width = levels.at(levelNum).layer0.size();
    int height = levels.at(levelNum).layer0.at(0).size();
    if (x < 0 || x >= width) return true;
    if (y < 0 || y >= height) return true;

    TGL_Id id = levels.at(levelNum).layer0.at(y).at(x);
    TGL_ObjectType type = objects.at(id).type;
    return type == TGL_ObjectType::BLOCK;
}

void TinyGameLibrary::moveObjects()
{
    int speed = 2;

    if (playerRef != -1) {
        TGL_Position& playerPosition = levels.at(levelNum).layer1.at(playerRef);

        if (playerDirection == TGL_Direction::LEFT && !hitTest(playerPosition, -1, 0)) playerPosition.x -= speed;
        if (playerDirection == TGL_Direction::RIGHT && !hitTest(playerPosition, 1, 0)) playerPosition.x += speed;
        if (playerDirection == TGL_Direction::UP && !hitTest(playerPosition, 0, -1)) playerPosition.y -= speed;
        if (playerDirection == TGL_Direction::DOWN && !hitTest(playerPosition, 0, 1)) playerPosition.y += speed;
    }
}

void TinyGameLibrary::mainLoop()
{
    run = true;

    while (run) {
        readKeys();
        SDL_RenderClear(renderer);
        moveObjects();
        drawLevel();
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 30);
    }
}

void TinyGameLibrary::drawSprite(TGL_Id id, int x, int y)
{
    TGL_Object& object = objects.at(id);
    TGL_SpriteSheetCoord& coord = object.spriteSheetCoord;

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

void TinyGameLibrary::setObjects(vector<TGL_Object> objectList)
{
    for (auto& object : objectList) {
        TGL_Id id = object.id;
        objects[id] = object;
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
            int x = tileWidth * j;
            int y = tileHeight * i;

            drawSprite(id, x, y);
        }
    }

    for (unsigned int i = 0; i < level.layer1.size(); i++) {
        TGL_Position& position = level.layer1.at(i);
        TGL_Id id = position.objectId;

        drawSprite(id, position.x, position.y);
    }
}

void TinyGameLibrary::setLevels(vector<TGL_Level> levelList, int tw, int th)
{
    levels = levelList;
    tileWidth = tw;
    tileHeight = th;

    for (unsigned int i = 0; i < levels.size(); i++) {
        TGL_Level& level = levels.at(i);
        for (unsigned int j = 0; j < level.layer1.size(); j++) {
            TGL_Position& position = level.layer1.at(j);
            TGL_Id id = position.objectId;
            TGL_SpriteSheetCoord& coord = objects.at(id).spriteSheetCoord;
            int x = tileWidth * position.x - coord.width / 2 + tileWidth / 2;
            int y = tileHeight * position.y - coord.height / 2 + tileHeight / 2;

            position.x = x;
            position.y = y;
        }
    }

    setLevel(0);
}

void TinyGameLibrary::setLevel(int num)
{
    levelNum = num;
    playerRef = -1;

    TGL_Level& level = levels.at(levelNum);
    for (unsigned int i = 0; i < level.layer1.size(); i++) {
        TGL_Position& position = level.layer1.at(i);
        TGL_Object& object = objects.at(position.objectId);
        
        if (object.type == TGL_ObjectType::PLAYER) {
            playerRef = i;
            break;
        }
    }
}
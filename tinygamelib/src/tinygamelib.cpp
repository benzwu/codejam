#include "tinygamelib.h"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

//#include <algorithm>

TinyGameLibrary::TinyGameLibrary(int width, int height, int scale)
{
    width *= scale;
    height *= scale;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cerr << "Unable to initialize SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_OPENGL, &m_window, &m_renderer)) {
        cerr << "Unable to create SDL window or renderer: " << SDL_GetError() << endl;
        exit(1);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024)) {
        cerr << "Unable to initialize SDL audio: " << SDL_GetError() << endl;
        exit(1);
    }

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderSetScale(m_renderer, (float)scale, (float)scale);

    m_spriteSheet = nullptr;
}

TinyGameLibrary::~TinyGameLibrary()
{
    for (const auto& sound : m_sounds) {
        Mix_Chunk* data = sound.second.data;
        Mix_FreeChunk(data);
    }
    
    if (m_spriteSheet != nullptr) SDL_DestroyTexture(m_spriteSheet);

    Mix_CloseAudio();
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void TinyGameLibrary::readKeys()
{
    //TGL_Object& player = levels.at(levelNum).layer1.at(playerId);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case ::SDLK_ESCAPE:
                    m_run = false;
                    break;
                /*case SDLK_LEFT:
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
                    break;*/
            }
        }
        //if (event.type == SDL_KEYUP) player.state = TGL_State::STOPPED;
    }
}


/*bool TinyGameLibrary::hitTest(TGL_Object& object, int deltaX, int deltaY)
{
    if (deltaX == 0 && deltaY == 0) return true;

    TGL_SpriteSheetCoord& coord = objectDefinitions.at(object.definitionId).spriteSheetCoord;
    int x = object.x + deltaX;
    int y = object.y + deltaY;
    
    int width = levels.at(levelNum).layer0.size() * tileWidth;
    int height = levels.at(levelNum).layer0.at(0).size() * tileHeight;
    if (x < 0 || x + coord.width > width) return true;
    if (y < 0 || y + coord.height > height) return true;

    return false;
    float x2 = (float)(object.x + coord.width / 2) / tileWidth;
    float y2 = (float)(object.y + coord.height / 2) / tileHeight;
    if (deltaX != 0) x2 = (deltaX < 0) ? floor(x2 - 1) : ceil(x2 - 0); else x2 = floor(x2);
    if (deltaY != 0) y2 = (deltaY < 0) ? floor(y2 - 1) : ceil(y2 - 0); else y2 = floor(y2);
    TGL_Id id = levels.at(levelNum).layer0.at((int)y2).at((int)x2);
    TGL_ObjectType type = objectDefinitions.at(id).type;
    return type == TGL_ObjectType::BLOCK;
}*/

void TinyGameLibrary::moveObjects()
{
    m_player->x++;

    /*int speed = 2;

    if (playerId != -1) {
        TGL_Object& player = levels.at(levelNum).layer1.at(playerId);

        if (player.state == TGL_State::WALKING) {
            TGL_SpriteSheetCoord& coord = objectDefinitions.at(player.definitionId).spriteSheetCoord;
            float x = (float)(player.x + coord.width / 2 - tileWidth / 2) / tileWidth;
            float y = (float)(player.y + coord.height / 2 - tileHeight / 2) / tileHeight;
            if (player.direction == TGL_Direction::LEFT) player.xTo = (int)(ceil(x) - 1) * tileWidth;
            else if (player.direction == TGL_Direction::RIGHT) player.xTo = (int)(floor(x) + 1) * tileWidth;
            else if (player.direction == TGL_Direction::UP) player.yTo = (int)(ceil(y) - 1) * tileHeight;
            else if (player.direction == TGL_Direction::DOWN) player.yTo = (int)(floor(y) + 1) * tileHeight;
        }

        int xDelta = player.xTo - player.x;
        int yDelta = player.yTo - player.y;
        int x = (xDelta < 0) ? max(-speed, xDelta) : min(speed, xDelta);
        int y = (yDelta < 0) ? max(-speed, yDelta) : min(speed, yDelta);
        if (!hitTest(player, x, 0)) player.x += x;
        if (!hitTest(player, 0, y)) player.y += y;
    }*/
}

void TinyGameLibrary::mainLoop()
{
    m_run = true;
    while (m_run) {
        SDL_RenderClear(m_renderer);
        readKeys();
        moveObjects();
        renderLevel();
        SDL_RenderPresent(m_renderer);
        SDL_Delay(1000 / 60);
    }
}

void TinyGameLibrary::renderSprite(TGL_Id id, int x, int y)
{
    if (m_objectDefinitions.count(id) == 0) return;

    const TGL_ObjectDefinition& def = m_objectDefinitions.at(id);
    const TGL_SpriteSheetCoord& coord = def.spriteSheetCoord;

    SDL_Rect srcRect = { coord.x, coord.y, coord.width, coord.height };
    SDL_Rect dstRect = { x, y, coord.width, coord.height };

    //srcRect.x += coord.frame * coord.width;
    //coord.frame = (SDL_GetTicks() / 50) % coord.totalFrames;
    
    SDL_RenderCopy(m_renderer, m_spriteSheet, &srcRect, &dstRect);
}

void TinyGameLibrary::loadSpriteSheet(const string& filename, const vector<TGL_ObjectDefinition>& spriteDefinitions)
{
    SDL_Color opaque = { 255, 0, 255 };
    SDL_DestroyTexture(m_spriteSheet);
    SDL_Surface* temp = IMG_Load(filename.c_str());
    if (temp == nullptr) {
        cerr << "Unable to load spritesheet: " << filename << endl;
        return;
    }
    SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, opaque.r, opaque.g, opaque.b));
    m_spriteSheet = SDL_CreateTextureFromSurface(m_renderer, temp);
    SDL_FreeSurface(temp);

    m_objectDefinitions.clear();
    for (const TGL_ObjectDefinition& def : spriteDefinitions) {
        TGL_Id id = def.id;
        m_objectDefinitions[id] = def;
    }
}

void TinyGameLibrary::loadSound(TGL_Id id, const string& filename, int channel)
{
    TGL_Sound sound;
    sound.channel = channel;
    sound.loop = false;
    sound.data = Mix_LoadWAV(filename.c_str());
    if (sound.data == nullptr) {
        cerr << "Unable to load sound: " << filename << endl;
        return;
    }
    m_sounds[id] = sound;
}

void TinyGameLibrary::playSound(TGL_Id id)
{
    if (m_sounds.count(id) == 0) return;

    TGL_Sound& sound = m_sounds[id];
    int loop = sound.loop ? -1 : 0;
    Mix_PlayChannel(sound.channel, sound.data, loop);
}

void TinyGameLibrary::renderLevel()
{
    if (m_levelAt >= m_levels.size()) return;

    const TGL_Level& level = m_levels.at(m_levelAt);

    for (unsigned int i = 0; i < level.layer0.size(); i++) {
        for (unsigned int j = 0; j < level.layer0.at(i).size(); j++) {
            TGL_Id id = level.layer0.at(i).at(j);
            int x = j * m_tileWidth;
            int y = i * m_tileHeight;

            renderSprite(id, x, y);
        }
    }

    for (const TGL_Object& object : level.layer1) {
        renderSprite(object.definitionId, object.x, object.y);
    }
}

void TinyGameLibrary::setLevels(const vector<TGL_Level>& levels, int tileWidth, int tileHeight)
{
    m_levels = levels;
    m_tileWidth = tileWidth;
    m_tileHeight = tileHeight;

    for (TGL_Level& level : m_levels) {
        for (TGL_Object& object : level.layer1) {
            TGL_Id id = object.definitionId;
            const TGL_SpriteSheetCoord& coord = m_objectDefinitions.at(id).spriteSheetCoord;
            int x = object.x * tileWidth + tileWidth / 2 - coord.width / 2;
            int y = object.y * tileHeight + tileHeight / 2 - coord.height / 2;

            object.x = x;
            object.y = y;
            //object.xTo = x;
            //object.yTo = y;
        }
    }

    setLevel(0);
}

void TinyGameLibrary::setLevel(int levelAt)
{
    m_levelAt = levelAt;
    m_player = nullptr;

    TGL_Level& level = m_levels.at(m_levelAt);
    for (TGL_Object& object : level.layer1) {
        TGL_Id id = object.definitionId;
        TGL_ObjectDefinition& def = m_objectDefinitions.at(id);
        
        if (def.type == TGL_ObjectType::Player) {
            m_player = &object;
            break;
        }
    }
}

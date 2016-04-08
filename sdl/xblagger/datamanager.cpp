#include "datamanager.h"

#include <SDL.h>
#include <SDL_mixer.h>
#include <tinyxml2.h>
#include <string>
#include <sstream>

using namespace std;
using namespace tinyxml2;

DataManager& DataManager::getInstance()
{
    static DataManager instance;
    return instance;
}

DataManager::DataManager()
{
    spriteSheet = NULL;
    renderer = NULL;
}

DataManager::~DataManager()
{
    if (spriteSheet)
        SDL_DestroyTexture(spriteSheet);

    for (auto& kv : sounds)
        Mix_FreeChunk(kv.second);
}

void DataManager::init(SDL_Renderer* renderer)
{
    this->renderer = renderer;
    loadSpriteSheet();
    loadSprite(Object::Type::BRICK, SDL_Rect{ 0, 71, 8, 8 });
    loadSprite(Object::Type::KEY, SDL_Rect{ 24, 71, 8, 8 });
    loadSprite(Object::Type::CACTUS, SDL_Rect{ 48, 71, 8, 8 });
    loadSprite(Object::Type::TRAVELATOR_LEFT, SDL_Rect{ 0, 96, 8, 8 }, 8);
    loadSprite(Object::Type::BLAGGER, SDL_Rect{ 0, 0, 24, 16 }, 8);
    loadSprite(Object::Type::CROSS, SDL_Rect{ 56, 71, 8, 8 });
    loadSprite(Object::Type::METAL_FLOOR, SDL_Rect{ 104, 87, 8, 8 });
    loadSprite(Object::Type::SINKING_FLOOR, SDL_Rect{ 128, 87, 8, 8 }, 8);
    loadSprite(Object::Type::TRAVELATOR_RIGHT, SDL_Rect{ 0, 104, 8, 8 }, 8);
    loadSprite(Object::Type::WALL_TOP, SDL_Rect{ 88, 87, 8, 8 });
    loadSprite(Object::Type::WALL_CENTER, SDL_Rect{ 96, 87, 8, 8 });
    loadSprite(Object::Type::WALL_BOTTOM, SDL_Rect{ 80, 87, 8, 8 });
    loadSound("data/hit.wav");
    loadSound("data/key.wav");
}

Level* DataManager::getLevel(string name)
{
    int gridSize = 8;

    XMLDocument doc;
    doc.LoadFile("data/levels.xml");

    XMLElement* element = doc.FirstChildElement("package")->FirstChildElement("levels")->FirstChildElement("level");
    int width = element->IntAttribute("width");
    int height = element->IntAttribute("height");
    //string name = element->Attribute("name");

    Level* level = new Level();
    level->width = width;
    level->height = height;
    level->name = name;

    stringstream ss;
    ss << hex << element->GetText();
    int i = 0;
    int j = 0;
    int value;
    while (ss >> value) {
        int x = i * gridSize;
        int y = j * gridSize;
        int type = getBits(value, 0, 5);
        switch (type) {
            case 0x01: level->objects.push_back(new Object(x, y, Object::Type::BRICK)); break;
            case 0x02: level->objects.push_back(new Object(x, y, Object::Type::KEY)); break;
            case 0x03: level->objects.push_back(new Object(x, y, Object::Type::CACTUS)); break;
            case 0x04: level->objects.push_back(new Object(x, y, Object::Type::TRAVELATOR_LEFT)); break;
            case 0x05: level->blagger = new Blagger(x, y); break;
            case 0x06: level->objects.push_back(new Object(x, y, Object::Type::CROSS)); break;
            case 0x07: level->objects.push_back(new Object(x, y, Object::Type::METAL_FLOOR)); break;
            case 0x08: level->objects.push_back(new Object(x, y, Object::Type::SINKING_FLOOR)); break;
            case 0x09: level->objects.push_back(new Object(x, y, Object::Type::TRAVELATOR_RIGHT)); break;
            case 0x0A: level->objects.push_back(new Object(x, y, Object::Type::WALL_TOP)); break;
            case 0x0B: level->objects.push_back(new Object(x, y, Object::Type::WALL_CENTER)); break;
            case 0x0C: level->objects.push_back(new Object(x, y, Object::Type::WALL_BOTTOM)); break;
        }
        i++;
        if (i >= width) {
            i = 0;
            j++;
        }
    }

    return level;
}

void DataManager::loadSpriteSheet()
{
    if (spriteSheet) return;

    SDL_Surface* temp = SDL_LoadBMP("data/sprites.bmp");
    SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, 255, 0, 255));
    spriteSheet = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
}

void DataManager::loadSprite(Object::Type type, SDL_Rect rect, int frames)
{
    if (sprites.count(type)) return;
    
    for (int i = 0; i < frames; i++)
    {
        sprites[type].push_back(rect);
        rect.x += rect.w;
    }
}

void DataManager::renderSprite(int x, int y, Object::Type type, int frame, bool flip)
{
    if (!spriteSheet || !sprites.count(type))
        return;

    int fps = 20;
    if (frame == -1)
        frame = SDL_GetTicks() / (1000 / fps);

    int idx = frame % sprites[type].size();
    SDL_Rect srcRect = sprites[type].at(idx);
    SDL_Rect dstRect;
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = srcRect.w;
    dstRect.h = srcRect.h;

    SDL_RenderCopyEx(renderer, spriteSheet, &srcRect, &dstRect, 0, NULL, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void DataManager::loadSound(string filename)
{
    if (!sounds.count(filename))
        sounds[filename] = Mix_LoadWAV(filename.c_str());
}

void DataManager::playSound(string filename)
{
    if (sounds.count(filename))
        Mix_PlayChannel(0, sounds[filename], 0);
}

int DataManager::getBits(int value, int from, int to)
{
    if (to < 0) to = from;
    return value >> from & (1 << (to - from + 1)) - 1;
}

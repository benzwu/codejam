#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "object.h"
#include "level.h"
#include <map>
#include <vector>

using namespace std;

struct SDL_Texture;
struct Mix_Chunk;
struct SDL_Renderer;
struct SDL_Rect;

class DataManager
{
public:
    static DataManager& getInstance();

    void init(SDL_Renderer* renderer);
    Level* getLevel(string name);
    void playSound(string filename);
    void renderSprite(int x, int y, Object::Type type, int frame = -1, bool flip = false);

private:
    DataManager();
    ~DataManager();

    void loadSpriteSheet();
    void loadSprite(Object::Type type, SDL_Rect rect, int frames = 1);
    void loadSound(string filename);
    int getBits(int value, int from, int to = -1);

    SDL_Renderer* renderer;
    SDL_Texture* spriteSheet;
    map<Object::Type, vector<SDL_Rect>> sprites;
    map<string, Mix_Chunk*> sounds;
};

#endif
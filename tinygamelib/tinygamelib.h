#ifndef TINYGAMELIB_H
#define TINYGAMELIB_H

#include <map>
#include <string>
#include <vector>

using namespace std;

struct Mix_Chunk;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Window;

typedef int TGL_Id;

enum class TGL_ObjectType {
    FLOOR,
    BLOCK,
    PLAYER,
    ENEMY
};

enum class TGL_Direction {
    STOP,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

typedef struct {
    int x;
    int y;
    int width;
    int height;
    int frame;
    int totalFrames;
} TGL_SpriteSheetCoord;

typedef struct {
    TGL_Id id;
    string name;
    TGL_SpriteSheetCoord spriteSheetCoord;
    TGL_ObjectType type;
} TGL_Object;

typedef struct {
    TGL_Id objectId;
    int x;
    int y;
} TGL_Position;

typedef struct {
    string name;
    vector<vector<TGL_Id>> layer0;
    vector<TGL_Position> layer1;
} TGL_Level;

typedef struct {
    int channel;
    bool loop;
    Mix_Chunk* data;
} TGL_Sound;

class TinyGameLibrary
{
public:
    TinyGameLibrary();
    ~TinyGameLibrary();
    void init(int width, int height, int scale = 1);
    void mainLoop();
    
    void setLevel(int num);
    void setLevels(vector<TGL_Level> levels, int tileWidth, int tileHeight);
    void setObjects(vector<TGL_Object> objects);
    void setSpriteSheet(string filename);

    TGL_Sound loadSound(string filename, int channel = 0);
    void playSound(TGL_Sound& sound);

private:
    void drawLevel();
    void drawSprite(TGL_Id id, int x, int y);
    void moveObjects();
    void readKeys();
    bool hitTest(TGL_Position& position, int deltaX, int deltaY);

    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Texture* spriteSheet;
    
    bool run;
    map<TGL_Id, TGL_Object> objects;

    int tileWidth;
    int tileHeight;
    int levelNum;
    vector<TGL_Level> levels;

    int playerRef;
    TGL_Direction playerDirection;
};

#endif
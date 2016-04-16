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
    DOWN,
    UP,
    LEFT,
    RIGHT  
};

enum class TGL_State {
    STOPPED,
    WALKING
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
} TGL_ObjectDefinition;

typedef struct {
    TGL_Id definitionId;
    int x;
    int y;
    TGL_Direction direction;
    TGL_State state;
} TGL_Object;

typedef struct {
    string name;
    vector<vector<TGL_Id>> layer0;
    vector<TGL_Object> layer1;
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
    void setLevels(const vector<TGL_Level>& levels, int tileWidth, int tileHeight);
    void setObjectDefinitions(vector<TGL_ObjectDefinition> objectDefinitions);
    void setSpriteSheet(string filename);

    TGL_Sound loadSound(string filename, int channel = 0);
    void playSound(TGL_Sound& sound);

private:
    void drawLevel();
    void drawSprite(TGL_Id id, int x, int y);
    void moveObjects();
    void readKeys();
    bool hitTest(TGL_Object& object, int deltaX, int deltaY);

    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Texture* spriteSheet;
    
    bool run;
    map<TGL_Id, TGL_ObjectDefinition> objectDefinitions;

    int tileWidth;
    int tileHeight;
    int levelNum;
    vector<TGL_Level> levels;

    TGL_Id playerId;
};

#endif
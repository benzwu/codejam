#ifndef TINYGAMELIB_H
#define TINYGAMELIB_H

#define DllExport __declspec(dllexport)

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
    Floor,
    Block,
    Player,
    Enemy,
    Background
};

enum class TGL_Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

enum class TGL_State {
    IDLE,
    WALK,
    JUMP
};

typedef struct {
    int x;
    int y;
    int width;
    int height;
    int frames;
} TGL_SpriteSheetCoord;

typedef struct {
    TGL_Id id;
    //string name;
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
    vector<vector<TGL_Id>> layer0; // background
    vector<TGL_Object> sprites; // sprites
} TGL_Level;

typedef struct {
    int channel;
    bool loop;
    Mix_Chunk* data;
} TGL_Sound;

class TinyGameLibrary
{
public:
    DllExport TinyGameLibrary(int width, int height, int scale);
    DllExport ~TinyGameLibrary();
    DllExport void mainLoop();
    
    void setLevel(int levelAt);
    DllExport void setLevels(const vector<TGL_Level>& levels, int tileWidth, int tileHeight);
    DllExport void loadSpriteSheet(const string& filename, const vector<TGL_ObjectDefinition>& spriteDefinitions);
    DllExport void loadSound(TGL_Id id, const string& filename, int channel = 0);
    DllExport void playSound(TGL_Id id);

private:
    void renderLevel();
    void moveObjects();
    void readKeys();
    void renderSprite(TGL_Id id, int x, int y);
    bool hitTest(const TGL_Object& objectA, const TGL_Object& objectB);

    SDL_Renderer* m_renderer;
    SDL_Window* m_window;
    SDL_Texture* m_spriteSheet;
    
    bool m_run;
    map<TGL_Id, TGL_Sound> m_sounds;
    map<TGL_Id, TGL_ObjectDefinition> m_objectDefinitions;

    int m_renderWidth;
    int m_renderHeight;
    int m_tileWidth;
    int m_tileHeight;
    unsigned int m_levelAt;
    vector<TGL_Level> m_levels;

    TGL_Object* m_player;
};

#endif
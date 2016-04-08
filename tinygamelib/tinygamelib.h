#ifndef TINYGAMELIB_H
#define TINYGAMELIB_H

#include <string>

using namespace std;

struct Mix_Chunk;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Window;

struct TGL_Id;
struct TGL_Level;
struct TGL_Objects;
struct TGL_Sound;

class TinyGameLibrary
{
public:
    TinyGameLibrary();
    ~TinyGameLibrary();
    void init(int width, int height, float scale = 1.0);
    void mainLoop();
    
    void drawObject(int x, int y, const TGL_Object& object);
    void setSpriteSheet(const string& filename);

    void drawLevel();
    void setLevel(const TGL_Level& level);

    void playSound(const TGL_Sound& sound);
    TGL_Sound setSound(const string& filename, bool loop = false);

private:
    void readKeys();
    SDL_Renderer* renderer;
    SDL_Window* window;
    
    bool run;
    
    SDL_Texture* spriteSheet;
    map<string, Mix_Chunk*> sounds;
};

#endif
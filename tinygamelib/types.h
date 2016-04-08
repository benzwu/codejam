#ifndef TGL_TYPES_H
#define TGL_TYPES_H

#include <map>
#include <string>
#include <vector>

typedef int TGL_Id;

typedef struct {
    int x;
    int y;
    int width;
    int height;
    int frames;
} TGL_SpriteSheetMap;

typedef struct {
    string name;
    int x;
    int y;
    TGL_SpriteSheetMap spriteSheetMap;
    bool lethal;
} TGL_Object;
//typedef map<TGL_Id, TGL_Object> TGL_Objects;

typedef struct {
    string name;
    vector<tuple<TGL_Id, int, int>> sprites;
    vector<vector<TGL_Id>> tiles;
} TGL_Level;
//typedef vector<TGL_Level> TGL_Levels;

//typedef map<string, string> TGL_Sounds;

typedef struct {
    int width;
    int height;
    int tileSize;
    string spriteSheet;
} TGL_GameData;

typedef struct {
    Mix_Chunk* data;
    bool loop;
} TGL_Sound;

#endif
#include "jumpinjackson.h"

Jumpinjackson::Jumpinjackson()
{
    /*int width = 320;
    int height = 200;
    int scale = 4;
    tgl.init(width, height, scale);

    tgl.setSpriteSheet("data/spritesheet.bmp");

    vector<TGL_ObjectDefinition> objectDefinitions = {
        { 0, "Water",{ {TGL_Direction::DOWN,{ 0, 0, 32, 24, 1 } } }, TGL_ObjectType::BLOCK }
        { 1, "Gray floor",{ 64, 96, 32, 24, 0, 1 } },
        { 2, "Pink switch",{ 192, 96, 32, 24, 0, 1 } },
        { 3, "Blue switch",{ 128, 96, 32, 24, 0, 1 } },
        { 4, "Yellow switch",{ 160, 96, 32, 24, 0, 1 } },
        { 5, "Gray chess floor",{ 96, 96, 32, 24, 0, 1 } },
        { 6, "Gray note floor",{ 0, 120, 32, 24, 0, 1 } },
        { 7, "Yellow turntable, stopped",{ 160, 120, 32, 24, 0, 1 } },
        { 8, "Pink turntable, stopped",{ 192, 120, 32, 24, 0, 1 } },
        { 9, "Jack",{ 0, 264, 56, 45, 0, 1 }, TGL_ObjectType::PLAYER },
        { 10, "Cymbal",{ 0, 439, 31, 30, 0, 6 }, TGL_ObjectType::ENEMY }
    };
    tgl.setObjectDefinitions(objectDefinitions);

    int tileWidth = 32;
    int tileHeight = 24;
    vector<TGL_Level> levels = {
        { "level 1",{
            { 0, 1, 1, 1, 6, 2, 0 },
            { 1, 7, 0, 1, 0, 3, 4 },
            { 1, 0, 0, 1, 0, 0, 1 },
            { 1, 1, 1, 5, 1, 1, 1 },
            { 1, 0, 0, 1, 0, 0, 1 },
            { 3, 4, 0, 1, 0, 8, 1 },
            { 0, 2, 6, 1, 1, 1, 0 }},
        {
            { 9, 3, 3 },
            { 10, 6, 3 }}
        },
        { "level 2",{
            { 0, 1, 1, 1, 6, 2, 0 },
            { 1, 7, 0, 1, 0, 3, 4 },
            { 1, 0, 0, 1, 0, 0, 1 },
            { 1, 1, 1, 5, 1, 1, 1 },
            { 1, 0, 0, 1, 0, 0, 1 },
            { 3, 4, 0, 1, 0, 8, 1 },
            { 0, 2, 6, 1, 1, 1, 0 }},
        {
            { 9, 3, 3 },
            { 10, 7, 4 }}
        }
    };
    tgl.setLevels(levels, tileWidth, tileHeight);

    TGL_Sound sound = tgl.loadSound("data/take_record.wav");
    tgl.playSound(sound);

    TGL_Sound music = tgl.loadSound("data/beat1.wav", 1);
    music.loop = true;
    tgl.playSound(music);

    tgl.mainLoop();*/
}

int main()
{
    Jumpinjackson();
}
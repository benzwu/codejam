#include "vampire.h"

Vampire::Vampire()
{
    TinyGameLibrary tgl(320, 200, 3);

    enum Object : TGL_Id { Hero, HUD, Floor };
    vector<TGL_ObjectDefinition> spriteDefinitions = {
        { Object::Hero,{ 0, 0, 16, 16, 1 }, TGL_ObjectType::Player },
        { Object::HUD,{ 0, 112, 254, 62, 1 }, TGL_ObjectType::Background },
        { Object::Floor,{ 0, 47, 16, 17, 1 }, TGL_ObjectType::Floor }
    };
    tgl.loadSpriteSheet("data/sprites.png", spriteDefinitions);

    vector<TGL_Level> levels = {
        {
            "Level 1",
            {
                { 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
                { 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
                { 2, -1,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
                { 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
                { 2, -1, -1, -1, -1,  2,  2, -1, -1, -1, -1, -1, -1, -1,  2, -1 },
                { 2, -1, -1, -1, -1, -1, -1, -1, -1, -1,  2, -1, -1, -1,  2, -1 },
                { 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
                { 2,  2,  2,  2,  2,  2,  2, -1, -1,  2,  2,  2,  2,  2,  2,  2 }
            },{
                { Object::HUD, 33, 137 },
                { Object::Hero, 85, 45, TGL_Direction::LEFT, TGL_State::IDLE }
            }
        }
    };
    tgl.setLevels(levels, 16, 17);

    tgl.mainLoop();
}

int main()
{
    Vampire();
}
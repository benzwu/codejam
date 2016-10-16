#include "flappybird.h"

#include <vector>

FlappyBird::FlappyBird()
{
    TinyGameLibrary tgl(320, 200, 3);

    enum Object : TGL_Id { Bird, Background };
    vector<TGL_ObjectDefinition> spriteDefinitions = {
        { Object::Bird, { 264, 64, 17, 12, 1 }, TGL_ObjectType::Player },
        { Object::Background,{ 0, 0, 144, 256, 1 }, TGL_ObjectType::Background }
    };
    tgl.loadSpriteSheet("data/sprites.png", spriteDefinitions);

    enum Sound : TGL_Id { Die, Hit, Point, Swooshing, Wing };
    tgl.loadSound(Sound::Die, "data/die.ogg");
    tgl.loadSound(Sound::Hit, "data/hit.ogg");
    tgl.loadSound(Sound::Point, "data/point.ogg");
    tgl.loadSound(Sound::Swooshing, "data/swooshing.ogg");
    tgl.loadSound(Sound::Wing, "data/wing.ogg");
    tgl.playSound(Sound::Hit);

    vector<TGL_Level> levels = {
        {
            "Level 1",
            {
                //{ 0, 0, 0 },
                //{ 0, 0, 0 },
                //{ 0, 0, 0 }
            }, {
                { Object::Background, 0, 0 },
                { Object::Bird, 1, 1, TGL_Direction::LEFT, TGL_State::IDLE }
            }
        }
    };
    tgl.setLevels(levels, 20, 20);

    tgl.mainLoop();
}

int main()
{
	FlappyBird();
}
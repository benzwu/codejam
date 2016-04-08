#include "blagger.h"

#include "gamedata.h"

Blagger::Blagger()
{
    tgl.init(GameData::width, GameData::height);
    tgl.setObjects(GameData::objects, GameData::spriteSheet);
    tgl.setSounds(GameData::sounds);

    setLevel(0);

    tgl.mainLoop();
}

void Blagger::setLevel(int level)
{
    tgl.setLevel(GameData::levels.at(level));
}

int main(int, char **)
{
    Blagger();
    return 0;
}
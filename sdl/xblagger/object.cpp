#include "object.h"

#include <SDL.h>
#include "datamanager.h"

void Object::render()
{
    DataManager::getInstance().renderSprite(x, y, type);
}

void Blagger::render()
{
    DataManager::getInstance().renderSprite(x, y, type, frame, direction == Direction::LEFT);

    if (moving)
    {
        if (direction == Object::Direction::LEFT)
            x--;
        if (direction == Object::Direction::RIGHT)
            x++;
        frame++;
        if (x % 8 == 5)
            DataManager::getInstance().playSound("data/hit.wav");
    }
}

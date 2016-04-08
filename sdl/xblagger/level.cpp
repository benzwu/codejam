#include "level.h"

#include "object.h"

Level::Level()
{
}

Level::~Level()
{
    delete blagger;
    objects.clear();
}

void Level::moveBlagger(Object::Direction direction)
{
    blagger->direction = direction;
    blagger->moving = true;
}

void Level::stopBlagger()
{
    blagger->moving = false;
}

void Level::render()
{
    for (Object* object : objects)
        object->render();
    blagger->render();
}

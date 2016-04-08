#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <string>

#include "object.h"

using namespace std;

class Blagger;

class Level
{
public:
    Level();
    ~Level();
    void moveBlagger(Object::Direction direction);
    void stopBlagger();
    void render();

    int width;
    int height;
    string name;
    vector<Object*> objects;
    Blagger* blagger;
};

#endif
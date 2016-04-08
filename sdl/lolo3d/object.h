#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include "object3d.h"

class Object : public Object3D
{
public:
    Object(float x, float y, float rot, const string & filename, const string & animation = "idle") : Object3D(filename)
    {
        position.x = x;
        position.y = y;
		rotation = rot;
        setAnimation(animation);
    }
};

class Tux : public Object
{
public:
    Tux(float x, float y, float rotation = 0) : Object(x, y, rotation, "data/tux.obj") {}
};

class Gol : public Object
{
public:
	Gol(float x, float y, float rotation = 0) : Object(x, y, rotation, "data/gol.obj") {}
};

class Rock : public Object
{
public:
	Rock(float x, float y, float rotation = 0) : Object(x, y, rotation, "data/rock.obj") {}
};

class Tree : public Object
{
public:
	Tree(float x, float y, float rotation = 0) : Object(x, y, rotation, "data/tree.obj") {}
};

#endif

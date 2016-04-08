#ifndef OBJECT_H
#define OBJECT_H

struct SDL_Renderer;

class Object
{
public:
    enum class Type
    {
        BRICK,
        KEY,
        CACTUS,
        TRAVELATOR_LEFT,
        BLAGGER,
        CROSS,
        METAL_FLOOR,
        SINKING_FLOOR,
        TRAVELATOR_RIGHT,
        WALL_TOP,
        WALL_CENTER,
        WALL_BOTTOM
    };

    enum class Direction
    {
        LEFT,
        RIGHT
    };

    Object(int x, int y, Type type) : x(x), y(y), type(type) {}
    virtual ~Object() {}
    virtual void render();

    Direction direction;
    bool moving = false;

protected:
    int x;
    int y;
    Type type;
};

class Blagger : public Object
{
public:
    Blagger(int x, int y) : Object(x, y, Type::BLAGGER) {}
    void render();

private:
    int frame;
};

#endif
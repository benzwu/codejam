#ifndef LEVEL_LOADER
#define LEVEL_LOADER

#include <string>

using namespace std;

class Level;

class LevelLoader
{
public:
    static void loadLevel(const string & filename, Level * level);

private:
    static int extractBits(int value, int from, int to = -1);
};

#endif

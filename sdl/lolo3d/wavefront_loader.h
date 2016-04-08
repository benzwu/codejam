#ifndef WAVEFRONT_LOADER
#define WAVEFRONT_LOADER

#include <string>

using namespace std;

class Object3D;

class WavefrontLoader
{
public:
    static void loadOBJ(const string & filename, Object3D * object);

private:
    static void loadANI(const string & filename, Object3D * object);
    static void loadMTL(const string & filename);
};

#endif
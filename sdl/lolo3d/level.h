#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <glm/glm.hpp>

using namespace std;
using glm::mat4;

class Object;

class Level
{
public:
	Level(const string & filename);
	void render(const mat4 & projectionMatrix);
	vector<Object*> objects;
	int width;
	int height;

private:
	mat4 viewMatrix;
};

#endif

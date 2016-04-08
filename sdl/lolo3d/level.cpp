#include "level.h"

#include <glm/gtc/matrix_transform.hpp>

#include "object.h"
#include "level_loader.h"

using glm::vec3;

Level::Level(const string & filename)
{
	LevelLoader::loadLevel(filename, this);

	double centerX = (width - 1) / 2.0;
	double centerY = (height - 1) / 2.0;
	viewMatrix = glm::lookAt(vec3(centerX, centerY, 18), vec3(centerX, centerY, 0), vec3(0, 1, 0));
	
	glClearColor(0, 0.44, 0.63, 1);
}

void Level::render(const mat4 & projectionMatrix)
{
	for (auto & object : objects)
	{
		object->render(viewMatrix, projectionMatrix);
	}
}

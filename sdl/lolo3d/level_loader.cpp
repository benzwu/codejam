#include "level_loader.h"

#include <tinyxml2.h>
#include <sstream>

#include "level.h"
#include "object.h"

using namespace tinyxml2;

void LevelLoader::loadLevel(const string & filename, Level * level)
{
	XMLDocument doc;
    XMLError status = doc.LoadFile(filename.c_str());
    if (status) return;
    
    XMLElement * element = doc.FirstChildElement("package")->FirstChildElement("level");
    level->width = element->IntAttribute("width");
    level->height = element->IntAttribute("height");

    stringstream ss;
    ss << hex << element->GetText();
    int x = 0;
    int y = level->height - 1;
    int value;
    while (ss >> value) {
        int layer1 = extractBits(value, 2, 4);
        int layer2 = extractBits(value, 5);
        int layer3 = extractBits(value, 6, 10);
        int direction = extractBits(value, 0, 1);

		switch (layer3)
		{
			case 1:
				level->objects.push_back(new Tux(x, y, 90 * direction));
				break;
			case 18:
				level->objects.push_back(new Rock(x, y));
				break;
			case 19:
				level->objects.push_back(new Tree(x, y));
				break;
		}

        x++;
        if (x >= level->width) {
            x = 0;
            y--;
        }
    }
}

int LevelLoader::extractBits(int value, int from, int to)
{
	if (to == -1) to = from;
    return (value >> from) & ((1 << to - from + 1) - 1);
}

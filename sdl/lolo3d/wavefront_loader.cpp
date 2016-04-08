#include "wavefront_loader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "object3d.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_opengl.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace std;
using glm::vec3;
using glm::vec4;

void WavefrontLoader::loadOBJ(const string & filename, Object3D * object)
{
    string group;
    vector<GLfloat> vertices;
    vector<GLfloat> normals;
    vector<GLfloat> textureCoords;
    GLsizei faceFirst;

    string prefix = filename.substr(0, filename.find_last_of("/")) + "/";
    string line;
    fstream file;
    file.open(filename.c_str());
    if (file.is_open()) {
        while (file.good()) {
            getline(file, line);
            istringstream iss(line);
            string type;
            iss >> type;

            if (type == "mtllib") {
                string filename;
                iss >> filename;
                filename = prefix + filename.substr(0, filename.find_last_of(".")) + ".mtl";
                loadMTL(filename);
            }
            if (type == "ani") {
                string filename;
                iss >> filename;
                filename = prefix + filename.substr(0, filename.find_last_of(".")) + ".ani";
                loadANI(filename, object);
            }
            if (type == "g" || type == "o") {
                group = "";
                while (iss.good()) {
                    string s;
                    iss >> s;
                    if (group != "") group += " ";
                    group += s;
                }

                faceFirst = 0;
                object->groups[group];
            }
            if (type == "usemtl") {
                string material;
                iss >> material;
                object->groups[group].material = material;
            }
            if (type == "v") {
                float x, y, z;
                iss >> x >> y >> z;
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
            if (type == "vn") {
                float x, y, z;
                iss >> x >> y >> z;
                normals.push_back(x);
                normals.push_back(y);
                normals.push_back(z);
            }
            if (type == "vt") {
                float u, v;
                iss >> u >> v;
                textureCoords.push_back(u);
                textureCoords.push_back(v);
            }
            if (type == "f") {
                GLsizei faceCount = 0;
                while (iss.good()) {
                    string index;
                    iss >> index;
                    istringstream iss2(index);
                    string tmp;
                    bool valid = false;

                    Object3D::Vertex v;

                    // position
                    if (getline(iss2, tmp, '/') && tmp != "") {
                        int vIdx;
                        istringstream(tmp) >> vIdx;
                        vIdx--;
                        v.position[0] = vertices[3 * vIdx];
                        v.position[1] = vertices[3 * vIdx + 1];
                        v.position[2] = vertices[3 * vIdx + 2];
                        valid = true;
                    }

                    // texcoord
                    if (getline(iss2, tmp, '/') && tmp != "") {
                        int tIdx;
                        istringstream(tmp) >> tIdx;
                        tIdx--;
                        v.textureCoord[0] = textureCoords[2 * tIdx];
                        v.textureCoord[1] = 1 - textureCoords[2 * tIdx + 1];
                    }

                    // normal
                    if (getline(iss2, tmp, '/') && tmp != "") {
                        int nIdx;
                        istringstream(tmp) >> nIdx;
                        nIdx--;
                        v.normal[0] = normals[3 * nIdx];
                        v.normal[1] = normals[3 * nIdx + 1];
                        v.normal[2] = normals[3 * nIdx + 2];
                    }

                    if (valid) {
                        object->groups[group].vertices.push_back(v);
                        faceCount++;
                    }
                }

                object->groups[group].faceFirst.push_back(faceFirst);
                object->groups[group].faceCount.push_back(faceCount);
                faceFirst += faceCount;
            }
        }
        file.close();
    }

    vertices.clear();
    normals.clear();
    textureCoords.clear();
}

void WavefrontLoader::loadANI(const string & filename, Object3D * object)
{
    unsigned int keyFrame;
    string group;

    string line;
    fstream file;
    file.open(filename.c_str());
    if (file.is_open()) {
        while (file.good()) {
            getline(file, line);
            istringstream iss(line);
            string type;
            iss >> type;

            if (type == "l") {
                string group, childGroup;
                iss >> childGroup >> group;
                object->groups[group].children.push_back(childGroup);

                string rootGroup = object->rootGroup;
                if (rootGroup == "" || childGroup == rootGroup)
                    rootGroup = group;
                object->rootGroup = rootGroup;
            }
            if (type == "p") {
                string group;
                float x, y, z;
                iss >> group >> x >> y >> z;
                object->groups[group].pivot = vec3(x, y, z);
            }
            if (type == "anim") {
                string animation;
                unsigned int from, to;
                iss >> animation >> from >> to;
                object->animations[animation] = pair<unsigned int, unsigned int>(from, to);
            }
            if (type == "keyframe") {
                iss >> keyFrame;
            }
            if (type == "g" || type == "o") {
                iss >> group;
            }
            /*if (type == "usemtl") {
             string material;
             iss >> material;
             object->groups[group].keyFrames[keyFrame].material = material;
             }*/
            if (type == "t") {
                float x, y, z;
                iss >> x >> y >> z;
                object->groups[group].translationKeyFrames[keyFrame] = vec4(x, y, z, 0);
            }
            if (type == "r") {
                float angle, x, y, z;
                iss >> angle >> x >> y >> z;
                object->groups[group].rotationKeyFrames[keyFrame] = vec4(angle, x, y, z);
            }
            if (type == "s") {
                float x, y, z;
                iss >> x >> y >> z;
                object->groups[group].scaleKeyFrames[keyFrame] = vec4(x, y, z, 0);
            }
        }
    }
}

void WavefrontLoader::loadMTL(const string & filename)
{
    string material;

    string line;
    fstream file;
    file.open(filename.c_str());
    if (file.is_open()) {
    	while (file.good()) {
            getline(file, line);
            istringstream iss(line);
            string type;
            iss >> type;

            if (type == "newmtl") {
                iss >> material;
                Object3D::materials[material];
            }
            if (type == "Ka") {
                float r, g, b;
                iss >> r >> g >> b;
                Object3D::materials[material].ambient[0] = r;
                Object3D::materials[material].ambient[1] = g;
                Object3D::materials[material].ambient[2] = b;
                Object3D::materials[material].ambient[3] = 1;
            }
            if (type == "Kd") {
                float r, g, b;
                iss >> r >> g >> b;
                Object3D::materials[material].diffuse[0] = r;
                Object3D::materials[material].diffuse[1] = g;
                Object3D::materials[material].diffuse[2] = b;
                Object3D::materials[material].diffuse[3] = 1;
            }
            if (type == "Ks") {
                float r, g, b;
                iss >> r >> g >> b;
                Object3D::materials[material].specular[0] = r;
                Object3D::materials[material].specular[1] = g;
                Object3D::materials[material].specular[2] = b;
                Object3D::materials[material].specular[3] = 1;
            }
            if (type == "d" || type == "Tr") {
                float transparency;
                iss >> transparency;
                Object3D::materials[material].transparency = transparency;
            }
            if (type == "map_Kd") {
                string texture;
                iss >> texture;

                // TODO move to object class
                GLuint textureId;
                glGenTextures(1, &textureId);
                glBindTexture(GL_TEXTURE_2D, textureId);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                SDL_Surface * image = IMG_Load(texture.c_str());
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
                SDL_FreeSurface(image);

                Object3D::materials[material].texture = texture;
                Object3D::materials[material].textureId = textureId;
            }
        }
    }
    file.close();
}
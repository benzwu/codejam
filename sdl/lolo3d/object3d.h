#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <vector>
#include <string>
#include <map>
#include <GL/glew.h>

#define GLM_SWIZZLE // enable xyz()
#include <glm/glm.hpp>

using namespace std;
using glm::mat4;
using glm::vec3;
using glm::vec4;

class Object3D
{
public:
    Object3D(const string & filename);
    ~Object3D();
    void render(const mat4 & viewMatrix, const mat4 & projectionMatrix);
    void setAnimation(const string & animation);

    struct Material {
        float ambient[4];
        float diffuse[4];
        float specular[4];
        float transparency;
        string texture;
        GLuint textureId;
    };
    static map<string, Material> materials;

    struct Vertex {
        GLfloat position[3];
        GLfloat normal[3];
        GLfloat textureCoord[2];
    };

    struct Group {
        string material;
        vector<string> children;
        vec3 pivot;
        map<int, vec4> translationKeyFrames;
        map<int, vec4> rotationKeyFrames;
        map<int, vec4> scaleKeyFrames;
        vector<Vertex> vertices;
        vector<GLsizei> faceFirst;
        vector<GLsizei> faceCount;
        GLuint vertexBuffer;
    };
    map<string, Group> groups;
    string rootGroup;

    map<string, pair<int, int>> animations;
    string animation;

    vec3 position;
	float rotation;
  
    enum {
        ATTRIB_POSITION,
        ATTRIB_NORMAL,
        ATTRIB_TEXCOORD
    };

    enum TransformationType {
        TRANSLATE,
        SCALE,
        ROTATE
    };

private:
    void findGroup(const string & groupName, const mat4 & modelMatrix, const mat4 & viewMatrix, const mat4 & projectionMatrix);
    void renderGroup(const Group & group, const mat4 & modelMatrix, const mat4 & viewMatrix, const mat4 & projectionMatrix);
    void setMaterial(const string & material);
    mat4 evaluateFrame(const mat4 & modelMatrix, Group & group);
    mat4 interpolate(mat4 & mat, map<int, vec4> & keyFrames, TransformationType type);

    GLuint modelMatrixUniformLocation;
    GLuint viewMatrixUniformLocation;
    GLuint projectionMatrixUniformLocation;
    GLuint samplerUniformLocation;
    GLuint enableTextureUniformLocation;
    GLuint colorUniformLocation;
    int frame;
};

#endif
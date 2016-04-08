#include "object3d.h"

#include "glsl_utils.h"
#include "wavefront_loader.h"
#include <glm/gtc/matrix_transform.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

map<string, Object3D::Material> Object3D::materials;

Object3D::Object3D(const string & filename)
{
    WavefrontLoader::loadOBJ(filename, this);
    setAnimation("idle");

    for (auto & it : groups) {
        Group & group = it.second;
        GLuint & vertexBuffer = group.vertexBuffer;
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * group.vertices.size(), group.vertices.data(), GL_STATIC_DRAW);
        group.vertices.clear();
    }

    modelMatrixUniformLocation = glGetUniformLocation(GLSLUtils::programId, "uModelMatrix");
    viewMatrixUniformLocation = glGetUniformLocation(GLSLUtils::programId, "uViewMatrix");
    projectionMatrixUniformLocation = glGetUniformLocation(GLSLUtils::programId, "uProjectionMatrix");
    samplerUniformLocation = glGetUniformLocation(GLSLUtils::programId, "uSampler");
    enableTextureUniformLocation = glGetUniformLocation(GLSLUtils::programId, "uEnableTexture");
    colorUniformLocation = glGetUniformLocation(GLSLUtils::programId, "uColor");
}

Object3D::~Object3D()
{
	for (auto & it : groups) {
        Group & group = it.second;
        glDeleteBuffers(1, &group.vertexBuffer);
    }
}

void Object3D::render(const mat4 & viewMatrix, const mat4 & projectionMatrix)
{
    glEnableVertexAttribArray(ATTRIB_POSITION);
    glEnableVertexAttribArray(ATTRIB_NORMAL);
    glEnableVertexAttribArray(ATTRIB_TEXCOORD);

	mat4 modelMatrix;
	modelMatrix = glm::translate(modelMatrix, vec3(position.x, position.y, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation, vec3(0, 0, 1));

    if (rootGroup == "") {
        for (auto & it : groups) {
            Group & group = it.second;
            renderGroup(group, modelMatrix, viewMatrix, projectionMatrix);
        }
    } else {
        findGroup(rootGroup, modelMatrix, viewMatrix, projectionMatrix);
    }

    if (animations.count(animation)) {
        int from = animations.at(animation).first;
        int to = animations.at(animation).second;
        frame++;
        if (frame > to)
            frame = from;
    }

    glDisableVertexAttribArray(ATTRIB_POSITION);
    glDisableVertexAttribArray(ATTRIB_NORMAL);
    glDisableVertexAttribArray(ATTRIB_TEXCOORD);
}

void Object3D::findGroup(const string & groupName, const mat4 & modelMatrix, const mat4 & viewMatrix, const mat4 & projectionMatrix)
{
    Group & group = groups[groupName];
    mat4 modelMatrixResult = evaluateFrame(modelMatrix, group);

    renderGroup(group, modelMatrixResult, viewMatrix, projectionMatrix);

	for (auto & it : group.children) {
        findGroup(it, modelMatrixResult, viewMatrix, projectionMatrix);
    }
}

void Object3D::renderGroup(const Group & group, const mat4 & modelMatrix, const mat4 & viewMatrix, const mat4 & projectionMatrix)
{
    setMaterial(group.material);

    glBindBuffer(GL_ARRAY_BUFFER, group.vertexBuffer);
    glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(sizeof(GLfloat) * 3));
    glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(sizeof(GLfloat) * 6));

    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    glMultiDrawArrays(GL_TRIANGLES, group.faceFirst.data(), group.faceCount.data(), GLsizei(group.faceFirst.size()));
}

void Object3D::setMaterial(const string & material)
{
    glUniform4fv(colorUniformLocation, 1, &materials[material].diffuse[0]);

    if (materials[material].texture != "") {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, materials[material].textureId);
        glUniform1i(samplerUniformLocation, 0); // GL_TEXTURE0
        glUniform1i(enableTextureUniformLocation, true);
    } else {
        glUniform1i(enableTextureUniformLocation, false);
    }
}

mat4 Object3D::interpolate(mat4 & mat, map<int, vec4> & keyFrames, TransformationType type)
{
    if (!animations.count(animation)) return mat;

    int prevKey = animations.at(animation).first;

	for (auto & it : keyFrames) {	
		if (prevKey > frame) break;

		int f = it.first;
		vec4 vec = it.second;

        if (f < animations.at(animation).first) continue;

        float alpha = (frame < f) ? float(frame - prevKey) / (f - prevKey) : 1.0f;
        alpha = sin(alpha * float(M_PI_2));

        if (type == TRANSLATE)
            mat = glm::translate(mat, glm::mix(vec3(), vec3(vec), alpha));
        else if (type == SCALE)
            mat = glm::scale(mat, glm::mix(vec3(1), vec3(vec), alpha));
        else
            mat = glm::rotate(mat, glm::mix(0.0f, vec.x, alpha), vec3(vec.yzw));

        prevKey = f;
    }

    return mat;
}

mat4 Object3D::evaluateFrame(const mat4 & modelMatrix, Group & group)
{
    vec3 pivot = group.pivot;

    mat4 result(modelMatrix);
    result = glm::translate(result, pivot);
    result = interpolate(result, group.translationKeyFrames, TRANSLATE);
    result = interpolate(result, group.scaleKeyFrames, SCALE);
    result = interpolate(result, group.rotationKeyFrames, ROTATE);
    result = glm::translate(result, -pivot);

    return result;
}

void Object3D::setAnimation(const string & animation)
{
    this->animation = animation;

    if (animations.count(animation))
        frame = animations.at(animation).first;
    else
        frame = 0;
}
#include "glsl_utils.h"

#include "object3d.h"
#include <iostream>
#include <fstream>

using namespace std;

GLuint GLSLUtils::programId = -1;

bool GLSLUtils::checkShaderStatus(GLuint shaderId)
{
    GLint compileStatus;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus != GL_TRUE) {
        GLint infoLogLength;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar * buffer = new GLchar[infoLogLength];
        GLsizei bufferSize;
        glGetShaderInfoLog(shaderId, infoLogLength, &bufferSize, buffer);
        cout << buffer << endl;
        delete[] buffer;
        return false;
    }

    return true;
}

bool GLSLUtils::installShaders()
{
    fstream file;
    string shaderStr;
    const GLchar * shaderStrs[1];

    GLSLUtils::programId = glCreateProgram();
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    file = fstream("vertex_shader.glsl");
    shaderStr = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    shaderStrs[0] = shaderStr.c_str();
    glShaderSource(vertexShaderId, 1, shaderStrs, NULL);
    glCompileShader(vertexShaderId);

    file = fstream("fragment_shader.glsl");
    shaderStr = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    shaderStrs[0] = shaderStr.c_str();
    glShaderSource(fragmentShaderId, 1, shaderStrs, NULL);
    glCompileShader(fragmentShaderId);

    if (!checkShaderStatus(vertexShaderId) || !checkShaderStatus(fragmentShaderId))
        return false;

    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glBindAttribLocation(programId, Object3D::ATTRIB_POSITION, "aPosition");
    glBindAttribLocation(programId, Object3D::ATTRIB_NORMAL, "aNormal");
    glBindAttribLocation(programId, Object3D::ATTRIB_TEXCOORD, "aTexcoord");

    glLinkProgram(programId);

    GLint linkStatus;
    glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        cout << "ERROR: glLinkProgram error" << endl;
        return false;
    }

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    glUseProgram(programId);

    return true;
}
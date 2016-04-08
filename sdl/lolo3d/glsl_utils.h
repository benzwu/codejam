#include <GL/glew.h>

class GLSLUtils
{
public:
    static bool installShaders();
    static GLuint programId;

private:
    static bool checkShaderStatus(GLuint shaderId);
};

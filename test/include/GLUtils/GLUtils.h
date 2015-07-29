#pragma once

#include "GL\glew.h"

class GLUtils {
public:
    GLUtils();
    ~GLUtils();

    static void loadShader(char* shaderSource, GLenum shaderType, GLuint &programId);
};


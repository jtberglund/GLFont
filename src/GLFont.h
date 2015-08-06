#pragma once

#include "GL\glew.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include "glm\glm.hpp"

#include <memory> // for use of shared_ptr
#include <map>

#ifndef PI
#define PI 3.141596
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG 180.0 / PI;
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD PI / 180.0
#endif

class FontAtlas;

class GLFont {
public:
    enum FontFlags {
        LeftAligned      = 1 << 1,
        RightAligned     = 1 << 2,
        CenterAligned    = 1 << 3,
        WordWrap         = 1 << 4,
        Underlined       = 1 << 5,
        Bold             = 1 << 6,
        Italic           = 1 << 7,
        Indented         = 1 << 8,
        HorizontalLayout = 1 << 9
    };

    struct Point {
        GLfloat x; // x offset in window coordinates
        GLfloat y; // y offset in window coordinates
        GLfloat s; // glyph x offset in texture coordinates
        GLfloat t; // glyph y offset in texture coordinates

        Point() {}

        Point(float x, float y, float s, float t) :
            x(x), y(y), s(s), t(t) 
        {}
    };

    GLFont(char* font, int windowWidth, int windowHeight);
    ~GLFont();

    void init();

    void setWindowSize(int width, int height);

    // Degrees to rotate & axis on which to rotate (e.g. (90 0 1 0) to rotate 90deg on the y axis)
    void rotate(float degrees, float x, float y, float z);
    // NOTE: This does not change the pixel size. Use setPixelSize() to scale evenly
    void scale(float x, float y, float z);

    // Setters
    void setFont(char* font); // points to font family file (.ttf)
    void setColor(float r, float b, float g, float a); // RGBA values are 0 - 1.0
    void setAlignment(FontFlags alignment);
    void setPixelSize(int size);
    void setIndentation(int pixels);
    void setFontFlags(int flags);
    void appendFontFlags(int flags);

    // Getters
    char* getFont();
    glm::vec4 getColor();
    FontFlags getAlignment();
    float getRotation();
    int getIndentation();
    int getFontFlags();

    // Print the specified text on the screen starting at coords x, y
    // NOTE: params x and y should be x and y offsets in **window** coordinates (0, 0 is at the top left corner)
    void drawString(const char *text, float x, float y);
    void drawString(const char* text, float width, float height, float x, float y);

private:
    FT_Error _error;
    FT_Library _ft;
    FT_Face _face;
    FT_GlyphSlot _g;

    GLuint _programId;
    GLuint _vao; 
    GLuint _vbo;

    GLint _uniformTextureHandle;
    GLint _uniformTextColorHandle;
    GLint _uniformMVPHandle;

    // Holds texture atlases for different character pixel sizes
    std::map<int, std::shared_ptr<FontAtlas>> _fontAtlas;

    // Currently enabled settings set via FontFlags
    int _flags;

    // Window dimensions
    int _windowWidth;
    int _windowHeight;

    // Used to scale x and y coords
    // Note: sx and sy are chosen so that one glyph pixel corresponds to one screen pixel
    float _sx;
    float _sy;
    
    // Dimensions of atlas texture
    int _atlasWidth;
    int _atlasHeight;

    glm::mat4 _projection;
    glm::mat4 _view;
    glm::mat4 _model;
    glm::mat4 _mvp;

    char* _font; // file path to font file
    glm::vec4 _textColor; // RGBA value we will use to color the font (0.0 - 1.0)
    FontFlags _alignment;
    bool _isInitialized;
    int _curPixSize;
    int _indentationPix;
    
    // Used for debugging opengl only
    inline void getError() {
        const GLubyte* error = gluGetString(glGetError());
        if(error != GL_NO_ERROR)
            printf("----------------------------- %i ----------------------", error);
    }

    // Compile shader from file
    void loadShader(char* shaderSource, GLenum shaderType);
    // Calculate offset needed for center- or left-aligned text
    void calculateAlignment(const char* text, float &x);

    void recalculateMVP();
};


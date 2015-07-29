#include "GLFont.h"
#include "GLUtils.h"
#include "FontAtlas.h"

#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

// GLM
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtx\transform.hpp"

GLFont::GLFont(char* font, int windowWidth, int windowHeight) :
  _windowWidth(windowWidth),
  _windowHeight(windowHeight),
  _isInitialized(false),
  _alignment(FontAlignment::CenterAligned),
  _textColor(0, 0, 0, 1),
  _uniformMVPHandle(-1)
{
    // Initialize FreeType
    _error = FT_Init_FreeType(&_ft);
    if(_error) {
        throw std::exception("Failed to initialize FreeType");
    }

    setFont(font);

    _sx = 2.0 / _windowWidth;
    _sy = 2.0 / _windowHeight;
    
    // Since we are dealing with 2D text, we will use an orthographic projection matrix 
    _projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, // View: left, right, bottom, top (we are using normalized coords)
                             0.1f,                     // Near clipping distance
                             100.0f);                  // Far clipping distance

    _view = glm::lookAt(glm::vec3(0, 0, 1),  // Camera position in world space (doesn't really apply when using an ortho projection matrix)
                        glm::vec3(0, 0, 0),  // look at origin
                        glm::vec3(0, 1, 0)); // Head is up (set to 0, -1, 0 to look upside down)

    _model = glm::mat4(1.0f); // Identity matrix

    recalculateMVP();
}

GLFont::~GLFont() {
    _fontAtlas.clear();
}

void GLFont::init() {
    // Load the shaders
    _programId = glCreateProgram();
    GLUtils::loadShader("shaders\\fontVertex.shader", GL_VERTEX_SHADER, _programId);
    GLUtils::loadShader("shaders\\fontFragment.shader", GL_FRAGMENT_SHADER, _programId);

    glUseProgram(_programId);

    // Create and bind the vertex array object
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // Set default pixel size and create the texture
    setPixelSize(48); // default pixel size

    // Get shader handles
    _uniformTextureHandle = glGetUniformLocation(_programId, "tex");
    _uniformTextColorHandle = glGetUniformLocation(_programId, "textColor");
    _uniformMVPHandle = glGetUniformLocation(_programId, "mvp");

    GLuint curTex = _fontAtlas[_curPixSize]->getTexId(); // get texture ID for this pixel size

    glActiveTexture(GL_TEXTURE0 + curTex);
    glBindTexture(GL_TEXTURE_2D, curTex);

    // Set our uniforms
    glUniform1i(_uniformTextureHandle, curTex);
    glUniform4fv(_uniformTextColorHandle, 1, glm::value_ptr(_textColor));
    glUniformMatrix4fv(_uniformMVPHandle, 1, GL_FALSE, glm::value_ptr(_mvp));

    // Create the vertex buffer object
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    
    glUseProgram(0);

    _isInitialized = true;
}

void GLFont::glPrint(const char *text, float x, float y) {
    if(!_isInitialized)
        throw std::exception("Error: you must first initialize GLFont.");

    FT_GlyphSlot slot = _face->glyph;
    std::vector<Point> coords;

    // Align text
    calculateAlignment((const unsigned char*)text, x);

    // Normalize window coordinates
    x = -1 + x * _sx;
    y = 1 - y * _sy;
    
    glUseProgram(_programId);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint curTex = _fontAtlas[_curPixSize]->getTexId();
    int atlasWidth = _fontAtlas[_curPixSize]->getAtlasWidth();
    int atlasHeight = _fontAtlas[_curPixSize]->getAtlasHeight();

    glActiveTexture(GL_TEXTURE0 + curTex);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBindTexture(GL_TEXTURE_2D, curTex);
    glUniform1i(_uniformTextureHandle, curTex);

    glEnableVertexAttribArray(0);

    FontAtlas::Character* chars = _fontAtlas[_curPixSize]->getCharInfo();

    for(const char *p = text; *p; ++p) {
        float x2 = x + chars[*p].bitmapLeft * _sx; // scaled x coord
        float y2 = -y - chars[*p].bitmapTop * _sy; // scaled y coord
        float w = chars[*p].bitmapWidth * _sx;     // scaled width of character
        float h = chars[*p].bitmapHeight * _sy;    // scaled height of character

        // Calculate kerning value
        FT_Vector kerning;
        FT_Get_Kerning(_face,              // font face handle
                       *p,                 // left glyph
                       *(p+1),             // right glyph
                       FT_KERNING_DEFAULT, // kerning mode
                       &kerning);          // variable to store kerning value

        // Advance cursor to start of next character
        x += (chars[*p].advanceX + (kerning.x >> 6)) * _sx;
        y += chars[*p].advanceY * _sy;

        // Skip glyphs with no pixels (e.g. spaces)
        if(!w || !h)
            continue;

        coords.push_back(Point(x2,                // window x
                               -y2,               // window y
                               chars[*p].xOffset, // texture atlas x offset
                               0));               // texture atlas y offset

        coords.push_back(Point(x2 + w,
                               -y2,
                               chars[*p].xOffset + chars[*p].bitmapWidth / atlasWidth,
                               0));

        coords.push_back(Point(x2,
                               -y2 - h, 
                               chars[*p].xOffset, 
                               chars[*p].bitmapHeight / atlasHeight));

        coords.push_back(Point(x2 + w, 
                               -y2,
                               chars[*p].xOffset + chars[*p].bitmapWidth / atlasWidth,
                               0));

        coords.push_back(Point(x2,
                               -y2 - h, 
                               chars[*p].xOffset, 
                               chars[*p].bitmapHeight / atlasHeight));

        coords.push_back(Point(x2 + w, 
                               -y2 - h, 
                               chars[*p].xOffset + chars[*p].bitmapWidth / atlasWidth,
                               chars[*p].bitmapHeight / atlasHeight));
    }
    
    // Send the data to the gpu
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Point), 0);
    glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(Point), coords.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, coords.size());

    glDisableVertexAttribArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisable(GL_BLEND);
    glUseProgram(0);
}

void GLFont::loadShader(char* shaderSource, GLenum shaderType) {
    GLuint shaderId = glCreateShader(shaderType);

    GLint result = GL_FALSE; // compilation result
    int infoLogLength; // length of info log

    std::ifstream shaderFile(shaderSource);
    std::string shaderStr;
    const char* shader;

    if(!shaderFile.is_open()) {
        std::string error = "Error: could not read file ";
        throw std::exception(error.append(shaderSource).c_str());
    }
    
    // Read shader
    std::string buffer;
    while(std::getline(shaderFile, buffer)) {
        shaderStr += buffer + "\n";
    }

    shader = shaderStr.c_str();

    // Compile shader
    printf("Compiling shader\n");
    glShaderSource(shaderId,        // Shader handle
                   1,               // Number of files
                   &shader,  // Shader source code
                   NULL);           // NULL terminated string
    glCompileShader(shaderId);

    // Check shader
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> errorMessage(infoLogLength);
    glGetShaderInfoLog(shaderId, infoLogLength, NULL, &errorMessage[0]);
    fprintf(stdout, "%s\n", &errorMessage[0]);

    // Link the program
    fprintf(stdout, "Linking program\n");
    glAttachShader(_programId, shaderId);
    glLinkProgram(_programId);

    // Check the program
    glGetProgramiv(_programId, GL_LINK_STATUS, &result);
    glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> programErrorMessage(std::max(infoLogLength, int(1)));
    glGetProgramInfoLog(_programId, infoLogLength, NULL, &programErrorMessage[0]);
    fprintf(stdout, "%s\n", &programErrorMessage[0]);

    glDeleteShader(shaderId);

    shaderFile.close();
}

void GLFont::setColor(float r, float b, float g, float a) {
    _textColor = glm::vec4(r, b, g, a);

    // Update the textColor uniform
    glUseProgram(_programId);
    glUniform4fv(_uniformTextColorHandle, 1, glm::value_ptr(_textColor));
    glUseProgram(0);
}

glm::vec4 GLFont::getColor() {
    return _textColor;
}

void GLFont::setFont(char* font) {
    _font = font;

    // Create a new font
    _error = FT_New_Face(_ft,     // FreeType instance handle
                         _font,   // Font family to use
                         0,       // index of font (in case there are more than one in the file)
                         &_face); // font face handle
    if(_error == FT_Err_Unknown_File_Format) {
        throw std::exception("Failed to open font: unknown font format");
    }
    else if(_error) {
        throw std::exception("Failed to open font");
    }
}

char* GLFont::getFont() {
    return _font;
}

void GLFont::setAlignment(GLFont::FontAlignment alignment) {
    _alignment = alignment;
}

GLFont::FontAlignment GLFont::getAlignment() {
    return _alignment;
}

void GLFont::calculateAlignment(const unsigned char* text, float &x) {
    if(_alignment == GLFont::FontAlignment::LeftAligned)
        return; // no need to calculate alignment

    FT_GlyphSlot slot = _face->glyph;
    const unsigned char* p;
    float totalWidth = 0; // total width of the text to render in window space
   
    // Calculate total width
    for(p = text; *p; ++p) {
        _error = FT_Load_Char(_face, *p, FT_LOAD_RENDER);
        if(_error)
            continue; // skip character

        totalWidth += slot->bitmap.width;
    }

    if(_alignment == GLFont::FontAlignment::CenterAligned)
        x -= totalWidth / 2.0;
    else if(_alignment == GLFont::FontAlignment::RightAligned)
        x -= totalWidth;
}

void GLFont::setPixelSize(int size) {
    _curPixSize = size;

    // Create texture atlas for characters of this pixel size if there isn't already one
    if(!_fontAtlas[_curPixSize])
        _fontAtlas[_curPixSize] = std::shared_ptr<FontAtlas>(new FontAtlas(_face, _curPixSize));

    if(_error)
        throw std::exception("Failed to size font (are you using a fixed-size font?)");
}

void GLFont::setWindowSize(int width, int height) {
    _windowWidth = width;
    _windowHeight = height;

    // Recalculate sx and sy
    _sx = 2.0 / _windowWidth;
    _sy = 2.0 / _windowHeight;
}

void GLFont::rotate(float degrees, float x, float y, float z) {
    float rad = degrees * DEG_TO_RAD;
    _model = glm::rotate(_model, rad, glm::vec3(x, y, z));
    recalculateMVP();
}

void GLFont::scale(float x, float y, float z) {
    _model = glm::scale(glm::vec3(x, y, z));
    recalculateMVP();
}

void GLFont::recalculateMVP() {
    _mvp = _projection * _view * _model;

    if(_uniformMVPHandle != -1) {
        glUseProgram(_programId);
        glUniformMatrix4fv(_uniformMVPHandle, 1, GL_FALSE, glm::value_ptr(_mvp));
        glUseProgram(0);
    }
}
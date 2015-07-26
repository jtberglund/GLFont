#pragma once

#include "GLWindow.h"

class GLFont;

class TestWindow : public GLWindow {
public:
    TestWindow();
    TestWindow(int width, int height, const char* windowTitle);
    ~TestWindow();

    void start();
    void init() override;
    void update() override;
    void render() override;

protected:
    // Event handlers - called from glfw callbacks
    void onKey(int key, int scancode, int action, int mods) override;
    void onResize(int width, int height) override;
    void onMouseButton(int button, int action, int mods) override;
    void onMouseMove(double x, double y);
    void onMouseEnter(int enter) override;
    void onCharacter(unsigned int codepoint) override;
    void onScroll(double x, double y) override;

private:
    GLuint _vbo;
    GLFont* _font;
};


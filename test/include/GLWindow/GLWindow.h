#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "stdio.h"
#include "stdlib.h"
#include <iostream>

class GLWindow {
public:
    GLWindow();
    GLWindow(int width, int height, const char* windowTitle);
    ~GLWindow();

    void run();

    inline int getWidth() { return _width; }
    inline int getHeight() { return _height; }

protected:
    GLFWwindow* getWindowHandle();
    virtual void init() = 0;
    // For state changes
    virtual void update() = 0;
    // Render the scene
    virtual void render() = 0;

    // Event handlers - called from glfw callbacks
    virtual void onKey(int key, int scancode, int action, int mods);
    virtual void onResize(int width, int height);
    virtual void onMouseButton(int button, int action, int mods);
    virtual void onMouseMove(double x, double y);
    virtual void onMouseEnter(int enter);
    virtual void onCharacter(unsigned int codepoint);
    virtual void onScroll(double x, double y);

    inline void setWidth(int width) { _width = width; }
    inline void setHeight(int height) { _height = height; }

private:
    GLFWwindow* _window;
    const char* _windowTitle;
    int _frame; // curret frame - used for calculating ms/frame
    
    // Window size
    int _width;
    int _height;

    // Invoked on window resize
    friend void resizeCallback(GLFWwindow* window, int width, int height);
    // Used to set the glViewport after a resize
    friend void framebufferCallback(GLFWwindow* window, int width, int height);

    // Callback functions
    friend void errorCallback(int error, const char* description);
    friend void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    friend void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    friend void mouseMoveCallback(GLFWwindow* window, double x, double y);
    friend void cursorEnterCallback(GLFWwindow* window, int enter);
    friend void characterCallback(GLFWwindow* window, unsigned int codepoint);
    friend void scrollCallback(GLFWwindow* window, double x, double y);
};


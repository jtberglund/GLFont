# GLFont
GLFont is a text-rendering library written in C++ for OpenGL 3.3, using FreeType 2.

## Dependencies
1. Freetype - http://freetype.org/
2. GLM - http://glm.g-truc.net/0.9.4/index.html
3. GLEW - http://glew.sourceforge.net/build.html
4. GLFW - http://www.glfw.org/

## C++ Quick Start

    // Create GLFont object
    GLFont* glfont = new GLFont(".../font.ttf", windowWidth, windowHeight);
    ...


    // Intialize GLFont object - this must be called before rendering text
    glfont->init();

    // Optionally, change color, rotate text, scale text, etc.
    glFont->rotate(90, 0, 1, 0); // rotate text 90 degrees on the y axis
    glFont->setColor(0.89, 0.26, 0.3, 0.9);
    ...


    // In the render loop, render the text
    // First, we can set the alignment, color, or size of our font object
    // These options will be in effect until they are changed back
    glfont->setPixelSize(24);
    glfont->setAlignment(GLFont::FontAlignment::LeftAligned);
    glfont->setColor()

    // Now we do the rendering:
    // Note: xPos and yPos should be in window coords -
             this means (0,0) is at the top-left corner
    glfont->glprint("Hello world!", xPos, yPos);
    ...


    // Note: Whenever the window is resized, you should update the
    //       window size of your GLFont object
    glfont->setWindowSize(windowWidth, windowSize);

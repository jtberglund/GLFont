# GLFont
GLFont is a text-rendering library written in C++ for OpenGL 3.3, using FreeType 2.

## Dependencies
1. Freetype - http://freetype.org/
2. GLM - http://glm.g-truc.net/0.9.4/index.html
3. GLEW - http://glew.sourceforge.net/build.html
4. GLFW - http://www.glfw.org/

## C++ Quick Start
```c++
// Create GLFont object
GLFont* glfont = new GLFont(".../myFont.ttf", windowWidth, windowHeight);

// Initialize GLFont object - this must be called before rendering text
glfont->init();
```  

```c++
// Optionally, change color, rotate text, scale text, etc.
glFont->rotate(90, 0, 1, 0); // rotate text 90 degrees on the y axis
glFont->setColor(0.89, 0.26, 0.3, 0.9);
```

```c++
// Before rendering,, we can set the alignment, color, or size of the font
// These options will be in effect for all text render operations
// until they are changed again
glfont->setPixelSize(24);
glfont->setIndentation(50);
glfont->setAlignment(GLFont::FontAlignment::LeftAligned);
glfont->setColor()
```

```c++
// Now we do the rendering:
// Note: xPos and yPos should be in window coords -
//       this means (0,0) is at the top-left corner
glfont->drawString("Hello world!", xPos, yPos);

// To render a paragraph, use the overloaded method,
// which allow you to specify a bounding box

// For example, the following will print a paragraph starting at
// window coords (100, 100), with a bounding box of 200 x 200
glfont->drawString("[long paragraph here]", 100, 100, 200, 200);

// Set font options with the setFontFlags() method
glfont->setFontFlags(FontFlags::WordWrap | FontFlags::Indented);
```

```c++
// Note: Whenever the window is resized, you should update the
//       window size of your GLFont object
glfont->setWindowSize(windowWidth, windowSize);
```

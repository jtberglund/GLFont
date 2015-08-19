# GLFont
GLFont is a text-rendering library written in C++ for OpenGL 3.3, using FreeType 2.

## Dependencies
1. Freetype - http://freetype.org/
2. GLM - http://glm.g-truc.net/0.9.4/index.html
3. GLEW - http://glew.sourceforge.net/build.html
4. GLFW - http://www.glfw.org/ (This is only necessary for the included test project. You can still use GLFont with your own application without GLFW)

## C++ Quick Start

### Initialize library and create label
First create a font face to use for our labels.
```c++
shared_ptr<GLFont> glFont = shared_ptr<GLFont>(new GLFont(".../myFont.ttf"));
```

Now create a label, passing in the font face we just created.

```c++
// Some values we will use to create our labels
int startX = 100;
int startY = 100;
int windowWidth = 800;
int windowHeight = 600;

unique_ptr<FTLabel> label = unique_ptr<FTLabel>(new FTLabel(
  glfont,         // Font face handle
  "Hello world!", // Text to render
  startX,
  startY,
  windowWidth,
  windowHeight
));
```
Note that the starting x and y coords should be in window space.
This means (0,0) is at the top-left corner.

Additionally, we could specify a max width and/or height for our text. For example, the following will print a paragraph starting at window coordinates (100, 100), with a bounding box of 200 x 100.
```c++
int maxWidth = 200;
int maxHeight = 100;
unique_ptr<FTLabel> label = unique_ptr<FTLabel>(new FTLabel(
  glFont,
  "[long paragraph here]",
  startX,
  startY,
  maxWidth,
  maxWidth,
  windowWidth,
  windowHeight
));
```  

You can manually change the size or starting position of your label at any time. This is often done when the window is resized (unless you are using absolute label positions that do not change depending on window size).

This will set our label to start being drawn at (500, 250):
```c++
label->setPosition(500, 250);
```

Set max width to 100 and max height to 100:
```c++
label->setSize(100, 100);
```

### Font Settings

Set the alignment, color, or size of the label text.
```c++
label->setPixelSize(24);
label->setIndentation(50);
label->setAlignment(FTLabel::FontFlags::LeftAligned);
label->setColor(0.89, 0.26, 0.3, 0.9);
```

Rotate text 90 degrees on the y axis:

```c++
label->rotate(90, 0, 1, 0);
```

Additionally, set font options with FTLabel::setFontFlags()
```c++
label->setFontFlags(FontFlags::WordWrap | FontFlags::Indented);
```

### Rendering Text
In the render loop, simply call FTLabel::render()
```c++
label->render();
```

### Additional Notes
Whenever the window is resized, you should update the window size of your label
```c++
label->setWindowSize(windowWidth, windowSize);
```

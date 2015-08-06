#include "TestWindow.h"
#include "GLFont.h"

TestWindow::TestWindow() {}

TestWindow::TestWindow(int width, int height, const char* windowTitle) :
  GLWindow(width, height, windowTitle)
{}


TestWindow::~TestWindow() {
    delete _font;
}

void TestWindow::start() {
    init();
}

void TestWindow::init() {
    _font = new GLFont("fonts\\13_5Atom_Sans_Regular.ttf", getWidth(), getHeight());
    _font->init();
    _font->setColor(0.89, 0.26, 0.3, 0.9);
    _font->appendFontFlags(GLFont::FontFlags::Indented);
}

void TestWindow::render() {
    _font->setPixelSize(64);
    _font->setAlignment(GLFont::FontFlags::CenterAligned);
    _font->drawString("Hello world!", 0.5 * getWidth(), 0.5 * getHeight());
    _font->setPixelSize(48);
    _font->setAlignment(GLFont::FontFlags::LeftAligned);
    _font->drawString("Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
        "Aliquam quis pellentesque ligula, sed imperdiet tortor. Curabitur eleifend "
        "facilisis orci, a accumsan felis hendrerit in. Duis nec fringilla quam. "
        "Proin accumsan nulla lacus, vel posuere diam imperdiet et. Nunc sed dui "
        "pellentesque, pretium justo vel, posuere justo. Integer mollis luctus "
        "condimentum. Vivamus quis ex quis nisl convallis ullamcorper sed a urna. "
        "Praesent eu libero dignissim, rutrum nisi in, euismod nibh. Phasellus est "
        "felis, malesuada suscipit leo ac, varius egestas turpis.", 
        10, 0.6 * getHeight(), getWidth() - 20, 100);
}

void TestWindow::update() {}

void TestWindow::onKey(int key, int scancode, int action, int mods) {}

void TestWindow::onResize(int width, int height) {
    _font->setWindowSize(width, height);
}

void TestWindow::onMouseMove(double x, double y) {}

void TestWindow::onMouseButton(int button, int action, int mods) {}

void TestWindow::onMouseEnter(int enter) {}

void TestWindow::onCharacter(unsigned int codepoint) {}

void TestWindow::onScroll(double x, double y) {}
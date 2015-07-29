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
}

void TestWindow::render() {
    _font->setAlignment(GLFont::FontAlignment::CenterAligned);
    _font->setPixelSize(64);
    _font->glPrint("Hello world!", 0.5 * getWidth(), 0.5 * getHeight());
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
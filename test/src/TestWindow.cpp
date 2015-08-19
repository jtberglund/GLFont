#include "TestWindow.h"
#include "GLFont.h"
#include "FTLabel.h"

TestWindow::TestWindow() {}

TestWindow::TestWindow(int width, int height, const char* windowTitle) :
  GLWindow(width, height, windowTitle)
{}

TestWindow::~TestWindow() {}

void TestWindow::start() {
    init();
}

void TestWindow::init() {
    // Create font face
    _font = shared_ptr<GLFont>(new GLFont("fonts\\13_5Atom_Sans_Regular.ttf"));

    // "Hello world" label
    lblHello = shared_ptr<FTLabel>(new FTLabel(_font, "Hello world", 0.5 * getWidth(), 0.5 * getHeight(), getWidth(), getHeight()));
    lblHello->setColor(0.89, 0.26, 0.3, 0.9);
    lblHello->setPixelSize(64);
    lblHello->setAlignment(FTLabel::FontFlags::CenterAligned);
    lblHello->appendFontFlags(FTLabel::FontFlags::Indented);

    _labels.push_back(lblHello);

    // Paragraph label
    char* p = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
              "Aliquam quis pellentesque ligula, sed imperdiet tortor. Curabitur eleifend "
              "facilisis orci, a accumsan felis hendrerit in. Duis nec fringilla quam. "
              "Proin accumsan nulla lacus, vel posuere diam imperdiet et. Nunc sed dui "
              "pellentesque, pretium justo vel, posuere justo. Integer mollis luctus "
              "condimentum. Vivamus quis ex quis nisl convallis ullamcorper sed a urna. "
              "Praesent eu libero dignissim, rutrum nisi in, euismod nibh. Phasellus est "
        "felis, malesuada suscipit leo ac, varius egestas turpis.";
    lblParagraph = shared_ptr<FTLabel>(new FTLabel(_font, p, 0, 0.6 * getHeight(), getWidth(), 0, getWidth(), getHeight()));
    lblParagraph->setColor(0, 1.0, 0.5, 1.0);
    lblParagraph->setAlignment(FTLabel::FontFlags::LeftAligned);

    _labels.push_back(lblParagraph);
}

void TestWindow::render() {
    for(Label& label : _labels)
        label->render();
}

void TestWindow::update() {}

void TestWindow::onKey(int key, int scancode, int action, int mods) {}

void TestWindow::onResize(int width, int height) {
    for(Label& label : _labels)
        label->setWindowSize(width, height);

    // Update label positions
    lblHello->setPosition(0.5 * width, 0.5 * height);

    lblParagraph->setSize(width, 0);
    lblParagraph->setPosition(0, 0.6 * height);
}

void TestWindow::onMouseMove(double x, double y) {}

void TestWindow::onMouseButton(int button, int action, int mods) {}

void TestWindow::onMouseEnter(int enter) {}

void TestWindow::onCharacter(unsigned int codepoint) {}

void TestWindow::onScroll(double x, double y) {}
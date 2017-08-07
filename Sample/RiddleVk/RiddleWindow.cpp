//
//  RiddleWindow.cpp
//
//
//  Created by Narendra Umate on 8/18/15.
//
//

#include "RiddleWindow.h"

#include "Platform.h"

RiddleWindow::RiddleWindow(Parameters& parameters)
: BaseWindow(parameters) {
	create();
}

RiddleWindow::~RiddleWindow() {
	destroy();
}

void RiddleWindow::pollEvents() {
	// BaseWindow::pollEvents();
}

void RiddleWindow::onResize(const int& xSize, const int& ySize) {
	BaseWindow::onResize(xSize, ySize);
}

void RiddleWindow::onKeyDown(int k) {
}

void RiddleWindow::update(float dt) {
	static int counter = 0;
#if defined(__linux__) || defined(_WIN32)
#endif
	if (counter++ == 300) {
		m_active = false;
	}
}

void RiddleWindow::create() {
}

void RiddleWindow::destroy() {
}

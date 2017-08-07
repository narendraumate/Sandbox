//
//  AbstractWindow.cpp
//
//
//  Created by Narendra Umate on 2/21/16.
//
//

#include "AbstractWindow.h"

AbstractWindow::AbstractWindow(Parameters& parameters)
: m_minimized(false)
, m_maximized(false)
, m_active(true)
, m_title(parameters.title)
, m_xOrigin(parameters.xOrigin)
, m_yOrigin(parameters.yOrigin)
, m_xSize(parameters.xSize)
, m_ySize(parameters.ySize)
, m_type(parameters.type) {
}

AbstractWindow::~AbstractWindow() {
}

void AbstractWindow::onMove(const int& x, const int& y) {
	m_xOrigin = x;
	m_yOrigin = y;
}

void AbstractWindow::onResize(const int& xSize, const int& ySize) {
	m_minimized = false;
	m_maximized = false;
	if (m_xSize != xSize || m_ySize != ySize) {
		m_xSize = xSize;
		m_ySize = ySize;
	}
}

void AbstractWindow::onMinimize() {
	m_minimized = true;
	m_maximized = false;
}

void AbstractWindow::onMaximize() {
	m_minimized = false;
	m_maximized = true;
}

void AbstractWindow::onRestore() {
	m_minimized = false;
	m_maximized = false;
}

void AbstractWindow::onFocus(const int& focus) {
	m_minimized = false;
	m_maximized = false;
}

void AbstractWindow::onClose() {
	m_active = false;
}

void AbstractWindow::onMouseUp(int button, int x, int y) {
}

void AbstractWindow::onMouseDown(int button, int x, int y) {
}

void AbstractWindow::onMouseMove(int x, int y) {
}

void AbstractWindow::onMouseScroll(float x, float y) {
}

void AbstractWindow::onKeyUp(int k) {
}

void AbstractWindow::onKeyDown(int k) {
}

int AbstractWindow::getMouseButton(int button) {
	return 0;
}

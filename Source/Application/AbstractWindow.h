//
//  AbstractWindow.h
//
//
//  Created by Narendra Umate on 2/21/16.
//
//

#pragma once

#include "Buffer.h"
#include "Bvh.h"
#include "Octree.h"
#include "Renderer.h"
#include "SpatialCamera.h"
#include "SpatialLight.h"
#include "VisualScreenSpace.h"

class Renderer;

enum RendererType {
	None,
	DirectX,
	Metal,
	OpenGL,
	Vulkan
};

struct Parameters {
	Parameters()
	: title("")
	, xOrigin(0)
	, yOrigin(0)
	, xSize(0)
	, ySize(0)
	, type(None) {
	}
	std::string title;
	int xOrigin;
	int yOrigin;
	int xSize;
	int ySize;
	RendererType type;
};

class AbstractWindow {

public:
	AbstractWindow(Parameters& parameters);
	virtual ~AbstractWindow() = 0;

	void setTitle(const std::string& title) {
		m_title = title;
	}

	std::string getTitle() {
		return m_title;
	}

	void setXOrigin(const int& xOrigin) {
		m_xOrigin = xOrigin;
	}

	int getXOrigin() {
		return m_xOrigin;
	}

	void setYOrigin(const int& yOrigin) {
		m_yOrigin = yOrigin;
	}

	int getYOrigin() {
		return m_yOrigin;
	}

	void setXSize(const int& xSize) {
		m_xSize = xSize;
	}

	int getXSize() {
		return m_xSize;
	}

	void setYSize(const int& ySize) {
		m_ySize = ySize;
	}

	int getYSize() {
		return m_ySize;
	}

	bool isMinimized() const {
		return m_minimized;
	}

	bool isMaximized() const {
		return m_maximized;
	}

	bool isActive() const {
		return m_active;
	}

	virtual void onMove(const int& x, const int& y);
	virtual void onResize(const int& xSize, const int& ySize);
	virtual void onMinimize();
	virtual void onMaximize();
	virtual void onRestore();
	virtual void onFocus(const int& focus);
	virtual void onClose();
	virtual void onMouseUp(int button, int x, int y);
	virtual void onMouseDown(int button, int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseScroll(float x, float y);
	virtual void onKeyUp(int k);
	virtual void onKeyDown(int k);

	virtual int getMouseButton(int button);

	virtual void getWindowPos(int& xPos, int& yPos) = 0;
	virtual void getWindowSize(int& width, int& height) = 0;
	virtual void getFramebufferSize(int& width, int& height) = 0;

	virtual void update(float dt) = 0;
	virtual void swapBuffers() = 0;
	virtual void pollEvents() = 0;

	virtual void getMetalHackBoth(void** texture, void** drawable) = 0;

public:
	int m_xDivs;
	int m_yDivs;

protected:
	bool m_minimized;
	bool m_maximized;
	bool m_active;

	std::string m_title;
	int m_xOrigin;
	int m_yOrigin;
	int m_xSize;
	int m_ySize;
	RendererType m_type;
};

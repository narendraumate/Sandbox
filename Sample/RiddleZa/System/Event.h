//
//  Event.h
//
//
//  Created by Narendra Umate on 8/18/16.
//
//

#pragma once

#include "AbstractWindow.h"

#include "ECS.h"

struct EventWindowCreated {
	ECS_DECLARE_TYPE;
	RendererType type;
	int xSize;
	int ySize;
	void* window;
};

struct EventWindowDestroyed {
	ECS_DECLARE_TYPE;
};

struct EventWindowResized {
	ECS_DECLARE_TYPE;
	int xSize;
	int ySize;
};

//
//  RiddleWindow.h
//
//
//  Created by Narendra Umate on 8/18/15.
//
//

#pragma once

#if defined(COMPILE_COMMON)
#define BaseWindow CommonWindow
#include "CommonWindow.h"
#elif defined(COMPILE_NATIVE)
#define BaseWindow NativeWindow
#include "NativeWindow.h"
#endif

class RiddleWindow : public BaseWindow {
public:
	RiddleWindow(Parameters& parameters);
	virtual ~RiddleWindow();

	virtual void pollEvents(); // TODO Should remove this but crashes without it

	virtual void onKeyDown(int k);
	virtual void onResize(const int& xSize, const int& ySize);
	virtual void update(float dt);

private:
	void create();
	void destroy();
};

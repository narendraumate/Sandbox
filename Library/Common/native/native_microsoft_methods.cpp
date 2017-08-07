//
//  native_microsoft_methods.cpp
//
//
//  Created by Narendra Umate on 7/17/16.
//
//

#if defined(_WIN32)

#include "native_microsoft_methods.h"
#include "native_window_microsoft_opengl.h"
#include "native_window_microsoft_vulkan.h"

#include "windows.h"

int platformInit() {
	return 1;
}

void platformPollEvents() {
	MSG msg;
	while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			// running = false;
		} else {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}
}

void platformTerminate() {
}

#endif // defined(_WIN32)

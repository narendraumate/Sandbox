//
//  native_window_linux.cpp
//
//
//  Created by Narendra Umate on 11/19/16.
//
//

#if defined(__linux__)

#include "native_linux_classes.h"

#include "native_window_linux.h"

NativeWindowLinux::NativeWindowLinux(const int& width, const int& height, const char* title)
: NativeWindowBasic(width, height, title) {
}

NativeWindowLinux::~NativeWindowLinux() {
}

void NativeWindowLinux::getMetalHackBoth(void** texture, void** drawable) {
	*texture = nullptr;
	*drawable = nullptr;
}

#endif // defined(__linux__)

//
//  native_window_microsoft.cpp
//
//
//  Created by Narendra Umate on 11/19/16.
//
//

#include "native_microsoft_classes.h"

#include "native_window_microsoft.h"

NativeWindowMicrosoft::NativeWindowMicrosoft(const int& width, const int& height, const char* title)
: NativeWindowBasic(width, height, title) {
}

NativeWindowMicrosoft::~NativeWindowMicrosoft() {
}

void NativeWindowMicrosoft::getMetalHackBoth(void** texture, void** drawable) {
	*texture = 0;
	*drawable = 0;
}

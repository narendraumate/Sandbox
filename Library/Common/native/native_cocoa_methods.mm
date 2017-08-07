//
//  native_cocoa_methods.mm
//
//
//  Created by Narendra Umate on 7/17/16.
//
//

#if defined(__APPLE__)

#include "native_cocoa_methods.h"
#include "native_window_cocoa_metal.h"
#include "native_window_cocoa_opengl.h"

#import <AppKit/AppKit.h>

int platformInit()
{
    return 1;
}

void platformPollEvents()
{
    while (true) {
        NSEvent *event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                            untilDate:[NSDate distantPast]
                                               inMode:NSDefaultRunLoopMode
                                              dequeue:YES];
        if (event == nil) {
            break;
        }
        [NSApp sendEvent:event];
    }
}

void platformTerminate()
{
    // do nothing
}

#endif // defined(__APPLE__)

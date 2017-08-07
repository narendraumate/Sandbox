//
//  native_cocoa_classes.h
//
//
//  Created by Narendra Umate on 11/20/16.
//
//

#ifndef native_cocoa_classes_h
#define native_cocoa_classes_h

#if defined(__APPLE__)

#import <AppKit/AppKit.h>

#include "native_window_basic.h"

@interface NativeApplication : NSApplication
@end

@interface NativeApplicationDelegate : NSObject
@end

@interface NativeWindowDelegate : NSObject
{
    NativeWindowBasic *window;
}

- (id)initWithNativeWindow:(NativeWindowBasic *)initWindow;

@end

@interface NativeMetalView : NSView
{
    NativeWindowBasic *window;
}

- (id)init;

@end

@interface NativeOpenGLView : NSOpenGLView
{
    NativeWindowBasic *window;
}

- (id)init;

@end

@interface NativeWindowCocoaClass : NSWindow
{
}
@end

#endif // defined(__APPLE__)

#endif /* native_cocoa_classes_h */

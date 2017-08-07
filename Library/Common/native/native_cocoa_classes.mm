//
//  native_cocoa_classes.mm
//
//
//  Created by Narendra Umate on 11/20/16.
//
//

#if defined(__APPLE__)

#include "native_cocoa_classes.h"

@implementation NativeApplication

// From http://cocoadev.com/index.pl?GameKeyboardHandlingAlmost
// This works around an AppKit bug, where key up events while holding
// down the command key don't get sent to the key window.
- (void)sendEvent:(NSEvent *)event
{
    if ([event type] == NSEventTypeKeyUp && ([event modifierFlags] & NSEventModifierFlagCommand)) {
        [[self keyWindow] sendEvent:event];
    } else {
        [super sendEvent:event];
    }
}

// No-op thread entry point
//
- (void)doNothing:(id)object
{
}
@end

@implementation NativeApplicationDelegate

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
    return NSTerminateCancel;
}

- (void)applicationDidChangeScreenParameters:(NSNotification *)notification
{
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
    [NSApp stop:nil];
}

- (void)applicationDidHide:(NSNotification *)notification
{
}

@end

@implementation NativeWindowDelegate

- (id)initWithNativeWindow:(NativeWindowBasic *)initWindow
{
    self = [super init];
    if (self != nil) {
        window = initWindow;
    }
    return self;
}

- (BOOL)windowShouldClose:(id)sender
{
    if (window->callbacks.close) {
        window->callbacks.close((NativeWindowBasic *)window);
    }
    return NO;
}

- (void)windowDidResize:(NSNotification *)notification
{
    if (window->callbacks.fbsize) {
        int width = 0;
        int height = 0;
        window->getWindowSize(width, height);
        window->callbacks.fbsize((NativeWindowBasic *)window, width, height);
    }
}

- (void)windowDidMove:(NSNotification *)notification
{
    if (window->callbacks.pos) {
        int xPos = 0;
        int yPos = 0;
        window->getWindowPos(xPos, yPos);
        window->callbacks.pos((NativeWindowBasic *)window, xPos, yPos);
    }
}

- (void)windowDidMiniaturize:(NSNotification *)notification
{
}

- (void)windowDidDeminiaturize:(NSNotification *)notification
{
}

- (void)windowDidBecomeKey:(NSNotification *)notification
{
}

- (void)windowDidResignKey:(NSNotification *)notification
{
}

@end

@implementation NativeMetalView

- (id)init
{
    self = [super init];
    if (self != nil) {
    }
    return self;
}

- (void)dealloc
{
    [super dealloc];
}

- (BOOL)isOpaque
{
    return YES;
}

- (BOOL)canBecomeKeyView
{
    return YES;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

@end

@implementation NativeOpenGLView

- (id)init
{
    self = [super init];
    if (self != nil) {
    }
    return self;
}

- (void)dealloc
{
    [super dealloc];
}

- (BOOL)isOpaque
{
    return YES;
}

- (BOOL)canBecomeKeyView
{
    return YES;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

@end

@implementation NativeWindowCocoaClass

- (BOOL)canBecomeKeyWindow
{
    // This must be YES for NSBorderlessWindowMask.
    // Otherwise it can be set to NO in most cases.
    // Return yes so that this borderless window can receive input.
    return YES;
}

- (void)keyDown:(NSEvent *)event
{
    // Implement keyDown since controller will not get [ESC] key event which
    // the controller uses to kill fullscreen
    [[self windowController] keyDown:event];
}

- (BOOL)canBecomeMainWindow
{
    return YES;
}

@end

#endif // defined(__APPLE__)

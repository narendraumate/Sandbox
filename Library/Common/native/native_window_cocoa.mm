//
//  native_window_cocoa.mm
//
//
//  Created by Narendra Umate on 11/19/16.
//
//

#if defined(__APPLE__)

// import does not compile if added inside header.
#import <AppKit/AppKit.h>

#import "native_cocoa_classes.h"

#include "native_window_cocoa.h"

NativeWindowCocoa::NativeWindowCocoa(const int &width, const int &height, const char *title)
    : NativeWindowBasic(width, height, title)
{
    // Initialize App Kit.
    if (NSApp == nullptr) {
        // Implicitly create shared NSApplication instance
        [NativeApplication sharedApplication];

        // Make Cocoa enter multi-threaded mode
        [NSThread detachNewThreadSelector:@selector(doNothing:)
                                 toTarget:NSApp
                               withObject:nil];

        // In case we are unbundled, make us a proper UI application
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

        NSString *appName = [NSString stringWithUTF8String:title];

        NSMenu *bar = [[NSMenu alloc] init];
        [NSApp setMainMenu:bar];

        NSMenuItem *appMenuItem =
            [bar addItemWithTitle:@""
                           action:NULL
                    keyEquivalent:@""];
        NSMenu *appMenu = [[NSMenu alloc] init];
        [appMenuItem setSubmenu:appMenu];

        [appMenu addItemWithTitle:[NSString stringWithFormat:@"About %@", appName]
                           action:@selector(orderFrontStandardAboutPanel:)
                    keyEquivalent:@""];
        [appMenu addItem:[NSMenuItem separatorItem]];
        NSMenu *servicesMenu = [[NSMenu alloc] init];
        [NSApp setServicesMenu:servicesMenu];
        [[appMenu addItemWithTitle:@"Services"
                            action:NULL
                     keyEquivalent:@""] setSubmenu:servicesMenu];
        [servicesMenu release];
        [appMenu addItem:[NSMenuItem separatorItem]];
        [appMenu addItemWithTitle:[NSString stringWithFormat:@"Hide %@", appName]
                           action:@selector(hide:)
                    keyEquivalent:@"h"];
        [[appMenu addItemWithTitle:@"Hide Others"
                            action:@selector(hideOtherApplications:)
                     keyEquivalent:@"h"]
            setKeyEquivalentModifierMask:NSEventModifierFlagOption | NSEventModifierFlagCommand];
        [appMenu addItemWithTitle:@"Show All"
                           action:@selector(unhideAllApplications:)
                    keyEquivalent:@""];
        [appMenu addItem:[NSMenuItem separatorItem]];
        [appMenu addItemWithTitle:[NSString stringWithFormat:@"Quit %@", appName]
                           action:@selector(terminate:)
                    keyEquivalent:@"q"];

        NSMenuItem *windowMenuItem =
            [bar addItemWithTitle:@""
                           action:NULL
                    keyEquivalent:@""];
        [bar release];
        NSMenu *windowMenu = [[NSMenu alloc] initWithTitle:@"Window"];
        [NSApp setWindowsMenu:windowMenu];
        [windowMenuItem setSubmenu:windowMenu];

        [windowMenu addItemWithTitle:@"Minimize"
                              action:@selector(performMiniaturize:)
                       keyEquivalent:@"m"];
        [windowMenu addItemWithTitle:@"Zoom"
                              action:@selector(performZoom:)
                       keyEquivalent:@""];
        [windowMenu addItem:[NSMenuItem separatorItem]];
        [windowMenu addItemWithTitle:@"Bring All to Front"
                              action:@selector(arrangeInFront:)
                       keyEquivalent:@""];

        // TODO: Make this appear at the bottom of the menu (for consistency)
        [windowMenu addItem:[NSMenuItem separatorItem]];
        [[windowMenu addItemWithTitle:@"Enter Full Screen"
                               action:@selector(toggleFullScreen:)
                        keyEquivalent:@"f"]
            setKeyEquivalentModifierMask:NSEventModifierFlagControl | NSEventModifierFlagCommand];

        // Prior to Snow Leopard, we need to use this oddly-named semi-private API
        // to get the application menu working properly.
        SEL setAppleMenuSelector = NSSelectorFromString(@"setAppleMenu:");
        [NSApp performSelector:setAppleMenuSelector withObject:appMenu];

        // There can only be one application delegate, but we allocate it the
        // first time a window is created to keep all window code in this file
        // TODO Make application delegate a member of some other global structure
        id nativeApplicationDelegate = [[NativeApplicationDelegate alloc] init];
        [NSApp setDelegate:nativeApplicationDelegate];
        [nativeApplicationDelegate release];
        [NSApp activateIgnoringOtherApps:YES];
        [NSApp run];
    }

    // Create Native Window.
    id nativeWindowDelegate = [[NativeWindowDelegate alloc] initWithNativeWindow:this];
    NSRect contentRect = NSMakeRect(0, 0, width, height);
    id window = [[NativeWindowCocoaClass alloc] initWithContentRect:contentRect
                                                          styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
                                                            backing:NSBackingStoreBuffered
                                                              defer:NO];
    [window center];
    [window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
    [window setLevel:NSFloatingWindowLevel];
    [window setTitle:[NSString stringWithUTF8String:title]];
    [window setDelegate:nativeWindowDelegate];
    [nativeWindowDelegate release];
    [window setAcceptsMouseMovedEvents:YES];
    [window setRestorable:NO];
    [window setOpaque:YES];
    [window setHidesOnDeactivate:YES];
    [window orderFront:nil];
    [window makeKeyAndOrderFront:nil];

    m_cocoaWindow = window;
    [window release];
}

NativeWindowCocoa::~NativeWindowCocoa()
{
    NativeWindowCocoaClass *window = (NativeWindowCocoaClass *)m_cocoaWindow;
    [window orderOut:nil];
    [window setDelegate:nil];
    [window close];
}

int NativeWindowCocoa::getWidth() const
{
    id window = (NativeWindowCocoaClass *)m_cocoaWindow;
    id view = [window contentView];
    return [view frame].size.width;
}

int NativeWindowCocoa::getHeight() const
{
    id window = (NativeWindowCocoaClass *)m_cocoaWindow;
    id view = [window contentView];
    return [view frame].size.height;
}

#endif // defined(__APPLE__)

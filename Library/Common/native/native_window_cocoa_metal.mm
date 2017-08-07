//
//  native_window_cocoa_metal.mm
//
//
//  Created by Narendra Umate on 11/19/16.
//
//

#if defined(__APPLE__)

// import and include can be used here interchangeably. both do not compile if added inside header.
#import "native_cocoa_classes.h"

#include "native_window_cocoa_metal.h"

#import <AppKit/AppKit.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

NativeWindowCocoaMetal::NativeWindowCocoaMetal(const int &width, const int &height, const char *title)
    : NativeWindowCocoa(width, height, title)
{
    id window = (NativeWindowCocoaClass *)m_cocoaWindow;
    id view = [[NativeMetalView alloc] init];
    [window setContentView:view];
    [window makeFirstResponder:view];

    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    CAMetalLayer* currentLayer = [CAMetalLayer layer];
    [currentLayer setDevice:device];
    [currentLayer setPixelFormat:MTLPixelFormatBGRA8Unorm_sRGB];
    [currentLayer setFramebufferOnly:NO];
    [view setLayer:currentLayer];
    [view setWantsLayer:YES];
    m_layer = (void*)CFBridgingRetain(currentLayer);
    [view release];
    [window release];
}

NativeWindowCocoaMetal::~NativeWindowCocoaMetal()
{
    CFBridgingRelease(m_layer);
}

void NativeWindowCocoaMetal::getWindowPos(int &xPos, int &yPos)
{
    id window = (NativeWindowCocoaClass *)m_cocoaWindow;
    const NSRect windowFrame = [window frame];
    const NSRect contentRect = [window contentRectForFrameRect:windowFrame];
    xPos = (int)contentRect.origin.x;
    yPos = (int)contentRect.origin.y;
    [window release];
}

void NativeWindowCocoaMetal::getWindowSize(int &width, int &height)
{
    id window = (NativeWindowCocoaClass *)m_cocoaWindow;
    id views = [window contentView];
    const NSRect contentRect = [views frame];
    const NSRect fbRect = [views convertRectToBacking:contentRect];
    width = (int)fbRect.size.width;
    height = (int)fbRect.size.height;
    [window release];
}

void NativeWindowCocoaMetal::getFramebufferSize(int &width, int &height)
{
    id window = (NativeWindowCocoaClass *)m_cocoaWindow;
    id view = [window contentView];
    const NSRect contentRect = [view frame];
    const NSRect fbRect = [view convertRectToBacking:contentRect];
    width = (int)fbRect.size.width;
    height = (int)fbRect.size.height;

    //const NSRect f = [window frame];
    //const NSRect contentRectTwo = [window contentRectForFrameRect:f];
    //int w = (int)contentRectTwo.size.width;
    //int h = (int)contentRectTwo.size.height;
    [window release];
}

void NativeWindowCocoaMetal::makeContextCurrent()
{
}

void NativeWindowCocoaMetal::swapBuffers()
{
    // This call is required to advance the frame. In an Objective-C or Swift application the MTKView base class calls this.
    //[(MTKView*)[(NativeWindowCocoaClass*)m_cocoaWindow contentView] draw];
    //[(MTKView *)m_view draw];
}

void NativeWindowCocoaMetal::swapInterval(const int &interval)
{
}

void* NativeWindowCocoaMetal::getProcAddress(const char *procname)
{
    CFStringRef symbolName = CFStringCreateWithCString(kCFAllocatorDefault, procname, kCFStringEncodingASCII);
    void *symbol = CFBundleGetFunctionPointerForName(CFBundleGetBundleWithIdentifier(CFSTR("com.apple.metal")), symbolName);
    CFRelease(symbolName);
    return symbol;
}

void NativeWindowCocoaMetal::getMetalHackBoth(void **texture, void **drawable) {
    id window = (NativeWindowCocoaClass *)m_cocoaWindow;
    id view = [window contentView];
    const NSRect contentRect = [view frame];
    const NSRect fbRect = [view convertRectToBacking:contentRect];
    int width = (int)fbRect.size.width;
    int height = (int)fbRect.size.height;

    //const NSRect f = [window frame];
    //const NSRect contentRectTwo = [window contentRectForFrameRect:f];
    //int w = (int)contentRectTwo.size.width;
    //int h = (int)contentRectTwo.size.height;
    [window release];

    CAMetalLayer* currentLayer = (__bridge CAMetalLayer*)(m_layer);
    currentLayer.drawableSize = CGSizeMake(width, height);
    id<CAMetalDrawable> nextDrawable = [currentLayer nextDrawable];
    *drawable = (void*)CFBridgingRetain(nextDrawable);
    id<MTLTexture> nextTexture = [nextDrawable texture];
    *texture = (void*)CFBridgingRetain(nextTexture);
}

#endif // defined(__APPLE__)

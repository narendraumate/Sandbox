//
//  native_window_cocoa_opengl.mm
//
//
//  Created by Narendra Umate on 11/19/16.
//
//

#if defined(__APPLE__)

// import does not compile if added inside header.
#import <AppKit/AppKit.h>

// import and include can be used here interchangeably. both do not compile if added inside header.
#import "native_cocoa_classes.h"

#include "native_window_cocoa_opengl.h"

NativeWindowCocoaOpenGL::NativeWindowCocoaOpenGL(const int &width, const int &height, const char *title)
    : NativeWindowCocoa(width, height, title)
{
    id window = (NativeWindowCocoaClass *)m_cocoaWindow;
    id view = [[NativeOpenGLView alloc] init];
    [view setWantsBestResolutionOpenGLSurface:YES];
    [window setContentView:view];
    [window makeFirstResponder:view];

    NSOpenGLPixelFormatAttribute attributes[40];
    unsigned int attributeCount = 0;
#define ADD_ATTR(x)                       \
    {                                     \
        attributes[attributeCount++] = x; \
    }
#define ADD_ATTR2(x, y) \
    {                   \
        ADD_ATTR(x);    \
        ADD_ATTR(y);    \
    }
    ADD_ATTR(NSOpenGLPFAAccelerated);
    ADD_ATTR(NSOpenGLPFAClosestPolicy);
    ADD_ATTR2(NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core);
    //ADD_ATTR2(NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core);
    ADD_ATTR2(NSOpenGLPFAColorSize, 24);
    ADD_ATTR2(NSOpenGLPFAAlphaSize, 8);
    ADD_ATTR2(NSOpenGLPFADepthSize, 24);
    ADD_ATTR2(NSOpenGLPFAStencilSize, 8);
    ADD_ATTR(NSOpenGLPFADoubleBuffer);
    ADD_ATTR2(NSOpenGLPFASampleBuffers, 1);
    ADD_ATTR2(NSOpenGLPFASamples, 32);
    ADD_ATTR(0);
#undef ADD_ATTR
#undef ADD_ATTR2

    id pixelformat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
    id openglcontext = [[NSOpenGLContext alloc] initWithFormat:pixelformat
                                                  shareContext:nullptr];
    [pixelformat release];
    [view setOpenGLContext:openglcontext];
    [view release];

    m_context = openglcontext;
}

NativeWindowCocoaOpenGL::~NativeWindowCocoaOpenGL()
{
    [(NSOpenGLContext *)m_context release];
}

void NativeWindowCocoaOpenGL::getWindowPos(int &xPos, int &yPos)
{
    id window = (NativeWindowCocoaClass *)m_cocoaWindow;
    const NSRect windowFrame = [window frame];
    const NSRect contentRect = [window contentRectForFrameRect:windowFrame];
    xPos = (int)contentRect.origin.x;
    yPos = (int)contentRect.origin.y;
}

void NativeWindowCocoaOpenGL::getWindowSize(int &width, int &height)
{
    id window = (NativeWindowCocoaClass *)m_cocoaWindow;
    id views = [window contentView];
    const NSRect contentRect = [views frame];
    const NSRect fbRect = [views convertRectToBacking:contentRect];
    width = (int)fbRect.size.width;
    height = (int)fbRect.size.height;
}

void NativeWindowCocoaOpenGL::getFramebufferSize(int &width, int &height)
{
    id window = (NativeWindowCocoaClass *)m_cocoaWindow;
    id view = [window contentView];
    const NSRect contentRect = [view frame];
    const NSRect fbRect = [view convertRectToBacking:contentRect];
    width = (int)fbRect.size.width;
    height = (int)fbRect.size.height;
}

void NativeWindowCocoaOpenGL::makeContextCurrent()
{
    [(NSOpenGLContext *)m_context makeCurrentContext];
}

void NativeWindowCocoaOpenGL::swapBuffers()
{
    [(NSOpenGLContext *)m_context flushBuffer];
}

void NativeWindowCocoaOpenGL::swapInterval(const int &interval)
{
    GLint sync = interval;
    [(NSOpenGLContext *)m_context setValues:&sync
                               forParameter:NSOpenGLCPSwapInterval];
}

void* NativeWindowCocoaOpenGL::getProcAddress(const char *procname)
{
    CFStringRef symbolName = CFStringCreateWithCString(kCFAllocatorDefault, procname, kCFStringEncodingASCII);
    void *symbol = CFBundleGetFunctionPointerForName(CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl")), symbolName);
    CFRelease(symbolName);
    return symbol;
}

void NativeWindowCocoaOpenGL::getMetalHackBoth(void **texture, void **drawable)
{
    *texture = nullptr;
    *drawable = nullptr;
}

#endif // defined(__APPLE__)

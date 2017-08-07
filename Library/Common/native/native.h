//
//  native.h
//
//
//  Created by Narendra Umate on 7/17/16.
//
//

#ifndef native_h
#define native_h

#ifdef __cplusplus
extern "C" {
#endif

// clang-format off

#ifndef NATIVEAPI
#  ifdef _WIN32
#     if defined(NATIVE_BUILD_SHARED) /* build dll */
#         define NATIVEAPI __declspec(dllexport)
#     elif !defined(NATIVE_BUILD_STATIC) /* use dll */
#         define NATIVEAPI __declspec(dllimport)
#     else /* static library */
#         define NATIVEAPI
#     endif
#  else
#     if __GNUC__ >= 4
#         define NATIVEAPI __attribute__((visibility("default")))
#     else
#         define NATIVEAPI
#     endif
#  endif
#endif

// BEGIN HACK
#define NATIVE_RESIZABLE              0x00020003
#define NATIVE_OPENGL_FORWARD_COMPAT  0x00022006
#define NATIVE_OPENGL_PROFILE         0x00022008
#define NATIVE_OPENGL_CORE_PROFILE    0x00032001
#define NATIVE_OPENGL_COMPAT_PROFILE  0x00032002
#define NATIVE_CONTEXT_VERSION_MAJOR  0x00022002
#define NATIVE_CONTEXT_VERSION_MINOR  0x00022003
// END HACK

#define NATIVE_TRUE                   1
#define NATIVE_FALSE                  0

#define NATIVE_RELEASE                0
#define NATIVE_PRESS                  1
#define NATIVE_REPEAT                 2

#define NATIVE_KEY_UNKNOWN            -1

#define NATIVE_KEY_SPACE              32
#define NATIVE_KEY_APOSTROPHE         39  /* ' */
#define NATIVE_KEY_COMMA              44  /* , */
#define NATIVE_KEY_MINUS              45  /* - */
#define NATIVE_KEY_PERIOD             46  /* . */
#define NATIVE_KEY_SLASH              47  /* / */
#define NATIVE_KEY_0                  48
#define NATIVE_KEY_1                  49
#define NATIVE_KEY_2                  50
#define NATIVE_KEY_3                  51
#define NATIVE_KEY_4                  52
#define NATIVE_KEY_5                  53
#define NATIVE_KEY_6                  54
#define NATIVE_KEY_7                  55
#define NATIVE_KEY_8                  56
#define NATIVE_KEY_9                  57
#define NATIVE_KEY_SEMICOLON          59  /* ; */
#define NATIVE_KEY_EQUAL              61  /* = */
#define NATIVE_KEY_A                  65
#define NATIVE_KEY_B                  66
#define NATIVE_KEY_C                  67
#define NATIVE_KEY_D                  68
#define NATIVE_KEY_E                  69
#define NATIVE_KEY_F                  70
#define NATIVE_KEY_G                  71
#define NATIVE_KEY_H                  72
#define NATIVE_KEY_I                  73
#define NATIVE_KEY_J                  74
#define NATIVE_KEY_K                  75
#define NATIVE_KEY_L                  76
#define NATIVE_KEY_M                  77
#define NATIVE_KEY_N                  78
#define NATIVE_KEY_O                  79
#define NATIVE_KEY_P                  80
#define NATIVE_KEY_Q                  81
#define NATIVE_KEY_R                  82
#define NATIVE_KEY_S                  83
#define NATIVE_KEY_T                  84
#define NATIVE_KEY_U                  85
#define NATIVE_KEY_V                  86
#define NATIVE_KEY_W                  87
#define NATIVE_KEY_X                  88
#define NATIVE_KEY_Y                  89
#define NATIVE_KEY_Z                  90
#define NATIVE_KEY_LEFT_BRACKET       91  /* [ */
#define NATIVE_KEY_BACKSLASH          92  /* \ */
#define NATIVE_KEY_RIGHT_BRACKET      93  /* ] */
#define NATIVE_KEY_GRAVE_ACCENT       96  /* ` */
#define NATIVE_KEY_WORLD_1            161 /* non-US #1 */
#define NATIVE_KEY_WORLD_2            162 /* non-US #2 */

#define NATIVE_KEY_ESCAPE             256
#define NATIVE_KEY_ENTER              257
#define NATIVE_KEY_TAB                258
#define NATIVE_KEY_BACKSPACE          259
#define NATIVE_KEY_INSERT             260
#define NATIVE_KEY_DELETE             261
#define NATIVE_KEY_RIGHT              262
#define NATIVE_KEY_LEFT               263
#define NATIVE_KEY_DOWN               264
#define NATIVE_KEY_UP                 265
#define NATIVE_KEY_PAGE_UP            266
#define NATIVE_KEY_PAGE_DOWN          267
#define NATIVE_KEY_HOME               268
#define NATIVE_KEY_END                269
#define NATIVE_KEY_CAPS_LOCK          280
#define NATIVE_KEY_SCROLL_LOCK        281
#define NATIVE_KEY_NUM_LOCK           282
#define NATIVE_KEY_PRINT_SCREEN       283
#define NATIVE_KEY_PAUSE              284
#define NATIVE_KEY_F1                 290
#define NATIVE_KEY_F2                 291
#define NATIVE_KEY_F3                 292
#define NATIVE_KEY_F4                 293
#define NATIVE_KEY_F5                 294
#define NATIVE_KEY_F6                 295
#define NATIVE_KEY_F7                 296
#define NATIVE_KEY_F8                 297
#define NATIVE_KEY_F9                 298
#define NATIVE_KEY_F10                299
#define NATIVE_KEY_F11                300
#define NATIVE_KEY_F12                301
#define NATIVE_KEY_F13                302
#define NATIVE_KEY_F14                303
#define NATIVE_KEY_F15                304
#define NATIVE_KEY_F16                305
#define NATIVE_KEY_F17                306
#define NATIVE_KEY_F18                307
#define NATIVE_KEY_F19                308
#define NATIVE_KEY_F20                309
#define NATIVE_KEY_F21                310
#define NATIVE_KEY_F22                311
#define NATIVE_KEY_F23                312
#define NATIVE_KEY_F24                313
#define NATIVE_KEY_F25                314
#define NATIVE_KEY_KP_0               320
#define NATIVE_KEY_KP_1               321
#define NATIVE_KEY_KP_2               322
#define NATIVE_KEY_KP_3               323
#define NATIVE_KEY_KP_4               324
#define NATIVE_KEY_KP_5               325
#define NATIVE_KEY_KP_6               326
#define NATIVE_KEY_KP_7               327
#define NATIVE_KEY_KP_8               328
#define NATIVE_KEY_KP_9               329
#define NATIVE_KEY_KP_DECIMAL         330
#define NATIVE_KEY_KP_DIVIDE          331
#define NATIVE_KEY_KP_MULTIPLY        332
#define NATIVE_KEY_KP_SUBTRACT        333
#define NATIVE_KEY_KP_ADD             334
#define NATIVE_KEY_KP_ENTER           335
#define NATIVE_KEY_KP_EQUAL           336
#define NATIVE_KEY_LEFT_SHIFT         340
#define NATIVE_KEY_LEFT_CONTROL       341
#define NATIVE_KEY_LEFT_ALT           342
#define NATIVE_KEY_LEFT_SUPER         343
#define NATIVE_KEY_RIGHT_SHIFT        344
#define NATIVE_KEY_RIGHT_CONTROL      345
#define NATIVE_KEY_RIGHT_ALT          346
#define NATIVE_KEY_RIGHT_SUPER        347
#define NATIVE_KEY_MENU               348

#define NATIVE_FOCUSED                0x00020001
#define NATIVE_ICONIFIED              0x00020002
#define NATIVE_RESIZABLE              0x00020003
#define NATIVE_VISIBLE                0x00020004
#define NATIVE_DECORATED              0x00020005
#define NATIVE_AUTO_ICONIFY           0x00020006
#define NATIVE_FLOATING               0x00020007
#define NATIVE_MAXIMIZED              0x00020008

// clang-format on

typedef struct NativeMonitorBasic NativeMonitorBasic;
typedef struct NativeWindowBasic NativeWindowBasic;

typedef void (*Nativeglproc)(void);
typedef void (*Nativeerrorfun)(int, const char*);
typedef void (*Nativewindowposfun)(NativeWindowBasic*, int, int);
typedef void (*Nativewindowsizefun)(NativeWindowBasic*, int, int);
typedef void (*Nativewindowclosefun)(NativeWindowBasic*);
typedef void (*Nativewindowrefreshfun)(NativeWindowBasic*);
typedef void (*Nativewindowfocusfun)(NativeWindowBasic*, int);
typedef void (*Nativewindowiconifyfun)(NativeWindowBasic*, int);
typedef void (*Nativewindowmaximizefun)(NativeWindowBasic*, int);
typedef void (*Nativeframebuffersizefun)(NativeWindowBasic*, int, int);
typedef void (*Nativekeyfun)(NativeWindowBasic*, int, int, int, int);

NATIVEAPI Nativeerrorfun nativeSetErrorCallback(Nativeerrorfun cbfun);

NATIVEAPI Nativewindowposfun nativeSetWindowPosCallback(NativeWindowBasic* window, Nativewindowposfun cbfun);

NATIVEAPI Nativewindowsizefun nativeSetWindowSizeCallback(NativeWindowBasic* window, Nativewindowsizefun cbfun);

NATIVEAPI Nativewindowclosefun nativeSetWindowCloseCallback(NativeWindowBasic* window, Nativewindowclosefun cbfun);

NATIVEAPI Nativewindowrefreshfun nativeSetWindowRefreshCallback(NativeWindowBasic* window, Nativewindowrefreshfun cbfun);

NATIVEAPI Nativewindowfocusfun nativeSetWindowFocusCallback(NativeWindowBasic* window, Nativewindowfocusfun cbfun);

NATIVEAPI Nativewindowiconifyfun nativeSetWindowIconifyCallback(NativeWindowBasic* window, Nativewindowiconifyfun cbfun);

NATIVEAPI Nativewindowmaximizefun nativeSetWindowMaximizeCallback(NativeWindowBasic* window, Nativewindowmaximizefun cbfun);

NATIVEAPI Nativeframebuffersizefun nativeSetFramebufferSizeCallback(NativeWindowBasic* window, Nativeframebuffersizefun cbfun);

NATIVEAPI Nativekeyfun nativeSetKeyCallback(NativeWindowBasic* window, Nativekeyfun cbfun);

NATIVEAPI void nativeWindowHint(int hint, int value);

NATIVEAPI int nativeInit(void);

NATIVEAPI void nativePollEvents(void);

NATIVEAPI void nativeTerminate(void);

NATIVEAPI NativeWindowBasic* nativeCreateWindowDirectX(int width, int height, const char* title, NativeMonitorBasic* monitor, NativeWindowBasic* share);

NATIVEAPI NativeWindowBasic* nativeCreateWindowMetal(int width, int height, const char* title, NativeMonitorBasic* monitor, NativeWindowBasic* share);

NATIVEAPI NativeWindowBasic* nativeCreateWindowOpenGL(int width, int height, const char* title, NativeMonitorBasic* monitor, NativeWindowBasic* share);

NATIVEAPI NativeWindowBasic* nativeCreateWindowVulkan(int width, int height, const char* title, NativeMonitorBasic* monitor, NativeWindowBasic* share);

NATIVEAPI void nativeDestroyWindow(NativeWindowBasic* window);

NATIVEAPI void nativeGetWindowPos(NativeWindowBasic* window, int* xPos, int* yPos);

NATIVEAPI void nativeGetWindowSize(NativeWindowBasic* window, int* width, int* height);

NATIVEAPI void nativeGetFramebufferSize(NativeWindowBasic* window, int* width, int* height);

NATIVEAPI void nativeMakeContextCurrent(NativeWindowBasic* window);

NATIVEAPI void nativeSwapBuffers(NativeWindowBasic* window);

NATIVEAPI void nativeSwapInterval(int interval);

NATIVEAPI void* nativeGetProcAddress(const char* procname);

NATIVEAPI void nativeGetMetalHackBoth(void** texture, void** drawable); // TODO Remove this hack api

#ifdef __cplusplus
}
#endif

#endif /* native_h */

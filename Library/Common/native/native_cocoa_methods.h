//
//  native_cocoa_methods.h
//
//
//  Created by Narendra Umate on 7/17/16.
//
//

#ifndef native_cocoa_methods_h
#define native_cocoa_methods_h

#if defined(__APPLE__)

int platformInit();

void platformPollEvents();

void platformTerminate();

#endif // defined(__APPLE__)

#endif /* native_cocoa_methods_h */

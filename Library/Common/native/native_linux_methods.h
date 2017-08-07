//
//  native_linux_methods.h
//
//
//  Created by Narendra Umate on 7/17/16.
//
//

#ifndef native_linux_methods_h
#define native_linux_methods_h

#if defined(__linux__)

int platformInit();

void platformPollEvents();

void platformTerminate();

#endif // defined(__linux__)

#endif /* native_linux_methods_h */

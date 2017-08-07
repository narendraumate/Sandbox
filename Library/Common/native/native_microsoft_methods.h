//
//  native_microsoft_methods.h
//
//
//  Created by Narendra Umate on 7/17/16.
//
//

#ifndef native_microsoft_methods_h
#define native_microsoft_methods_h

#if defined(_WIN32)

int platformInit();

void platformPollEvents();

void platformTerminate();

#endif // defined(_WIN32)

#endif /* native_microsoft_methods_h */

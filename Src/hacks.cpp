/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#include <cstdlib>
#include <sys/types.h>


/*
 * The default pulls in 70K of garbage
 */

namespace __gnu_cxx {

  void __verbose_terminate_handler() {
    for(;;);
  }
}


/*
 * The default pulls in about 12K of garbage
 */

extern "C" void __cxa_pure_virtual() {
  for(;;);
}


/*
 * Implement C++ new/delete operators using the heap
 */

void *operator new(size_t size) {
	return (void*)0;
}

void *operator new(size_t,void *ptr) {
	return (void*)0;
}

void *operator new[](size_t size) {
	return (void*)0;
}

void *operator new[](size_t,void *ptr) {
	return (void*)0;
}

void operator delete(void *p) {
}

void operator delete[](void *p) {
}

void *malloc(size_t size){
	return (void*)0;
}

void free(void *ptr){
}

/*
 * EABI builds can generate reams of stack unwind code for system generated exceptions
 * e.g. (divide-by-zero). Since we don't support exceptions we'll wrap out these
 * symbols and save a lot of flash space.
 */

extern "C" void __wrap___aeabi_unwind_cpp_pr0() {}
extern "C" void __wrap___aeabi_unwind_cpp_pr1() {}
extern "C" void __wrap___aeabi_unwind_cpp_pr2() {}


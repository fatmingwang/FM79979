/**
	@file cFUThreadRef.h
	This file contains the cFUThreadRef type definition.

	Separated from cFUThread.h to avoid circular dependencies between
	cFUThread and cFUCriticalSection.
*/

#ifndef _FU_THREADREF_H_
#define _FU_THREADREF_H_

#if defined(WIN32)
typedef unsigned long cFUThreadRef;
#elif defined(FP_APPLE) 
typedef MPTaskID cFUThreadRef;
#elif defined(LINUX) || defined(IOS) || defined(ANDROID)
#include <pthread.h>
typedef pthread_t cFUThreadRef;
#else
#error "cFUThreadRef is not defined  for this platform"
#endif // OS-dependent

#endif // _FU_THREADREF_H_

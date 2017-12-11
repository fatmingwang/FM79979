/**
	@file cFUThread.h
	This file contains the cFUThread class.
*/

#ifndef _FU_THREAD_H_
#define _FU_THREAD_H_

#ifndef _FU_SEMAPHORE_H_
#include "FUSemaphore.h"
#endif // _FU_SEMAPHORE_H_
#ifndef _FU_CRITICAL_SECTION_H_
#include "FUCriticalSection.h"
#endif // _FU_CRITICAL_SECTION_H_
#ifndef _FU_THREADREF_H_
#include "FUThreadRef.h"
#endif // _FU_THREADREF_H_

//Platform-specific type for the function pointer to pass to the constructor.
#if defined(WIN32)
typedef unsigned (__stdcall *TaskProc)(void*);
#define THREAD_ROUTINE_POINTER TaskProc
#define DeclareThreadRoutine(_name, param) static unsigned __stdcall _name(void *param);
#define ImplementThreadRoutine(_name, param) unsigned __stdcall _name(void *param)
#elif defined(FP_APPLE)
#define THREAD_ROUTINE_POINTER TaskProc
#define DeclareThreadRoutine(_name, param) static OSStatus _name(void* param);
#define ImplementThreadRoutine(_name, param) OSStatus _name(void* param)
#elif defined(LINUX) || defined(IOS) || defined(ANDROID)|| defined(WASM)
typedef void* (*TaskProc)(void*);
#define THREAD_ROUTINE_POINTER TaskProc
#define DeclareThreadRoutine(_name, param) static void* _name(void* param);
#define ImplementThreadRoutine(_name, param) void* _name(void* param)
#endif

/**
	An OS independent thread. 
	@ingroup FUtils
*/
class  cFUThread
{
private:
	bool isSuspended;
	cFUCriticalSection isSuspendedCS;
	FUBinarySemaphore semaphore, threadStartedSemaphore;
	bool terminate;
	uint32 exitCode;
	std::string name;

	DeclareThreadRoutine(InternalFunc, paramPtr);

#if defined(WIN32)
	HANDLE thread;
#elif defined(FP_APPLE)
	MPTaskID thread;
	MPQueueID queue;
#elif defined(LINUX) || defined(IOS)|| defined(ANDROID)
	pthread_t thread;
#endif

public:
	/** Constructor.
		@param startAddress The procedure to start the new thread running.
		@param parameter The parameter to pass to the new thread.
		@param suspended Whether the created thread should start immediately or not. Defaults to false.
		@param szName The thread name used in debuggers.*/
	cFUThread(THREAD_ROUTINE_POINTER startAddress, void* parameter, bool suspended = false, const char* szName = nullptr);

	/** Destructor. */
	virtual ~cFUThread();
	
	/** Starts the thread if the thread was created suspended. Does nothing otherwise. */
	void StartThread();
	
	/** Waits for the thread to exit and clean up after it. */
	void StopThread();

	/** Yields the current thread to other threads, giving them a chance to run. */
	static void YieldCurrentThread();
	
	/** Sleeps the current thread for a minimum specified duration.
		@param milliseconds The duration to sleep. */
	static void SleepCurrentThread(unsigned long milliseconds);

	/** Retrieves the current thread id.*/
	static cFUThreadRef CurrentThreadID();

	/** Sets the name of the thread specified by the given id.
		This function will not modify the cFUThread::name member and should
		be used to set the name of threads not created by the application.
		@param id Give "-1" to set the calling thread name.
		@param name The zero-terminated name to set in the debugger.*/
	static void SetThreadName(cFUThreadRef id, const char* name);

private:
	void SetThreadName();
};

#endif // _FU_THREAD_H_

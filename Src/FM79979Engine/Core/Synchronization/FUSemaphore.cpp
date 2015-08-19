#include "../stdAfx.h"
#include "FUSemaphore.h"


#if defined(LINUX) || defined(ANDROID) || defined(IOS)
#include <time.h>
#endif

cFUSemaphore::cFUSemaphore(uint32 initialValue, uint32 maximumValue)
#if defined(WIN32)
:	semaphoreHandle(nullptr)
#endif // windows-only
{	
	assert(initialValue <= maximumValue);
#if defined(WIN32)
	semaphoreHandle = CreateSemaphore(nullptr, initialValue, maximumValue, nullptr);
	assert(semaphoreHandle != nullptr);
#elif defined(FP_APPLE)
	MPCreateSemaphore(maximumValue, initialValue, &semaphoreHandle);
#elif defined(LINUX) || defined(ANDROID) || defined(IOS)
	sem_init(&semaphoreHandle, 0, initialValue);
#endif
}

cFUSemaphore::~cFUSemaphore()
{
#if defined(WIN32)
	CloseHandle(semaphoreHandle);
#elif defined(FP_APPLE)
	MPDeleteSemaphore(semaphoreHandle);
#elif defined(LINUX) || defined(ANDROID) || defined(IOS)
	sem_destroy(&semaphoreHandle);
#endif
}

void cFUSemaphore::Up()
{
#if defined(WIN32)
	ReleaseSemaphore(semaphoreHandle, 1, nullptr);
#elif defined(FP_APPLE)
	MPSignalSemaphore(semaphoreHandle);
#elif defined(LINUX) || defined(ANDROID) || defined(IOS)
	sem_post(&semaphoreHandle);
#endif
}

bool cFUSemaphore::Down(uint32 blockTime)
{
#if defined(WIN32)
	return WaitForSingleObject(semaphoreHandle, blockTime) == WAIT_OBJECT_0;
#elif defined(FP_APPLE)
	return MPWaitOnSemaphore(semaphoreHandle, (blockTime == (uint32)-1) ? kDurationForever : blockTime) == noErr;
#elif defined(LINUX) || defined(ANDROID) || defined(IOS)
	if (blockTime == (uint32) -1)
	{
		return sem_wait(&semaphoreHandle) == 0;
	}
	else
	{
#ifdef IOS
		//fuck because I am fucking lazy to implement this so...
		return sem_wait(&semaphoreHandle) == 0;
#else

		// The basic POSIX semaphore does not support a block time, but rather a deadline.
		timespec deadline;
		
		clock_gettime(CLOCK_REALTIME, &deadline);
		
		deadline.tv_nsec += (blockTime * 1000);
		
		return sem_timedwait(&semaphoreHandle, &deadline) == 0;
#endif
	}
#endif
}

#include "../stdAfx.h"
#include "FUCriticalSection.h"
#include "FUThread.h"

#if defined(LINUX) || defined(ANDROID)|| defined(IOS)
#include <pthread.h>
#endif

cFUCriticalSection::cFUCriticalSection()
:	ownerThread(0)
,	lockCount(0)
{
#if defined(WIN32)
	InitializeCriticalSection(&criticalSection);
#elif defined(FP_APPLE)
	MPCreateCriticalRegion(&criticalSection);
#elif defined(LINUX)|| defined(ANDROID) || defined(IOS)
	criticalSection = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;//or PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;  PTHREAD_RMUTEX_INITIALIZER?
#endif
}

cFUCriticalSection::~cFUCriticalSection()
{
	assert(ownerThread == 0);
#if defined(WIN32)
	DeleteCriticalSection(&criticalSection);
#elif defined(FP_APPLE)
	MPDeleteCriticalRegion(criticalSection);
#elif defined(LINUX) || defined(ANDROID) || defined(IOS)
	pthread_mutex_destroy(&criticalSection);
#endif
}

void cFUCriticalSection::Enter() const
{
#if defined(WIN32)
	EnterCriticalSection(&criticalSection);
#elif defined(FP_APPLE)
	MPEnterCriticalRegion(criticalSection, kDurationForever);
#elif defined(LINUX) || defined(ANDROID)|| defined(IOS)
	pthread_mutex_lock(&criticalSection);
#endif
	// it is an error to enter a critical section that thinks it is owned.
	assert((ownerThread == 0  && lockCount == 0) ||
			 (cFUThread::CurrentThreadID() == ownerThread));

	ownerThread = cFUThread::CurrentThreadID();
	++ lockCount;
}

void cFUCriticalSection::Leave() const
{
	// it is an error to leave a critical section you don't own it.
	assert(cFUThread::CurrentThreadID() == ownerThread && lockCount > 0);

	-- lockCount;
	if (lockCount == 0)
	{
		ownerThread = 0;
	}

#if defined(WIN32)
	LeaveCriticalSection(&criticalSection);
#elif defined(FP_APPLE)
	MPExitCriticalRegion(criticalSection);
#elif defined(LINUX) || defined(ANDROID)|| defined(IOS)
	pthread_mutex_unlock(&criticalSection);
#endif
}

/////////////////////////////////////////////////////////////////////
// cFUCriticalSectionHold
/////////////////////////////////////////////////////////////////////

cFUCriticalSectionHold::cFUCriticalSectionHold(const cFUCriticalSection& _cs)
:	cs(const_cast<cFUCriticalSection*>(&_cs))
,	left(true)
{
	Enter();
}

cFUCriticalSectionHold::~cFUCriticalSectionHold()
{
	if (!left)
	{
		Leave();
	}
}

void cFUCriticalSectionHold::Enter()
{
	assert(left);
	cs->Enter();
	left = false;
}

void cFUCriticalSectionHold::Leave()
{
	left = true;
	cs->Leave();
}

cFUCriticalSectionUnlocker::cFUCriticalSectionUnlocker(const cFUCriticalSection& _cs)
:	cs(const_cast<cFUCriticalSection*>(&_cs))
,	numLocks(0)
{
	assert(_cs.GetOwnerThreadRef() == cFUThread::CurrentThreadID());
	numLocks = cs->GetLockCount();
	while (cs->GetLockCount() > 0)
		cs->Leave();
}

cFUCriticalSectionUnlocker::~cFUCriticalSectionUnlocker()
{
	for (int i = 0; i < numLocks; ++i)
		cs->Enter();
}

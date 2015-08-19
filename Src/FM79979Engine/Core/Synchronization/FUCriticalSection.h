/**
	@file cFUCriticalSection.h
	This file contains the cFUCriticalSection class.
*/

#ifndef _FU_CRITICAL_SECTION_H_
#define _FU_CRITICAL_SECTION_H_

#if defined(FP_APPLE)
#include <CoreServices/CoreServices.h>
#elif defined(LINUX) || defined(ANDROID)|| defined(IOS)
#include <pthread.h>
#endif

//#ifndef _FU_THREAD_H_
#include "FUThreadRef.h"
//#endif // _FU_THREAD_H_

/**
	An OS independent critical section.
	Currently only supported for Windows.
	@ingroup FUtils
*/
class  cFUCriticalSection
{
private:
#if defined(WIN32)
	mutable CRITICAL_SECTION criticalSection; // windows-only
#elif defined(FP_APPLE)
	mutable MPCriticalRegionID criticalSection;
#elif defined(LINUX) || defined(IOS) || defined(ANDROID)
	mutable pthread_mutex_t criticalSection;
#else
#warning "cFUCriticalSection: Critical section not implemented for other platforms."
#endif
	mutable cFUThreadRef ownerThread;
	mutable int lockCount;

public:
	/** Constructor. */
	cFUCriticalSection();

	/** Destructor. */
	virtual ~cFUCriticalSection();

	/** Enters the critical section, blocking if another thread is already in.
		Note: a thread may call Enter multiple times and it will still enter to prevent it from deadlocking itself. It
				must then call Leave the same number of times. */
	void Enter() const;

	/** Leaves the critical section, allowing other threads to enter.
		It is an error to leave a critical section if the current thread does not own it.*/
	void Leave() const;

	/** Returns the thread that currently owns this critical section.
		@return The owner thread. 0 if none.*/
	inline cFUThreadRef GetOwnerThreadRef() const { return ownerThread; }

	/** Returns the number of times the current owner thread entered this
		critical section.*/
	inline int GetLockCount() const { return lockCount; }
};

/** A utility class to safely and automatically Enter/Leave a critical section
	using object construction/destruction.*/
class  cFUCriticalSectionHold
{
private:
	cFUCriticalSection* cs;
	bool left;

	cFUCriticalSectionHold() {}
	cFUCriticalSectionHold(const cFUCriticalSectionHold&) {}

public:
	/** Constructor.
		Creates a critical section holder.
		Enters the provided critical section, waiting if necessary to lock it.
		@param cs A critical section. */
	cFUCriticalSectionHold(const cFUCriticalSection& cs);

	/** Destructor.
		If it is currently inside the critical section, it will leave it. */
	virtual ~cFUCriticalSectionHold();

	/** Re-enters the critical section.
		If it is already inside the critical section, nothing happens.
		If another thread is inside the critical section, it will wait for the other thread to be done. */
	void Enter();

	/** Leaves the critical section.
		If it is not inside the critical section, nothing happens. */
	void Leave();
};

/** A utility class to safely and automatically unlock a critical section completely
	and then re-lock it.*/
class  cFUCriticalSectionUnlocker
{
private:
	cFUCriticalSection* cs;
	int numLocks;

	cFUCriticalSectionUnlocker() {}
	cFUCriticalSectionUnlocker(const cFUCriticalSectionUnlocker&) {}

public:
	/** Constructor.
		Creates a critical section unlocker.
		Leaves the provided critical section.
		@param cs A critical section. */
	cFUCriticalSectionUnlocker(const cFUCriticalSection& cs);

	/** Destructor.
		Re-enters the critical section, waiting if necessary to lock it. */
	virtual ~cFUCriticalSectionUnlocker();
};

/** Runs a block of code within a critical section.
	Using this macro for small blocks of code will help keep your code clean.
	@param criticalSection A reference to the critical section to lock
		while executing this block of code. */
#define ExecuteCriticalCodeBlock(criticalSection, ...) { cFUCriticalSectionHold __hold(criticalSection); __VA_ARGS__; }

#endif // _FU_CRITICAL_SECTION_H_


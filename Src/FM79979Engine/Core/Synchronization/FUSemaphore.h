/**
	@file cFUSemaphore.h
	This file contains the cFUSemaphore and FUBinarySemaphore classes.
*/

#ifndef _FU_SEMAPHORE_H_
#define _FU_SEMAPHORE_H_

#if defined(FP_APPLE)
#include <CoreServices/CoreServices.h>
#elif defined(LINUX) || defined(IOS)|| defined(ANDROID)|| defined(WASM)
#include <semaphore.h>
#endif

/**
	An OS independent semaphore. 
	Currently only supported for Windows.
	@ingroup FUtils
*/
class  cFUSemaphore
{
private:
#if defined(WIN32)
	HANDLE semaphoreHandle;
#elif defined(FP_APPLE)
	MPSemaphoreID semaphoreHandle;
#elif defined(LINUX) || defined(IOS) || defined(ANDROID)|| defined(WASM)
	sem_t semaphoreHandle;
#else
#warning "cFUSemaphore is not implemented on this platform"
#endif

public:
	/** Constructor. 
		@param initialValue The initial value for the semaphore.
		@param maximumValue The maximum value for the semaphore. Must be higher than initialValue. */
	cFUSemaphore(uint32 initialValue, uint32 maximumValue);

	/** Destructor. */
	virtual ~cFUSemaphore();

	/** Increments the value of the semaphore. 
		Do not increment it above the maximum value set in the constructor. No check is made. */
	void Up();

	/** Decrements the value of the semaphore.

		The value of the semaphore never drops below 0. If the current value is 0, and this method is called,
		then this method blocks until it is possible to decrement it and then does so. This is made possible when
		another thread calls Up. 

		If more than one thread is blocked, and Up is called, any one of the waiting threads may be unblocked.
		
		@param blockTime The maximum amount of time that this function call will wait for the semaphore to be available.
		@return Whether the semaphore is Down'ed. If False, then the function call returned because the blockTime has expired. */
	bool Down(uint32 blockTime = (uint32) -1); 
};

/**
	An OS dependent binary semaphore.

	It only has the values 0, and 1.
*/
class  FUBinarySemaphore : public cFUSemaphore
{
public:
	FUBinarySemaphore() : cFUSemaphore(0, 1) {}
};

#endif // _FU_SEMAPHORE_H_


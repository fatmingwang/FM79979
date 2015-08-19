/**
	@file cFUSynchronized.h
	This file contains the cFUSynchronized class.
*/

#ifndef _FU_SYNCHRONIZED_H_
#define _FU_SYNCHRONIZED_H_

#ifndef _FU_CRITICAL_SECTION_H_
#include "FUCriticalSection.h"
#endif

/**
	A synchronized object.
	Has the ability to lock itself so only one thread may be using it. 
	This can be done by calling the Lock and Unlock methods of the object.
	@ingroup FUtils
*/

//bool condition = false;
//cFUSynchronized	g_TestFUSynchronized;

//Thread A:
//    cFUSynchronizedHold(&g_TestFUSynchronized);
//    while (!condition) {
//        g_TestFUSynchronized.WaitCondition();
//    }
//
//Thread B:
//    cFUSynchronizedHold(&g_TestFUSynchronized);
//    ...
//    condition = true;
//    ...
//    ConditionOk(cond);

class  cFUSynchronized
{
private:
	cFUCriticalSection criticalSection;
	bool		m_bWaitingSingnale;
public:
	cFUSynchronized(){m_bWaitingSingnale = false;}
	/** Destructor. */
	virtual ~cFUSynchronized();

	/** Locks the object, blocking if another thread has locked it. */
	virtual void Lock();

	/** Unlocks the object, allowing other threads to lock it. */
	virtual void Unlock();
	//
	inline	bool	IsWaitingSingnale(){ return m_bWaitingSingnale; }

	//wait for condition
	void		WaitCondition();
	//condition is ok
	void		ConditionOk();
};


/** 
	This small helper will lock a synchronized object when created and hold on to the
	locked synchronized for its lifetime. This is useful for all the member functions of a synchronized object
	or concurrent accesses to a synchronized object that may include 'return', 'break' or such statements.
	@ingroup FUtils
*/
class cFUSynchronizedHold
{
private:
	/** The resource currently locked. */
	cFUSynchronized* held; 

public: 
	/** Constructor.
		@param toLock The resource to lock. */
	cFUSynchronizedHold(const cFUSynchronized* toLock) : held(const_cast<cFUSynchronized*>(toLock)) { held->Lock(); }

	/** Destructor. */
	virtual ~cFUSynchronizedHold() { held->Unlock(); }
	void		ForceUnlock(){ held->Unlock(); }
};

//
//cFUSynchronized	l_FUSynchronized;
//cFUSynchronizedHold hold(l_FUSynchronized);
//
//
	

#endif // _FU_SYNCHRONIZED_H_

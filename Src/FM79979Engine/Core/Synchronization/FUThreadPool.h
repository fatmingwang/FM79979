/**
	@file cFUThreadPool.h
	This file contains the cFUThreadPool class.
*/

#ifndef _FU_THREAD_POOL_H_
#define _FU_THREAD_POOL_H_

#ifndef _FU_THREAD_H_
#include "cFUThread.h"
#endif // _FU_THREAD_H_

#include "FUFunctor.h"
class cFUThread;
class cFUCriticalSection;

/**
	A thread pool.
	Allocates multiple threads that can be shared to do finite work.
	@ingroup FUtils
*/
class  cFUThreadPool
{
private:
	std::vector<cFUThread*> threads;

	struct WorkItem
	{
		IFunctor2<size_t, size_t, void>* workFunctor;
		IFunctor2<size_t, size_t, void>* doneFunctor;
		size_t parameter1;
		size_t parameter2;
	};

	typedef std::vector<WorkItem*> WorkItemQueue;
	WorkItemQueue workQueue;
	cFUCriticalSection* workQueueCS;
	mutable bool killAllFlag;
	mutable int32 idleThreadCount;

public:
	/** Constructor.
		Creates an empty thread pool. */
	cFUThreadPool();

	/** Destructor. */
	virtual ~cFUThreadPool();

	/** Clears all remaining work items and wait for the existing items to complete.
		Any spawned threads will be stopped. */
	virtual	void Clear();

	/** Spawns the necessary number of thread. */
	void Spawn(size_t threadCount);

	/** Executes a work function on the next available worker thread.
		If no worker thread has yet been spawned, the work will not be executed.
		@param workFunctor The functor to call that executes the work.
			This functor is not guaranteed to be called.
		@param doneFunctor The functor to call when the work is done
			or when the work item is killed before any work is executed.
			This functor is guaranteed to be called.
		@param parameter1 A first parameter for the functors.
		@param parameter2 A second parameter for the functors. */
	void ExecuteWork(IFunctor2<size_t, size_t, void>* workFunctor, IFunctor2<size_t, size_t, void>* doneFunctor, size_t parameter1, size_t parameter2);

	/** Will stall the current thread, until all this pool's worker thread
		are finished and the work queue is empty.
		Use this function carefully, because it is easy to create dead-locks.
		@param maxWaitTime The maximum number of milliseconds to stall.
			This parameters defaults to (uint32) -1 which implies a complete stall.
		@return Whether the thread pool is now idle. */
	bool WaitForIdle(uint32 maxWaitTime = (uint32) -1);

private:
	DeclareThreadRoutine(WorkerThread, threadPool);
};

//cFUThreadPool*g_pFUThreadPool = 0;
//
//static void DoWorkTest(size_t _workParameter, size_t _pUri)
//{
//
//}
//
//static void DoneWorkTest(size_t _workParameter, size_t _pUri)
//{
//
//}
//	g_pFUThreadPool = new cFUThreadPool();
//	g_pFUThreadPool->Spawn(2);
//	int*l_pInputData = new int[50];
//
//	FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&DoWorkTest);
//	FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&DoneWorkTest);
//	g_pFUThreadPool->ExecuteWork(workFunctor,doneFunctor,(size_t)l_pInputData,0);

#endif // _FU_THREAD_POOL_H_

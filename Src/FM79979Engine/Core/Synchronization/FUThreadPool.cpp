#include "../StdAfx.h"
#include "FUCriticalSection.h"
#include "FUThread.h"
#include "FUThreadPool.h"

//
// cFUThreadPool
//

cFUThreadPool::cFUThreadPool()
:	threads()
,	workQueue(), workQueueCS(nullptr), killAllFlag(true), idleThreadCount(0)
{
	workQueueCS = new cFUCriticalSection();
	this->Spawn(1);
}

cFUThreadPool::~cFUThreadPool()
{
	Clear();
	SAFE_DELETE(workQueueCS);
}

void cFUThreadPool::Clear()
{
	// Trigger the deletion flag and wait for all the threads to complete their work and stop.
	ExecuteCriticalCodeBlock(*workQueueCS, killAllFlag = true);
	for (size_t i = 0; i < threads.size(); ++i)
	{
		threads[i]->StopThread();
		SAFE_DELETE(threads[i]);
	}

	// Remove all existing work items.
	size_t workItemCount = 0;
	ExecuteCriticalCodeBlock(*workQueueCS, workItemCount = workQueue.size());
	while (workItemCount > 0)
	{
		WorkItem* workItem = nullptr;
		ExecuteCriticalCodeBlock(*workQueueCS, if (workQueue.empty()) break; else workItem = workQueue.back(); workQueue.pop_back());
		(*(workItem->doneFunctor))(workItem->parameter1, workItem->parameter2);
		SAFE_DELETE(workItem->workFunctor);
		SAFE_DELETE(workItem->doneFunctor);
		SAFE_DELETE(workItem);
		ExecuteCriticalCodeBlock(*workQueueCS, workItemCount = workQueue.size());
	}
	threads.clear();
}

void cFUThreadPool::Spawn(size_t threadCount)
{
	if (threadCount <= threads.size()) return; // the thread pool doesn't support shrinking.

	// For clear & re-spawn support -> reset the 'kill-all' flag.
	ExecuteCriticalCodeBlock(*workQueueCS, killAllFlag = false);
	// Resize to the wanted number of worker threads, creating threads as needed.
	threads.resize(threadCount);
	for (size_t i = 0; i < threadCount; ++i)
	{
		if (threads[i] == nullptr)
		{
			threads[i] = new cFUThread(&cFUThreadPool::WorkerThread, this, false, "cFUThreadPool_WorkerThread");
		}
	}
}

void cFUThreadPool::ExecuteWork(IFunctor2<size_t, size_t, void>* _workFunctor, IFunctor2<size_t, size_t, void>* _doneFunctor, size_t _parameter1, size_t _parameter2)
{
	bool isValid = false;
	ExecuteCriticalCodeBlock(*workQueueCS, isValid = !killAllFlag);
	if (isValid)
	{
		WorkItem* workItem = new WorkItem();
		workItem->workFunctor = _workFunctor;
		workItem->doneFunctor = _doneFunctor;
		workItem->parameter1 = _parameter1;
		workItem->parameter2 = _parameter2;

		ExecuteCriticalCodeBlock(*workQueueCS, workQueue.push_back(workItem));
	}
	else
	{
		(*_doneFunctor)(_parameter1, _parameter2);
	}
}

bool cFUThreadPool::WaitForIdle(uint32 maxWaitTime)
{
	while (maxWaitTime >= 0)
	{
		ExecuteCriticalCodeBlock(*workQueueCS, if (workQueue.empty() && idleThreadCount == (int32) threads.size()) return true);
		if (maxWaitTime == 0) break;
		else
		if (maxWaitTime <= 10)
		{
			cFUThread::SleepCurrentThread(maxWaitTime);
			maxWaitTime = 0; 
		}
		else
		{
			cFUThread::SleepCurrentThread(10);
			maxWaitTime -= 10;
		}
	}
	return false;
}

ImplementThreadRoutine(cFUThreadPool::WorkerThread, threadPool)
{
	cFUThreadPool* pool = (cFUThreadPool*) threadPool;
	ExecuteCriticalCodeBlock(*pool->workQueueCS, ++pool->idleThreadCount);
	bool isDone = false;
	while (!isDone)
	{
		ExecuteCriticalCodeBlock(*pool->workQueueCS, isDone = pool->killAllFlag);
		if (isDone) break;

		cFUThreadPool::WorkItem* workItem = nullptr;
		ExecuteCriticalCodeBlock(*pool->workQueueCS, 
			if (!pool->workQueue.empty()) 
			{
				workItem = pool->workQueue.front();
				pool->workQueue.erase(pool->workQueue.begin());
			}
		);
		if (workItem != nullptr)
		{
			ExecuteCriticalCodeBlock(*pool->workQueueCS, --pool->idleThreadCount);
			(*(workItem->workFunctor))(workItem->parameter1, workItem->parameter2);
				(*(workItem->doneFunctor))(workItem->parameter1, workItem->parameter2);
			SAFE_DELETE(workItem->workFunctor);
			SAFE_DELETE(workItem->doneFunctor);
			SAFE_DELETE(workItem);
			ExecuteCriticalCodeBlock(*pool->workQueueCS, ++pool->idleThreadCount);
		}

		cFUThread::SleepCurrentThread(10);
	}

	ExecuteCriticalCodeBlock(*pool->workQueueCS, --pool->idleThreadCount);
	return 0;
}

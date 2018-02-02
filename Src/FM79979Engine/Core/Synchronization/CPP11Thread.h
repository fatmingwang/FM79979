#pragma once

#include <thread>
#include <mutex>
#include <functional>

typedef std::lock_guard<std::mutex> cPP11MutexHolder;
typedef std::function<void(float)>  f_ThreadWorkingFunction;

class cCPP11Thread:public virtual NamedTypedObject
{
	f_ThreadWorkingFunction			m_ThreadWorkingFunction;
	bool							m_bThreadWorking;
protected:
	bool							m_bLeaveThread;
	std::mutex						m_Mutex;
	void							WorkingThread();
public:
	cCPP11Thread();
	virtual ~cCPP11Thread();
	void		ThreadDetach(f_ThreadWorkingFunction e_WorkingFunction);
	void		CloseThreadAndWaitUntilFinish();
	std::mutex* GetMutex() { return &m_Mutex; }
	bool		IsThreadWorking() {return m_bThreadWorking;}
};

//mutex
////cPP11MutexHolder l_cPP11MutexHolder(m_Mutex);
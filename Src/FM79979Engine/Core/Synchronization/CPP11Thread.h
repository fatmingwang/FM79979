#pragma once

#include <thread>
#include <mutex>
#include <functional>

typedef std::lock_guard<std::mutex> cPP11MutexHolder;
class cPP11MutexHolderDebug
{
	std::mutex& m_Mutex;
	std::wstring l_strDebugInfo;
public:
	cPP11MutexHolderDebug(std::mutex& e_mutex) :m_Mutex(e_mutex)
	{
		m_Mutex.lock();
	}
	cPP11MutexHolderDebug(std::mutex& e_mutex,const wchar_t*e_strDebugInfo):m_Mutex(e_mutex)
	{
		m_Mutex.lock();
		if (e_strDebugInfo)
		{
			l_strDebugInfo = e_strDebugInfo;
			//OutputDebugString(e_strDebugInfo);
			//OutputDebugString(L" in\n");
		}
	}
	~cPP11MutexHolderDebug()
	{
		m_Mutex.unlock();
		if (l_strDebugInfo.length())
		{
			//OutputDebugString(l_strDebugInfo.c_str());
			//OutputDebugString(L" out\n");
		}
	}
};

typedef std::function<void(float)>  f_ThreadWorkingFunction;
namespace FATMING_CORE
{
	class cCPP11Thread
	{
		f_ThreadWorkingFunction			m_ThreadWorkingFunction;
		bool							m_bThreadWorking;
	protected:
		bool							m_bLeaveThread;
		std::mutex						m_Mutex;
		void							WorkingThread();
	public:
		cCPP11Thread();
		~cCPP11Thread();
		void		ThreadDetach(f_ThreadWorkingFunction e_WorkingFunction);
		void		CloseThreadAndWaitUntilFinish();
		std::mutex* GetMutex() { return &m_Mutex; }
		bool		IsThreadWorking() { return m_bThreadWorking; }
	};

	//class cWriteFileWithThreadManager :public cCPP11Thread
	//{
	//public:
	//	std::mutex						m_pMutex;
	//	void	WriteFileThread(float e_fElpaseTime)
	//	{
	//		{
	//			cPP11MutexHolder l_cPP11MutexHolder(m_Mutex);
	//		}
	//		Sleep(10);
	//	}
	//public:
	//	cWriteFileWithThreadManager::cWriteFileWithThreadManager()
	//	{
	//		cPP11MutexHolder l_cPP11MutexHolder(m_Mutex);
	//		f_ThreadWorkingFunction l_f_ThreadWorkingFunction = std::bind(&cWriteFileWithThreadManager::WriteFileThread, this, std::placeholders::_1);
	//		ThreadDetach(l_f_ThreadWorkingFunction);
	//	}
	//	virtual	NamedTypedObject*	Clone()override { return nullptr; };
	//};
}
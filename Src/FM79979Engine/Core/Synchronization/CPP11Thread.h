#pragma once

#include <thread>
#include <mutex>
#include <functional>

typedef std::lock_guard<std::mutex> cPP11MutexHolder;
typedef std::function<void(float)>  f_ThreadWorkingFunction;
namespace FATMING_CORE
{
	class cPP11MutexHolderDebug
	{
		std::mutex& m_Mutex;
		std::wstring l_strDebugInfo;
	public:
		cPP11MutexHolderDebug(std::mutex& e_mutex);
		cPP11MutexHolderDebug(std::mutex& e_mutex, const wchar_t*e_strDebugInfo);
		virtual ~cPP11MutexHolderDebug();
	};
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
#pragma once
#include <string>
#include <thread>
#include <mutex>
#include <map>
#include <functional>
#include "../Common/NamedTypedObject.h"
#include "../Common/TimeUT.h"

typedef std::lock_guard<std::mutex> cPP11MutexHolder;
typedef std::function<void(float)>  f_ThreadWorkingFunction;
#ifdef DEBUG
#define MUTEX_PLACE_HOLDER(MUTEX,INFO)	cPP11MutexHolderDebug l_PP11MutexHolderDebugTemp123456789(MUTEX,INFO,__FILE__, __LINE__);
#else
#define MUTEX_PLACE_HOLDER(MUTEX,INFO)	cPP11MutexHolder l_PP11MutexHolderTemp123456789(MUTEX);
#endif

namespace FATMING_CORE
{
	class cPP11MutexHolderDebug
	{
		UT::sTimeAndFPS m_TimeAndFPS;
		std::mutex&		m_Mutex;
		std::string		m_strDebugInfo;
	public:
		cPP11MutexHolderDebug(std::mutex& e_mutex, const char*e_strFileName, int32_t e_iCodeLine);
		cPP11MutexHolderDebug(std::mutex& e_mutex, const char*e_strDebugInfo, const char* e_strFileName, int32_t e_iCodeLine);
		virtual ~cPP11MutexHolderDebug();
	};
	class cCPP11Thread
	{
		static std::map<uint64, cCPP11Thread*>*	m_spui64PIDAndNamedObject;
		static std::mutex*				m_spMapMutex;
		uint64							m_i64PID;
		std::string						m_strInfo;
//
		f_ThreadWorkingFunction			m_ThreadWorkingFunction;
		bool							m_bThreadWorking;
	protected:
		NamedTypedObject*				m_pThreadOwner;
		bool							m_bLeaveThread;
		std::mutex						m_Mutex;
		void							WorkingThread();
	public:
		cCPP11Thread(NamedTypedObject*e_pOwner = nullptr);
		~cCPP11Thread();
		void		ThreadDetach(f_ThreadWorkingFunction e_WorkingFunction,const char*e_strThreadName = nullptr);
		void		CloseThreadAndWaitUntilFinish();
		std::mutex* GetMutex() { return &m_Mutex; }
		bool		IsThreadWorking() { return m_bThreadWorking; }
		void		DumpThreadInfo(const char*e_strExtraInfo = nullptr);
		static void		DumpThreadMapInfo();
		static void		AllThreadStopAndDumpInfo();
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
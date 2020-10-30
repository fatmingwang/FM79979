#include "../stdafx.h"
#include "CPP11Thread.h"
#include <sstream>
#include "../Common/StringToStructure.h"
#include "../Common/NamedTypedObject.h"
namespace FATMING_CORE
{
	cPP11MutexHolderDebug::cPP11MutexHolderDebug(std::mutex& e_mutex) :m_Mutex(e_mutex)
	{
		m_Mutex.lock();
	}
	cPP11MutexHolderDebug::cPP11MutexHolderDebug(std::mutex& e_mutex, const wchar_t*e_strDebugInfo) :m_Mutex(e_mutex)
	{
		m_Mutex.lock();
		if (e_strDebugInfo)
		{
			l_strDebugInfo = e_strDebugInfo;
			//OutputDebugString(e_strDebugInfo);
			//OutputDebugString(L" in\n");
		}
	}
	cPP11MutexHolderDebug::~cPP11MutexHolderDebug()
	{
		m_Mutex.unlock();
		if (l_strDebugInfo.length())
		{
			//OutputDebugString(l_strDebugInfo.c_str());
			//OutputDebugString(L" out\n");
		}
	}	


	std::map<uint64, cCPP11Thread*>*	cCPP11Thread::m_spui64PIDAndNamedObject = nullptr;
	std::mutex*							cCPP11Thread::m_spMapMutex = nullptr;
	cCPP11Thread::cCPP11Thread(NamedTypedObject*e_pOwner)
	{
#ifdef DEBUG
		if (!m_spui64PIDAndNamedObject)
		{
			m_spui64PIDAndNamedObject = new std::map<uint64, cCPP11Thread*>;
			m_spMapMutex = new std::mutex;
		}
#endif
		m_i64PID = 0;
		m_pThreadOwner = e_pOwner;
		m_ThreadWorkingFunction = nullptr;
		m_bThreadWorking = false;
		m_bLeaveThread = false;
		//https://stackoverflow.com/questions/10673585/start-thread-with-member-function
		//call with std::function
		//std::thread WriteFileThread(&cWriteFileWithThreadManager::WriteFileThread,this);
		//call with lambda
		//std::thread WriteFileThread([this]()
		//{

		//});
		//call with member function to get thread
		//std::thread WriteFileThread = Spawn();
		//WriteFileThread.detach();
	}
	cCPP11Thread::~cCPP11Thread()
	{
		m_bLeaveThread = true;
		while (m_bThreadWorking)
		{
		}
		if (m_spMapMutex)
		{
			{
				MUTEX_PLACE_HOLDER(*m_spMapMutex)
				if (m_spui64PIDAndNamedObject && m_spui64PIDAndNamedObject->size() == 0)
				{
					SAFE_DELETE(m_spui64PIDAndNamedObject);
				}
			}
			if (m_spui64PIDAndNamedObject == nullptr)
			{
				SAFE_DELETE(m_spMapMutex);
			}
		}
	}
	void	cCPP11Thread::ThreadDetach(f_ThreadWorkingFunction e_WorkingFunction, const char*e_strThreadName)
	{
		m_bLeaveThread = false;
		m_ThreadWorkingFunction = e_WorkingFunction;
		std::thread l_Thread = std::thread([=] {cCPP11Thread::WorkingThread(); });
		l_Thread.detach();
		if (e_strThreadName != nullptr)
		{
			m_strInfo = e_strThreadName;
		}
	}
	void	cCPP11Thread::CloseThreadAndWaitUntilFinish()
	{
		const float l_cfForceToBreakTime = 3.f;
		UT::sTimeAndFPS	l_FPS;
		m_bLeaveThread = true;
		l_FPS.Update();
		float l_fElpaseTime = 0.f;
		while (m_bThreadWorking)
		{
			Sleep(10);
			l_FPS.Update();
			l_fElpaseTime += l_FPS.fElpaseTime;
			if (l_fElpaseTime >= l_cfForceToBreakTime)
			{
				std::string l_strInfo = "Error:cCPP11Thread::CloseThreadAndWaitUntilFinish wait over 3 seconds force break:";
				if(m_pThreadOwner)
					l_strInfo += this->m_pThreadOwner->GetCharName();
				FMLog::Log(l_strInfo.c_str(),true);
				break;
			}
		}
	}

	void cCPP11Thread::DumpThreadInfo(const char*e_strExtraInfo)
	{
		std::string l_str = m_strInfo;
		if (m_pThreadOwner)
		{
			l_str = " :";
			l_str += m_pThreadOwner->GetCharName();
			l_str = ",";
			l_str += ValueToString(m_pThreadOwner->Type());
		}
		l_str += ",pid=";
		l_str += ValueToString(m_i64PID);
		if (e_strExtraInfo)
		{
			l_str += "\n";
			l_str += e_strExtraInfo;
		}
		FMLog::Log(l_str.c_str(), true);
	}

	void cCPP11Thread::DumpThreadMapInfo()
	{
		if (m_spMapMutex)
		{
			MUTEX_PLACE_HOLDER(*m_spMapMutex);
			if (m_spui64PIDAndNamedObject)
			{
				for (auto l_BeginIterator = m_spui64PIDAndNamedObject->begin();
					l_BeginIterator != m_spui64PIDAndNamedObject->end(); ++l_BeginIterator)
				{
					l_BeginIterator->second->DumpThreadInfo();
				}
			}
		}
	}

	void cCPP11Thread::AllThreadStopAndDumpInfo()
	{
		if (m_spMapMutex)
		{
			MUTEX_PLACE_HOLDER(*m_spMapMutex);
			if (m_spui64PIDAndNamedObject)
			{
				std::vector<cCPP11Thread*>l_ThreadVector;
				for (auto l_BeginIterator = m_spui64PIDAndNamedObject->begin();
					l_BeginIterator != m_spui64PIDAndNamedObject->end(); ++l_BeginIterator)
					l_ThreadVector.push_back(l_BeginIterator->second);
				for (auto i=0;i<l_ThreadVector.size();++i)
				{
					auto l_pThread = l_ThreadVector[i]; 
					l_pThread->CloseThreadAndWaitUntilFinish();
					if (l_pThread->IsThreadWorking())
					{
						l_pThread->DumpThreadInfo("Error:thread is not finish!!");
					}
				}
			}
		}
	}

	void	cCPP11Thread::WorkingThread()
	{
		if (m_i64PID == 0 && m_spui64PIDAndNamedObject)
		{
			//https://stackoverflow.com/questions/7432100/how-to-get-integer-thread-id-in-c11
			std::stringstream l_ss;
			l_ss << std::this_thread::get_id();
			uint64 id = std::stoull(l_ss.str());
			m_i64PID = id;
			m_spui64PIDAndNamedObject->insert(std::make_pair(m_i64PID,this));
			DumpThreadInfo();
		}
		UT::sTimeAndFPS l_TimeAndFPS;
		l_TimeAndFPS.Update();
		m_bThreadWorking = true;
		while (!m_bLeaveThread)
		{
			l_TimeAndFPS.Update();
			if (m_ThreadWorkingFunction)
			{
#ifdef DEBUG
				try
				{
#endif
					m_ThreadWorkingFunction(l_TimeAndFPS.fElpaseTime);
#ifdef DEBUG
				}
				catch (std::exception e)
				{
					std::string l_strInfo = this->m_strInfo;
					l_strInfo += " occur exception:";
					l_strInfo += e.what();
					FMLog::Log(l_strInfo.c_str(),true);
				}
#endif
			}
		}
		m_bThreadWorking = false;
		if (m_spMapMutex)
		{
			MUTEX_PLACE_HOLDER(*m_spMapMutex)
			(*m_spui64PIDAndNamedObject).erase(m_i64PID);
			DumpThreadInfo("thread left as expecting!!");
		}
	}
}
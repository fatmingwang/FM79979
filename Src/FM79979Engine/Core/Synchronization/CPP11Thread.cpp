#include "../stdafx.h"
#include "CPP11Thread.h"
#include "../GameplayUT/GameApp.h"
namespace FATMING_CORE
{
	cCPP11Thread::cCPP11Thread()
	{
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
	}
	void	cCPP11Thread::ThreadDetach(f_ThreadWorkingFunction e_WorkingFunction)
	{
		m_ThreadWorkingFunction = e_WorkingFunction;
		std::thread l_Thread = std::thread([=] {cCPP11Thread::WorkingThread(); });
		l_Thread.detach();
	}
	void	cCPP11Thread::CloseThreadAndWaitUntilFinish()
	{
		m_bLeaveThread = true;
		while (m_bThreadWorking)
		{
			Sleep(10);
		}
	}

	void	cCPP11Thread::WorkingThread()
	{
		UT::sTimeAndFPS l_TimeAndFPS;
		l_TimeAndFPS.Update();
		m_bThreadWorking = true;
		while (!m_bLeaveThread)
		{
			l_TimeAndFPS.Update();
			if (m_ThreadWorkingFunction)
				m_ThreadWorkingFunction(l_TimeAndFPS.fElpaseTime);
		}
		m_bThreadWorking = false;
	}
}
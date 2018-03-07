#include "../../stdafx.h"
#include "../Synchronization/CPP11Thread.h"
#include "WriteFileWithThread.h"

namespace FATMING_CORE
{
	class cWriteFileWithThreadManager :public cNamedTypedObjectVector<cWriteFileWithThread>, public cCPP11Thread
	{
	public:
		std::vector<cWriteFileWithThread*>	m_WriteBinaryFileVector;
		void	WriteFileThread(float e_fElpaseTime);
	public:
		cWriteFileWithThreadManager();
		virtual ~cWriteFileWithThreadManager();
		virtual	NamedTypedObject*	Clone()override { return nullptr; }
	};

	cWriteFileWithThreadManager::cWriteFileWithThreadManager()
	{
		cPP11MutexHolder l_cPP11MutexHolder(m_Mutex);
		f_ThreadWorkingFunction l_f_ThreadWorkingFunction = std::bind(&cWriteFileWithThreadManager::WriteFileThread, this, std::placeholders::_1);
		ThreadDetach(l_f_ThreadWorkingFunction);
	}

	cWriteFileWithThreadManager::~cWriteFileWithThreadManager()
	{
	}

	void	cWriteFileWithThreadManager::WriteFileThread(float e_fElpaseTime)
	{
		{
			cPP11MutexHolder l_cPP11MutexHolder(m_Mutex);
			int l_iSize = this->Count();
			if (l_iSize == 0)
				return;
			for (int i = 0; i < l_iSize; ++i)
			{
				auto l_pObject = GetObject(i);
				l_pObject->WriteFileUpdate(e_fElpaseTime);
			}
		}
		Sleep(1);
	}


	cWriteFileWithThreadManager*g_pWriteFileWithThreadManager = nullptr;

	cWriteFileWithThread::cWriteFileWithThread(bool e_bAddToAutoSave)
	{
		auto l_pFile = this->m_pFile;
		m_TimeToWriteFile.SetTargetTime(0.05f);//default is 0.05 second
		m_TimeToWriteFile.SetLoop(true);
		m_bActiveToWriteFile = false;
		if (e_bAddToAutoSave)
		{
			if (!g_pWriteFileWithThreadManager)
			{
				g_pWriteFileWithThreadManager = new cWriteFileWithThreadManager();
			}
			cPP11MutexHolder l_cPP11MutexHolder(*g_pWriteFileWithThreadManager->GetMutex());
			g_pWriteFileWithThreadManager->AddObject(this);
		}
	}

	cWriteFileWithThread::~cWriteFileWithThread()
	{
		if (g_pWriteFileWithThreadManager)
		{
			{
				cPP11MutexHolder l_cPP11MutexHolder(*g_pWriteFileWithThreadManager->GetMutex());
				g_pWriteFileWithThreadManager->RemoveObjectWithoutDelete(this);
			}
			if (g_pWriteFileWithThreadManager->Count() == 0)
			{
				g_pWriteFileWithThreadManager->CloseThreadAndWaitUntilFinish();
				SAFE_DELETE(g_pWriteFileWithThreadManager);
			}
		}
	}

	bool	cWriteFileWithThread::OpenFileWhatEverFileIsExitOrNotExit(const char*e_strFileName, sRegister_Header_Struct e_sRegister_Header_Struct, bool e_bActiveWriteToFile, bool e_bRoolFilePositionToEnd)
	{
		bool	l_bResult = false;
		if (cBinaryFile::Writefile(e_strFileName, true, true, "rb+"))
		{
			int		l_iFileSize = GetDataFileSize();
			if (l_iFileSize == 0)
			{//new file,write header
				if (m_pFile)
				{
					sRegister_Header_Struct l_Register_Header_Struct = e_sRegister_Header_Struct;
					NvFWrite(&l_Register_Header_Struct, sizeof(sRegister_Header_Struct), 1, m_pFile);
					this->Flush();
					l_bResult = true;
				}
			}
			else
			{
				char*l_pData = (char*)GetDataFile(0);
				sRegister_Header_Struct*l_pRegister_Header_Struct = (sRegister_Header_Struct*)l_pData;
				if (l_pRegister_Header_Struct->chID[0] == e_sRegister_Header_Struct.chID[0] &&
					l_pRegister_Header_Struct->chID[1] == e_sRegister_Header_Struct.chID[1] &&
					l_pRegister_Header_Struct->chID[2] == e_sRegister_Header_Struct.chID[2] &&
					l_pRegister_Header_Struct->chID[3] == e_sRegister_Header_Struct.chID[3])
				{
					l_iFileSize -= sizeof(sRegister_Header_Struct);
					l_pData += sizeof(sRegister_Header_Struct);
					l_bResult = OpenFileGetData(l_iFileSize, l_pData, l_pRegister_Header_Struct);
				}
			}
		}
		if (e_bRoolFilePositionToEnd && m_pFile)
		{
			NvFSeek(m_pFile, 0, SEEK_END);
		}
		if (l_bResult)
		{
			m_TimeToWriteFile.Start();
			if (e_bActiveWriteToFile)
				m_bActiveToWriteFile = true;
		}
		return l_bResult;
	}

	void	cWriteFileWithThread::WriteFileUpdate(float e_fElpaseTime)
	{
		if (!m_bActiveToWriteFile)
			return;
		m_TimeToWriteFile.Update(e_fElpaseTime);
		if (m_TimeToWriteFile.bTragetTimrReached)
		{
			WriteFile();
		}
	}

	cSampleForWriteBinaryFile::cSampleForWriteBinaryFile()
	{
		m_fTimeToWrite = 0.0016f;
	}

	void	cSampleForWriteBinaryFile::Update(float e_fTime)
	{
		m_fTimeToWrite -= e_fTime;
		if (m_fTimeToWrite < 0.f)
		{
			m_fTimeToWrite = 0.f;
			cPP11MutexHolder l_cPP11MutexHolder(m_Mutex);
			static int l_iTestValue = 0;
			m_iTest.push_back(l_iTestValue);
			++l_iTestValue;
		}
	}

	void	cSampleForWriteBinaryFile::WriteFile()
	{
		//cPP11MutexHolder l_cPP11MutexHolder(m_Mutex);
		//while (m_iTest.size())
		//{
		//	int l_iValue = m_iTest[0];
		//	NvFWrite(&l_iValue, sizeof(int), 1, m_pFile);
		//	Flush();
		//	m_iTest.erase(m_iTest.begin());
		//}
		std::vector<int>	l_TestVector;
		{
			cPP11MutexHolder l_cPP11MutexHolder(m_Mutex);
			l_TestVector = m_iTest;
		}
		while (l_TestVector.size())
		{
			int l_iValue = l_TestVector[0];
			NvFWrite(&l_iValue, sizeof(int), 1, m_pFile);
			Flush();
			l_TestVector.erase(l_TestVector.begin());
		}
	}

	bool	cSampleForWriteBinaryFile::OpenFileGetData(int e_iDataSizeWithOutFileExtension, char*e_pData, sRegister_Header_Struct*e_pRegister_Header_Struct)
	{
		int a = 0;
		return true;
	}
//end namesapce FATMING_CORE
}
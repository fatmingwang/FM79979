#pragma once

#include <thread>
#include <mutex>
#include <functional>
#include "../BinaryFile.h"
namespace FATMING_CORE
{
	//a basic write file with a virtual function WriteFile
	//a Manager it will do multithread to write file
	class cWriteFileWithThread :virtual public cBinaryFile
	{
	public:
		struct sRegister_Header_Struct
		{
			char				chID[4];//if ID is REG it use 3 but in 64 bit OS 3+sizeof(float) = 8 not 7,becareful the size you write.
			float				fVersion;//1.0
			sRegister_Header_Struct() { memset(chID, 0, sizeof(char) * 4); fVersion = 0.f; }
			~sRegister_Header_Struct() {}
		};
	protected:
		sTimeCounter	m_TimeToWriteFile;//default is 0.05 second
		bool			m_bActiveToWriteFile;
		//
		virtual void	WriteFile() {}
		virtual bool	OpenFileGetData(int e_iDataSizeWithOutFileExtension, char*e_pData, sRegister_Header_Struct*e_pRegister_Header_Struct) { return false; }
	public:
		//something you dont want to write file autoly set as false
		cWriteFileWithThread(bool e_bAddToAutoSave = true);
		virtual ~cWriteFileWithThread();
		bool			OpenFileWhatEverFileIsExitOrNotExit(const char*e_strFileName, sRegister_Header_Struct e_sRegister_Header_Struct, bool e_bActiveWriteToFile, bool e_bRoolFilePositionToEnd = true);
		virtual void	WriteFileUpdate(float e_fElpaseTime);
	};

	//for thread mutex
	//class cPP11MutexHolder
	//{
	//	std::mutex* m_pMutex;
	//	std::lock_guard<std::mutex>	m_LockGuard;
	//public:
	//	cPP11MutexHolder(std::mutex*e_pMutex) 
	//	{
	//		m_pMutex = e_pMutex;
	//		if (m_pMutex)
	//			m_LockGuard(*m_pMutex);
	//			//m_pMutex->lock(); 
	//	}
	//	~cPP11MutexHolder() { if (m_pMutex)m_pMutex->unlock(); }
	//};

	class cSampleForWriteBinaryFile :public cWriteFileWithThread
	{
		std::mutex			m_Mutex;
		std::vector<int>	m_iTest;
		float				m_fTimeToWrite;
		virtual void	WriteFile() override;

		virtual bool	OpenFileGetData(int e_iDataSizeWithOutFileExtension, char*e_pData, sRegister_Header_Struct*e_pRegister_Header_Struct)override;
	public:
		cSampleForWriteBinaryFile();
		virtual~cSampleForWriteBinaryFile() {}
		void	Update(float e_fTime);
	};
//end namesapce FATMING_CORE
}
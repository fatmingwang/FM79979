//#include "../stdafx.h"
#include <stdlib.h>  
#include "BinaryFile.h"
#include "../Utility.h"
#include "../Log/FMLog.h"
#include "../../Math/MathUtility.h"
#include "assert.h"
#ifdef _WIN32
#include <io.h>
#endif
#include <time.h>
#include "../../Android/nv_file.h"
//p4 test
namespace FATMING_CORE
{
	cBinaryFile::cBinaryFile(bool e_bBigEndian)
	{
		this->SetName(L"cBinaryFile");
		m_bBigEndian = e_bBigEndian;
		if( e_bBigEndian )
			SwapToPc();
		else
			SwapToXBOX();
		m_pData = nullptr;
		m_pFile = nullptr;
#ifdef WIN32
		m_FileHandle = nullptr;
#endif
#if defined(WIN32) || defined(LINUX)
		m_iFileDescriptor = -1;
#endif
	}

	cBinaryFile::~cBinaryFile()
	{
		CloseFile();
	};

	void cBinaryFile::SwapToPc()
	{
		D0 = 0;D1 = 1;D2 = 2;D3 = 3;
		D4 = 4;D5 = 5;D6 = 6;D7 = 7;
		W0 = 0;W1 = 1;
	}

	void cBinaryFile::SwapToXBOX()
	{
		D0 = 7;D1 = 6;D2 = 5;D3 = 4;
		D4 = 3;D5 = 2;D6 = 1;D7 = 0;
		W0 = 1;W1 = 0;
	}

	void cBinaryFile::WriteToFile(float val )
	{
		char data[4] = { ((char*)&val)[D0], ((char*)&val)[D1],
						 ((char*)&val)[D2], ((char*)&val)[D3] };
		NvFWrite( data, sizeof(data), 1, m_pFile );
	}

	//void cBinaryFile::WriteToFile(double val )
	//{
	//	//fwrite( &val, sizeof(double), 1, m_pFile );
	//	WriteToFile((int64) val );
	//}
	//void cBinaryFile::WriteToFile(wchar_t val )
	//{
	//	CHAR data[2] = { ((CHAR*)&val)[W0], ((CHAR*)&val)[W1] };
	//	fwrite( data, sizeof(data), 1, m_pFile ); 
	//}

	void cBinaryFile::WriteToFile(int	  val )
	{
		char data[4] = { ((char*)&val)[D0], ((char*)&val)[D1],
						 ((char*)&val)[D2], ((char*)&val)[D3] };
		NvFWrite( data, sizeof(data), 1, m_pFile );
	}

	void 	cBinaryFile::WriteToFile(unsigned int  val )
	{
		char data[4] = { ((char*)&val)[D0], ((char*)&val)[D1],
						 ((char*)&val)[D2], ((char*)&val)[D3] };
		NvFWrite( data, sizeof(data), 1, m_pFile );	

	}

	void cBinaryFile::WriteToFile(unsigned long val )
	{
		char data[4] = { ((char*)&val)[D0], ((char*)&val)[D1],
						 ((char*)&val)[D2], ((char*)&val)[D3] };
		NvFWrite( data, sizeof(data), 1, m_pFile ); 
	}


	void cBinaryFile::WriteToFile(short val )
	{
		char data[2] = { ((char*)&val)[W0], ((char*)&val)[W1] };
		NvFWrite( data, sizeof(data), 1, m_pFile ); 
	}

#ifndef LINUX
	void cBinaryFile::WriteToFile(long val )
	{
		NvFWrite( &val, sizeof(val), 1, m_pFile );
	}
#endif
	void cBinaryFile::WriteToFileImmediatelyWithLine(const char* val ,bool e_bTimeStamp)
	{
		if( !m_pFile )
			return;
		MUTEX_PLACE_HOLDER(m_LogMutex);
		if( e_bTimeStamp )
			CreateTimestamp();
		int	l_iLength = (int)strlen(val);
		NvFWrite( val, sizeof(char),l_iLength, m_pFile );
		NvFWrite( "\n", sizeof(char),1, m_pFile );
		Flush();
	}
	void cBinaryFile::WriteToFileImmediatelyWithLine(const wchar_t* val,bool e_bTimeStamp)
	{
		if( !m_pFile )
			return;
		MUTEX_PLACE_HOLDER(m_LogMutex);
		if( e_bTimeStamp )
			CreateTimestamp();
		int	l_iLength = (int)wcslen(val);
		if( m_bBigEndian )
		{
			NvFWrite( val, sizeof(wchar_t),l_iLength, m_pFile ); 
		}
		else
		{
			for( int i=0;i<l_iLength;++i )
				WriteToFile((int64)val[i]);
		}
		if( m_bBigEndian )
			NvFWrite( L"\n", sizeof(wchar_t),1, m_pFile );
		else
			WriteToFile(L"\n");
		Flush();
	}

	void cBinaryFile::WriteToFile(const char* val)
	{
		if( !m_pFile )
			return;
		int	l_iLength = (int)strlen(val);
		NvFWrite( val, sizeof(char),l_iLength, m_pFile );
	}

	void cBinaryFile::WriteToFile(int64	  val )
	{
		if( !m_pFile )
			return;
		CHAR data[8] = { ((CHAR*)&val)[D0], ((CHAR*)&val)[D1],
						 ((CHAR*)&val)[D2], ((CHAR*)&val)[D3],
						 ((CHAR*)&val)[D4], ((CHAR*)&val)[D5],
						 ((CHAR*)&val)[D6], ((CHAR*)&val)[D7]};
		NvFWrite( data, sizeof(data), 1, m_pFile );
	}

	bool cBinaryFile::WriteToFile(const wchar_t* val )
	{
		if( !m_pFile )
			return false;
		int	l_iLength = (int)wcslen(val);
		if( m_bBigEndian )
		{
			NvFWrite( val, sizeof(wchar_t),l_iLength, m_pFile ); 
		}
		else
		{
			for( int i=0;i<l_iLength;++i )
				WriteToFile((int64)val[i]);
		}
		return true;
	}

	bool 	cBinaryFile::WriteToFile(const char*e_pData, size_t e_iLength, int e_iPos)
	{
		if (NvFSeek(this->m_pFile,e_iPos, SEEK_SET) == 0)
		{
			return WriteToFile(e_pData, e_iLength);
		}
		return false;
	}

	bool 	cBinaryFile::WriteToFile(const char*e_pData,size_t e_iLength )
	{
		assert(m_pFile&&"file not open yet!?");
		auto l_iWriteSize = NvFWrite(e_pData, 1, e_iLength*sizeof(char), m_pFile);
		if(l_iWriteSize == e_iLength)
			return true;
		return false;
	}

	bool	cBinaryFile::Flush()
	{
		if(m_pFile)
		{
#ifdef WIN32
			if (m_FileHandle)
			{
				bool l_bResult = FlushFileBuffers(m_FileHandle);
				//don't remove the code or it wont write immediately...
				//https://groups.google.com/forum/#!topic/microsoft.public.win32.programmer.kernel/WsbWN9RkN6E
				long l_lCurrentPosition = NvFTell(m_pFile);
				NvFSeek(m_pFile, l_lCurrentPosition, SEEK_SET);
			}
			else
				NvFFlush(m_pFile);
#else
			NvFFlush(m_pFile);
#ifdef LINUX
			//https://raspberrypi.stackexchange.com/questions/72993/file-content-disappears-when-raspberry-pi-is-unplugged
			if(m_iFileDescriptor != -1)
				fsync(m_iFileDescriptor);
#endif

#endif
			return true;
		}
		return false;
	}
	bool	cBinaryFile::Writefile(const char*e_str,bool e_bBinary,bool e_bForceToWrite, const char*e_strFileMode)
	{
		CloseFile();
#if defined(WIN32) && !defined(UWP)
		if( e_bForceToWrite )
		{
			DWORD	l_iFlag = 0;// FILE_ATTRIBUTE_NORMAL;
			if (e_bForceToWrite)
			{
				//https://stackoverflow.com/questions/317801/win32-write-to-file-without-buffering
				//https://stackoverflow.com/questions/25899806/is-there-a-way-not-to-use-cache-in-c
				l_iFlag |= (FILE_FLAG_WRITE_THROUGH);
				//l_iFlag |= (FILE_FLAG_NO_BUFFERING);
//				l_iFlag |= (FILE_FLAG_SEQUENTIAL_SCAN);				
			}
			m_FileHandle = CreateFile(UT::CharToWchar(e_str).c_str(), GENERIC_READ | GENERIC_WRITE,0, nullptr, OPEN_ALWAYS, l_iFlag, nullptr);
			//m_FileHandle = CreateFile(UT::CharToWchar(e_str).c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, l_iFlag, nullptr);
			//m_FileHandle = CreateFile(UT::CharToWchar(e_str).c_str(), GENERIC_ALL, 0, nullptr, OPEN_ALWAYS, l_iFlag, nullptr);
			if (m_FileHandle != INVALID_HANDLE_VALUE) 
			{
				m_iFileDescriptor = _open_osfhandle((intptr_t)m_FileHandle, 0);				
				if (m_iFileDescriptor != -1)
				{
					if (e_strFileMode == nullptr)
					{
						if (e_bBinary)
							m_pFile = _fdopen(m_iFileDescriptor, "wb+");//b for windows only
						else
							this->m_pFile = _fdopen(m_iFileDescriptor, "w");
					}
					else
					{
						m_pFile = _fdopen(m_iFileDescriptor, e_strFileMode);//b for windows only
					}
					if (!m_pFile)
						_close(m_iFileDescriptor);
				}
				else
				{
					CloseHandle(m_FileHandle);
					m_FileHandle = nullptr;
				}
			}
		}
		else
		{
			if(e_strFileMode)
				this->m_pFile = MyFileOpen(e_str, e_strFileMode);
			else
			{
				if (e_bBinary)
					this->m_pFile = MyFileOpen(e_str, "wb+");
				else
					this->m_pFile = MyFileOpen(e_str, "w");
			}
		}
#else
		std::string	l_strFileName = e_str;
		if (e_strFileMode == nullptr)
		{
			if (e_bBinary)
				this->m_pFile = MyFileOpen(l_strFileName.c_str(), "wb+");
			else
				this->m_pFile = MyFileOpen(l_strFileName.c_str(), "w");
		}
		else
		{
			this->m_pFile = MyFileOpen(l_strFileName.c_str(), e_strFileMode);
		}
#ifdef LINUX
		if (e_bForceToWrite && m_pFile)
		{
			m_iFileDescriptor = fileno(m_pFile);
		}
#endif
#endif
		if(m_pFile)
			return true;
		return false;
	}

	void	cBinaryFile::CloseFile()
	{
		SAFE_DELETE(m_pData);
#ifdef WIN32
		if( m_FileHandle )
		{
			this->Flush();
			SystemErrorCheck();
			if (m_pFile)
			{
				fclose(m_pFile);
				m_pFile = nullptr;
			}
			//SystemErrorCheck();
			//_close(m_iFileDescriptor);
			//SystemErrorCheck();
			//bool l_bResult = CloseHandle(m_FileHandle);
			//SystemErrorCheck();
			//SystemErrorCheck();
			m_iFileDescriptor = -1;
			m_FileHandle = 0;
			m_pFile = nullptr;
		}
		else
		if( m_pFile )
		{
			this->Flush();
			fclose( m_pFile );
		}
#else
		if( m_pFile )
		{
			this->Flush();
			NvFClose( m_pFile );
		}
#endif
		m_pFile = 0;
	}

	bool	cBinaryFile::Openfile(const char*e_strFileName,const char*e_strformat)
	{
		this->m_pFile = MyFileOpen(e_strFileName,e_strformat );
		if(m_pFile)
			return true;
		return false;
	}

	int		cBinaryFile::GetDataFileSize()
	{
		if(  !m_pFile )
			return 0;
		return UT::GetFileSize(this->m_pFile);

	}
	void*	cBinaryFile::GetDataFile(unsigned int e_uiStart)
	{
		if(!m_pData)
		{
			if( !m_pFile )
				return 0;
			long l_lFileSize = UT::GetFileSize(this->m_pFile);
			if( l_lFileSize == 0 )
				return 0;
			m_pData = new char[l_lFileSize];
			memset(m_pData, 0, l_lFileSize);
			auto l_uiReadSize = NvFRead(m_pData,sizeof(char),l_lFileSize,m_pFile);
			if ((long)l_uiReadSize != l_lFileSize)
			{
				FMLog::Log("cBinaryFile::GetDataFile fread size not match!!file open mode wrong!?",true);
				SAFE_DELETE(m_pData);
				return nullptr;
			}
		}
		char*l_pOutPutData = m_pData;
		l_pOutPutData += e_uiStart;
		return (void*)l_pOutPutData;
	}

	//push data into e_pDestData
	//https://stackoverflow.com/questions/3348637/fgetpos-fsetpos-and-ftell-fseek
	void*	cBinaryFile::GetDataFileSegment(unsigned int e_uiStart,unsigned int e_uiSLength,void*e_pDestData)
	{
	#ifdef WIN32
		fpos_t pos = e_uiStart;
		fsetpos(m_pFile,&pos);
		fread(e_pDestData,sizeof(char),e_uiSLength,m_pFile);
		rewind(m_pFile);
		return e_pDestData;
	#else
			//or just indtead by pos.__pos = e_uiStart;
		//and call as win32
			unsigned short*l_pData = (unsigned short*)GetDataFile(e_uiStart);
			wchar_t*l_Transto2Wchar = (wchar_t*)e_pDestData;        
			for(int i=0;i<(int)e_uiSLength/2;++i )
			{
				l_Transto2Wchar[i] = l_pData[i];
			}
			l_Transto2Wchar[e_uiSLength/2] = L'\0';
		return e_pDestData;
	#endif
	}

	void	cBinaryFile::CreateTimestamp()
	{
	//#ifdef WIN32
			char buffer[32];
			time_t ltime; time(&ltime);
#ifdef WIN32
			struct tm now; _localtime64_s(&now, &ltime);
			strftime(buffer, 32, "[%H:%M:%S] ", &now);
#else
			auto now = localtime(&ltime);
			strftime(buffer, 32, "[%H:%M:%S] ", now);
#endif
			int	l_iLength = (int)strlen(buffer);
			NvFWrite( buffer, sizeof(char),l_iLength, m_pFile );
	//#endif //WIN32
	}

	bool	cBinaryFile::SetFilePos(int e_iPos)
	{
		return NvFSeek(m_pFile,e_iPos,SEEK_SET)==0?true:false;
	}

	int64		cBinaryFile::GetInt64(void*e_pData)
	{
		return *(int64*)e_pData;
	}

	float		cBinaryFile::GetFloat(void*e_pData)
	{
		return *(float*)e_pData;
	}

	//double		cBinaryFile::GetDouble(void*e_pData)
	//{
	//	double	l_dbValue = 0.f;
	//	memcpy(&l_dbValue,e_pData,sizeof(double));
	//	//return *(double*)e_pData;
	//	return l_dbValue;
	//}

	int			cBinaryFile::GetInt(void*e_pData)
	{
		return *(int*)e_pData;
	}

	char		cBinaryFile::GetChar(void*e_pData)
	{
		return *(char*)e_pData;
	}

	cSkipHeaderBinaryFile::cSkipHeaderBinaryFile(int e_iMagicID)
	{
		this->SetName(L"cSkipHeaderBinaryFile");
		m_iMagicNumberID = e_iMagicID;
		m_iSkpiHeaderSize = 0;
	}

	cSkipHeaderBinaryFile::~cSkipHeaderBinaryFile()
	{
	
	}

	bool	cSkipHeaderBinaryFile::SetFilePos(int e_iPos)
	{
		if( m_pFile && m_iSkpiHeaderSize == 0 )//empty file do need to set position
		{
			//create header
			if(!CreateHeader())
				return false;
		}
		int	l_iSkipSize = GetSkipHeaderAndHMagicNumberHeader();	
		int	ret = NvFSeek(m_pFile,e_iPos+l_iSkipSize,SEEK_SET);
		return ret==0?true:false;
	}

	void*		cSkipHeaderBinaryFile::GetDataFile(UINT e_uiStart,int* e_piRealFileLength)
	{
		void*l_pData = cBinaryFile::GetDataFile(e_uiStart);
		if( !l_pData )
			return 0;
		int	l_iFileLength = GetDataFileSize();
		char*l_pOutputData = (char*)l_pData;
		if( l_iFileLength < (int)sizeof(m_iMagicNumberID) )
		{//not cSkipHeaderBinaryFile file.
			return 0;
		}
		int	l_iHeaderNum = 0;
		int	l_iSkipHeaderData = 0;
		memcpy(&l_iHeaderNum,l_pOutputData,sizeof(int));
		if( m_iMagicNumberID != l_iHeaderNum)
		{
			return 0;
		}
		l_pOutputData += sizeof(int);
		//not fetch second int data then skip random data
		memcpy(&this->m_iSkpiHeaderSize,l_pOutputData,sizeof(int));

		int	l_iTotalSkipHeader = GetSkipHeaderAndHMagicNumberHeader();
		if( l_iFileLength < l_iTotalSkipHeader )
			return 0;
		if( e_piRealFileLength )
		{
			*e_piRealFileLength = l_iFileLength-l_iTotalSkipHeader;
		}
		return this->m_pData+l_iTotalSkipHeader;
		//return l_pOutputData;
	}

	bool		cSkipHeaderBinaryFile::CreateHeader()
	{
		if( !m_pFile )
			return false;
		this->m_iSkpiHeaderSize	= rand(1,1024);
		this->WriteToFile(this->m_iMagicNumberID);
		this->WriteToFile(this->m_iSkpiHeaderSize);
		if( m_iSkpiHeaderSize > 0 )
		{
			char*l_pRandData = new char[m_iSkpiHeaderSize];
			for( int i=0;i<m_iSkpiHeaderSize;++i )
				l_pRandData[i] = (char)rand()%255;
			NvFWrite( l_pRandData, sizeof(char),m_iSkpiHeaderSize, m_pFile );
			delete[] l_pRandData;
		}
		return true;	
	}

	bool		cSkipHeaderBinaryFile::Writefile(const char*e_strFileName,bool e_bBinary,bool e_bForceToWrite, const char*e_strFileMode)
	{
		if(cBinaryFile::Writefile(e_strFileName,e_bBinary,e_bForceToWrite, e_strFileMode))
		{
			return CreateHeader();
		}
		return false;
	}

	int			cSkipHeaderBinaryFile::GetSkipHeaderAndHMagicNumberHeader()
	{
		if( m_pFile )
			return sizeof(m_iMagicNumberID)+sizeof(m_iSkpiHeaderSize)+m_iSkpiHeaderSize;
		return -1;
	}

	void		cSkipHeaderBinaryFile::AddGarbageEndFile()
	{
		if( m_pFile )
		{
			int	l_iGrabageSize	= rand(1,1024);
			char*l_pRandData = new char[l_iGrabageSize];
			for( int i=0;i<l_iGrabageSize;++i )
				l_pRandData[i] = (char)rand()%255;
			NvFWrite( l_pRandData, sizeof(char),l_iGrabageSize, m_pFile );
			delete[] l_pRandData;
		}
	}
}
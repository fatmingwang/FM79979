#include "stdafx.h"
//#include "BinaryFile.h"
//
//#ifdef WIN32
//#pragma warning( disable : 4996 )
//#endif
//
//
//
//cBinaryFile::cBinaryFile(bool e_bBigEndian)
//{
//	m_bBigEndian = e_bBigEndian;
//	if( e_bBigEndian )
//		SwapToPc();
//	else
//		SwapToXBOX();
//	m_pData = 0;
//	m_pFile = 0;
//}
//
//cBinaryFile::~cBinaryFile()
//{
//	CloseFile();
//};
//
//void cBinaryFile::SwapToPc()
//{
//	D0 = 0;D1 = 1;D2 = 2;D3 = 3;
//	W0 = 0;W1 = 1;
//}
//
//void cBinaryFile::SwapToXBOX()
//{
//	D0 = 3;D1 = 2;D2 = 1;D3 = 0;
//	W0 = 1;W1 = 0;
//}
//
//VOID cBinaryFile::WriteToFile(FLOAT val )
//{
//	CHAR data[4] = { ((CHAR*)&val)[D0], ((CHAR*)&val)[D1],
//					 ((CHAR*)&val)[D2], ((CHAR*)&val)[D3] };
//	fwrite( data, sizeof(data), 1, m_pFile ); 
//}
//
//VOID cBinaryFile::WriteToFile(WCHAR val )
//{
//	CHAR data[2] = { ((CHAR*)&val)[W0], ((CHAR*)&val)[W1] };
//	fwrite( data, sizeof(data), 1, m_pFile ); 
//}
////this is wrong,and I don't know why
//VOID cBinaryFile::WriteToFile(INT   val )
//{
//	assert(0);
//	CHAR data[4] = { ((CHAR*)&val)[D0], ((CHAR*)&val)[D1],
//					 ((CHAR*)&val)[D2], ((CHAR*)&val)[D3] };
//	fwrite( data, sizeof(data), 1, m_pFile ); 
//}
//
//VOID cBinaryFile::WriteToFile(DWORD val )
//{
//	CHAR data[4] = { ((CHAR*)&val)[D0], ((CHAR*)&val)[D1],
//					 ((CHAR*)&val)[D2], ((CHAR*)&val)[D3] };
//	fwrite( data, sizeof(data), 1, m_pFile ); 
//}
//
//
//VOID cBinaryFile::WriteToFile(SHORT val )
//{
//	CHAR data[2] = { ((CHAR*)&val)[W0], ((CHAR*)&val)[W1] };
//	fwrite( data, sizeof(data), 1, m_pFile ); 
//}
//
//
//VOID cBinaryFile::WriteToFile(WORD val )
//{
//	CHAR data[2] = { ((CHAR*)&val)[W0], ((CHAR*)&val)[W1] };
//	fwrite( data, sizeof(data), 1, m_pFile ); 
//}
//
//VOID cBinaryFile::WriteToFile(char* val)
//{
//	int	l_iLength = strlen(val);
//	fwrite( val, sizeof(char),l_iLength, m_pFile ); 
//}
//
//VOID cBinaryFile::WriteToFile(WCHAR* val )
//{
//	int	l_iLength = wcslen(val);
//	if( m_bBigEndian )
//	{
//		fwrite( val, sizeof(WCHAR),l_iLength, m_pFile ); 
//	}
//	else
//	{
//		for( int i=0;i<l_iLength;++i )
//			WriteToFile(val[i]);
//	}
//}
//
//bool	cBinaryFile::Writefile(char*e_str)
//{
//	this->m_pFile = fopen(e_str,"wb");
//	if(m_pFile)
//		return true;
//	return false;
//}
//
//void	cBinaryFile::CloseFile()
//{
//	if( m_pData )
//		free(m_pData);
//	m_pData = 0;
//	if( m_pFile )
//		fclose( m_pFile );
//	m_pFile = 0;
//}
//
//bool	cBinaryFile::Openfile(char*e_strFileName)
//{
//	this->m_pFile = fopen(e_strFileName,"rb");
//	if(m_pFile)
//		return true;
//	return false;
//}
//
//void*	cBinaryFile::GetDataFile(UINT e_uiStart)
//{
//	if(!m_pData)
//	{
//		assert(m_pFile&&"file is not existed");
//		long l_lFileSize = GetFileSize(this->m_pFile);
//		m_pData = malloc(l_lFileSize);
//		fread(m_pData,sizeof(char),l_lFileSize,m_pFile);
//	}
//	return (void*)((BYTE*)m_pData+e_uiStart);
//}
//
////push data into e_pDestData
//void*	cBinaryFile::GetDataFileSegment(UINT e_uiStart,UINT e_uiSLength,void*e_pDestData)
//{
//#ifdef WIN32
//	fpos_t pos = e_uiStart;
//	fsetpos(m_pFile,&pos);
//	fread(e_pDestData,sizeof(BYTE),e_uiSLength,m_pFile);
//	rewind(m_pFile);
//	return e_pDestData;
//#else
//		//or just indtead by pos.__pos = e_uiStart;
//	//and call as win32
//        unsigned short*l_pData = (unsigned short*)GetDataFile(e_uiStart);
//        WCHAR*l_Transto2Wchar = (WCHAR*)e_pDestData;        
//        for(int i=0;i<e_uiSLength/2;++i )
//        {
//            l_Transto2Wchar[i] = l_pData[i];
//        }
//        l_Transto2Wchar[e_uiSLength/2] = L'\0';
//	return e_pDestData;
//#endif
//}
//#include "../stdafx.h"
#include "CustomBinaryData.h"
#include "BinaryFile.h"
#include "assert.h"
namespace FATMING_CORE
{
	cCustomBinaryData::sSizeAndData::sSizeAndData(int e_iSize,char*e_pData)
	{
		iSize = e_iSize;
		pData = new char[iSize];
		memcpy(pData,e_pData,iSize);
	}
	cCustomBinaryData::sSizeAndData::~sSizeAndData()
	{
		SAFE_DELETE(pData);
	}

	cCustomBinaryData::sNameAndData::sNameAndData()
	{
		pName = nullptr;
		pData = nullptr;
	}

	//cCustomBinaryData::sNameAndData::sNameAndData(int e_iNameSzie,char*e_pNameData,int e_iDataSize,char*e_pData)
	//{
	//	pName = new sSizeAndData(e_iNameSzie,e_pNameData);
	//	pData = new sSizeAndData(e_iDataSize,e_pData);
	//}

	cCustomBinaryData::sNameAndData::~sNameAndData()
	{
		SAFE_DELETE(pName);
		SAFE_DELETE(pData);
	}

	cCustomBinaryData::cDataAndPos::cDataAndPos()
	{
		iStartPos = -1;
		iID = -1;
		iLength = 0;
		pData = new sNameAndData;
	}

	cCustomBinaryData::cDataAndPos::~cDataAndPos()
	{
		SAFE_DELETE(pData);
	}

	cCustomBinaryData::cCustomBinaryData(int e_iMagicIDForDevice)
	{
		m_pSkipHeaderBinaryFile = new cSkipHeaderBinaryFile(e_iMagicIDForDevice);
	}

	cCustomBinaryData::~cCustomBinaryData()
	{
		SAFE_DELETE(m_pSkipHeaderBinaryFile);
	}

	bool		cCustomBinaryData::OpenFile(const char*e_strFileName,const char*e_strformat)
	{
		m_strFileName = e_strFileName;
		if(m_pSkipHeaderBinaryFile->Openfile(e_strFileName,e_strformat))
		{
			int	l_iCurrentPos = 0;
			int	l_iFileLength = -1;
			char*l_pData = (char*)m_pSkipHeaderBinaryFile->GetDataFile(0,&l_iFileLength);
			//char	l_strName[MAX_PATH];
			while( l_iFileLength > 0 )
			{
				cDataAndPos*l_pDataAndPos = new cDataAndPos();
				int	l_iCurrentDataLength = sizeof(int)*2;
				l_pDataAndPos->iStartPos = l_iCurrentPos;
				//for name
				int	l_iNameLength = *(int*)l_pData;
				l_pData += 4;
				if( l_iNameLength > 0 )
				{
					l_iCurrentDataLength += l_iNameLength;
					l_pDataAndPos->pData->pName = new sSizeAndData(l_iNameLength,l_pData);
					l_pData += l_iNameLength;
				}
				//for data
				int	l_iDataLength = *(int*)l_pData;
				l_pData += 4;
				if( l_iDataLength > 0 )
				{
					l_iCurrentDataLength += l_iDataLength;
					l_pDataAndPos->pData->pData = new sSizeAndData(l_iDataLength,l_pData);
					l_pData += l_iDataLength;
				}
				l_iFileLength -= l_iCurrentDataLength;
				l_pDataAndPos->iLength = l_iCurrentDataLength;
				l_iCurrentPos += l_iCurrentDataLength;
			}
			return true;
		}
		return false;
	}
	//use m_DataAndPosVector to re write all file.
	bool		cCustomBinaryData::SaveFile(bool e_bRewriteData)
	{
		if( m_strFileName.length() > 0 )
		{
			if( e_bRewriteData )
			{//close current file and reopen.
				if( this->m_pSkipHeaderBinaryFile->GetFile()!= nullptr )
				{
					m_pSkipHeaderBinaryFile->CloseFile();
					if(m_pSkipHeaderBinaryFile->Writefile(this->m_strFileName.c_str(),true,"w+"))
					{
						int	l_iCount = this->m_DataAndPosVector.Count();
						int	l_iCurrentPos = 0;
						for (int i = 0; i < l_iCount; i++)
						{
							cDataAndPos*l_pDataAndPos = m_DataAndPosVector[i];
							l_pDataAndPos->iStartPos = l_iCurrentPos;
							if(l_pDataAndPos->bKeepData)
							{
								sNameAndData*l_pNameAndData = l_pDataAndPos->pData;
								l_pDataAndPos->iLength = sizeof(int)*2+l_pNameAndData->pData->iSize+l_pNameAndData->pName->iSize;
								l_iCurrentPos += l_pDataAndPos->iLength;
								assert(l_pNameAndData->pName->iSize > 0&&"name id must bigger than 0");
								m_pSkipHeaderBinaryFile->WriteToFile(l_pNameAndData->pName->iSize);
								m_pSkipHeaderBinaryFile->WriteToFile(l_pNameAndData->pName->pData,l_pNameAndData->pName->iSize);
								if( l_pNameAndData->pData->iSize > 0 )
								{
									m_pSkipHeaderBinaryFile->WriteToFile(l_pNameAndData->pData->iSize);
									m_pSkipHeaderBinaryFile->WriteToFile(l_pNameAndData->pData->pData,l_pNameAndData->pData->iSize);
								}
							}
						}
						m_pSkipHeaderBinaryFile->Flush();
					}
					else
					{
						return false;
					}
				}
			}
			return true;
		}
		return false;
	}

	cCustomBinaryData::cDataAndPos*		cCustomBinaryData::GetObject(int e_iID)
	{
		int	l_iCount = m_DataAndPosVector.Count();
		for (int i = 0; i < l_iCount; i++)
		{
			if(m_DataAndPosVector[i]->iID == e_iID)
			{
				return m_DataAndPosVector[i];
			}
		}
		return nullptr;
	}

	cCustomBinaryData::cDataAndPos*		cCustomBinaryData::AssignData(cDataAndPos*e_pDataAndPos,char*e_pData,int e_iDataLength)
	{
		cDataAndPos*l_pDataAndPos = e_pDataAndPos;
		if( l_pDataAndPos == nullptr )
		{
			l_pDataAndPos = new cDataAndPos();
			l_pDataAndPos->iID = -1;
			l_pDataAndPos->bKeepData = true;
			l_pDataAndPos->pData->pData->iSize = e_iDataLength;
			m_DataAndPosVector.AddObjectNeglectExist(l_pDataAndPos);
		}
		SAFE_DELETE(l_pDataAndPos->pData->pData->pData);
		SAFE_DELETE(l_pDataAndPos->pData->pName->pData);
		l_pDataAndPos->pData->pName->iSize = -1;
		l_pDataAndPos->pData->pData->pData = new char[e_iDataLength];
		memcpy(l_pDataAndPos->pData->pData->pData,e_pData,sizeof(e_iDataLength));
		//the data will add later
		l_pDataAndPos->iLength = e_iDataLength+sizeof(int)*2;
		return l_pDataAndPos;
	}

	cCustomBinaryData::cDataAndPos*		cCustomBinaryData::AddData(int e_iID,char*e_pData,int e_iDataLength)
	{
		cDataAndPos*l_pDataAndPos = this->GetObject(e_iID);
		l_pDataAndPos = AssignData(l_pDataAndPos,e_pData,e_iDataLength);
		l_pDataAndPos->iID = e_iID;
		l_pDataAndPos->bKeepData = true;
		l_pDataAndPos->pData->pName->iSize = sizeof(int);
		l_pDataAndPos->pData->pName->pData = new char[sizeof(int)];
		memcpy(l_pDataAndPos->pData->pName->pData,&e_iID,sizeof(int));
		l_pDataAndPos->iLength += sizeof(int);
		return l_pDataAndPos;
	}
	cCustomBinaryData::cDataAndPos*		cCustomBinaryData::AddData(const char*e_strID,char*e_pData,int e_iDataLength)
	{
		cDataAndPos*l_pDataAndPos = m_DataAndPosVector.GetObject(e_strID);
		l_pDataAndPos = AssignData(l_pDataAndPos,e_pData,e_iDataLength);
		l_pDataAndPos->pData->pName->iSize = (int)(sizeof(char)*strlen(e_strID));
		l_pDataAndPos->pData->pName->pData = new char[l_pDataAndPos->pData->pName->iSize];
		memcpy(l_pDataAndPos->pData->pName->pData,e_strID,l_pDataAndPos->pData->pName->iSize);
		l_pDataAndPos->iLength += l_pDataAndPos->pData->pName->iSize;
		return l_pDataAndPos;	
	}
	cCustomBinaryData::cDataAndPos*		cCustomBinaryData::AddData(const wchar_t*e_strID,char*e_pData,int e_iDataLength)
	{
		cDataAndPos*l_pDataAndPos = m_DataAndPosVector.GetObject(e_strID);
		l_pDataAndPos = AssignData(l_pDataAndPos,e_pData,e_iDataLength);
		l_pDataAndPos->pData->pName->iSize = (int)(sizeof(wchar_t)*wcslen(e_strID));
		l_pDataAndPos->pData->pName->pData = (char*)new wchar_t[l_pDataAndPos->pData->pName->iSize];
		memcpy(l_pDataAndPos->pData->pName->pData,e_strID,l_pDataAndPos->pData->pName->iSize*sizeof(wchar_t));
		l_pDataAndPos->iLength += l_pDataAndPos->pData->pName->iSize;
		return l_pDataAndPos;		
	}

	cCustomBinaryData::cDataAndPos*cCustomBinaryData::GetDataByID(int e_iID)
	{
		return GetObject(e_iID);
	}
	cCustomBinaryData::cDataAndPos*cCustomBinaryData::GetDataByName(const wchar_t*e_strName)
	{
		return m_DataAndPosVector.GetObject(e_strName);
	}
	////if the data size is not same,it might occur file crupption
	//void		cCustomBinaryData::ChangeData(cDataAndPos*e_pData,bool e_bWriteFileimmediately)
	//{
	//
	//}
}
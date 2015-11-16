#pragma once
//#define GET_OBJECT_BY_ID_FUNCTION_1
//	inline T*	GetObjectByID(int e_iID)
//	{
//		int	l_iCount = Count();
//		for (int i = 0; i < l_iCount; i++)
//		{
//			T*l_pData = GetObject(i);
//			if(l_pData->iID == e_iID)
//			{
//				return l_pData[i];
//			}
//		}
//		return nullptr;
//	}
namespace FATMING_CORE
{
	//
	//to store game data and game progress.
	//
	//
	//
	class cSkipHeaderBinaryFile;

	class cCustomBinaryData
	{
		std::string	m_strFileName;
	public:
		struct sSizeAndData
		{
			int iSize;
			char*pData;
			sSizeAndData(int e_iSize,char*e_pData);
			~sSizeAndData();
		};
		struct sNameAndData
		{
			sSizeAndData	*pName;
			sSizeAndData	*pData;
			//sNameAndData(int e_iNameSzie,char*e_pNameData,int e_iDataSize,char*e_pData);
			sNameAndData();
			~sNameAndData();
		};
		class cDataAndPos:public NamedTypedObject
		{
			friend class cCustomBinaryData;
			//in file pos
			int				iStartPos;
			//for safe check of pData
			int				iLength;
			//if -1,the ID is string
			int				iID;
			sNameAndData*	pData;
		public:
			cDataAndPos();
			~cDataAndPos();
			//sometimes we do not hop this file keep again.
			bool			bKeepData;
			sNameAndData*	GetNameAndData();
		};
	private:
		cNamedTypedObjectVector<cDataAndPos>	m_DataAndPosVector;
		cSkipHeaderBinaryFile*					m_pSkipHeaderBinaryFile;
		cCustomBinaryData::cDataAndPos*			AssignData(cDataAndPos*e_pDataAndPos,char*e_pData,int e_iDataLength);
		cCustomBinaryData::cDataAndPos*			GetObject(int e_iID);
	public:
		//for local file save e_iMagicIDForDevice,
		//if google cloud storage is actived this one should use 79979 not device ID.
		cCustomBinaryData(int e_iMagicIDForDevice = 79979);
		~cCustomBinaryData();
		//open mode should be keep old file data
		//"r+",Open a file for update both reading and writing. The file must exist
		//if file is not exists use w
		bool								OpenFile(const char*e_strFileName,const char*e_strformat);
		//use m_DataAndPosVector to re write all file.
		bool								SaveFile(bool e_bRewriteData = false);
		cCustomBinaryData::cDataAndPos*		AddData(int e_iID,char*e_pData,int e_iDataLength);
		cCustomBinaryData::cDataAndPos*		AddData(const char*e_strID,char*e_pData,int e_iDataLength);
		cCustomBinaryData::cDataAndPos*		AddData(const wchar_t*e_strID,char*e_pData,int e_iDataLength);
		cDataAndPos*						GetDataByID(int e_iID);
		cDataAndPos*						GetDataByName(const wchar_t*e_strName);
		////if the data size is not same,it might occur file crupption
		//void								ChangeData(cDataAndPos*e_pData,bool e_bWriteFileimmediately);

	};
//end namespace FATMING_CORE
}
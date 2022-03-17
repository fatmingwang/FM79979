#pragma once
#ifdef WASM
#include "../../XML/XMLLoader.h"
#include "../Render/CommonRender/BaseImage.h"
//if download data is not latest please clear cached...
//I spent a lot time to deubg why data is not same...-_-
//
//caching in IndexedDB not in memory.
//https://emscripten.org/docs/api_reference/fetch.html
//https://stackoverflow.com/questions/54617194/how-to-save-files-from-c-to-browser-storage-with-emscripten
namespace FATMING_CORE
{
	//preload and keep until game relaease
	//now only support wasm
	class cPreLoadFromInternet :public cNodeISAX
	{
		//Mar/06/2022
		struct sFileData
		{
			std::wstring	strMD5;
			std::wstring	strFileName;
			size_t			uiSize;
			bool			Assign(TiXmlElement* e_pTiXmlElement);
		};
		std::map<std::wstring, sFileData>	m_FromCache_FileNameAndFileDataMap;
		std::map<std::wstring, sFileData>	m_FromWASMData_FileNameAndFileDataMap;
		void								WriteCacheFileData();
		std::map<std::wstring, sFileData>	ParseCacheFileData(TiXmlElement*e_pRoot);
		std::string							m_strDomainName;
		sFileData							m_CurrentFileData;
		bool								m_bWaitIndexedDBInitOkay;
		bool								m_bDoAllFileDownloadedSyncIndexedDB;
		//
		std::vector<std::string>			m_strDownloadFailedFileName;
		bool								m_bWaitForDownloadFromInternet;
		float								m_fElpaseTime;
		UT::sTimeCounter					m_WaitIndexedDBsyncTC;
		UT::sTimeAndFPS						m_ParseTC;
		UT::sTimeAndFPS						m_TimeElpaseTC;
		std::wstring						m_strCurrentObjectInfo;
		std::wstring						m_strLastObject;
		int									m_iResourceCount;
		int									m_iCurrentResourceIndex;
		int									m_iSkipDomainNameIndex;
		int									m_iNumByteDownload;
		void								DoFetchFile();
	public:
		cPreLoadFromInternet();
		~cPreLoadFromInternet();
		void	AddDownloadSize(int e_iSize) { m_iNumByteDownload += e_iSize; }
		void	SetWaitForDownloadFromInternet(bool e_bWaitForDownloadFromInternet,std::string e_strFileName);
		void	AddDownloadFailedFileName(const char*e_strFileName);
		bool	Init(const char*e_strPreloadFileName);
		void	Run();
		void	Update(float e_fElpaseTime);
		void	Render();
		float	GetProgress();
		int		GetSkipDomainNameIndex() {return m_iSkipDomainNameIndex;}
		bool	IsFinish();
	};
//end namespace FATMING_CORE
}
#endif
#pragma once
#ifdef WASM
#include "../../XML/XMLLoader.h"
#include "../Render/CommonRender/BaseImage.h"
//if download data is not latest please clear cached...
//I spent a lot time to deubg why data is not same...-_-
namespace FATMING_CORE
{
	//preload and keep until game relaease
	//now only support wasm
	class cPreLoadFromInternet :public cNodeISAX
	{
		std::vector<std::string> m_strDownloadFailedFileName;
		bool			m_bWaitForDownloadFromInternet;
		float			m_fElpaseTime;
		UT::sTimeAndFPS	m_ParseTC;
		UT::sTimeAndFPS	m_TimeElpaseTC;
		std::wstring	m_strCurrentObjectInfo;
		std::wstring	m_strLastObject;
		int				m_iResourceCount;
		int				m_iCurrentResourceIndex;
		int				m_iSkipDomainNameIndex;
		std::vector<cBaseImage*>	m_BaseImageVector;
	public:
		cPreLoadFromInternet();
		~cPreLoadFromInternet();
		void	SetWaitForDownloadFromInternet(bool e_bWaitForDownloadFromInternet);
		void	AddDownloadFailedFileName(const char*e_strFileName);
		bool	Init(const char*e_strPreloadFileName);
		void	Run();
		void	Update(float e_fElpaseTime);
		void	Render();
		float	GetProgress();
		int		GetSkipDomainNameIndex() {return m_iSkipDomainNameIndex;}
	};
//end namespace FATMING_CORE
}
#endif
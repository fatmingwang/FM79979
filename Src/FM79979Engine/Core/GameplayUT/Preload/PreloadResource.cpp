#include "../../stdafx.h"
#include "../GameplayUT/Log/FMLog.h"
#ifdef WASM
#include "PreloadResource.h"
#include <emscripten/fetch.h>
#include "../GameApp.h"
#include "../BinaryFile.h"
namespace FATMING_CORE
{
	void downloadSucceeded(emscripten_fetch_t *fetch)
	{
		printf("Finished downloading %llu bytes from URL %s.\n", fetch->numBytes, fetch->url);
		cPreLoadFromInternet*l_pPreLoadFromInternet = (cPreLoadFromInternet*)fetch->userData;
		int l_iSkipDomainNameIndex = l_pPreLoadFromInternet->GetSkipDomainNameIndex();
		int	l_iLength = strlen(fetch->url);
		int l_iCopyLength = l_iLength - l_iSkipDomainNameIndex;
		cBinaryFile l_cBinaryFile;
		char l_strSaveFileName[MAX_PATH];
		memcpy(l_strSaveFileName, &fetch->url[l_iSkipDomainNameIndex], l_iCopyLength);
		l_strSaveFileName[l_iCopyLength] = 0;
		std::string l_strFileName = l_strSaveFileName;
		if (l_cBinaryFile.Writefile(l_strFileName.c_str(), true, true))
		{
			//if (l_strFileName.find("xml") != std::string::npos)
			//{
			//	FMLog::LogWithFlag(fetch->data);
			//}
			l_cBinaryFile.WriteToFile(&fetch->data[0], fetch->numBytes);
			FMLog::LogWithFlag("save file finish", CORE_LOG_FLAG);
		}
		else
		{
			l_strFileName += " save file failed";
			FMLog::LogWithFlag(l_strFileName.c_str(), CORE_LOG_FLAG);
		}
		// The data is now available at fetch->data[0] through fetch->data[fetch->numBytes-1];
		l_pPreLoadFromInternet->SetWaitForDownloadFromInternet(false);
		emscripten_fetch_close(fetch); // Free data associated with the fetch.
		FMLog::LogWithFlag("emscripten_fetch_close", CORE_LOG_FLAG);
	}

	void downloadFailed(emscripten_fetch_t *fetch)
	{
		cPreLoadFromInternet*l_pPreLoadFromInternet = (cPreLoadFromInternet*)fetch->userData;
		l_pPreLoadFromInternet->AddDownloadFailedFileName(fetch->url);
		printf("Downloading %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);
		emscripten_fetch_close(fetch); // Also free data on failure.
	}

	cPreLoadFromInternet::cPreLoadFromInternet()
	{
		m_fElpaseTime = 0.f;
		m_iResourceCount = 0;
		m_iCurrentResourceIndex = 0;
		m_bWaitForDownloadFromInternet = false;
		m_iSkipDomainNameIndex = 0;
	}
	cPreLoadFromInternet::~cPreLoadFromInternet()
	{

	}

	void	cPreLoadFromInternet::SetWaitForDownloadFromInternet(bool e_bWaitForDownloadFromInternet)
	{
		m_bWaitForDownloadFromInternet = e_bWaitForDownloadFromInternet;
		++m_iCurrentResourceIndex;
		std::wstring l_str = L"Progress:";
		l_str += ValueToStringW(GetProgress());
		FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
	}
	void	cPreLoadFromInternet::AddDownloadFailedFileName(const char*e_strFileName)
	{
		std::string l_strFileName = e_strFileName;
		m_strDownloadFailedFileName.push_back(l_strFileName);
		m_bWaitForDownloadFromInternet = false;
		//++m_iCurrentResourceIndex;
	}

	bool	cPreLoadFromInternet::Init(const char*e_strPreloadFileName)
	{
		m_iCurrentResourceIndex = 0;
		m_strDownloadFailedFileName.clear();
		m_ParseTC.Update();
		m_fElpaseTime = 0.f;
		if (this->ParseDataIntoXMLNode(e_strPreloadFileName))
		{
			FMLog::LogWithFlag("cPreLoadFromInternet start ok", CORE_LOG_FLAG);
			this->m_pCurrentTiXmlElement = this->m_pRootElement;
			if (m_pCurrentTiXmlElement)
			{
				auto l_strCount = m_pCurrentTiXmlElement->Attribute(L"Count");
				auto l_strDomainName = m_pCurrentTiXmlElement->Attribute(L"DomainName");
				if (l_strCount)
					m_iResourceCount = GetInt(l_strCount);
				if (l_strDomainName)
					m_iSkipDomainNameIndex = wcslen(l_strDomainName);
				m_pCurrentTiXmlElement = m_pCurrentTiXmlElement->FirstChildElement();
				if (m_pCurrentTiXmlElement)
				{
					auto l_pAttribute = m_pCurrentTiXmlElement->FirstAttribute();
					m_strCurrentObjectInfo = l_pAttribute->Value();
				}
			}
			m_ParseTC.Update();
			m_TimeElpaseTC.Update();
			return true;
		}
		FMLog::LogWithFlag("cPreLoadFromInternet start failed", CORE_LOG_FLAG);
		return false;
	}

	void	cPreLoadFromInternet::Run()
	{
		m_TimeElpaseTC.Update();
		Update(m_TimeElpaseTC.fElpaseTime);
		Render();
	}

	void	cPreLoadFromInternet::Update(float e_fElpaseTime)
	{
		m_fElpaseTime += m_ParseTC.fElpaseTime;
		m_ParseTC.Update();
		if (m_bWaitForDownloadFromInternet)
			return;
		if (m_pCurrentTiXmlElement)
		{
			auto l_pAttribute = m_pCurrentTiXmlElement->FirstAttribute();
			if (l_pAttribute)
			{
				const wchar_t*l_pValue = l_pAttribute->Value();
				std::string l_strFileName = ValueToString(l_pValue);
				//if (!IsFileExists(l_strFileName.c_str()))
				{
					m_bWaitForDownloadFromInternet = true;
					//add command  -s FETCH=1
					emscripten_fetch_attr_t attr;
					emscripten_fetch_attr_init(&attr);
					strcpy(attr.requestMethod, "GET");
					attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;//EMSCRIPTEN_FETCH_REPLACE
					attr.onsuccess = downloadSucceeded;
					attr.onerror = downloadFailed;
					attr.userData = this;
					emscripten_fetch(&attr, l_strFileName.c_str());
					
					//FMLog::LogWithFlag("start to download");
					//FMLog::LogWithFlag(l_strFileName.c_str());
					FMLog::LogWithFlag(UT::ComposeMsgByFormat("start to download %s", l_strFileName.c_str()).c_str(), CORE_LOG_FLAG);
				}
				//else
				//{
				//	//FMLog::LogWithFlag(l_strFileName.c_str());
				//	//FMLog::LogWithFlag("exists skip download!");
				//	FMLog::LogWithFlag(UT::ComposeMsgByFormat("%s exists skip download!", l_strFileName.c_str()).c_str());
				//}
			}
			m_pCurrentTiXmlElement = m_pCurrentTiXmlElement->NextSiblingElement();
			if (m_pCurrentTiXmlElement)
			{
				l_pAttribute = m_pCurrentTiXmlElement->FirstAttribute();
				m_strLastObject = m_strCurrentObjectInfo;
				m_strCurrentObjectInfo = l_pAttribute->Value();
			}
		}
		m_ParseTC.Update();
		//if (m_ParseTC.fElpaseTime > 0.1f)
		{
			std::wstring l_strInfo = m_strLastObject;
			l_strInfo += L":";
			l_strInfo += ValueToStringW(m_ParseTC.fElpaseTime);
			if (m_ParseTC.fElpaseTime >= 0.5f)
				FMLog::LogWithFlag("*********start*****************", CORE_LOG_FLAG);
			cGameApp::WriteLog(ValueToString(l_strInfo));
			if (m_ParseTC.fElpaseTime >= 0.5f)
				FMLog::LogWithFlag("*********  end   **************", CORE_LOG_FLAG);
		}
	}
	void	cPreLoadFromInternet::Render()
	{
		Vector2 l_vPos(cGameApp::m_svGameResolution.x / 2 - 200, cGameApp::m_svGameResolution.y / 2 - 50);
		std::wstring l_strInfo = L"Progress:";
		l_strInfo += ValueToStringW(GetProgress());
		cGameApp::RenderFont(l_vPos, l_strInfo.c_str());
		l_vPos.y += 100;
		l_strInfo = L"CurrentParsingObject:";
		l_strInfo += UT::GetFileNameWithoutFullPath(m_strCurrentObjectInfo.c_str(), false);
		if(m_strCurrentObjectInfo.length())
			cGameApp::RenderFont(l_vPos, l_strInfo.c_str());
		l_vPos.y += 100;
		l_strInfo = UT::GetFileNameWithoutFullPath(m_strLastObject.c_str(), false);
		if(m_strLastObject.length())
			cGameApp::RenderFont(l_vPos, l_strInfo.c_str());
		l_vPos.y += 50;
		l_strInfo = L"Time:";
		l_strInfo += ValueToStringW(m_ParseTC.fElpaseTime);
		cGameApp::RenderFont(l_vPos, l_strInfo.c_str());
		l_vPos.y += 100;
		l_strInfo = L"TotalTime:";
		l_strInfo += ValueToStringW(m_fElpaseTime);
		cGameApp::RenderFont(l_vPos, l_strInfo.c_str());
	}

	float	cPreLoadFromInternet::GetProgress()
	{
		if ((!m_pCurrentTiXmlElement && !this->m_bWaitForDownloadFromInternet) || m_iResourceCount == 0)
		{
			return 1.f;
		}
		float l_fValue = m_iCurrentResourceIndex / (float)m_iResourceCount;
		return l_fValue;
	}
//end namespace FATMING_CORE
}
#endif
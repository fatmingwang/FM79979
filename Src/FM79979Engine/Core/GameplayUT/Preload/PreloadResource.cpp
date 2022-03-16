#include "PreloadResource.h"
#ifdef WASM
#include <emscripten/fetch.h>
#include <emscripten.h>
#include "../../Common/Log/FMLog.h"
#include "../../Common/BinaryFile/BinaryFile.h"
#include "../GameApp/GameApp.h"
#include <fstream>
#include <sys/stat.h>

namespace FATMING_CORE
{
	void downloadProgress(emscripten_fetch_t* fetch) 
	{
		if (fetch->totalBytes) 
		{
			printf("Downloading %s.. %.2f%% complete.\n", fetch->url, fetch->dataOffset * 100.0 / fetch->totalBytes);
		}
		else 
		{
			printf("Downloading %s.. %lld bytes complete.\n", fetch->url, fetch->dataOffset + fetch->numBytes);
		}
	}
	void downloadSucceeded(emscripten_fetch_t *fetch)
	{
		printf("Finished downloading %llu bytes from URL %s.\n", fetch->numBytes, fetch->url);
		cPreLoadFromInternet*l_pPreLoadFromInternet = (cPreLoadFromInternet*)fetch->userData;
		l_pPreLoadFromInternet->AddDownloadSize(fetch->numBytes);
		int l_iSkipDomainNameIndex = l_pPreLoadFromInternet->GetSkipDomainNameIndex();
		int	l_iLength = strlen(fetch->url);
		int l_iCopyLength = l_iLength - l_iSkipDomainNameIndex;
		cBinaryFile l_cBinaryFile;
		//  add /GameApp for IndexDB.
		auto l_strFileName = UT::ComposeMsgByFormat("%s",fetch->url);
		if (l_cBinaryFile.Writefile(l_strFileName.c_str(), true, true))
		{
			//if (l_strFileName.find("xml") != std::string::npos)
			//{
			//	FMLog::LogWithFlag(fetch->data);
			//}
			l_cBinaryFile.WriteToFile(fetch->data, fetch->numBytes);
			FMLog::LogWithFlag("save file finish", CORE_LOG_FLAG);
		}
		else
		{
			l_strFileName += " save file failed";
			FMLog::LogWithFlag(l_strFileName.c_str(), CORE_LOG_FLAG);
		}
		// The data is now available at fetch->data[0] through fetch->data[fetch->numBytes-1];
		emscripten_fetch_close(fetch); // Free data associated with the fetch.
		FMLog::LogWithFlag("emscripten_fetch_close", CORE_LOG_FLAG);
		l_pPreLoadFromInternet->SetWaitForDownloadFromInternet(false);
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
		m_iNumByteDownload = 0;
		m_bWaitIndexDBResponse = true;
		m_bDoAllFileDownloadedSyncIndexDB = false;
		m_fElpaseTime = 0.f;
		m_iResourceCount = 0;
		m_iCurrentResourceIndex = 0;
		m_bWaitForDownloadFromInternet = false;
		m_iSkipDomainNameIndex = 0;
		m_WaitIndexDBsyncTC.SetTargetTime(15.f);
		////http://www.alternativegames.net/blog/porting-to-emscripten/
		EM_ASM
		(
			//FS.createFolder('/', 'user_data', true, true)
			//FS.mount(IDBFS, {}, '/user_data');
			// Make a directory other than '/'
			Module.IndexDBName = '/GameApp';
			FS.mkdir(Module.IndexDBName);
			// Then mount with IDBFS type
			FS.mount(IDBFS, {}, Module.IndexDBName);
			Module.print("start file sync");
			//flag to check when data are synchronized
			Module.syncdone = 0;
			Module.WaitAllFileFinish = 1;
			// Then sync
			FS.syncfs(true, function(err)
			{
				if (err)
				{
					console.log('ERROR!', err);
				}
				Module.syncdone = 1;
				console.log('finished syncing..');
			});
		);

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
		m_iNumByteDownload = 0;
		m_iCurrentResourceIndex = 0;
		m_bDoAllFileDownloadedSyncIndexDB = false;
		m_strDownloadFailedFileName.clear();
		m_ParseTC.Update();
		m_fElpaseTime = 0.f;
		FMLog::LogWithFlag("cPreLoadFromInternet::Init", CORE_LOG_FLAG);
		if (this->ParseDataIntoXMLNode(e_strPreloadFileName))
		{
			FMLog::LogWithFlag("cPreLoadFromInternet start ok1", CORE_LOG_FLAG);
			this->m_pCurrentTiXmlElement = this->m_pRootElement;
			if (m_pCurrentTiXmlElement)
			{
				auto l_strCount = m_pCurrentTiXmlElement->Attribute(L"Count");
				auto l_strDomainName = m_pCurrentTiXmlElement->Attribute(L"DomainName");
				if (l_strCount)
				{
					m_iResourceCount = GetInt(l_strCount);
				}
				if (l_strDomainName)
				{
					m_iSkipDomainNameIndex = wcslen(l_strDomainName);
					m_strDomainName = ValueToString(l_strDomainName);
				}
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
		{
			return;
		}
		if (m_pCurrentTiXmlElement && emscripten_run_script_int("Module.syncdone") != 1 && !m_WaitIndexDBsyncTC.bTragetTimrReached)
		{
			m_WaitIndexDBsyncTC.Update(e_fElpaseTime);
			FMLog::LogWithFlag("wait sync ", CORE_LOG_FLAG, false);
			if (m_WaitIndexDBsyncTC.bTragetTimrReached)
			{
				FMLog::LogWithFlag("wait sync too long,fuck", CORE_LOG_FLAG, false);
			}
			return;
		}
		if (m_pCurrentTiXmlElement)
		{
			m_WaitIndexDBsyncTC.Start();
			m_CurrentFileData.Assign(m_pCurrentTiXmlElement);
			bool l_bDownloadFile = true;
			auto l_pAttribute = m_pCurrentTiXmlElement->FirstAttribute();
			if (l_pAttribute)
			{
				const wchar_t*l_pValue = l_pAttribute->Value();
				std::string l_strFileName = m_strDomainName+"/" + ValueToString(l_pValue);
				NvFile* l_pFile = MyFileOpen(l_strFileName.c_str(), "rb");
				if (l_pFile)
				{
					auto l_uiSize = UT::GetFileSize(l_pFile);
					if (m_CurrentFileData.uiSize == l_uiSize)
					{
						l_bDownloadFile = false;
					}
					else
					{
						
						FMLog::LogWithFlag(UT::ComposeMsgByFormat("size not same,%d:%d", l_uiSize, m_CurrentFileData.uiSize).c_str(), CORE_LOG_FLAG);
					}
					NvFClose(l_pFile);
				}
				if (l_bDownloadFile)
				{
					m_bWaitForDownloadFromInternet = true;
					//add command  -s FETCH=1
					emscripten_fetch_attr_t attr;
					emscripten_fetch_attr_init(&attr);
					strcpy(attr.requestMethod, "GET");
					//attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;//EMSCRIPTEN_FETCH_REPLACE
					attr.attributes = EMSCRIPTEN_FETCH_PERSIST_FILE;
					attr.onsuccess = downloadSucceeded;
					attr.onerror = downloadFailed;
					attr.onprogress = downloadProgress;
					attr.userData = this;
					FMLog::LogWithFlag(UT::ComposeMsgByFormat("start to download %s", l_strFileName.c_str()).c_str(), CORE_LOG_FLAG);
					emscripten_fetch(&attr, l_strFileName.c_str());
					
					//FMLog::LogWithFlag("start to download");
					//FMLog::LogWithFlag(l_strFileName.c_str());
				}
				else
				{
					//FMLog::LogWithFlag(l_strFileName.c_str());
					//FMLog::LogWithFlag("exists skip download!");
					FMLog::LogWithFlag(UT::ComposeMsgByFormat("%s exists skip download!", l_strFileName.c_str()).c_str(), CORE_LOG_FLAG);
				}
			}
			m_pCurrentTiXmlElement = m_pCurrentTiXmlElement->NextSiblingElement();
			if (m_pCurrentTiXmlElement)
			{
				l_pAttribute = m_pCurrentTiXmlElement->FirstAttribute();
				m_strLastObject = m_strCurrentObjectInfo;
				m_strCurrentObjectInfo = l_pAttribute->Value();
			}
			else
			{
			}
		}
		else
		{
			//persist Emscripten current data to Indexed Db
			if(!m_bDoAllFileDownloadedSyncIndexDB)
			{
				m_bDoAllFileDownloadedSyncIndexDB = true;
				EM_ASM
				(
					Module.print("all file downloaded do IndexDB sync..");
					Module.syncdone = 0;
					FS.syncfs(false, function(err)
					{
						if (err)
						{
							console.log('ERROR!', err);
						}
						Module.print("End File sync..");
						Module.syncdone = 1;
						Module.WaitAllFileFinish = 0;
					});
				);
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
		cGameApp::m_spOpenGLRender->Render();
		glEnable2D(720.f, 1280.f);
		GLRender::RenderRectangle(600, 50, cMatrix44::TranslationMatrix(Vector3(50, 625, 0)), Vector4::Red);
		GLRender::RenderFilledRectangle(Vector2(50, 625), 600 * this->GetProgress(), 50, Vector4::Green, 0);
		//if (!m_strCurrentObjectInfo.length())
		{
			//return;
		}
		cGameApp::RenderFont(100,100, UT::ComposeMsgByFormat(L"Download size:%d", m_iNumByteDownload).c_str());		
		Vector2 l_vPos(cGameApp::m_spOpenGLRender->m_vGameResolution.x / 2 - 200, cGameApp::m_spOpenGLRender->m_vGameResolution.y / 2 - 50);
		std::wstring l_strInfo = L"Progress:";
		l_strInfo += ValueToStringW(GetProgress());
		cGameApp::RenderFont(l_vPos, l_strInfo.c_str());
		l_vPos.y += 100;
		l_strInfo = L"CurrentParsingObject:";
		if (m_strCurrentObjectInfo.length())
		{
			l_strInfo += UT::GetFileNameWithoutFullPath(m_strCurrentObjectInfo.c_str(), false);
			cGameApp::RenderFont(l_vPos, l_strInfo.c_str());
		}
		l_vPos.y += 100;
		if (m_strLastObject.length())
		{
			l_strInfo = UT::GetFileNameWithoutFullPath(m_strLastObject.c_str(), false);
			cGameApp::RenderFont(l_vPos, l_strInfo.c_str());
		}
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
			if (IsFinish())
			{
				return 1.f;
			}
			return 0.979f;
		}
		float l_fValue = m_iCurrentResourceIndex / (float)m_iResourceCount;
		return l_fValue;
	}
	bool cPreLoadFromInternet::IsFinish()
	{
		if (emscripten_run_script_int("Module.WaitAllFileFinish") == 0)
		{
			return true;
		}
		return false;
	}
	bool cPreLoadFromInternet::sFileData::Assign(TiXmlElement* e_pTiXmlElement)
	{
		auto l_strMD5 = e_pTiXmlElement->Attribute(L"MD5");
		if (l_strMD5)
		{
			strMD5 = l_strMD5;
		}
		auto l_strFileName = e_pTiXmlElement->Attribute(L"FileName");
		if (l_strFileName)
		{
			strFileName = l_strFileName;
		}
		auto l_strSize = e_pTiXmlElement->Attribute(L"Size");
		if (l_strSize)
		{
			uiSize = GetUint64(l_strSize);
		}
		return true;
	}
	//end namespace FATMING_CORE
}
#endif
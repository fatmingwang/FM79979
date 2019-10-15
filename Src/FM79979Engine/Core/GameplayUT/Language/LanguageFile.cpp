#include "../../stdafx.h"
#include "LanguageFile.h"
#include "../GameApp.h"
namespace FATMING_CORE
{
	//<cXMLFileList Name="CH">
	//	<File Key="KeyName" FileName="somewhere/filename.extension" />
	//</cXMLFileList>

	cXMLFileList::cXMLFileList()
	{

	}
	cXMLFileList::~cXMLFileList()
	{
		DEL_MAP(std::wstring, std::wstring*, m_KeyAndFileNameMap);
	}
	bool	cXMLFileList::MyParse(TiXmlElement*e_pRoot)
	{
		DEL_MAP(std::wstring, std::wstring*, m_KeyAndFileNameMap);
		//const wchar_t*l_strName = e_pRoot->Attribute(L"Name");
		//if(!l_strName)
		//{ 
		//	UT::ErrorMsg("cXMLFileList dont has a name!", m_strFileName);
		//	return false;
		//}
		//this->SetName(l_strName);
		e_pRoot = e_pRoot->FirstChildElement();
		while (e_pRoot)
		{
			const wchar_t*l_strElementValue = e_pRoot->Value();
			if(wcscmp(l_strElementValue,L"File") == 0)
			{
				std::wstring*l_pstrFileName = nullptr;
				std::wstring l_strKey;
				PARSE_ELEMENT_START(e_pRoot)
					COMPARE_NAME("Key")
					{
						l_strKey = l_strValue;
					}
					else
					COMPARE_NAME("FileName")
					{
						l_pstrFileName = new std::wstring(l_strValue);
					}
				PARSE_NAME_VALUE_END
				assert(l_pstrFileName &&"FileName no data!?");
				if (m_KeyAndFileNameMap.find(l_strKey) != m_KeyAndFileNameMap.end())
				{
					UT::ErrorMsg(L"Error Key exists!", l_strName);
					auto l_pValue = m_KeyAndFileNameMap[l_strKey];
					SAFE_DELETE(l_pValue);
					m_KeyAndFileNameMap.erase(l_strKey);
					this->RemoveObject(l_strKey);
				}
				m_KeyAndFileNameMap.insert(std::make_pair(l_strKey, l_pstrFileName));
			}
			e_pRoot = e_pRoot->NextSiblingElement();
		}
		return true;
	}
		

	const wchar_t*		cXMLFileList::GetFileName(const wchar_t*e_strKey)
	{
		auto l_Iterator = m_KeyAndFileNameMap.find(e_strKey);
		if (l_Iterator != m_KeyAndFileNameMap.end())
			return l_Iterator->second->c_str();
		return nullptr;
	}

	bool cXMLFileList::ParseKeyAndValueXMLFile()
	{
		bool l_bParseResult = true;
		for(auto l_Iterator : m_KeyAndFileNameMap)
		{
			cKeyAndValueXML*l_pKeyAndValueXML = new cKeyAndValueXML();
			if (!l_pKeyAndValueXML->ParseWithMyParse(l_Iterator.second->c_str()))
			{
				FMLog::Log(UT::ComposeMsgByFormat(L"file:%ls not exists", l_Iterator.second->c_str()).c_str(), true);
				SAFE_DELETE(l_pKeyAndValueXML);
				l_bParseResult = false;
			}
			else
			{
				l_pKeyAndValueXML->SetName(l_Iterator.first.c_str());
				this->AddObjectNeglectExist(l_pKeyAndValueXML);
			}
		}
		return l_bParseResult;
	}


	cLanguageFile::cLanguageFile(const char*e_strSetupFile)
	{
		m_strDefaultLanguage = L"";
		this->ParseWithMyParse(e_strSetupFile);
		//m_strDefaultLanguage = m_strCurrentLanguage = e_strDefaultLanguage;
		assert(m_strDefaultLanguage.length() > 0 && "default language wrong!?");
	}
	cLanguageFile::~cLanguageFile()
	{

	}
	//<cLanguageFile Default="CH">
	//<Language Name="" FileName=""/>
	//</cLanguageFile>
	bool	cLanguageFile::MyParse(TiXmlElement*e_pRoot)
	{
		this->Destroy();
		auto l_strDefault = e_pRoot->Attribute(L"Default");
		if (l_strDefault)
		{
			m_strCurrentLanguage = this->m_strDefaultLanguage = l_strDefault;
		}
		e_pRoot = e_pRoot->FirstChildElement();
		while (e_pRoot)
		{
			std::wstring l_strLanguageName;
			cXMLFileList*l_pXMLFileList = nullptr;
			PARSE_ELEMENT_START(e_pRoot)
				COMPARE_NAME("Name")
				{
					l_strLanguageName = l_strValue;
				}
				else
				COMPARE_NAME("FileName")
				{
					l_pXMLFileList = new cXMLFileList();
					if (!l_pXMLFileList->ParseWithMyParse(l_strValue))
					{
						UT::ErrorMsg(L"language parse failed!", l_strValue);
					}
				}
			PARSE_NAME_VALUE_END
			l_pXMLFileList->SetName(l_strLanguageName);
			if (!this->AddObject(l_pXMLFileList))
			{
				SAFE_DELETE(l_pXMLFileList);
				UT::ErrorMsg(L"same language!", l_strLanguageName.c_str());
			}
			e_pRoot = e_pRoot->NextSiblingElement();
		}
		return true;
	}
	const wchar_t*		cLanguageFile::GetFileName(const wchar_t*e_strKey)
	{
		auto l_pObject = this->GetObject(m_strCurrentLanguage.c_str());
		if (l_pObject)
		{
			auto l_pstrFeilName = l_pObject->GetFileName(e_strKey);
			if (l_pstrFeilName)
			{
				return l_pstrFeilName;
			}
		}
		l_pObject = this->GetObject(m_strDefaultLanguage.c_str());
		auto l_strResult = l_pObject->GetFileName(e_strKey);
		if (l_strResult)
		{
			return l_strResult;
		}
		std::wstring l_strInfo = e_strKey;
		l_strInfo += L" no such key ";
		l_strInfo += e_strKey;
		l_strInfo += L" in ";
		l_strInfo += this->m_strCurrentLanguage;
		FMLog::LogWithFlag(l_strInfo, CORE_LOG_FLAG);
		return nullptr;
	}

	cXMLFileList * cLanguageFile::GetCurrentLanguageXMLFileList()
	{
		return this->GetObject(m_strCurrentLanguage.c_str());
	}

	cKeyAndValueXML * cLanguageFile::GetCurrentLanguageXMLFileListKeyAndValueXMLByName(const wchar_t * e_strFileName)
	{
		auto l_pCurrentLanguageXMLFileList = GetCurrentLanguageXMLFileList();
		if (l_pCurrentLanguageXMLFileList)
		{
			return l_pCurrentLanguageXMLFileList->GetObject(e_strFileName);
		}
		return nullptr;
	}

	bool cKeyAndValueXML::MyParse(TiXmlElement * e_pRoot)
	{
		DELETE_MAP(m_KeyAndValueMap);
		//auto l_strXMLName = e_pRoot->Value(L"Name");
		//if (l_strXMLName)
		//	this->SetName(l_strXMLName);
		FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pRoot)
			PARSE_ELEMENT_START(e_pRoot)
#ifdef DEBUG
			if (m_KeyAndValueMap.find(l_strName) != m_KeyAndValueMap.end())
			{
				UT::ErrorMsgByFormat(L"Name:%ls exists!", l_strName);
			}
#endif
			m_KeyAndValueMap[l_strName] = new std::wstring(l_strValue);
			TO_NEXT_VALUE
		FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pRoot)
		return true;
	}

	cKeyAndValueXML::cKeyAndValueXML()
	{
	}

	cKeyAndValueXML::~cKeyAndValueXML()
	{
		DELETE_MAP(m_KeyAndValueMap);
	}

	const wchar_t * cKeyAndValueXML::GetValueByKey(const wchar_t * e_strKey)
	{
		auto l_Iterator = m_KeyAndValueMap.find(e_strKey);
		if (l_Iterator != m_KeyAndValueMap.end())
			return l_Iterator->second->c_str();
		return nullptr;
	}

	//end namespace FATMING_CORE
}
#pragma once
#include "../../XML/XMLLoader.h"

namespace FATMING_CORE
{
	//<cXMLFileList Name="CH">
	//	<File Key="KeyName" FileName="somewhere/filename.extension" />
	//</cXMLFileList>
	class cXMLFileList :public cNodeISAX,public NamedTypedObject
	{
		std::map<std::wstring, std::wstring*>	m_KeyAndFileNameMap;
		virtual	bool	MyParse(TiXmlElement*e_pRoot);
	public:
		cXMLFileList();
		~cXMLFileList();
		const wchar_t*		GetFileName(const wchar_t*e_strKey);
	};
	//if target language cann find the file use default file.
	//<cLanguageFile Default="CH">
		//<Language Name="CH" FileName="CH_LanguageSetting.xml"/>
		//<Language Name="EN" FileName="EN_LanguageSetting.xml"/>
	//</cLanguageFile>
	class cLanguageFile :public cNamedTypedObjectVector<cXMLFileList> , public cNodeISAX
	{
		std::wstring		m_strCurrentLanguage;
		std::wstring		m_strDefaultLanguage;
		virtual	bool		MyParse(TiXmlElement*e_pRoot);
	public:
		cLanguageFile(const char*e_strSetupFile);
		~cLanguageFile();
		const std::wstring	GetCurrentLanguage() { return m_strCurrentLanguage; }
		void				SetCurrentLanguage(const wchar_t*e_strCurrentLanguage) { m_strCurrentLanguage = e_strCurrentLanguage; }
		const wchar_t*		GetFileName(const wchar_t*e_strKey);
	};

	//cLanguageFile l_LanguageFile("Language/Language.xml");
	//auto l_p1 = l_LanguageFile.GetFileName(L"KeyName");
	//l_LanguageFile.SetCurrentLanguage(L"EN");
	//auto l_p11 = l_LanguageFile.GetFileName(L"KeyName");
	//l_LanguageFile.SetCurrentLanguage(L"CN");
	//auto l_p2 = l_LanguageFile.GetFileName(L"KeyName2");
	//auto l_p3 = l_LanguageFile.GetFileName(L"KeyName4");
//end namespace FATMING_CORE
}
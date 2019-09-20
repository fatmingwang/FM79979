#pragma once
#include "../../XML/XMLLoader.h"

namespace FATMING_CORE
{
	//you can make your data in a element,it will save all data into map
	//<cKeyAndValueXML>
	//	<Data Key1="Value1" Key3="Value3"/>
	//	<Data Key2="Value2"/>
	//</cKeyAndValueXML>
	class cKeyAndValueXML :public cNodeISAX, public NamedTypedObject
	{
		std::map<std::wstring, std::wstring*>	m_KeyAndValueMap;
		virtual	bool	MyParse(TiXmlElement*e_pRoot);
	public:
		cKeyAndValueXML();
		~cKeyAndValueXML();
		const wchar_t*		GetValueByKey(const wchar_t*e_strKey);
	};
	//if DoParseFileByValue=1 it will parse file and generate cKeyAndValueXML.
	//then you can have multi file,get your file then get your text
	//cXMLFileList::GetObject(Key)->GetValueByKey(Key);
	//<cXMLFileList Name="CH" DoParseFileByValue="1">
	//	<File Key="KeyName" FileName="somewhere/filename.extension" />
	//</cXMLFileList>
	class cXMLFileList :public cNodeISAX,public cNamedTypedObjectVector<cKeyAndValueXML>
	{
		std::map<std::wstring, std::wstring*>	m_KeyAndFileNameMap;
		virtual	bool	MyParse(TiXmlElement*e_pRoot);
	public:
		cXMLFileList();
		~cXMLFileList();
		const wchar_t*		GetFileName(const wchar_t*e_strKey);
		//for multi files not only 1 file(use GetFileName to get value)
		bool				ParseKeyAndValueXMLFile();
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
		//for 1 file only
		const wchar_t*		GetFileName(const wchar_t*e_strKey);
		//
		cXMLFileList*		GetCurrentLanguageXMLFileList();
	};

	//cLanguageFile l_LanguageFile("Language/Language.xml");
	//only 1 file,directly to get value by key
	//auto l_p1 = l_LanguageFile.GetFileName(L"KeyName");
	//l_LanguageFile.SetCurrentLanguage(L"EN");
	//auto l_p11 = l_LanguageFile.GetFileName(L"KeyName");
	//l_LanguageFile.SetCurrentLanguage(L"CN");
	//auto l_p2 = l_LanguageFile.GetFileName(L"KeyName2");
	//auto l_p3 = l_LanguageFile.GetFileName(L"KeyName4");
	//
	//multi file(each file has its own key and value map)
	//auto l_pTargetLanguage = cLanguageFile::GetObject(TargetLanguage);
	//l_pTargetLanguage->ParseKeyAndValueXMLFile();
	//auto l_pTargetFile = l_pTargetLanguage->GetObject(FileKey);
	//auto l_strValue = l_pTargetFile->GetValueByKey(Key);
//end namespace FATMING_CORE
}
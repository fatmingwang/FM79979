#include "../../../stdafx.h"
#include "FontManager.h"
#include "GlyphFontRender.h"
#include "FreetypeGlypth.h"

namespace FATMING_CORE
{
	int		cFontManager::m_siFreeTypeFontSize = 12;
	void cFontManager::Process_GlyphFontRender(TiXmlElement * e_pElement)
	{
		int l_iFontCount = 300;
		auto l_strFileName = e_pElement->Attribute(L"FileName");
		auto l_strFontCount = e_pElement->Attribute(L"FontCount");
		if (l_strFileName)
		{
			auto l_strcFileName = ValueToString(l_strFileName);
			if (l_strFontCount)
			{
				l_iFontCount = GetInt(l_strFontCount);
			}
			cGlyphFontRender*l_pFont = new cGlyphFontRender(l_strcFileName.c_str(), l_iFontCount);
			this->AddObjectNeglectExist(l_pFont);
			if (!this->AddObjectNeglectExist(l_pFont))
			{
				SAFE_DELETE(l_pFont);
			}
		}
	}

	void cFontManager::Process_FreeTypeFontRender(TiXmlElement * e_pElement)
	{
		int l_iFontCount = 300;
		int l_iFontSize = m_siFreeTypeFontSize;
		auto l_strFileName = e_pElement->Attribute(L"FileName");
		auto l_strFontCount = e_pElement->Attribute(L"FontCount");
		auto l_strFontSize = e_pElement->Attribute(L"FontSize");
		if (l_strFileName)
		{
			auto l_strcFileName = ValueToString(l_strFileName);
			if (l_strFontCount)
			{
				l_iFontCount = GetInt(l_strFontCount);
			}
			if (l_strFontSize)
			{
				l_iFontSize = GetInt(l_strFontSize);
			}
			cFreetypeGlyphRender*l_pFont = new cFreetypeGlyphRender(l_strcFileName.c_str(), l_iFontSize, l_iFontCount);
			if (!this->AddObjectNeglectExist(l_pFont))
			{
				SAFE_DELETE(l_pFont);
			}
		}
	}
	//<FontManager>
	//	<cGlyphFontRender FileName="" FontCount="300" />
	//	<cFreetypeGlyphRender FileName="" FontSize="" FontCount="300"/>
	//<FontManager/>
	bool cFontManager::MyParse(TiXmlElement * e_pRoot)
	{
		if (!wcscmp(e_pRoot->Value(), L"cFontManager"))
		{
			e_pRoot = e_pRoot->FirstChildElement();
			while (e_pRoot)
			{
				e_pRoot = e_pRoot->NextSiblingElement();
				if (!wcscmp(e_pRoot->Value(), cGlyphFontRender::TypeID))
				{
					Process_GlyphFontRender(e_pRoot);
				}
				else
				if (!wcscmp(e_pRoot->Value(), cFreetypeGlyphRender::TypeID))
				{
					Process_FreeTypeFontRender(e_pRoot);
				}
				else
				{
					FMLog::Log(UT::ComposeMsgByFormat(L"cFontManager::MyParse:%ls not support Element!", e_pRoot->Value()).c_str(),true);
				}
			}
		}
		return false;
	}

	NamedTypedObject* cFontManager::GetObjectByFileName(const char * e_strFileName)
	{
		NamedTypedObject*l_pFont = cNamedTypedObjectVector::GetObjectByFileName(e_strFileName);
		if (l_pFont)
			return l_pFont;
		auto l_strExtension = UT::GetFileExtensionName(e_strFileName);
		if (l_strExtension.compare("FontInfo") == 0)
		{
			l_pFont = new cGlyphFontRender(e_strFileName);
		}
		else
		{
			l_pFont = new cFreetypeGlyphRender(e_strFileName, m_siFreeTypeFontSize);
		}
		return l_pFont;
	}
}
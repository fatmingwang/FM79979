#pragma once
namespace FATMING_CORE
{
	//<FontManager>
	//	<cGlyphFontRender FileName="" FontCount = "300" />
	//	<cFreetypeGlyphRender FileName="" FontSize="" FontCount="300"/>
	//<FontManager/>
	//Glypth and freetype
	class cFontManager :public cNamedTypedObjectVector<cRenderObject>, public cNodeISAX
	{
		void						Process_GlyphFontRender(TiXmlElement*e_pElement);
		void						Process_FreeTypeFontRender(TiXmlElement*e_pElement);
		virtual	bool				MyParse(TiXmlElement*e_pRoot)override;
	public:
		virtual NamedTypedObject*	GetObjectByFileName(const char*e_strFileName)override;
		static  int					m_siFreeTypeFontSize;
	};
}
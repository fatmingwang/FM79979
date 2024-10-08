#include "FreetypeGlypth.h"
#include "../../../Common/CommonApp.h"
#ifdef LINUX
#include <freetype2/ft2build.h>
#include <freetype2/freetype/freetype.h>
#include <freetype2/freetype/ftglyph.h>
#include <freetype2/freetype/ftoutln.h>
#include <freetype2/freetype/fttrigon.h>
#else
	#if defined(UWP)
	#define generic GenericFromFreeTypeLibrary
	#endif
#include "../../../../Freetype/include/ft2build.h"
#include "../../../../Freetype/include/freetype/freetype.h"
#include "../../../../Freetype/include/freetype/ftglyph.h"
#include "../../../../Freetype/include/freetype/ftoutln.h"
#include "../../../../Freetype/include/freetype/fttrigon.h"
	#if defined(UWP)
	#undef generic
	#endif
#endif
#include "FreetypeGlypth.h"
#include "../Texture/Texture.h"
#include "../../OpenGL/GLSL/Shader.h"
#include "../CommonRender/SimplePrimitive.h"
#include "../../../Common/Log/FMLog.h"
namespace FATMING_CORE
{
	//fuck this for android parse ttf file
	//https://devtut.github.io/android/getting-system-font-names-and-using-the-fonts.html#getting-system-font-names
	//Getting system font names and using the fonts
	//	The following examples show how to retrieve the default names of the system fonts that are store in the / system / fonts / directory and how to use a system font to set the typeface of a TextView element.

	//	#Getting system font names
	//	ArrayList<String> fontNames = new ArrayList<String>();
	//File temp = new File("/system/fonts/");
	//String fontSuffix = ".ttf";

	//for (File font : temp.listFiles()) {
	//	String fontName = font.getName();
	//	if (fontName.endsWith(fontSuffix)) {
	//		fontNames.add(fontName.subSequence(0, fontName.lastIndexOf(fontSuffix)).toString());
	//	}
	//}

	//#Applying a system font to a TextView
	//	In the following code you need to replace fontsname by the name of the font you would like to use :

	//TextView lblexample = (TextView)findViewById(R.id.lblexample);
	//lblexample.setTypeface(Typeface.createFromFile("/system/fonts/" + "fontsname" + ".ttf"));
	//
	//this another method to list all fonts
	//https://github.com/aosp-mirror/platform_frameworks_base/blob/master/graphics/java/android/graphics/FontListParser.java
	//
	class cDynamicFontTexture :public cSmartObject, public NamedTypedObject
	{
		friend class cFreetypeGlyphRender;
	public:
		struct sGlyphInfo
		{
			POINT	Size;
			POINT	Offset;
			Vector4	vUV;
		};
	protected:
		//
		FT_Library						m_FT_Library;
		FT_Face							m_Face;
		//
		POINT							m_TextureSize;
		int								m_iLastCharacterPosX;//right of character
		int								m_iLastCharacterPosY;//top of character
		int								m_iFontSize;
		std::map<wchar_t, sGlyphInfo>	m_CharacterAndGlyphInfo;
		//
		class cTexture*					m_pTexture;//default size is 1024*1024
		bool							AddNewCharacterToTexture(wchar_t e_Wchar);
	public:
		cDynamicFontTexture(const char*e_strFontName, cFreetypeGlyphRender*e_pcFreetypeGlyphRender, unsigned int e_uiFontSize = 12, POINT e_DefaultTextureSize = { 512,512 });
		cDynamicFontTexture(cDynamicFontTexture*e_pDynamicFontTexture) = delete;
		virtual ~cDynamicFontTexture();
		sGlyphInfo*	GetGlyphInfo(wchar_t e_Wchar);
		bool		ApplyImage();
		void		DebugRender(Vector2 e_vPos);
		cTexture* GetTexture() {return m_pTexture;}
	};
	TYPDE_DEFINE_MARCO(cFreetypeGlyphRender);
	cFreetypeGlyphRender::cFreetypeGlyphRender(const char * e_strFontName, int e_iFontSize, int e_iFontCount, POINT e_DefaultFontTextureSize)
	{
		m_vHalfSize = Vector2::Zero;
		m_pDynamicFontTexture = new cDynamicFontTexture(e_strFontName, this, e_iFontSize, e_DefaultFontTextureSize);
		m_pDynamicFontTexture->AddRef(this);
		m_fScale = 1.f;
		m_bTextChanged = true;
		m_iFontCount = e_iFontCount;
		m_pvVertexBuffer = 0;
		m_pvTextureUVBuffer = 0;
		m_pvColorBuffer = 0;
		char	l_strForStripExtensionName[TEMP_SIZE];
		sprintf(l_strForStripExtensionName, "%s", UT::GetFileNameWithoutFullPath(e_strFontName, false).c_str());
		char	l_strForDirectory[TEMP_SIZE];
		std::string	l_strFoneName = UT::GetDirectoryWithoutFileName(e_strFontName);
		if (l_strFoneName.length())
			sprintf(l_strForDirectory, "%s", l_strFoneName.c_str());
		else
			l_strForDirectory[0] = '\0';
		SetName(UT::CharToWchar(l_strForStripExtensionName));
		m_pvVertexBuffer = new Vector2[TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT * e_iFontCount];//one quad four vertex,a vertex 2 data(x,y)
		m_pvTextureUVBuffer = new Vector2[TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT * e_iFontCount];
		m_pvColorBuffer = new Vector4[TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT * e_iFontCount];
		for (int i = 0; i < e_iFontCount * TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT; ++i)
			m_pvColorBuffer[i] = Vector4::One;
	}

	cFreetypeGlyphRender::cFreetypeGlyphRender(cFreetypeGlyphRender * e_pGlyphFontRender)
	{
		m_pvVertexBuffer = nullptr;
		m_pvTextureUVBuffer = nullptr;
		m_pvColorBuffer = nullptr;
		m_vHalfSize = Vector2::Zero;
		m_pDynamicFontTexture = e_pGlyphFontRender->m_pDynamicFontTexture;
		m_pDynamicFontTexture->AddRef(this);
		m_bTextChanged = true;
		m_fScale = e_pGlyphFontRender->m_fScale;
		m_iFontCount = e_pGlyphFontRender->m_iFontCount;
		std::wstring l_strName = e_pGlyphFontRender->GetName();
		l_strName += L"_Cloned";
		this->SetName(l_strName);
		SetTextBuffer(e_pGlyphFontRender->m_iFontCount);
	}

	cFreetypeGlyphRender::~cFreetypeGlyphRender()
	{
		SAFE_DELETE(m_pvVertexBuffer);
		SAFE_DELETE(m_pvTextureUVBuffer);
		SAFE_DELETE(m_pvColorBuffer);
		SAFE_RELEASE(m_pDynamicFontTexture,this);
	}

	void cFreetypeGlyphRender::RenderFont(float e_fX, float e_fY, const wchar_t * e_pString)
	{
		RenderFont(e_fX, e_fY, e_pString, 0);
	}

	void cFreetypeGlyphRender::RenderFont(float e_fX, float e_fY, const wchar_t * e_pString, Vector4 * e_pDrawRect)
	{
		int	l_iLen = (int)wcslen(e_pString);
		if (l_iLen == 0 || !m_pDynamicFontTexture)
			return;
		if (wcscmp(m_strText.c_str(), e_pString) || m_bTextChanged)
		{
			m_bTextChanged = false;
			m_strText = e_pString;
			if (l_iLen >= m_iFontCount)
			{
				l_iLen = m_iFontCount;
			}
			auto	l_pFirstGlyph = m_pDynamicFontTexture->GetGlyphInfo(e_pString[0]);
			float	l_fXOffset = 0;
			float	l_fYOffset = (float)-l_pFirstGlyph->Offset.y;
			float	l_fDrawWidth = 0.f;
			float	l_fDrawHeight = (float)m_pDynamicFontTexture->m_iFontSize;
			int		l_iAliveIndex = 0;
			for (int i = 0; i < l_iLen; ++i)
			{
				float*l_pfVertexData = (float*)&m_pvVertexBuffer[l_iAliveIndex*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
				float*l_pfTextData = (float*)&m_pvTextureUVBuffer[l_iAliveIndex*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
				//unicode
				if (e_pString[i] == 13 && ((i + 1 < l_iLen) && e_pString[i + 1] == 10))
				{
					continue;
				}
				//ansi
				if (e_pString[i] != L'\n')
				{
					auto l_pGlyph = m_pDynamicFontTexture->GetGlyphInfo(e_pString[i]);
					if (!l_pGlyph)
					{
						continue;
					}
					if (e_pString[i] == L' ')
					{
						l_fXOffset += m_pDynamicFontTexture->m_iFontSize/2;
						continue;
					}
					else
					if (e_pString[i] == L'\t')
					{
						l_fXOffset += m_pDynamicFontTexture->m_iFontSize/2*3;
						continue;
					}
					float   l_fCharacterWidth = (float)l_pGlyph->Size.x;
					float   l_fCharacterHeight = (float)l_pGlyph->Size.y;
					float	l_fGlyphYOffset = 0.f;
					//if (i != 0)
					{
						l_fXOffset += l_pGlyph->Offset.x;
						l_fGlyphYOffset = (float)l_pGlyph->Offset.y;
					}
					//pos
					*l_pfVertexData = l_fXOffset;											++l_pfVertexData;
					*l_pfVertexData = l_fYOffset + l_fGlyphYOffset;							++l_pfVertexData;
					*l_pfVertexData = l_fXOffset + l_fCharacterWidth;						++l_pfVertexData;
					*l_pfVertexData = l_fYOffset + l_fGlyphYOffset;							++l_pfVertexData;
					*l_pfVertexData = l_fXOffset;											++l_pfVertexData;
					*l_pfVertexData = l_fYOffset + l_fCharacterHeight + l_fGlyphYOffset;	++l_pfVertexData;

					*l_pfVertexData = l_fXOffset;											++l_pfVertexData;
					*l_pfVertexData = l_fYOffset + l_fCharacterHeight + l_fGlyphYOffset;	++l_pfVertexData;
					*l_pfVertexData = l_fXOffset + l_fCharacterWidth;						++l_pfVertexData;
					*l_pfVertexData = l_fYOffset + l_fGlyphYOffset;							++l_pfVertexData;
					*l_pfVertexData = l_fXOffset + l_fCharacterWidth;						++l_pfVertexData;
					*l_pfVertexData = l_fYOffset + l_fCharacterHeight + l_fGlyphYOffset;

					//UV
					*l_pfTextData = l_pGlyph->vUV.x;						++l_pfTextData;
					*l_pfTextData = l_pGlyph->vUV.y;						++l_pfTextData;
					*l_pfTextData = l_pGlyph->vUV.z;						++l_pfTextData;
					*l_pfTextData = l_pGlyph->vUV.y;						++l_pfTextData;
					*l_pfTextData = l_pGlyph->vUV.x;						++l_pfTextData;
					*l_pfTextData = l_pGlyph->vUV.w;						++l_pfTextData;

					*l_pfTextData = l_pGlyph->vUV.x;						++l_pfTextData;
					*l_pfTextData = l_pGlyph->vUV.w;						++l_pfTextData;
					*l_pfTextData = l_pGlyph->vUV.z;						++l_pfTextData;
					*l_pfTextData = l_pGlyph->vUV.y;						++l_pfTextData;
					*l_pfTextData = l_pGlyph->vUV.z;						++l_pfTextData;
					*l_pfTextData = l_pGlyph->vUV.w;

					l_fXOffset += (l_fCharacterWidth);
					if (l_fDrawWidth < l_fXOffset)
						l_fDrawWidth = l_fXOffset;
					++l_iAliveIndex;
				}
				else
				{
					if (l_fDrawWidth < l_fXOffset)
						l_fDrawWidth = l_fXOffset;
					l_fXOffset = 0;
					l_fYOffset += m_pDynamicFontTexture->m_iFontSize;
					l_fDrawHeight += m_pDynamicFontTexture->m_iFontSize;
				}
			}
			if (l_iAliveIndex == 0)
				return;
			m_iDrawCount = l_iAliveIndex;
			if (e_pDrawRect)
			{
				e_pDrawRect->x = e_fX;
				e_pDrawRect->y = e_fY;
				e_pDrawRect->z = e_fX + l_fDrawWidth;
				e_pDrawRect->w = e_fY + l_fDrawHeight;
			}
			float	l_fHalfWidth = l_fDrawWidth;
			float	l_fHalfHeight = l_fDrawHeight;
			for (int i = 0; i < l_iAliveIndex; ++i)
			{
				float*l_pfVertexData = (float*)&m_pvVertexBuffer[i*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
				*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;
				*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;
				*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;

				*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;
				*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;
				*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight;
			}
			m_vHalfSize.x = l_fHalfWidth* m_fScale;
			m_vHalfSize.y = l_fHalfHeight* m_fScale;
			auto l_pLocalBound = GetLocalBound();
			if (l_pLocalBound)
			{
				GenerateBound();
			}
		}
		auto l_pTexture = m_pDynamicFontTexture->GetTexture();
		if (l_pTexture && l_pTexture->GetImageIndex() != -1)
		{
			UseShaderProgram(DEFAULT_SHADER);
			if (m_pDynamicFontTexture->ApplyImage())
			{
				cMatrix44	l_mat = cMatrix44::TranslationMatrix(Vector3(e_fX + m_vHalfSize.x, e_fY + m_vHalfSize.y, 0.f));
				l_mat *= this->GetWorldTransform() * cMatrix44::ScaleMatrix(Vector3(m_fScale, m_fScale, m_fScale));
				RenderTrianglesWithTexture((float*)m_pvVertexBuffer, (float*)m_pvTextureUVBuffer, (float*)m_pvColorBuffer, l_mat, 2, m_iDrawCount* A_QUAD_TWO_TRIANGLES, l_pTexture);
			}
		}
	}

	void cFreetypeGlyphRender::Render()
	{
		RenderFont(0, 0, this->m_strText.c_str(), nullptr);
	}

	void	cFreetypeGlyphRender::DebugRender()
	{
		if (this->m_pDynamicFontTexture)
		{
			m_pDynamicFontTexture->DebugRender(Vector2(0,100));
		}
	}

	void cFreetypeGlyphRender::SetFontColor(Vector4 e_vColor)
	{
		if (m_pvColorBuffer[0] != e_vColor)
		{
			for (int i = 0; i < m_iFontCount * TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT; ++i)
				m_pvColorBuffer[i] = e_vColor;
		}
	}

	void cFreetypeGlyphRender::AppendTextAndSetFontColor(const wchar_t * e_strText, Vector4 e_vColor)
	{
		//first find skip character
		int l_iSkipCount = 0;
		auto l_iLen = (int)this->m_strText.length();
		for (size_t i = 0; i < l_iLen; ++i)
		{
			auto l_wchar_t = m_strText[i];
			if (l_wchar_t == 13 && ((i + 1 < l_iLen) && m_strText[i + 1] == 10))
			{
				++l_iSkipCount;
			}
			else
			if (l_wchar_t == L' ' || l_wchar_t == L'\n' || l_wchar_t == '\t')
			{
				++l_iSkipCount;
			}
			else
			{
				auto l_pGlyph = m_pDynamicFontTexture->GetGlyphInfo(l_wchar_t);
				if (!l_pGlyph)
				{
					++l_iSkipCount;
				}
			}
		}
		int l_iIndex = l_iLen - l_iSkipCount;
		m_strText += e_strText;
		int l_iEndndex = (int)this->m_strText.length();
		// n t space
		for (int i = l_iIndex; i < l_iEndndex ; ++i)
		{
			if (l_iIndex >= m_iFontCount)
			{
				break;
			}
			auto l_wchar_t = m_strText[i];
			if (l_wchar_t == 13 && ((i + 1 < l_iLen) && m_strText[i + 1] == 10))
			{
				continue;
			}
			else
			if (l_wchar_t == L' ' || l_wchar_t == L'\n' || l_wchar_t == '\t')
			{
				continue;
			}
			else
			{
				auto l_pGlyph = m_pDynamicFontTexture->GetGlyphInfo(l_wchar_t);
				if (!l_pGlyph)
				{
					continue;
				}
			}
			for (int j = 0; j < TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT; ++j)
			{
				m_pvColorBuffer[l_iIndex * TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT + j] = e_vColor;
			}
			++l_iIndex;
		}
		this->m_bTextChanged = true;
	}

	Vector2 cFreetypeGlyphRender::GetRenderSize(const wchar_t * e_strText)
	{
		int	l_iLen = (int)wcslen(e_strText);
		if (l_iLen == 0 || !m_pDynamicFontTexture)
			return Vector2::Zero;
		float	l_fXOffset = 0.f;
		float	l_fYOffset = 0.f;
		float	l_fFontHeight = 0.f;
		float	l_fDrawWidth = 0.f;
		float	l_fMaxHeight = (float)m_pDynamicFontTexture->m_iFontSize;
		for (int i = 0; i < l_iLen; ++i)
		{
			if (e_strText[i] != L'\n')
			{
				auto l_pGlyph = m_pDynamicFontTexture->GetGlyphInfo(e_strText[i]);
				if (!l_pGlyph)
					continue;
				if (e_strText[i] == L' ')
				{
					l_fXOffset += m_pDynamicFontTexture->m_iFontSize/2;
					continue;
				}
				else
				if (e_strText[i] == L'\t')
				{
					l_fXOffset += m_pDynamicFontTexture->m_iFontSize/2*3;
					continue;
				}
				float   l_fCharacterWidth = (float)l_pGlyph->Size.x;
				float   l_fCharacterHeight = (float)l_pGlyph->Size.y;
				l_fFontHeight = l_fCharacterHeight;
				l_fXOffset += (l_fCharacterWidth);
				if (l_fDrawWidth < l_fXOffset)
					l_fDrawWidth = l_fXOffset;
			}
			else
			{
				if (l_fDrawWidth < l_fXOffset)
					l_fDrawWidth = l_fXOffset;
				l_fXOffset = 0;
				l_fYOffset += l_fFontHeight;
				l_fMaxHeight += l_fFontHeight;
			}
		}
		return Vector2(l_fDrawWidth* m_fScale, l_fMaxHeight* m_fScale);
	}

	std::wstring cFreetypeGlyphRender::GetText()
	{
		return this->m_strText.c_str();
	}

	void cFreetypeGlyphRender::SetText(const wchar_t * e_strText)
	{
		if (m_strText.compare(e_strText))
		{
			m_bTextChanged = true;
			this->m_strText = e_strText;
		}
	}

	const cBound * cFreetypeGlyphRender::GenerateBound()
	{
		RECT l_DrawSize = { 0,0,(long)m_vHalfSize.x*2,(long)m_vHalfSize.y*2};
		cBound l_Bound(l_DrawSize);
		this->SetLocalBound(&l_Bound);
		return this->GetLocalBound();
	}

	POINT cFreetypeGlyphRender::GetSize()
	{
		if (this->m_bTextChanged)
		{
			m_vHalfSize = this->GetRenderSize(this->m_strText.c_str());
		}
		POINT l_Size = { (long)this->m_vHalfSize.x,(long)this->m_vHalfSize.y };
		return l_Size;
	}

	void cFreetypeGlyphRender::SetTextBuffer(int e_iLength)
	{
		SAFE_DELETE(m_pvVertexBuffer);
		SAFE_DELETE(m_pvTextureUVBuffer);
		SAFE_DELETE(m_pvColorBuffer);
		m_iFontCount = e_iLength;
		if (m_iFontCount > 0)
		{
			m_pvVertexBuffer = new Vector2[TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT * m_iFontCount];//one quad four vertex,a vertex 2 data(x,y)
			m_pvTextureUVBuffer = new Vector2[TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT * m_iFontCount];
			m_pvColorBuffer = new Vector4[TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT * m_iFontCount];
			for (int i = 0; i < m_iFontCount * TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT; ++i)
				m_pvColorBuffer[i] = Vector4::One;
		}
	}

	float cFreetypeGlyphRender::GetScale()
	{
		return m_fScale;
	}
	void cFreetypeGlyphRender::SetScale(float e_fScale)
	{
		if (m_fScale != e_fScale)
		{
			m_fScale = e_fScale;
			this->m_bTextChanged = true;
		}
	}


	cDynamicFontTexture::cDynamicFontTexture(const char*e_strFontName, cFreetypeGlyphRender*e_pcFreetypeGlyphRender, unsigned int e_uiFontSize, POINT e_DefaultTextureSize)
		:cSmartObject(e_pcFreetypeGlyphRender)
	{
		this->m_iFontSize = e_uiFontSize;
		std::wstring l_strName = ValueToStringW(e_strFontName);
		m_TextureSize = e_DefaultTextureSize;
		m_iLastCharacterPosX = 0;
		m_iLastCharacterPosY = 0;
		this->m_pTexture = nullptr;

		FT_Error l_Err = FT_Init_FreeType(&m_FT_Library);
		//freetype font init
		if (l_Err)
		{
			//std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			FMLog::LogWithFlag("FT_Init_FreeType failed", CORE_LOG_FLAG, false);
			return;
		}
		const FT_F26Dot6 ptSize26Dot6 = e_uiFontSize;
#if defined(WIN32)
		#if defined(UWP)
			std::string* l_PathArray[] = { cCommonApp::m_spUWPAssetsDirectory ,cCommonApp::m_spUWPAppDataLocalDirectory };
			std::string l_strFontName;
			for (int i = 0; i < 2; ++i)
			{
				if (l_PathArray[i])
					l_strFontName = *l_PathArray[i];
				l_strFontName += e_strFontName;
				l_Err = FT_New_Face(m_FT_Library, l_strFontName.c_str(), 0, &m_Face);
				if (l_Err == 0)
					break;
			}
			if(l_Err)
			{
				FMLog::LogWithFlag(UT::ComposeMsgByFormat("FT_New_Face failed:%s", e_strFontName), CORE_LOG_FLAG, false);
				//std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
				return;
			}
		#else
			l_Err = FT_New_Face(m_FT_Library, e_strFontName, 0, &m_Face);
			if (l_Err)
			{
				FMLog::LogWithFlag(UT::ComposeMsgByFormat("FT_New_Face failed:%s", e_strFontName), CORE_LOG_FLAG, false);
				//std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
				return;
			}
		#endif
#else
		int32_t font_memory_image_length = 0;
		char *font_memory_image = GetFileContent(e_strFontName, font_memory_image_length);
		if (font_memory_image)
		{
			const FT_Byte *ft2_font_memory_image = reinterpret_cast<FT_Byte*>(font_memory_image);
			assert(font_memory_image);
			l_Err = FT_New_Memory_Face(m_FT_Library, ft2_font_memory_image, font_memory_image_length, 0, &m_Face);
			assert(!l_Err);
			delete[] font_memory_image;
			font_memory_image = nullptr;
			FMLOG("not test ,if ttf get wrong please check this cDynamicFontTexture::cDynamicFontTexture");
		}
#endif
		//l_Err = FT_Set_Char_Size(m_Face, 0, ptSize26Dot6, cGameApp::m_spOpenGLRender->m_vGameResolution.x, cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		FT_Set_Pixel_Sizes(m_Face, 0, e_uiFontSize);
		this->m_iFontSize = e_uiFontSize;

		m_TextureSize = e_DefaultTextureSize;
		m_iLastCharacterPosX = 0;
		m_iLastCharacterPosY = 0;

		unsigned int l_uiPixelCount = m_TextureSize.x*m_TextureSize.y * TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT;
		char*l_pPixel = new char[l_uiPixelCount];
		memset(l_pPixel, 1, sizeof(char)*l_uiPixelCount);
		this->m_pTexture = new cTexture(this, l_pPixel, m_TextureSize.x, m_TextureSize.y, l_strName.c_str(), false, false);
		delete[] l_pPixel;
	}


	cDynamicFontTexture::~cDynamicFontTexture()
	{
		SAFE_RELEASE(this->m_pTexture,this);
		FT_Done_Face(m_Face);
		FT_Done_FreeType(m_FT_Library);
	}
	//https://learnopengl.com/In-Practice/Text-Rendering
	//https://learnopengl-cn.readthedocs.io/zh/latest/06%20In%20Practice/02%20Text%20Rendering/
	//http://nehe.gamedev.net/tutorial/freetype_fonts_in_opengl/24001/
	bool cDynamicFontTexture::AddNewCharacterToTexture(wchar_t e_Wchar)
	{
		if (!this->m_pTexture)
			return false;
		// Load The Glyph For Our Character.
		if (FT_Load_Glyph(m_Face, FT_Get_Char_Index(m_Face, e_Wchar), FT_LOAD_DEFAULT))
		{
			FMLog::LogWithFlag("FT_Load_Glyph failed", CORE_LOG_FLAG, false);
			//throw std::runtime_error("FT_Load_Glyph failed");
			return false;
		}
		// Move The Face's Glyph Into A Glyph Object.
		FT_Glyph l_Glyph;
		if (FT_Get_Glyph(m_Face->glyph, &l_Glyph))
		{
			FMLog::LogWithFlag("FT_Get_Glyph failed", CORE_LOG_FLAG, false);
			return false;
			//throw std::runtime_error("FT_Get_Glyph failed");
		}

		// Convert The Glyph To A Bitmap.
		FT_Glyph_To_Bitmap(&l_Glyph, ft_render_mode_normal, 0, 1);
		FT_BitmapGlyph l_bitmap_glyph = (FT_BitmapGlyph)l_Glyph;
		// This Reference Will Make Accessing The Bitmap Easier.
		FT_Bitmap& l_Bitmap = l_bitmap_glyph->bitmap;
		if (m_iLastCharacterPosX + (int)l_Bitmap.width + 1 >= (int)this->m_TextureSize.x)
		{
			m_iLastCharacterPosX = 0;
			this->m_iLastCharacterPosY += this->m_iFontSize + 1;
		}
		if (m_iLastCharacterPosY + (int)l_Bitmap.rows >= (int)this->m_TextureSize.y)
		{
			FMLog::LogWithFlag("cDynamicFontTexture:texture is too small", CORE_LOG_FLAG, false);
			//increase texture size!
			return false;
		}

		int l_iColorCount = 4;
		int l_iTotalSize = l_iColorCount * l_Bitmap.width * l_Bitmap.rows;
		// Allocate Memory For The Texture Data.
		GLubyte* l_pExpanded_data = new GLubyte[l_iTotalSize];
		memset(l_pExpanded_data, 0, sizeof(char)*l_iTotalSize);
		// Here We Fill In The Data For The Expanded Bitmap.
		// Notice That We Are Using A Two Channel Bitmap (One For
		// Channel Luminosity And One For Alpha), But We Assign
		// Both Luminosity And Alpha To The Value That We
		// Find In The FreeType Bitmap.
		// We Use The ?: Operator To Say That Value Which We Use
		// Will Be 0 If We Are In The Padding Zone, And Whatever
		// Is The FreeType Bitmap Otherwise.
		for (unsigned int j = 0; j < l_Bitmap.rows; j++)
		{
			for (unsigned int i = 0; i < l_Bitmap.width; i++)
			{
				int l_iBitmapIndex = i + j * l_Bitmap.width;
				int l_iIndex = l_iColorCount* l_iBitmapIndex;
				GLubyte l_ucColor = l_Bitmap.buffer[l_iBitmapIndex];
				l_pExpanded_data[l_iIndex    ] =
					l_pExpanded_data[l_iIndex + 1] =
					l_pExpanded_data[l_iIndex + 2] =
					l_pExpanded_data[l_iIndex + 3] =
					l_ucColor;
			}
		}
		m_pTexture->ApplyImage();
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glTexSubImage2D(GL_TEXTURE_2D, 0, m_iLastCharacterPosX, m_iLastCharacterPosY, l_Bitmap.width, l_Bitmap.rows, GL_RGBA, GL_UNSIGNED_BYTE, l_pExpanded_data);
		sGlyphInfo l_sGlyphInfo;
		l_sGlyphInfo.Size.x = l_Bitmap.width;
		l_sGlyphInfo.Size.y = l_Bitmap.rows;
		l_sGlyphInfo.Offset.x = l_bitmap_glyph->left;
		l_sGlyphInfo.Offset.y = -l_bitmap_glyph->top;//have no idea why here is negative...
		l_sGlyphInfo.vUV = Vector4(
			(float)m_iLastCharacterPosX / this->m_TextureSize.x,
			(float)m_iLastCharacterPosY / this->m_TextureSize.y,
			(float)(m_iLastCharacterPosX + l_Bitmap.width) / this->m_TextureSize.x,
			(float)(m_iLastCharacterPosY + l_Bitmap.rows) / this->m_TextureSize.y);
		m_CharacterAndGlyphInfo[e_Wchar] = l_sGlyphInfo;
		m_iLastCharacterPosX += l_Bitmap.width + 1;
		delete[] l_pExpanded_data;
		FT_Done_Glyph(l_Glyph);
		return true;
	}

	cDynamicFontTexture::sGlyphInfo* cDynamicFontTexture::GetGlyphInfo(wchar_t e_Wchar)
	{
		if (!this->m_pTexture)
			return nullptr;
		sGlyphInfo*l_pGlyphInfo = nullptr;
		auto l_Iterator = m_CharacterAndGlyphInfo.find(e_Wchar);
		if (l_Iterator == m_CharacterAndGlyphInfo.end())
		{
			if (AddNewCharacterToTexture(e_Wchar))
			{
				l_Iterator = m_CharacterAndGlyphInfo.find(e_Wchar);
				l_pGlyphInfo = &l_Iterator->second;
			}
		}
		else
		{
			l_pGlyphInfo = l_pGlyphInfo = &l_Iterator->second;
		}
		return l_pGlyphInfo;
	}

	bool cDynamicFontTexture::ApplyImage()
	{
		if (m_pTexture)
		{
			return m_pTexture->ApplyImage();
		}
		return false;
	}
	void cDynamicFontTexture::DebugRender(Vector2 e_vPos)
	{
		if (m_pTexture->ApplyImage())
		{
			float l_fUV[4] = {0.f,0.f,1.f,1.f};
			GLRender::RenderQuadWithTextureAndColorAndCoordinate(e_vPos.x , e_vPos.y ,0.f, (float)m_pTexture->GetWidth(), (float)m_pTexture->GetHeight(), Vector4::One, l_fUV);
			GLRender::RenderRectangle(512, 512, cMatrix44::TranslationMatrix(e_vPos.x, e_vPos.y, 0.f), Vector4::One);
		}
	}
}
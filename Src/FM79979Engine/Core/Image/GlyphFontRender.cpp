#include "../stdafx.h"
#include "GlyphFontRender.h"
#include "SimplePrimitive.h"
#include "../GLSL/Shader.h"
#include "../GameplayUT/GameApp.h"
namespace FATMING_CORE
{

	TYPDE_DEFINE_MARCO(cGlyphFontRender);

	cGlyphFontRender::cGlyphFontRender(int e_iVertexBufferSize)
	{
		m_iVertexBufferCount = e_iVertexBufferSize;
		m_vHalfSize = Vector2::Zero;
		m_pFontImage = 0;
		m_pGlyphReader = 0;
		m_fDepth = 0.f;
		m_fScale = 1.f;
		m_iDrawCount = 0;
		m_pvVertexBuffer = new Vector2[4*e_iVertexBufferSize];//one quad four vertex,a vertex 2 data(x,y)
		m_pvTextureUVBuffer = new Vector2[4*e_iVertexBufferSize];
		m_pvColorBuffer = new Vector4[4*e_iVertexBufferSize];
		m_iBufferLength = e_iVertexBufferSize;
		for( int i=0;i<e_iVertexBufferSize*4;++i )
			m_pvColorBuffer[i] = Vector4::One;
	}

	cGlyphFontRender::cGlyphFontRender(const char* e_strFontName,int e_iVertexBufferSize)
	{
		m_iVertexBufferCount = e_iVertexBufferSize;
		m_pvVertexBuffer = 0;
		m_pvTextureUVBuffer = 0;
		m_pvColorBuffer = 0;
		char	l_strForStripExtensionName[TEMP_SIZE];
		sprintf(l_strForStripExtensionName,"%s\0",UT::GetFileNameWithoutFullPath(e_strFontName).c_str());
		char	l_strForDirectory[TEMP_SIZE];
		std::string	l_strFoneName = UT::GetDirectoryWithoutFileName(e_strFontName);
		if( l_strFoneName.length() )
			sprintf(l_strForDirectory,"%s\0",l_strFoneName.c_str());
		else
			l_strForDirectory[0] = '\0';
		SetName(UT::CharToWchar(l_strForStripExtensionName));
		char	l_str[TEMP_SIZE];
		sprintf(l_str,"%s%s.png",l_strForDirectory,l_strForStripExtensionName);
		m_pFontImage = new cBaseImage(l_str);
		if(m_pFontImage->GetTexture()->GetImageIndex() == 0 )
		{
			bool	l_b = FATMING_CORE::cGameApp::m_sbShowErrorMsgBox;
			FATMING_CORE::cGameApp::m_sbShowErrorMsgBox = true;
			SAFE_DELETE(m_pFontImage);
			m_pGlyphReader = 0;
			UT::ErrorMsg(e_strFontName,"font image not exist");
			FATMING_CORE::cGameApp::m_sbShowErrorMsgBox = l_b;
			return;
		}
		m_pGlyphReader = new GlyphReader();
		m_pGlyphReader->AddRef(this);
		sprintf(l_str,"%s%s.FontInfo",l_strForDirectory,l_strForStripExtensionName);
		if(!m_pGlyphReader->LoadFontDataFile(l_str))
			UT::ErrorMsg("read font info failed",l_str);
		m_pvVertexBuffer = new Vector2[4*e_iVertexBufferSize];//one quad four vertex,a vertex 2 data(x,y)
		m_pvTextureUVBuffer = new Vector2[4*e_iVertexBufferSize];
		m_pvColorBuffer = new Vector4[4*e_iVertexBufferSize];
		m_iBufferLength = e_iVertexBufferSize;
		for( int i=0;i<e_iVertexBufferSize*4;++i )
			m_pvColorBuffer[i] = Vector4::One;
		m_fDepth = 0.f;
		m_fScale = 1.f;

	}
	cGlyphFontRender::cGlyphFontRender(cGlyphFontRender*e_pGlyphFontRender)
	{
		m_iVertexBufferCount = e_pGlyphFontRender->m_iVertexBufferCount;
		m_pFontImage = dynamic_cast<cBaseImage*>(e_pGlyphFontRender->m_pFontImage->Clone());
		m_pGlyphReader = e_pGlyphFontRender->m_pGlyphReader;
		m_pGlyphReader->AddRef(this);
		m_pvVertexBuffer = new Vector2[4*m_iVertexBufferCount];//one quad four vertex,a vertex 2 data(x,y)
		m_pvTextureUVBuffer = new Vector2[4*m_iVertexBufferCount];
		m_pvColorBuffer = new Vector4[4*m_iVertexBufferCount];
		m_iBufferLength = m_iVertexBufferCount;
		for( int i=0;i<m_iVertexBufferCount*4;++i )
			m_pvColorBuffer[i] = Vector4::One;
		m_fDepth = 0.f;
		m_fScale = 1.f;
	}

	cGlyphFontRender::~cGlyphFontRender()
	{
		SAFE_RELEASE(m_pGlyphReader,this);
		SAFE_DELETE(m_pFontImage);
		SAFE_DELETE(m_pvVertexBuffer);
		SAFE_DELETE(m_pvTextureUVBuffer);
		SAFE_DELETE(m_pvColorBuffer);
	}

	void	cGlyphFontRender::RenderFont(float e_fX,float e_fY,const wchar_t*e_pString)
	{
		RenderFont(e_fX,e_fY,e_pString,0);
	}

	void	cGlyphFontRender::RenderFont()
	{
		RenderFont(0,0,this->m_strText.c_str(),nullptr);
	}

	void	cGlyphFontRender::RenderFont(float e_fX,float e_fY,const wchar_t*e_pString,Vector4*e_pDrawRect)
	{
		int	l_iLen = (int)wcslen(e_pString);
		if( l_iLen == 0 )
			return;
		if( wcscmp(m_strText.c_str(),e_pString) )
		{
			m_strText = e_pString;
			if( l_iLen > m_iBufferLength )
			{
				l_iLen = m_iBufferLength;
			}
			float	l_fXOffset = 0.f;
			float	l_fYOffset = 0.f;
			float	l_fFonyHeight = 0.f;
			for( int i=0;i<l_iLen;++i )
			{
				if(!m_pGlyphReader->IsLegalCharacter(e_pString[i]))
					continue;
				FILE_GLYPH_ATTR	l_FILE_GLYPH_ATTR = m_pGlyphReader->GetCharInfo(e_pString[i]);
				l_fFonyHeight =	m_pGlyphReader->GetCharInfo(e_pString[i]).fHeight*m_fScale;
			}
			float	l_fMaxWidth = 0.f;
			float	l_fMaxHeight = l_fFonyHeight;
			int	l_iAliveIndex = 0;
			for(int i=0;i<l_iLen;++i)
			{
				float*l_pfVertexData = (float*)&m_pvVertexBuffer[l_iAliveIndex*6];
				float*l_pfTextData = (float*)&m_pvTextureUVBuffer[l_iAliveIndex*6];
				//unicode
				if( e_pString[i] == 13 && ((i+1<l_iLen)&&e_pString[i+1] == 10) )
				{
					continue;
				}
				//ansi
				if( e_pString[i] != L'\n' )
				{
					if(!m_pGlyphReader->IsLegalCharacter(e_pString[i]))
						continue;
					FILE_GLYPH_ATTR l_FILE_GLYPH_ATTR = this->m_pGlyphReader->GetCharInfo(e_pString[i]);
					float   l_fCharacterWidth = l_FILE_GLYPH_ATTR.fWidth*m_fScale;
					float   l_fCharacterHeight = l_FILE_GLYPH_ATTR.fHeight*m_fScale;
					//pos
					*l_pfVertexData = l_fXOffset;							++l_pfVertexData;
					*l_pfVertexData = l_fYOffset;							++l_pfVertexData;
					*l_pfVertexData = l_fXOffset+l_fCharacterWidth;			++l_pfVertexData;
					*l_pfVertexData = l_fYOffset;							++l_pfVertexData;
					*l_pfVertexData = l_fXOffset;							++l_pfVertexData;
					*l_pfVertexData = l_fYOffset+l_fCharacterHeight;		++l_pfVertexData;
				
					*l_pfVertexData = l_fXOffset;							++l_pfVertexData;
					*l_pfVertexData = l_fYOffset+l_fCharacterHeight;		++l_pfVertexData;
					*l_pfVertexData = l_fXOffset+l_fCharacterWidth;			++l_pfVertexData;
					*l_pfVertexData = l_fYOffset;							++l_pfVertexData;
					*l_pfVertexData = l_fXOffset+l_fCharacterWidth;			++l_pfVertexData;
					*l_pfVertexData = l_fYOffset+l_fCharacterHeight;

					//UV
					*l_pfTextData = l_FILE_GLYPH_ATTR.fLeft;				++l_pfTextData;
					*l_pfTextData = l_FILE_GLYPH_ATTR.fTop;					++l_pfTextData;
					*l_pfTextData = l_FILE_GLYPH_ATTR.fRight;				++l_pfTextData;
					*l_pfTextData = l_FILE_GLYPH_ATTR.fTop;					++l_pfTextData;
					*l_pfTextData = l_FILE_GLYPH_ATTR.fLeft;				++l_pfTextData;
					*l_pfTextData = l_FILE_GLYPH_ATTR.fBottom;				++l_pfTextData;

					*l_pfTextData = l_FILE_GLYPH_ATTR.fLeft;				++l_pfTextData;
					*l_pfTextData = l_FILE_GLYPH_ATTR.fBottom;				++l_pfTextData;
					*l_pfTextData = l_FILE_GLYPH_ATTR.fRight;				++l_pfTextData;
					*l_pfTextData = l_FILE_GLYPH_ATTR.fTop;					++l_pfTextData;
					*l_pfTextData = l_FILE_GLYPH_ATTR.fRight;				++l_pfTextData;
					*l_pfTextData = l_FILE_GLYPH_ATTR.fBottom;
				
					l_fXOffset += l_fCharacterWidth;
					if( l_fMaxWidth<l_fXOffset )
						l_fMaxWidth = l_fXOffset;
					++l_iAliveIndex;
				}
				else
				{
					//memset(l_pfVertexData,0,sizeof(float)*8);
					//memset(l_pfTextData,0,sizeof(float)*8);
					l_fXOffset = 0;
					l_fYOffset += l_fFonyHeight;
					l_fMaxHeight  += l_fFonyHeight;
				}
			}
			if( l_iAliveIndex == 0 )
				return;
			m_iDrawCount = l_iAliveIndex;
			if( e_pDrawRect )
			{
				e_pDrawRect->x = e_fX;
				e_pDrawRect->y = e_fY;
				e_pDrawRect->z = e_fX+l_fMaxWidth;
				e_pDrawRect->w = e_fY+l_fMaxHeight;
			}
			float	l_fHalfWidth = l_fMaxWidth;
			float	l_fHalfHeight = l_fMaxHeight;
			for(int i=0;i<l_iAliveIndex;++i)
			{
				float*l_pfVertexData = (float*)&m_pvVertexBuffer[i*6];
				*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;
				*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;
				*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;

				*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;
				*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;			
				*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight;
			}
			m_vHalfSize.x = l_fHalfWidth;
			m_vHalfSize.y = l_fHalfHeight;
		}
		UseShaderProgram(DEFAULT_SHADER);
		m_pFontImage->ApplyImage();
		cMatrix44	l_mat = cMatrix44::TranslationMatrix(Vector3(e_fX+m_vHalfSize.x,e_fY+m_vHalfSize.y,0.f));
		l_mat *= this->GetWorldTransform();
		FATMING_CORE::SetupShaderWorldMatrix(l_mat);
		myGlVertexPointer(2,m_pvVertexBuffer);
		myGlUVPointer(2,m_pvTextureUVBuffer);
		myGlColorPointer(4,m_pvColorBuffer);
		MY_GLDRAW_ARRAYS(GL_TRIANGLES, 0, 6*m_iDrawCount);
	}

	void	cGlyphFontRender::SetFontColor(Vector4 e_vColor)
	{
		for( int i=0;i<m_iBufferLength*4;++i )
			m_pvColorBuffer[i] = e_vColor;
	}

	Vector2		cGlyphFontRender::GetRenderSize(const wchar_t*e_strText)
	{
		int	l_iLen = (int)wcslen(e_strText);
		if( l_iLen == 0 )
			return Vector2::Zero;
		float	l_fXOffset = 0.f;
		float	l_fYOffset = 0.f;
		float	l_fFonyHeight = 0.f;
		for( int i=0;i<l_iLen;++i )
		{
			if(!m_pGlyphReader->IsLegalCharacter(e_strText[i]))
				continue;
			l_fFonyHeight =	m_pGlyphReader->GetCharInfo(e_strText[i]).fHeight*m_fScale;
		}
		float	l_fMaxWidth = 0.f;
		float	l_fMaxHeight = l_fFonyHeight;
		for(int i=0;i<l_iLen;++i)
		{
			if( e_strText[i] != L'\n' )
			{
				if(!m_pGlyphReader->IsLegalCharacter(e_strText[i]))
					continue;
				FILE_GLYPH_ATTR l_FILE_GLYPH_ATTR = this->m_pGlyphReader->GetCharInfo(e_strText[i]);
				float   l_fCharacterWidth = l_FILE_GLYPH_ATTR.fWidth*m_fScale;
				float   l_fCharacterHeight = l_FILE_GLYPH_ATTR.fHeight*m_fScale;
				l_fXOffset += l_fCharacterWidth;
				if( l_fMaxWidth<l_fXOffset )
					l_fMaxWidth = l_fXOffset;
			}
			else
			{
				l_fXOffset = 0;
				l_fYOffset += l_fFonyHeight;
				l_fMaxHeight  += l_fFonyHeight;
			}
		}
		return Vector2(l_fMaxWidth,l_fMaxHeight);
	}
}
#include "GlyphFontRender.h"
#include "FreetypeGlypth.h"
#include "../Texture/Texture.h"
#include "../../OpenGL/GLSL/Shader.h"
#include "../CommonRender/SimplePrimitive.h"
#include "../../../Common/Log/FMLog.h"
namespace FATMING_CORE
{

	TYPDE_DEFINE_MARCO(cGlyphFontRender);

	cGlyphFontRender::cGlyphFontRender(int e_iVertexBufferSize)
	{
		m_vHalfSize = Vector2::Zero;
		m_bTextChanged = true;
		m_iVertexBufferCount = e_iVertexBufferSize;
		m_vHalfSize = Vector2::Zero;
		m_pFontImage = 0;
		m_pGlyphReader = 0;
		m_fDepth = 0.f;
		m_fScale = 1.f;
		m_iDrawCount = 0;
		m_pvVertexBuffer = new Vector2[TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT*e_iVertexBufferSize];//one quad four vertex,a vertex 2 data(x,y)
		m_pvTextureUVBuffer = new Vector2[TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT*e_iVertexBufferSize];
		m_pvColorBuffer = new Vector4[TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT*e_iVertexBufferSize];
		for( int i=0;i<e_iVertexBufferSize*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT;++i )
			m_pvColorBuffer[i] = Vector4::One;
	}

	cGlyphFontRender::cGlyphFontRender(const char* e_strFontName,int e_iVertexBufferSize)
	{
		m_vHalfSize = Vector2::Zero;
		m_iDrawCount = 0;
		m_bTextChanged = true;
		m_iVertexBufferCount = e_iVertexBufferSize;
		m_pvVertexBuffer = 0;
		m_pvTextureUVBuffer = 0;
		m_pvColorBuffer = 0;
		char	l_strForStripExtensionName[TEMP_SIZE];
		sprintf(l_strForStripExtensionName,"%s\0",UT::GetFileNameWithoutFullPath(e_strFontName, false).c_str());
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
			int l_iShowErrorMsgType = FMLog::m_siShowErrorType;
			FMLog::m_siShowErrorType = 1;
			SAFE_DELETE(m_pFontImage);
			m_pGlyphReader = 0;
			UT::ErrorMsg(e_strFontName,"font image not exist");
			FMLog::m_siShowErrorType = l_iShowErrorMsgType;
			return;
		}
		m_pGlyphReader = new GlyphReader();
		m_pGlyphReader->AddRef(this);
		sprintf(l_str,"%s%s.FontInfo",l_strForDirectory,l_strForStripExtensionName);
		if(!m_pGlyphReader->LoadFontDataFile(l_str))
			UT::ErrorMsg("read font info failed",l_str);
		m_pvVertexBuffer = new Vector2[TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT*e_iVertexBufferSize];//one quad four vertex,a vertex 2 data(x,y)
		m_pvTextureUVBuffer = new Vector2[TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT*e_iVertexBufferSize];
		m_pvColorBuffer = new Vector4[TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT*e_iVertexBufferSize];
		for( int i=0;i<e_iVertexBufferSize*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT;++i )
			m_pvColorBuffer[i] = Vector4::One;
		m_fDepth = 0.f;
		m_fScale = 1.f;

	}

	cGlyphFontRender::cGlyphFontRender(cGlyphFontRender*e_pGlyphFontRender):cRenderObject(e_pGlyphFontRender)
	{
		m_bTextChanged = true;
		m_iVertexBufferCount = e_pGlyphFontRender->m_iVertexBufferCount;
		m_pFontImage = dynamic_cast<cBaseImage*>(e_pGlyphFontRender->m_pFontImage->Clone());
		m_pGlyphReader = e_pGlyphFontRender->m_pGlyphReader;
		m_pGlyphReader->AddRef(this);
		m_pvVertexBuffer = new Vector2[TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT*m_iVertexBufferCount];//one quad four vertex,a vertex 2 data(x,y)
		m_pvTextureUVBuffer = new Vector2[TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT*m_iVertexBufferCount];
		m_pvColorBuffer = new Vector4[TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT*m_iVertexBufferCount];
		for( int i=0;i<m_iVertexBufferCount*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT;++i )
			m_pvColorBuffer[i] = Vector4::One;
		m_fDepth = 0.f;
		m_fScale = 1.f;
        m_fLazyMinusCharacterWidth = e_pGlyphFontRender->m_fLazyMinusCharacterWidth;
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

	void	cGlyphFontRender::Render()
	{
		RenderFont(0,0,this->m_strText.c_str(),nullptr);
	}

	void	cGlyphFontRender::RenderFont(float e_fX,float e_fY,const wchar_t*e_pString,Vector4*e_pDrawRect)
	{
		int	l_iLen = (int)wcslen(e_pString);
		if( l_iLen == 0 )
			return;
		if( wcscmp(m_strText.c_str(),e_pString) || m_bTextChanged )
		{
			m_bTextChanged = false;
			m_strText = e_pString;
			if( l_iLen >= m_iVertexBufferCount)
			{
				l_iLen = m_iVertexBufferCount;
			}
			float	l_fXOffset = 0.f;
			float	l_fYOffset = 0.f;
			float	l_fFonyHeight = 0.f;
			for( int i=0;i<l_iLen;++i )
			{
				if(!m_pGlyphReader->IsLegalCharacter(e_pString[i]) )
					continue;
				//FILE_GLYPH_ATTR	l_FILE_GLYPH_ATTR = m_pGlyphReader->GetCharInfo(e_pString[i]);
				l_fFonyHeight =	m_pGlyphReader->GetCharInfo(e_pString[i]).fHeight*m_fScale;
				break;
			}
			float	l_fMaxWidth = 0.f;
			float	l_fMaxHeight = l_fFonyHeight;
			int	l_iAliveIndex = 0;
			for(int i=0;i<l_iLen;++i)
			{
				float*l_pfVertexData = (float*)&m_pvVertexBuffer[l_iAliveIndex*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
				float*l_pfTextData = (float*)&m_pvTextureUVBuffer[l_iAliveIndex*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
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
					//if (e_pString[i] == L' ')
					//{
					//	l_fXOffset += m_pDynamicFontTexture->m_iFontSize/2;
					//	continue;
					//}
					//else
					//if (e_pString[i] == L'\t')
					//{
					//	l_fXOffset += m_pDynamicFontTexture->m_iFontSize/2*3;
					//	continue;
					//}
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
				
					l_fXOffset += (l_fCharacterWidth*m_fLazyMinusCharacterWidth);
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
				float*l_pfVertexData = (float*)&m_pvVertexBuffer[i*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
				*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;
				*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;
				*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;

				*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;
				*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;			
				*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight;
			}
			m_vHalfSize.x = l_fHalfWidth;
			m_vHalfSize.y = l_fHalfHeight;
			auto l_pLocalBound = GetLocalBound();
			if (l_pLocalBound)
			{
				GenerateBound();
			}
		}
		UseShaderProgram(DEFAULT_SHADER);
		m_pFontImage->ApplyImage();
		cMatrix44	l_mat = cMatrix44::TranslationMatrix(Vector3(e_fX+m_vHalfSize.x,e_fY+m_vHalfSize.y,0.f));
		l_mat *= this->GetWorldTransform();
		RenderTrianglesWithMatrix((float*)m_pvVertexBuffer, (float*)m_pvTextureUVBuffer, (float*)m_pvColorBuffer, l_mat, 2, m_iDrawCount*ONE_QUAD_IS_TWO_TRIANGLES);
	}

	void	cGlyphFontRender::SetFontColor(Vector4 e_vColor)
	{
		if (m_pvColorBuffer[0] != e_vColor)
		{
			for (int i = 0; i < m_iVertexBufferCount * TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT; ++i)
				m_pvColorBuffer[i] = e_vColor;
		}
	}

	void cGlyphFontRender::AppendTextAndSetFontColor(const wchar_t * e_strText, Vector4 e_vColor)
	{
		//first find skip character
		int l_iSkipCount = 0;
		auto l_iLen = (int)this->m_strText.length();
		for (size_t i = 0; i < l_iLen; ++i)
		{
			auto l_wchar_t = m_strText[i];
			if (l_wchar_t == 13 && ((i + 1 < l_iLen) && m_strText[i + 1] == 10))
				++l_iSkipCount;
			else
			if (l_wchar_t == L' ' || l_wchar_t == L'\n' || l_wchar_t == '\t')
				++l_iSkipCount;
			else
			{
				if (!m_pGlyphReader->IsLegalCharacter(l_wchar_t))
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
			auto l_wchar_t = m_strText[i];
			if (l_wchar_t == 13 && ((i + 1 < l_iLen) && m_strText[i + 1] == 10))
				continue;
			else
			if (l_wchar_t == L' ' || l_wchar_t == L'\n' || l_wchar_t == '\t')
				continue;
			else
			{
				if (!m_pGlyphReader->IsLegalCharacter(l_wchar_t))
				{
					continue;
				}
			}
			for (int j = 0; j < TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT; ++j)
				m_pvColorBuffer[l_iIndex*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT+j] = e_vColor;
			++l_iIndex;
		}
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
				l_fXOffset += (l_fCharacterWidth*m_fLazyMinusCharacterWidth);
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

	std::wstring	cGlyphFontRender::GetText()
	{
		return this->m_strText.c_str();
	}

	void	cGlyphFontRender::SetText(const wchar_t*e_strText)
	{
		if (m_strText.compare(e_strText))
		{
			m_bTextChanged = true;
			this->m_strText = e_strText;
		}
	}
	float cGlyphFontRender::GetScale()
	{
		return m_fScale;
	}
	void cGlyphFontRender::SetScale(float e_fScale)
	{
		if (m_fScale != e_fScale)
		{
			m_fScale = e_fScale;
			this->m_bTextChanged = true;
		}
	}
	POINT cGlyphFontRender::GetSize()
	{
		if (this->m_bTextChanged)
		{
			m_vHalfSize = this->GetRenderSize(this->m_strText.c_str());
		}
		POINT l_Size = {(long)this->m_vHalfSize.x,(long)this->m_vHalfSize.y};
		return l_Size;
	}
	const cBound * cGlyphFontRender::GenerateBound()
	{
		RECT l_DrawSize = { 0,0,(long)m_vHalfSize.x,(long)m_vHalfSize.y };
		cBound l_Bound(l_DrawSize);
		this->SetLocalBound(&l_Bound);
		return this->GetLocalBound();
	}
}
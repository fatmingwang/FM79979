#ifndef Glyph_Font_Render
#define Glyph_Font_Render
#include "../Glyph/GlyphReader.h"
#include "BaseImage.h"
namespace FATMING_CORE
{
	//==========================
	//change color and angle by m_pFontImage->SetColor SetAngle;
	//==========================
	class cGlyphFontRender:public cRenderObject
	{
		GET_SET_DEC(GlyphReader*,m_pGlyphReader,GetGlyphReader,SetGlyphReader);
		GET_SET_DEC(float,m_fDepth,GetDepth,SetDepth);
		cBaseImage*		m_pFontImage;
		GET_SET_DEC(float,m_fScale,GetScale,SetScale);
		Vector2	*		m_pvVertexBuffer;
		Vector2	*		m_pvTextureUVBuffer;
		Vector4	*		m_pvColorBuffer;
		Vector2			m_vHalfSize;
		int				m_iDrawCount;
		int				m_iVertexBufferCount;
		//for how many words
		int				m_iBufferLength;
		std::wstring	m_strText;
		bool			m_bTextChanged;
	public:
		DEFINE_TYPE_INFO();
		cGlyphFontRender(const char* e_strFontName,int e_iVertexBufferSize = 300);
		cGlyphFontRender(int e_iVertexBufferSize = 300);
		cGlyphFontRender(cGlyphFontRender*e_pGlyphFontRender);
		CLONE_MYSELF(cGlyphFontRender);
		virtual ~cGlyphFontRender();
		cBaseImage* GetFontImage(){ return m_pFontImage; }
		//it should put all data at a block and call at once,not like now call by separate
		inline void		RenderFont(int e_iX,int e_iY,const wchar_t*e_pString){RenderFont((float)e_iX,(float)e_iY,e_pString);}
		void			RenderFont(float e_fX,float e_fY,const wchar_t*e_pString);
		void			RenderFont(float e_fX,float e_fY,std::wstring e_pString){ RenderFont(e_fX,e_fY,e_pString.c_str()); }
		void			RenderFont(float e_fX,float e_fY,const wchar_t*e_pString,Vector4*e_pDrawRect);
		inline void		RenderFont(Vector2 e_vPos,const wchar_t*e_pString){RenderFont(e_vPos.x,e_vPos.y,e_pString);}
		virtual void	Render();
		void			SetFontColor(Vector4 e_vColor);
		Vector2			GetRenderSize(const wchar_t*e_strText);
		std::wstring	GetText();
		void			SetText(const wchar_t*e_strText);
	};

//end namespace FATMING_CORE
}
#endif
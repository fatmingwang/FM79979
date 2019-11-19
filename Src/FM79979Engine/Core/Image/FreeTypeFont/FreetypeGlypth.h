#pragma once

namespace FATMING_CORE
{
	//==========================
	//
	//==========================
	class cFreetypeGlyphRender :public cRenderObject
	{
		class cDynamicFontTexture*	m_pDynamicFontTexture;
		Vector2	*					m_pvVertexBuffer;
		Vector2	*					m_pvTextureUVBuffer;
		Vector4	*					m_pvColorBuffer;
		Vector2						m_vHalfSize;
		int							m_iDrawCount;
		//for how many character
		int							m_iVertexBufferCount;
		std::wstring				m_strText;
		bool						m_bTextChanged;
		float						m_fScale;
	public:
		DEFINE_TYPE_INFO();
		cFreetypeGlyphRender(const char* e_strFontName, int e_iFontSize, int e_iVertexBufferSize = 300, POINT e_DefaultFontTextureSize = {512,512});
		cFreetypeGlyphRender(cFreetypeGlyphRender*e_pGlyphFontRender);
		CLONE_MYSELF(cFreetypeGlyphRender);
		virtual ~cFreetypeGlyphRender();
		//it should put all data at a block and call at once,not like now call by separate
		inline void		RenderFont(int e_iX, int e_iY, const wchar_t*e_pString) { RenderFont((float)e_iX, (float)e_iY, e_pString); }
		void			RenderFont(float e_fX, float e_fY, const wchar_t*e_pString);
		void			RenderFont(float e_fX, float e_fY, std::wstring e_pString) { RenderFont(e_fX, e_fY, e_pString.c_str()); }
		void			RenderFont(float e_fX, float e_fY, const wchar_t*e_pString, Vector4*e_pDrawRect);
		inline void		RenderFont(Vector2 e_vPos, const wchar_t*e_pString) { RenderFont(e_vPos.x, e_vPos.y, e_pString); }
		virtual void	Render()override;
		virtual	void	DebugRender()override;
		virtual	void	SetColor(Vector4 e_vColor)override { SetFontColor(e_vColor); }
		void			SetFontColor(Vector4 e_vColor);
		Vector2			GetRenderSize(const wchar_t*e_strText);
		float			GetScale();
		void			SetScale(float e_fScale);
		std::wstring	GetText();
		void			SetText(const wchar_t*e_strText);
		virtual const	cBound*	GenerateBound()override;
		virtual POINT	GetSize()override;

	};

	//end namespace FATMING_CORE
}
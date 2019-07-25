#ifndef FONT_FRAME_H
#define FONT_FRAME_H

#include "../AllImageInclude.h"
#include "Mouse_Touch.h"
namespace FATMING_CORE
{
	class	cFontWithFrame:public cRenderObject
	{
	protected:
		cGlyphFontRender*		m_pGlyphFontRender;
		cBaseImage*				m_pConnectRadianImage;
		cBaseImage*				m_pLineImage;
		Vector4					m_vFrameRange;
		std::wstring			m_strText;
		std::wstring*			m_pstrPressedText;
		const wchar_t*			m_pstrCurrentText;
		Vector4*				m_pvTextBGColor;
		Vector2					m_vRenderSize;
	public:
		DEFINE_TYPE_INFO()
		cFontWithFrame(cGlyphFontRender*e_pGlyphFontRender,const wchar_t* e_strText,cBaseImage*e_pConnectRadianImage = 0,cBaseImage*e_pLineImage = 0);
		cFontWithFrame(cFontWithFrame*e_pFontWithFrame);
		CLONE_MYSELF(cFontWithFrame);
		virtual ~cFontWithFrame();
		const wchar_t*			GetTxt(){ return m_strText.c_str(); }
		virtual void			SetText(const wchar_t*e_strText);
		void					SetPressedText(const wchar_t*e_strText);
		void					Render(int e_iX,int e_iY);
		virtual void			Init()override {}
		virtual void			Update(float e_fElpaseTime)override {}
		virtual	void			Render()override;
		virtual	void			Destroy()override {}
		//void			Render();
		Vector4*				GetTextBGColor(){ return m_pvTextBGColor; }
		void					SetTextBGColor(Vector4 e_vColor);
		cBaseImage*				GetConnectRadianImage(){return m_pConnectRadianImage;}
		cBaseImage*				GetLineImage(){return m_pLineImage;}
		cGlyphFontRender*		GetGlyphFontRender() { return m_pGlyphFontRender; }
		static cFontWithFrame*	GetMe(TiXmlElement*e_pTiXmlElement);
	};

	//sample
	//cFontWithFrame	l_FontWithFrame(cGameApp::m_spGlyphFontRender,L"cFontWithFrame Test");
	//Vector4	l_vBGColor(0,0,0,1);
	//l_FontWithFrame.SetTextBGColor(&l_vBGColor);
	//l_FontWithFrame.Render(500,500);

	//cTextButton just reference from cImageButton
	class	cTextButton:public cFontWithFrame,public cClickMouseBehavior
	{
		virtual	void			SetTransformInternalData()override { cRenderObject::SetTransformInternalData(); }
		UT::sTimeCounter		m_TCForShortUpdateDelay;
        virtual void			InternalMouseDown(int e_iPosX,int e_iPosY)override;
		virtual void			InternalMouseLeave(int e_iPosX,int e_iPosY)override;
        virtual void			InternalMouseUp(int e_iPosX,int e_iPosY)override;
		//
		UT::sTimeCounter		m_TCForRepeat;
		GET_SET_DEC(bool,m_bPressedRepeatMouseUp,IsPressedRepeatMouseUp,SetPressedRepeatMouseUp);
		Vector4					m_vPressedColor;
		virtual void			SetColor(Vector4 e_vColor)override;
		GET_SET_DEC(Vector4,m_vFontColor,GetFontColor,SetFontColor);
	public:
		DEFINE_TYPE_INFO()
		cTextButton(Vector2 e_vRenderPos,cGlyphFontRender*e_pGlyphFontRender,Vector4 e_vPressedColor,const wchar_t* e_strText,cBaseImage*e_pConnectRadianImage = 0,cBaseImage*e_pLineImage = 0);
		cTextButton(cTextButton*e_pTextButton);
		CLONE_MYSELF(cTextButton);
		virtual ~cTextButton();
		virtual void			SetText(const wchar_t*e_strText)override;
		virtual bool			IsSatisfiedCondition()override;
		virtual void			Init()override;
		virtual void			InitRepeat();
		virtual void			Update(float e_fElpaseTime)override;
		virtual	void			Render()override;
		virtual	void			Destroy()override;
		static	cTextButton*	GetMe(TiXmlElement*e_pElement);
		void					SetScale(float e_fScale);
		void					SetShortUpdateDelay(float e_fElpaseTime);
	};
	//sample
	//cTextButton	*g_pTextButton = 0;
	//SAFE_DELETE(g_pTextButton);
	//g_pTextButton = new cTextButton(Vector2(200,200),cGameApp::m_spGlyphFontRender,Vector4(0.5,0.5,0.5,1),L"cFontWithFrame Test");
	//g_pTextButton->Update(e_fElpaseTime);
	//Vector4	l_vBGColor(0,0.5,0.5,1);
	//g_pTextButton->SetTextBGColor(&l_vBGColor);
	//g_pTextButton->Render();
	//g_pTextButton->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	//g_pTextButton->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	//g_pTextButton->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);

//end namespace FATMING_CORE
}
#endif
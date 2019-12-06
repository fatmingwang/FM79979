#pragma once

#include "Mouse_Touch.h"
#include "../CommonRender/BaseImage.h"
#include "../PuzzleImage/PuzzleImage.h"
#include "../MPDI/MPDI.h"
namespace FATMING_CORE
{
	class	cExternalFunction:public NamedTypedObject
	{
	public:
		DoButtonGoal_Callback	m_pDoButtonGoal_Callback;
	};
	//cPuzzleImageUnit*l_pPuzzleImageUnit;
	//m_pCancelButton = new cImageButton(l_pPuzzleImageUnit,Vector3(0,0,0));
	//m_pCancelButton = new cImageButton(e_pMouse_TouchWithCancelButton->m_pCancelButton);
	//m_pCancelButton->SetName(CANCEL_BUTTON_NAME);
	//m_pCancelButton->Setm_pCancelButton(m_pDoButtonGoal_Callback);
	//
	//must call update !! or condition won't be satisfied.
	extern Vector4	g_vImageButtonClickedColor;
	class	cImageButton:public cClickMouseBehavior,public cBaseImage
	{
	protected:
		std::wstring				m_strButtonClickSoundName;
		virtual	void				SetTransformInternalData()override;
		//while mouse is ip set a little offset let player c it again to avoid change phase too fast
		UT::sTimeCounter			m_TC;
        virtual void				InternalMouseDown(int e_iPosX,int e_iPosY)override;
        virtual void				InternalMouseUp(int e_iPosX,int e_iPosY)override;
        virtual void				InternalMouseLeave(int e_iPosX,int e_iPosY)override;
		virtual	bool				InternalCollide(int e_iPosX,int e_iPosY)override;
		//virtual bool    InternalCollide(int e_iPosX,int e_iPosY);
		//if mouse pressed render this image
		cRenderObject*		m_pPressedImage;
		cRenderObject*		m_pFinishImage;
		bool						ProcessFinishAndPressedImage(TiXmlElement*e_pTiXmlelement);
		GET_SET_DEC(Vector4,m_vOriginalColor,GetOriginalColor,SetOriginalColor);
		GET_SET_DEC(Vector4,m_vPressedColor,GetPressedColor,SetPressedColor);
	public:
		DEFINE_TYPE_INFO()
		cImageButton();
		cImageButton(cPuzzleImageUnit*e_pImage,Vector3 e_vPos);
		cImageButton(cBaseImage*e_pImage,Vector3 e_vPos);
		cImageButton(cSubMPDI*e_pSubMPDI);
		//fetch first SubMPDI
		cImageButton(cMPDI*e_pMPDI);
		cImageButton(cImageButton*e_pImageButton);
		virtual bool				Collide(int e_iPosX, int e_iPosY)override { return cClickMouseBehavior::Collide(e_iPosX,e_iPosY); }
		static	cImageButton*		GetMe(TiXmlElement*e_pElement);
		CLONE_MYSELF(cImageButton);
		virtual ~cImageButton();
		virtual	void				SetEnable(bool e_b)override;
		virtual	void				SetColor(Vector4 e_vColor)override;
		virtual bool				IsSatisfiedCondition()override;
		virtual void				Init()override;
		virtual void				InitRepeat();
		virtual void				Update(float e_fElpaseTime)override;
		virtual	void				Render()override;
		virtual	void				DebugRender()override { cClickMouseBehavior::DebugRender(); }
		//virtual	void	SetPosByImageCenter(Vector3 e_vPos);
		cRenderObject*				GetPressedImage(){return m_pPressedImage;}
		void						SetPressedImage(cRenderObject*e_pImage,bool e_bLinkParent);
		cRenderObject*				GetFinishImage(){return m_pFinishImage;}
		void						SetFinishImage(cRenderObject*e_pImage,bool e_bLinkParent);
		//sound
		const wchar_t*				GetButtonClickSoundName(){ return m_strButtonClickSoundName.c_str(); }
		void						SetButtonClickSoundName(const wchar_t*e_strName){ m_strButtonClickSoundName = e_strName; }
	};

	enum	eYesNoDialogResult
	{
		eYNDR_YES = 0,
		eYNDR_NO,
		eYNDR_ORTHER,
		eYNDR_NONE,
	};
	//cButtonDialog*g_pYesNoDialog = 0;
	//cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(L"Image/Common.mpdi");
	//cMPDI*l_pMPDI = l_pMPDIList->GetObject(L"GoToStoreDialog");
	//g_pYesNoDialog = new cButtonDialog(l_pMPDI,L"Test");
	//g_pYesNoDialog->Init();
	//SAFE_DELETE(g_pYesNoDialog);
	//g_pYesNoDialog->Update(e_fElpaseTime);
	//g_pYesNoDialog->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	//g_pYesNoDialog->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	//g_pYesNoDialog->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	//g_pYesNoDialog->Render();
	class	cButtonDialog:public cClickMouseBehaviorVector<cClickMouseBehavior>
	{
	protected:
		//virtual	void			SetTransformInternalData(){ cImageButton::SetTransformInternalData(); }
		//Vector3									m_vPos;
		cClickMouseBehavior*						m_pYesImageButton;
		cClickMouseBehavior*						m_pNoImageButton;
		cRenderObject*								m_pBG;
		cRenderObject*								m_pFont;//if m_strDescription is empty m_pFont will be render if exists.
		Frame										m_FontPos;
		std::wstring								m_strDescription;
		GET_SET_DEC(eYesNoDialogResult,m_eYesNoDialogResult,GetYesNoDialogResult,SetYesNoDialogResult);
		void										BGStripButtonByName(const wchar_t*e_strName);
		bool										ProcessData(TiXmlElement*e_pElement);
		void										ProcessData(cMPDI*e_pMPDI,const wchar_t*e_strDescription,const wchar_t*e_strYesButton,const wchar_t*e_strNoButton,const wchar_t*e_strBG,bool e_bStripButtonForBG);
	public:
		DEFINE_TYPE_INFO()
		cButtonDialog();
		//e_bStripButtonForBG  if true will strip button and font,and the rest are BG animation
		cButtonDialog(cMPDI*e_pMPDI,const wchar_t*e_strDescription,const wchar_t*e_strYesButton = 0,const wchar_t*e_strNoButton = 0,const wchar_t*e_strBG = 0,bool e_bStripButtonForBG = false);
		cButtonDialog(cBaseImage*e_pYesImageButton,cBaseImage*e_pNoImageButton,cRenderObject*e_pBK,wchar_t*e_strDescription,Vector3 e_vYesPos,Vector3 e_vNoPos,Vector3 e_vBKPos,Vector3 e_vFontPos);
		cButtonDialog(cButtonDialog*e_pYesNoDialog);
		CLONE_MYSELF(cButtonDialog);
		virtual ~cButtonDialog();
		//
		void					SetPos();
		virtual void			Init()override;
		virtual void			Update(float e_fElpaseTime)override;
		virtual	void			Render()override;
		virtual	void			Destroy()override;
        //virtual void			MouseMove(int e_iPosX,int e_iPosY);
        ////first time into
        //virtual void			MouseDown(int e_iPosX,int e_iPosY);
        ////
        //virtual void			MouseUp(int e_iPosX,int e_iPosY);
		//
		//virtual	bool			IsSatisfiedCondition(){ return m_eYesNoDialogResult != eYNDR_NONE; }
		eYesNoDialogResult		GetResult(){ return m_eYesNoDialogResult; }

		cClickMouseBehavior*	GetYesImageButton(){return m_pYesImageButton;}
		cClickMouseBehavior*	GetNoImageButton(){return m_pNoImageButton;}
		cRenderObject*	GetBG(){return m_pBG;}
		cRenderObject*	GetFont(){return m_pFont;}
		std::wstring			GetDescription(){return m_strDescription;}
		Frame					GetFontPos(){return m_FontPos;}
		static	cButtonDialog*	GetMe(TiXmlElement*e_pElement);
		static	cButtonDialog*	GetMe(const char*e_strFileName);
	};

	cClickMouseBehaviorVector<cImageButton>*	GetClickMouseBehaviorList(TiXmlElement*e_pElement);
//end namespace namespace FATMING_CORE
}
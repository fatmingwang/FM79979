#pragma once

#include "MPDIToGameObject.h"
namespace FATMING_CORE
{
	struct sSmoothSlide
	{
		//if mouse down and up time gap smaller than this do a smooth slide
		static  float		m_sfSmoothSlideTC;
		static  float		m_sfMinimumDistanceToDoSmoothSlide;
		static float		sfMaxTime;
		UT::sTimeCounter	TC;
		Vector3				vTargetPos;
		Vector3				vStartPos;
		Vector3				vGapDistance;
		Vector3				vCurrentPos;
		cCurveWithTime		m_LazySmoothCurve;
		sSmoothSlide();
		~sSmoothSlide();
		void				Init(Vector3 e_vStartPos, Vector3 e_vMoveDistance, float e_fMouseDownUpTime);
		void				Update(float e_fElpaseTime);
		bool				IsDone();
	};

	class   cScrollBox :public cMPDIToGameObject
	{
		std::vector<std::function<void(Vector2)> >	m_ScorllMovingCallbackVecor;
		friend class cScrollerBar;
	public:
		struct sDataForScrollBox
		{
			cClickBehavior*			m_pSelectedObject;
			float					m_fMouseDownUpCounter;
			POINT					m_MouseDownPosition;
			POINT					m_MouseMovePosition;
			Vector3					m_vMouseUpRenderObjectPos;
			UT::sTimeCounter		m_RollBackTC;
			Vector3					m_vRollbackForProperPos;
			//for all object's area
			Vector2					m_vScrollerSizeOfAllObject;
			Vector4					m_vLocalViewRect;
			//while image is over the total rect and mouse is up,make it roll back
			bool					m_bRollBackToProperPosition;
			//while objects are too much player can do a long slide then all objects move smoothly.
			bool					m_bDoSmoothSlide;
			int						m_iColumnCountForMultiArrange;
			sDataForScrollBox();
			~sDataForScrollBox();
			static float			m_sfRollbackTC;
		};
	private:
		//
		//avoid to call this.
		virtual bool				SetupMPDIAndButtonNameVector(cMPDI*e_pMPDI, std::vector<std::wstring>e_strButtonName, bool e_bKeepMPDIAsBG)override { return true; }
		virtual void				AddRenderObject(cRenderObject*e_pRenderObject)override;
	protected:
		sSmoothSlide				m_SmoothSlide;
		//under m_pRenderObject
		cRenderObject*				m_pObjectsMovingRoot;
		class cScissorRenderObject*	m_pScissorRenderObject;
		sDataForScrollBox			m_DataForScrollBox;
		//the direction for move
		eOrientation				m_eOrientation;
		int							m_iLeftTopObjectIndex;
		GET_SET_DEC(Vector2,		m_vNextObjectGap, GetNextObjectGap, SetNextObjectGap);
		GET_SET_DEC(bool,			m_bArrangeWithMultiLine, IsArrangeWithMultiLine, SetArrangeWithMultiLine);
		GET_SET_DEC(Vector2,		m_vRenderObjectOffsetPos, GetRenderObjectOffsetPos, SetRenderObjectOffsetPos);
		//if hit the boundry stop smooth slide
		bool						IsHitBoundary(eOrientation e_eOrientation, Vector3 e_vPos, Vector3 &e_vOverdDistance);
		bool						DoSmoothSlideCheck();
		void						DoSmoothSlideUpdate(float e_fElpaseTime);
		void						DoRollBackToProperPositionUpdate(float e_fElpaseTime);
		void						ObjectUpdateVisible();
		void						InternalMouseMoveUp(int e_iPosX, int e_iPosY, bool e_bMouseMove);
		bool						GetObjectPosByBisibleIndex(int e_iIndex, Vector3&e_vPos);
	public:
		DEFINE_TYPE_INFO()
		cScrollBox();
		virtual ~cScrollBox();
		//set data to correct
		virtual void				Init()override;
		virtual cClickBehavior*		AddRenderObjectandGenerateClickBehavior(cRenderObject * e_pRenderObject, bool e_bEnableClickScale = true)override;
		virtual void				ArrangeObjectPos(Vector2 e_vStartOffsetPos, Vector2 e_vObjectGap, bool e_bMultiLines);
		virtual	void				Update(float e_fElpaseTime)override;
		//please input clone one,because e_pBGMPDI will be delete.
		bool						AddBG(cMPDI*e_pBGMPDI);
		//also bind collide function
		//this for local position,will change by SetScrollerPosition
		void						SetScrollerPositionAndViewRect(Vector3 e_vPos, Vector4 e_vLocalViewRect);
		eOrientation				GetOrientation() { return m_eOrientation; }
		void						SetOrientation(eOrientation e_eOrientation);
		bool						ScrollMoveToTargetItemIndex(int e_iTargetIndex, bool e_bDoSmooth);
		bool						ScrollMoveToNextItem(bool e_bDoSmooth);
		bool						ScrollMoveToPrevioudItem(bool e_bDoSmooth);
		bool						MiltilineScrollMove(int e_iRowMoveIndex, int e_iColumnMoveIndex, bool e_bDoSmooth);
		bool						CreateScrollBar(const wchar_t*e_strPIFileName, eOrientation e_eOrientation);
		//e_fXPercentage for eO_HORIZONTAL,e_fYPercentage for eO_VERTICAL
		bool						MoveScrollByScrollBarPercent(float e_fXPercentage, float e_fYPercentage);
		int							GetLeftTopObjectIndex() { return m_iLeftTopObjectIndex; }
		bool						IsRollBackToProperPosition() { return m_DataForScrollBox.m_bRollBackToProperPosition; }
		bool						IsDoSmoothSlide() { return m_DataForScrollBox.m_bDoSmoothSlide; }
		sDataForScrollBox*			GetDataForScrollBox() { return &m_DataForScrollBox; }
		//
		Vector2						GetPercentageByCurrentRootRenderPosOfScrollerSizeOfAllObject();
		void						SetScrollMovingCallback(std::function<void(Vector2)>e_Function) { m_ScorllMovingCallbackVecor.push_back(e_Function); }
		void						SetScissorRectColor(bool e_bRender, Vector4 e_vColor);
		//
		virtual cClickBehavior*		MouseDown(int e_iPosX, int e_iPosY)override;
		virtual cClickBehavior*   	MouseMove(int e_iPosX, int e_iPosY)override;
		virtual cClickBehavior*    	MouseUp(int e_iPosX, int e_iPosY)override;
		//static	cScrollBox*			GetMe(TiXmlElement*e_pElement);
	};
//end namespace FATMING_CORE
}

//1.
//cClickBehaviorDispatcher	m_ClickBehaviorDispatcher;
//cMPDIToGameObject*			m_pMPDIToGameObject;
//cScrollBox*m_pScrollBox;
//2.
//m_pScrollBox = new cScrollBox();
//m_pMPDIToGameObject = new cMPDIToGameObject();
//3.
//m_pScrollBox->SetOrientation(eOrientation::eO_BOTH);
//m_pScrollBox->SetArrangeWithMultiLine(true);
//cMPDI*l_pBG = new cMPDI(l_pBGMPDI);
//m_pScrollBox->AddBG(l_pBG);
//auto l_pClickFunction = [](int e_iPosX, int e_iPosY, cClickBehavior*e_pObject)
//{
//	std::wstring l_str = e_pObject->GetName();
//	l_str += L" Collided";
//	FMLog::Log(l_str.c_str(), false);
//};
//for (int i = 0; i < 150; i++)
//{
//	cGlyphFontRender*l_pGlyphFontRender = new cGlyphFontRender(g_pCarGoApp->m_pFont);
//	int	l_iID = l_iCount - i;
//	std::wstring	l_strID;
//	wchar_t l_strPadding0[250];
//	wsprintf(l_strPadding0, L"%03d", l_iID);
//	l_strID += l_strPadding0;
//	l_strID += L":";
//	l_pGlyphFontRender->SetText(l_strID.c_str());
//	auto l_pClickObject = m_pScrollBox->AddRenderObjectandGenerateClickBehavior(l_pGlyphFontRender);
//	l_pClickObject->SetName(ValueToStringW(l_strID).c_str());
//	l_pClickObject->SetMouseUpFunction(l_pClickFunction);
//}
//4.
//if (m_pMPDIToGameObject)
//{
//	m_pMPDIToGameObject->CreateRenderObject();
//	m_pMPDIToGameObject->AddObject(m_pScrollBox);
//	m_pMPDIToGameObject->GetRenderObject()->SetLocalPosition(Vector3(100, 100, 0));
//	m_pScrollBox->SetNextObjectGap(Vector2(30.f, 30.f));
//	m_pScrollBox->SetScrollerPositionAndViewRect(Vector2(0, 0), Vector4(0, 0, 1200, 700));;
//	auto l_pHorizontalScrollBar = cScrollBar::GetMe(m_pScrollBox, eOrientation::eO_HORIZONTAL, L"CarDrivingServer/ScrollPI.pi");
//	auto l_pVerticalScrollBar = cScrollBar::GetMe(m_pScrollBox, eOrientation::eO_VERTICAL, L"CarDrivingServer/ScrollPI.pi");
//	m_pMPDIToGameObject->AddObject(l_pHorizontalScrollBar);
//	m_pMPDIToGameObject->AddObject(l_pVerticalScrollBar);
//}
//	m_ClickBehaviorDispatcher.AddObject(m_pMPDIToGameObject);
//m_ClickBehaviorDispatcher.Init();
//for m_ClickBehaviorDispatcher do not call update and render
//5.
//void cTestPhase::Update(float e_fElpaseTime)
//{
//	if (m_pMPDIToGameObject)
//	{
//		m_pMPDIToGameObject->Update(e_fElpaseTime);
//	}
//}
//
//void cTestPhase::Render()
//{
//	if (m_pMPDIToGameObject)
//		m_pMPDIToGameObject->Render();
//}
//
//void cTestPhase::InternalMouseUp(int e_iPosX, int e_iPosY)
//{
//	m_ClickBehaviorDispatcher.MouseUp(e_iPosX, e_iPosY);
//	this->m_eObjectMouseBehavior = eOMB_NONE;
//}
//
//void cTestPhase::InternalMouseMove(int e_iPosX, int e_iPosY)
//{
//	m_ClickBehaviorDispatcher.MouseMove(e_iPosX, e_iPosY);
//}
//
//void cTestPhase::InternalMouseDown(int e_iPosX, int e_iPosY)
//{
//	m_ClickBehaviorDispatcher.MouseDown(e_iPosX, e_iPosY);
//}

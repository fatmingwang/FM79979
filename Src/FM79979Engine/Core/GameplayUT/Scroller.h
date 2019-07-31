#ifndef _SCROLLER_H_
#define _SCROLLER_H_
#include "Mouse_Touch.h"
#include "Tabpage.h"
namespace FATMING_CORE
{
	//cScroller* g_pScrollerTest = 0;
	//g_pScrollerTest = new cScroller(cGameApp::m_pImageParser->GetPuzzleImage(0),Vector4(200,100,800,600),false);
	//m_pScroller = new cScroller(&l_ImageList, l_vSCrollerViewRect);
	//m_pScroller->SetObjectGap(l_pObjectData->Size.x);
	//auto l_uiSize = l_DeliverPointNodeIDVector.size();
	//Vector3 l_vObjectStartPosRenderPos = l_pObjectSubMPDI->GetPositionByTime(0.f);
	//Vector3 l_vRenderPos = Vector2(0, 0);
	//for (size_t i = 0; i < l_uiSize; i++)
	//{
	//	//cTextButton(Vector2 e_vRenderPos, cGlyphFontRender*e_pGlyphFontRender, Vector4 e_vPressedColor, const wchar_t* e_strText, cBaseImage*e_pConnectRadianImage = 0, cBaseImage*e_pLineImage = 0);
	//	cTextButton*l_pTextButton = new cTextButton(Vector2(l_vRenderPos.x, l_vRenderPos.y), cGameApp::m_spGlyphFontRender, Vector4::HalfBright, L"0");
	//	l_pTextButton->GetGlyphFontRender()->SetScale(3.f);
	//	l_pTextButton->SetText(ValueToStringW(l_DeliverPointNodeIDVector[i]).c_str());
	//	m_pScroller->AddObjectNeglectExist(l_pTextButton);
	//	l_vRenderPos.x += l_pObjectData->Size.x;
	//	if (l_vRenderPos.x >= l_vSCrollerViewRect.Width())
	//	{
	//		l_vRenderPos.x = 0;
	//		l_vRenderPos.y += l_pObjectData->Size.y;
	//	}
	//}
	//SAFE_DELETE(g_pScrollerTest);
	//g_pScrollerTest->Update(e_fElpaseTime);
	//g_pScrollerTest->Render();
	//g_pScrollerTest->DebugRender();
	//g_pScrollerTest->MouseDown(e_iPosX,e_iPosY);
	//g_pScrollerTest->MouseMove(e_iPosX,e_iPosY);
	//g_pScrollerTest->MouseUp(e_iPosX,e_iPosY);
	//very similare cScrollingMSOGroup,but Scroller only scrool image without MPDI setup.
    class   cScroller:public cTabPage
    {
		bool	CollideObjectIndex(cClickMouseBehavior*e_pImageButton,int e_iPosX,int e_iPosY);
	protected:
		//
		GET_SET_DEC(float,m_fObjectGap,GetObjectGap,SetObjectGap);
		//
		float	m_fTotalLength;
		Vector3	m_vAddedDis;
		bool	IsScrollable();
		//the direction for move
		eOrientation	m_eOrientation;
		//bool	m_bDirectionIsHorizontal;
		//while image is over the total rect and mouse is up,make it roll back
		bool	m_bRollBack;
		UT::sTimeCounter	m_RollBackTC;
		Vector3	m_vDisToRightPos;
		int		m_iTopViewableIndex;
        //if mouse up and the position is not similar,it's mean the mouse move from the other,so ignore it
        //virtual void    InternalMouseMove(int e_iPosX,int e_iPosY);
        //virtual void    InternalMouseDown(int e_iPosX,int e_iPosY);
        //virtual void    InternalMouseUp(int e_iPosX,int e_iPosY);
        ////
        //virtual bool    InternalCollide(int e_iPosX,int e_iPosY);
    public :
		DEFINE_TYPE_INFO()
        //if e_pPI is nullptr,uadd images by yourself or add pi later
		cScroller();
        cScroller(cPuzzleImage*e_pPI,Vector4 e_vTotalViewRect,cClickMouseBehavior*e_pCancelButton = nullptr,cPuzzleImageUnit*e_pIcon = nullptr,bool e_bHorizontal = true,Vector3* e_pvIconPos = nullptr);
		cScroller(cPuzzleImage*e_pPI,Vector4 e_vTotalViewRect,cClickMouseBehavior*e_pCancelButton,cClickMouseBehavior*e_pIcon,bool e_bHorizontal);
		cScroller(std::vector<cBaseImage*>*e_pImageList,Vector4 e_vTotalViewRect,cClickMouseBehavior*e_pCancelButton = nullptr,bool e_bHorizontal = true,cPuzzleImageUnit*e_pIcon = nullptr,Vector3* e_pvIconPos = nullptr);
        virtual ~cScroller();
        //set data to correct
        virtual void			Init()override;
		virtual void			ArrangeObjectInitWithMultiLines(Vector2 e_vStartOffsetPos,Vector2 e_vObjectGap);
        virtual	void			Update(float e_fElpaseTime)override;
        virtual	void			Render()override;
        virtual	void			DebugRender()override;
		//
        virtual void			MouseMove(int e_iPosX,int e_iPosY)override;
        virtual void			MouseDown(int e_iPosX,int e_iPosY)override;
        virtual void			MouseUp(int e_iPosX,int e_iPosY)override;
		static	cScroller*		GetMe(TiXmlElement*e_pElement);
		//
		//static cScroller*GetMe(TiXmlElement*e_pTiXmlElement);
	};
//end namespace
};
#endif
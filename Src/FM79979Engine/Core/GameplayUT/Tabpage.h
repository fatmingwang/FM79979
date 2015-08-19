#ifndef _TAB_PAGE_H_
#define _TAB_PAGE_H_
#include "Mouse_TouchWithCancelButton.h"
#include "../Image/PuzzleImage.h"
#include "ImageButton.h"
namespace FATMING_CORE
{
	//
	//advance useage look at ImageWithSlider.h
	//
	//
	//
	//ensure reset imagebutton's position at Init function
	class	cTabPage:public cMouse_TouchWithCancelButton
	{
	protected:
		//for category icon
		//not necessary but if it's a tabpage's children
		cClickMouseBehavior*		m_pIcon;
	public:
		DEFINE_TYPE_INFO();
		cTabPage();
		cTabPage(cPuzzleImage*e_pPI,Vector4 e_vWorkingZone,cClickMouseBehavior*e_pCancelButton = 0,cBaseImage*e_pIcon = 0,Vector3 *e_pvIconPos = 0,bool e_bForceSetCancelButtonPos = false);
		cTabPage(cPuzzleImage*e_pPI,Vector4 e_vWorkingZone,cClickMouseBehavior*e_pCancelButton = 0,cClickMouseBehavior*e_pIcon = 0,bool e_bForceSetCancelButtonPos = false);
		cTabPage(cTabPage*e_pTabPage);
		CLONE_MYSELF(cTabPage);
		virtual ~cTabPage();
		void						SetIconColor(Vector4 e_vColor);
		cClickMouseBehavior*		GetTabPageIcon(){return m_pIcon;}
		void						RenderIcon();
		bool						IsCollideIcon(int e_iPosX,int e_iPosY);
		virtual	void				SetTabPageIcon(cClickMouseBehavior*e_pIcon);
		//add one by one,it might be orther things nit only a image,ensure input data is a new one because
		//it will be delete by Clear function
		//e_pClickMouseBehavior might be cImageButton
		//virtual	bool				AddObject(cClickMouseBehavior*e_pClickMouseBehavior);
		virtual	bool				AddObject(cPuzzleImage*e_pPuzzleImage,bool e_bClearOldData = false);
		virtual	bool				AddObject(std::vector<cBaseImage*>*e_pImageList,bool e_bClearOldData = false);
		//notice!input data might be delete if e_bDeleteData is true
		virtual	bool				AddObject(cBaseImage*e_pBaseImage,bool e_bDeleteData = true);
		virtual	bool				AddObject(cSubMPDI*e_pSubMPDI,bool e_bDeleteData = true);
		virtual	bool				AddObject(cImageButton*e_pButton,bool e_bDeleteData = true);
		virtual	bool				AddObject(NamedTypedObject*e_pNamedTypedObject,bool e_bDeleteData = true);
		virtual	bool				AddObject(cClickMouseBehavior*e_pNamedTypedObject,bool e_bDeleteData = true);
		static	cTabPage*			GetMe(TiXmlElement*e_pElement);
	};
	//================
	//this class is a cTabPage list controller
	//but it's allow add the object is not TabPage
	//================
	class	cTabPageControl:public cMouse_TouchWithCancelButton//public cClickMouseBehaviorVector<cTabPage>
	{
		cClickMouseBehavior*m_pCurrentPage;
	public:
		//if e_pBG is not null,after this is delete e_pBG will delete too.
		cTabPageControl(Vector4 e_vWorkingZone,cImageButton*e_pCancelButton = 0,cCueToStartCurveWithTime*e_pBG = 0,bool e_bForceSetCancelButtonPos = false);
		virtual ~cTabPageControl();
		//
		virtual void    MouseDown(int e_iPosX,int e_iPosY);
		virtual void    MouseUp(int e_iPosX,int e_iPosY);
		//
		virtual void    Init();
		virtual	void    Update(float e_fElpaseTime);
		virtual	void    Render();
		//not necessary,but we could quickly to setup focus data
		virtual	void	SetFocusItem(int e_iTabPageIndex,cPuzzleImageUnit*e_pPIUnit){}
		//
		virtual	void	SetWorkingTabPage(int e_iTabPageIndex);
		int				GetWorkingTabPageIndex();
		virtual	NamedTypedObject*	GetSelectedObject();
	};


	//cMPDIList*e_pMPDIList;
	//cMouse_TouchWithCancelButton*m_pSubMenu = ParseFileToSliderBar(e_strSubMenuFileName,IN_HHOUSE_SUB_MEUN_MONEY_PI_FILE_NAME,&e_pMPDIList);
	//cMPDI*l_pBG = e_pMPDIList->GetObject(L"BG");
	//cSubMPDI*l_pSubMPDI = l_pBG->GetObject(L"EarnMoneyBtn");
	//Vector3	l_vPos = l_pSubMPDI->GetOriginalPointList()[0];
	//l_vPos -= l_pSubMPDI->GetPointData(0)->Size/2.f;
	//cImageButton*l_pEarnMoneyButton = new cImageButton(l_pSubMPDI->GetPointData(0)->GetPuzzleImageUnit(),l_vPos);
	//l_pEarnMoneyButton->SetDoButtonGoal(EarnMoneyMsg);
	//m_pSubMenu->m_MiscButtonEventList.AddObjectNeglectExist(l_pEarnMoneyButton);
	//m_pSubMenu->Init();
	//m_pSubMenu->MouseMove(e_iPosX,e_iPosY);
	//m_pSubMenu->MouseMove(e_iPosX,e_iPosY);
	//m_pSubMenu->MouseMove(e_iPosX,e_iPosY);
	//m_pSubMenu->MouseMove(e_iPosX,e_iPosY);
	//bg frame,first object position
	//wht do not just return a Scroller object is because,I am lazy to fix scroller's object potision.(object's show position is decided by total view rect.)
	cTabPageControl*	ParseFileToSliderBar(char*e_strMPDIFile,char*e_strPIFileName,cMPDIList**e_pMPDIList = 0);
	cTabPageControl*	ParseFileToSliderBar(cMPDI*e_pMPDI,cPuzzleImage*e_pPI,cMPDIList**e_pMPDIList = 0);
	//bg frame,first object position
	//all category pi file.
	cTabPageControl*	ParseFileToSliderBarWithTabPagesController(char*e_strMPDIFile,char*e_strIconPIFileName);
	//parse icon pi and sub icon pi file by self.
	cTabPageControl*	ParseFileToSliderBarWithTabPagesController(cMPDI*e_pMPDI,cPuzzleImage*e_pIconPI,std::vector<cPuzzleImage*>*e_pPageIcon);
	//bg frame,arrow,and pages item place
	cTabPageControl*	ParseFileToMultiPages(char*e_strMPDIFile,char*e_strPIFileName);
	cTabPageControl*	ParseFileToMultiPages(cMPDI*e_pMPDI,cPuzzleImage*e_pPI);
	//bg frame,arrow,and pages item place
	//all category pi file.
	cTabPageControl*	ParseFileToMultiPagesWithTabPagesController(char*e_strMPDIFile,char*e_strIconPIFileName);
	//parse icon pi and sub icon pi file by self.
	cTabPageControl*	ParseFileToMultiPagesWithTabPagesController(cMPDIList*e_pMPDIList,cPuzzleImage*e_pIconPI,std::vector<cPuzzleImage*>*e_pPageIcon);
//end namespace FATMING_CORE
}
#endif
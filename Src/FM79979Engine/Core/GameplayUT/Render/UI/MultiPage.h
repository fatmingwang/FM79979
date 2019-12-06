#pragma once
#include "Tabpage.h"

namespace FATMING_CORE
{
	//it has a arrow to flip pages,whil turn pages is called new items will appear
	//the Objects of cClickMouseBehaviorVector will change while page index is changed.
	//m_Objects store all image but
	//index order 
	//0 1 2 3
	//4 5 6 7
	class	cMultiPages:public cTabPage
	{
		cImageButton*	m_pPriorPageImageButton;
		cImageButton*	m_pNextPageImageButton;
		cNamedTypedObjectVector<cImageButton>	m_ImageButtonObjects;
		int	m_iTotalPages;
		//get from construction images putton
		//Vector4	m_vWorkingRect;
		//images object count
		int	m_iColumn,m_iRow;
		//cuent page index
		int	m_iCurentPageIndex;
	public:
		cMultiPages(cPuzzleImageUnit*e_pIcon,cPuzzleImage*e_pPI,
			cPuzzleImageUnit*e_pArrowButtonImage,Vector3 e_vNextPageButtonPos,Vector3 e_vPriorPageButtonPos,
			Vector3 e_vStartPos,Vector2 e_vObjectDistance,
					int e_iRowCount,int e_iColumnCount,Vector4 e_vWorkingZone,cImageButton*e_pCancelButton = 0);
		virtual ~cMultiPages();
		virtual void    Init();
		virtual void    Update(float e_fElpaseTime);
		bool	TurnToPage(int e_iPagse);
		bool	NextPage();
		bool	PriorPage();
	};
	//Vector4	l_vTabPageCollidedRect = Vector4(0,0,480,200);
	//m_pTabPageControl = new cTabPageControl(l_vTabPageCollidedRect);
	//cPuzzleImageUnit*l_pPIUnit = m_pUIAnimationMPDIList->GetPuzzleImage()->GetObject(L"Arrow");
	//for( int i=0;i<4;++i )
	//{
	//	cPuzzleImageUnit*l_pPuzzleImageUnit = m_pUIAnimationMPDIList->GetPuzzleImage()->GetObject(i);
	//	l_pPuzzleImageUnit->SetPos(Vector3(i*100.f,0.f,0.f));
	//	cMultiPages*l_pMultiPages = new cMultiPages(l_pPuzzleImageUnit,
	//		m_pUIAnimationMPDIList->GetPuzzleImage(),
	//		l_pPIUnit,Vector3(400,100,0),Vector3(0,100,0),Vector3(50,50,0),Vector2(100,100),2,4,l_vTabPageCollidedRect);
	//	m_pTabPageControl->AddObjectNeglectExist(l_pMultiPages);
	//}
//end namespace FATMING_CORE
}
#pragma once
#include "Scroller.h"
namespace FATMING_CORE
{
	class	cMultiPathDynamicImage;
	//a image with a slider could let player to know the category what this is
	//
	//basicly it's a demo to show u how to add extra info to show on the screen
	class	cImageSlider:public cScroller
	{
	public:
		cImageSlider(cPuzzleImageUnit*e_pIcon,cPuzzleImage*e_pPuzzleImage,Vector4 e_vTotalViewRect,Vector3 e_vIconPos,cImageButton*e_pCancelButton = 0,bool e_bHorizontal = true);
		virtual ~cImageSlider();
        virtual	void    Render();
		//virtual bool    IsSatisfiedCondition();
	};
	//=====================
	//
	//=====================
	//cMultiPathDynamicImage*l_pSliderBarBK = m_pUIAnimationMPDIList->GetObject(L"SliderBarBK");
	//l_pSliderBarBK->Start();
	//l_pSliderBarBK->Update(0.0001f);
	//cMultiPathDynamicImage*l_pMultiPathDynamicImage = m_pUIAnimationMPDIList->GetObject(L"IconPosition");
	//std::vector<Vector3>	l_AllIconPosition = l_pMultiPathDynamicImage->GetObject(0)->GetOriginalPointList();
	//size_t	l_iCount = l_AllIconPosition.size();
	//if( l_iCount )
	//{
	//	//InHouseDecoration.xml
	//	//Vector2	l_vStartPos(15,60);
	//	char*l_strAllItemsName[5] = {"NL1/InHouse/Characters/Characters.pi",
	//									"NL1/InHouse/Floor/Floor.pi",
	//									"NL1/InHouse/Props/Props.pi",
	//									"NL1/InHouse/Props/no.pi",
	//									"NL1/InHouse/Props/no.pi"};
	//	wchar_t*l_strAllPIName[5] = {		L"Characters",
	//									L"Floor",
	//									L"Props",
	//									L"no",
	//									L"no"};
	//	Vector3	l_vFirstIconImagePos = l_AllIconPosition[0];
	//	cPuzzleImageUnit*l_pPIUnit = 0;
	//	sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = l_pSliderBarBK->GetObject(0)->GetPointData(0);
	//	Vector4	l_vScrollerRect(l_vFirstIconImagePos.x,l_vFirstIconImagePos.y,
	//		l_vFirstIconImagePos.x+l_pTexBehaviorDataWithImageIndexData->Size.x,
	//		l_vFirstIconImagePos.y+l_pTexBehaviorDataWithImageIndexData->Size.y);
	//	cImageButton*l_pImageButton = new cImageButton(l_pTexBehaviorDataWithImageIndexData->pPI->GetObject(0),Vector3(200,200,0));
	//	m_pBuildItemTabePage = new cTabPageControl(l_vScrollerRect,l_pSliderBarBK,l_pImageButton);
	//	for(size_t	i = 0;i<l_iCount;++i)
	//	{//here should get image by specific image name from its own MPDI File.
	//		cPuzzleImage*l_pPI = cGameApp::m_spAnimationParser->GetAllBaseImageList()->GetPuzzleImage(l_strAllPIName[i]);
	//		if( !l_pPI )
	//		{
	//			bool	l_b = cGameApp::m_spAnimationParser->GetAllBaseImageList()->Parse(l_strAllItemsName[i]);
	//			if( !l_b )
	//				continue;
	//			assert(l_b);
	//			l_pPI = cGameApp::m_spAnimationParser->GetAllBaseImageList()->GetPuzzleImage(l_strAllPIName[i]);
	//			m_AllDecorationPI.AddObjectNeglectExist(l_pPI);
	//		}
	//		if( l_pPI )
	//		{
	//			l_pPIUnit = l_pPI->GetObject(L"Icon");
	//			if( !l_pPIUnit )
	//			{
	//				//l_pPIUnit = (*l_pPI)[0];
	//			}
	//			if( l_pPIUnit )
	//				l_vScrollerRect.y = l_vFirstIconImagePos.y+(*l_pPIUnit->GetHeight());
	//			else
	//				l_vScrollerRect.y = l_vFirstIconImagePos.y+30.f;
	//		}
	//		Vector3	l_vPos = l_AllIconPosition[i];
	//		//remove Icon
	//		l_pPI->RemoveObjectWithoutDelete(l_pPIUnit->GetName());
	//		cImageSlider*l_pImageWithSlider = new cImageSlider(l_pPIUnit,l_pPI,l_vScrollerRect,l_vPos);
	//		//add again to aviod memory leak
	//		l_pPI->AddObjectNeglectExist(l_pPIUnit);
	//		if( l_pPIUnit )
	//			l_pImageWithSlider->SetName(l_pPIUnit->GetName());
	//		else
	//		{
	//			l_pImageWithSlider->SetName(l_strAllPIName[i]);
	//		}
	//		m_pBuildItemTabePage->AddObjectNeglectExist(l_pImageWithSlider);
	//	}
//end namespace FATMING_CORE
}
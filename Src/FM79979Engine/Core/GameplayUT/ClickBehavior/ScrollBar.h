#pragma once

#include "MPDIToGameObject.h"
namespace FATMING_CORE
{
	class cScrollBar :public cMPDIToGameObject
	{
		class cScrollBox*	m_pScrollBox;
		//
		void				ButtonNextCliclk(int e_iPosX, int e_iPosY, cClickBehavior*e_pObject);
		void				ButtonPreviousClick(int e_iPosX, int e_iPosY, cClickBehavior*e_pObject);
		void				SliderMove(int e_iPosX, int e_iPosY, cClickBehavior*e_pObject);
		void				BGClick(int e_iPosX, int e_iPosY, cClickBehavior*e_pObject);
		void				ScrollMoving(Vector2 e_vPercentage);
		//
		eOrientation		m_eOrientation;
		bool				m_bDebugRender;
		cBaseImage*			m_pSliderImage;
		cBaseImage*			m_pBGImage;
	public:
		DEFINE_TYPE_INFO();
		cScrollBar(class cScrollBox*e_pScrollBox);
		~cScrollBar();
		static cScrollBar*	GetMe(class cScrollBox*e_pScrollBox, eOrientation e_eOrientation, const wchar_t*e_strPIFileName);
		bool	SetupUI(eOrientation e_eOrientation, cBaseImage*e_pPreviousButton, cBaseImage*e_pNextButton, cBaseImage*e_pSlider, cBaseImage*e_pBG);
	};
//end namespace FATMING_CORE
}
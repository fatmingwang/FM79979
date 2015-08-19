#ifndef _SLIDER_BAR_H_
#define _SLIDER_BAR_H_
#include "Mouse_Touch.h"
namespace FATMING_CORE
{
	class	cCueToStartCurveWithTime;
	class	cSliderBar:public cClickMouseBehavior
	{
		//
		GET_SET_DEC(eOrientation,m_eOrientation,GetOrientation,SetOrientation);
		//
		void	Slide(int e_iPosX,int e_iPosY);
		//
		cBaseImage*m_pBar;
		cBaseImage*m_pSlider;
		//
		GET_SET_DEC(float,m_fMinValue,GetMinValue,SetMinValue);
		GET_SET_DEC(float,m_fMaxValue,GetMaxValue,SetMaxValue);
		//
		//float	m_fBarLength;
		//the value to show
		float			m_fCurrentValue;
		//the value where slider indicator
		float	m_fCurrentBarValue;
		//=======================================
        virtual void    InternalMouseMove(int e_iPosX,int e_iPosY);
        virtual void    InternalMouseDown(int e_iPosX,int e_iPosY);
        virtual void    InternalMouseUp(int e_iPosX,int e_iPosY);
	public:
		DEFINE_TYPE_INFO();
		cSliderBar(Vector3	e_vPos,cBaseImage*e_pBar,cBaseImage*e_pSlider,eOrientation e_eOrientation = eO_HORIZONTAL);
		cSliderBar(cCueToStartCurveWithTime*e_pBar,cCueToStartCurveWithTime*e_pSlider,eOrientation e_eOrientation = eO_HORIZONTAL);
		cSliderBar(cBaseImage*e_pBar,cBaseImage*e_pSlider);
		virtual ~cSliderBar();
		void			SetPos(Vector3 e_vPos);
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Render();
		virtual	void	DebugRender();
		float*	GetValue(){return &m_fCurrentValue;}
		void	SetValue(float e_fValue);
		static  cSliderBar*		GetMe(TiXmlElement*e_pElement);
	};

	//cSliderBar*g_pSliderBar = 0;
	//g_pSliderBar = new cSliderBar(Vector3(100,100,100),0,0);
	//g_pSliderBar->Render();
	//g_pSliderBar->DebugRender();
	//if( g_pSliderBar )
	//	g_pSliderBar->MouseDown(e_iPosX,e_iPosY);
	//if( g_pSliderBar )
	//	g_pSliderBar->MouseMove(e_iPosX,e_iPosY);
	//if( g_pSliderBar )
	//	g_pSliderBar->MouseUp(e_iPosX,e_iPosY);
//end namespace
}
#endif
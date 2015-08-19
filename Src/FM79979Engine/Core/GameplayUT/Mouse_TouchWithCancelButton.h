#ifndef _MOUSE_CLICK_CONTROLLER_H_
#define _MOUSE_CLICK_CONTROLLER_H_

#include "Mouse_Touch.h"
#include "ImageButton.h"

namespace FATMING_CORE
{
//============================
//a object has a bunch button or click event
//if player is click out of working zone set it as satisfied and m_bOutOfWorkingZone as true
//it also should contain a Cancel button to make same deal.
//most time it works for UI.
//============================

	class	cMouse_TouchWithCancelButton:public cClickMouseBehaviorVector<cClickMouseBehavior>
	{
		//if misc button is clicked
		bool					m_bMiscButtonClicked;
	protected:
		cRenderObject*	m_pBG;
		//if true while mouse is out of collision range it will be satisfied,or ignore OutOfWorkingMouse check
		GET_SET_DEC(bool,m_bCheckOutOfWorking,IsOutOfWorking,SetOutOfWorking);
		cClickMouseBehavior*	m_pCancelButton;
		//if true ut could be click out of working zone of click on cancel button
		bool					m_bOutOfWorkingZone;
		//check if mouse click on out of working zone
		void    				OutOfWorkingMouseDown(int e_iPosX,int e_iPosY);
		void    				OutOfWorkingMouseMove(int e_iPosX,int e_iPosY);
		void    				OutOfWorkingMouseUp(int e_iPosX,int e_iPosY);
		void    				MiscUpdate(float e_fElpaseTime);
		void    				MiscRender();
		//
		void					SetData(cClickMouseBehavior*e_pCancelButton,Vector4 e_vWorkingRectRange,bool e_bForceSetCancelButtonPositionbyWorkingZone);
	public:
		cMouse_TouchWithCancelButton();
		cMouse_TouchWithCancelButton(cClickMouseBehavior*e_pCancelButton,Vector4 e_vWorkingRectRange,bool e_bForceSetCancelButtonPositionbyWorkingZone = true,cRenderObject*e_pBG = 0);
		cMouse_TouchWithCancelButton(cMouse_TouchWithCancelButton*e_pMouse_TouchWithCancelButton);
		virtual ~cMouse_TouchWithCancelButton();
		//call IsClickMouseBehaviorSatisfied and check this.
		//if m_bClickMouseBehaviorSatisfied is true and m_bCollided isa false mouse,it means player want to leave this UI or event.
		bool					IsLeavingConditionSatisfied(){return m_bOutOfWorkingZone;}
		//addition button event,it won't effect click satisfied condition
		cClickMouseBehaviorVector<cClickMouseBehavior>	m_MiscButtonEventList;
		//
		virtual void    		Init();
		virtual void    		MouseDown(int e_iPosX,int e_iPosY);
		virtual void    		MouseMove(int e_iPosX,int e_iPosY);
		virtual void    		MouseUp(int e_iPosX,int e_iPosY);
		virtual void    		Update(float e_fElpaseTime);
		virtual void    		Render();
		cRenderObject*	GetBG(){return this->m_pBG;}
		void					SetBG(cRenderObject*e_pBG);
		bool					IsChancelButtonSatisfied();
		cClickMouseBehavior*	GetCancelButton(){return m_pCancelButton;}
		//the chancel button will be delete
		void					SetCancelButton(cBaseImage*e_pImage,Vector3 e_vPos);
		void					SetCancelButton(cSubMPDI*e_pSubMPDI);
		void					SetCancelButton(cMPDI*e_pMPDI);
	};
}

#endif
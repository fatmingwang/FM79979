#pragma once

#include "ClickBehavior.h"

namespace FATMING_CORE
{
	class cRenderObject;

	//5/Nov/2019 deprecated
	//please use cMPDIToGameObject instead of this
	//because this one keep render object and  cClickBehavior for 2 things
	class cDefaultRenderClickBehavior:public cClickBehavior
	{
		virtual	void	MouseLeave();
		cMatrix44		m_OnClickMatrix;
		cMatrix44		m_OnHorverMatrix;
		bool			m_bIsOnClick;
		//this won't be delete,just a reference,because render is keep i another render tree.
		//if crush happen,make sure render object is not deleted!
		cRenderObject*	m_pRenderObject;
		virtual bool					Collide(int e_iPosX,int e_iPosY)override;
        //first time into
        virtual cClickBehavior*			MouseDown(int e_iPosX,int e_iPosY)override;
        //horver and move
        virtual cClickBehavior*			MouseMove(int e_iPosX,int e_iPosY)override;
		//
		GET_SET_DEC(bool, m_bEnableClickScale, IsEnableClickScale, SetEnableClickScale);
	public:
		cDefaultRenderClickBehavior();
		~cDefaultRenderClickBehavior();
		DEFINE_TYPE_INFO()
		virtual void		Update(float e_fElpaseTime)override;
		cRenderObject*		GetRenderObject(){ return m_pRenderObject;}
		void				SetRenderObject(cRenderObject*e_pRenderObject);
		void				SetEnableAndImageStatus(bool e_bEnable,Vector4 e_vColor);//if Color alpha is 0 set visible to false else to true
	};
	//end namespace FATMING_CORE
}
#pragma once

#include "ClickBehavior.h"

namespace FATMING_CORE
{
	class cRenderObject;


	class cDefaultRenderClickBehavior:public cClickBehavior
	{
		virtual	void	MouseLeave();
		cMatrix44		m_OnClickMatrix;
		cMatrix44		m_OnHorverMatrix;
		bool			m_bIsOnClick;
		//this won't be delete,just a reference,because render is keep i another render tree.
		//if crush happen,make sure render object is not deleted!
		cRenderObject*	m_pRenderObject;
		virtual bool					Collide(int e_iPosX,int e_iPosY);
        //first time into
        virtual cClickBehavior*			MouseDown(int e_iPosX,int e_iPosY);
        //horver and move
        virtual cClickBehavior*			MouseMove(int e_iPosX,int e_iPosY);
	public:
		cDefaultRenderClickBehavior();
		~cDefaultRenderClickBehavior();
		DEFINE_TYPE_INFO()
		void			Update(float e_fElpaseTime);
		cRenderObject*	GetRenderObject(){ return m_pRenderObject;}
		void			SetRenderObject(cRenderObject*e_pRenderObject);
	};
//end namespace FATMING_CORE
}
#pragma once
#include "FAIFiledPathBehavior.h"
namespace FATMING_AI
{
	//<cFAILeaveFiledBehavior Wall="0,0,1280,720" MinPointDistance="300" MaxPointDistance="300" MinTime="10" MaxTime="" MinPoint="" MaxPoint="" LOD="3" />
	//time,distance,point,collision Wall.
    class   cFAILeaveFiledBehavior:public cFAIBaseBehave
    {
		Vector4				m_vWall;
		//for move out of screen safely
		float				m_fRadius;
		float				m_fSpeed;
		virtual void		InternalInit()override;
		virtual void		InternalUpdate(float e_fElpaseTime)override;
		GETP_SET_DEC(cCurveWithTime,m_CurveWithTime,GetPath,SetPath);
    public:
        DEFINE_TYPE_INFO();
		cFAILeaveFiledBehavior(TiXmlElement*e_pTiXmlElement);
        cFAILeaveFiledBehavior(cFAILeaveFiledBehavior*e_pFAILeaveFiledBehavior);
		CLONE_MYSELF(cFAILeaveFiledBehavior);
        virtual ~cFAILeaveFiledBehavior();
    };
}
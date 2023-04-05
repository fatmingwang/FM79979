#pragma once

namespace FATMING_AI
{
	//time,distance,point,collision Wall.
    class   cFAIPathBehavior:public cFAIBaseBehave
    {
		cCurveManager*	m_pCurveManager;
		cCurveWithTime	m_CurveWithTime;
		virtual void    InternalInit()override;
		virtual void    InternalUpdate(float e_fElpaseTime)override;
    public:
        DEFINE_TYPE_INFO();
		cFAIPathBehavior(TiXmlElement*e_pTiXmlElement);
        cFAIPathBehavior(cFAIPathBehavior*e_pFAIPathBehavior);
		CLONE_MYSELF(cFAIPathBehavior);
        virtual ~cFAIPathBehavior();
		cCurveWithTime*		GetPath() { return &m_CurveWithTime; }
		void				SetPath(cCurveWithTime*e_CurveWithTime) { m_CurveWithTime = *e_CurveWithTime; }
    };
}
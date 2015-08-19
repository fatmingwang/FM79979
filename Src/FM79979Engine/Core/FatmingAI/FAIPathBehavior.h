#ifndef _FAI_PATH_BEHAVIOR_H_
#define _FAI_PATH_BEHAVIOR_H_

namespace FATMING_AI
{
	//time,distance,point,collision Wall.
    class   cFAIPathBehavior:public cFAIBaseBehave
    {
		cCurveManager*	m_pCurveManager;
		GETP_SET_DEC(cCurveWithTime,m_CurveWithTime,GetPath,SetPath);
		virtual void    InternalInit();
		virtual void    InternalUpdate(float e_fElpaseTime);
    public:
        DEFINE_TYPE_INFO();
		cFAIPathBehavior(TiXmlElement*e_pTiXmlElement);
        cFAIPathBehavior(cFAIPathBehavior*e_pFAIPathBehavior);
		CLONE_MYSELF(cFAIPathBehavior);
        virtual ~cFAIPathBehavior();
    };
}

#endif
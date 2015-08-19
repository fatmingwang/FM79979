#ifndef _FAI_ANIMATION_BEHAVIOR_H_
#define _FAI_ANIMATION_BEHAVIOR_H_

namespace FATMING_AI
{
    //================================
    //usually we will set next phase name if we want to wake it up.
    //ex: bfeore AI walking
    //================================
	//please reference cFAIAnimationCharacter
	//<cFAIMPDIAnimationChangeAIBehavior Name="" Probability="10" NextAIName="Idle" AnimationName="Moving" />
	class	cFAIAnimationCharacter;
    class   cFAIMPDIAnimationChangeAIBehavior:public cFAIBaseBehave
    {
		//from data parse it as same as m_pSrcCharacter, or u could set it as expected
		//but ensure resource control
        GET_SET_DEC(cFAIAnimationCharacter*,m_pFMAnimationChanger,GetFMAnimationChanger,SetFMAnimationChanger);
		std::wstring	m_strTargetAnimation;
	    virtual void    InternalInit();
	    virtual void    InternalUpdate(float e_fElpaseTime);
    public:
        DEFINE_TYPE_INFO();
		cFAIMPDIAnimationChangeAIBehavior(TiXmlElement*e_pTiXmlElement);
        cFAIMPDIAnimationChangeAIBehavior(cFAIMPDIAnimationChangeAIBehavior*e_pMPDIAnimationChangeAIBehavior);
		CLONE_MYSELF(cFAIMPDIAnimationChangeAIBehavior);
        virtual ~cFAIMPDIAnimationChangeAIBehavior();
        virtual void    Render();
		virtual void	SetSrcCharacter(cFAICharacterInterface*e_pAICharacterInterface);
    };
    //===============
    //a destination and a safe distance to avoid animation move over
    //
    //===============
	//<cFAIMPDIAnimationChangeAIBehavior Name="" Probability="10" NextAIName="Idle" AnimationName="Moving" DestPos="0,0,0" SafeDistance="10" />
    class   cFAIMPDIAnimationMovingAIBehavior:public cFAIMPDIAnimationChangeAIBehavior
    {
        //
        GET_SET_DEC(Vector3,m_vDestPos,GetDestPos,SetDestPos);
        GET_SET_DEC(float,m_fSafeDistance,GetSafeDistance,SetSafeDistance);
	    virtual void    InternalInit();
	    virtual void    InternalUpdate(float e_fElpaseTime);        
    public:
        DEFINE_TYPE_INFO();
        cFAIMPDIAnimationMovingAIBehavior(TiXmlElement*e_pTiXmlElement);
        cFAIMPDIAnimationMovingAIBehavior(cFAIMPDIAnimationMovingAIBehavior*e_pMPDIAnimationMovingAIBehavior);
		CLONE_MYSELF(cFAIMPDIAnimationMovingAIBehavior);
        virtual ~cFAIMPDIAnimationMovingAIBehavior(){}
    };    
}

#endif
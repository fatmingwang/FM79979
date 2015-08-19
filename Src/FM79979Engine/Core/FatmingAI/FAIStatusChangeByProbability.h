#ifndef _FAI_STATUS_ANIMATION_CHANGE_H_
#define _FAI_STATUS_ANIMATION_CHANGE_H_

namespace FATMING_AI
{
	//==================================================
	//chamge type by probability
	//==================================================
    class   cFAIStatusChangeByProbability:public cFAIBaseBehave
    {
		struct	sHoldTimeAndType
		{
			UT::sTimeCounter			TC;
			eFAICharacterBehaviorStatus	FAICharacterBehaviorStatus;
			UT::sMinMaxData<float>		RandHoldTime;
		};
		sProbabilityWithValue<float,sHoldTimeAndType>	m_ProbabilityValue;
		sHoldTimeAndType*								m_pCurrentTypeHoldTimeUpdate;
		//
		bool											m_bForceToChangeBehavior;
		cFAICharacterStatus*							m_pCharacterStatus;
		eFAICharacterBehaviorStatus						m_eStartBehaviodStatus;
		//cFAICharacterStatus								m_PriorCharacterStatus;
		//eFAICharacterBehaviorStatus						m_ePriorBehaviodStatus;
		virtual void    InternalInit();
		virtual void    InternalUpdate(float e_fElpaseTime);
    public:
        DEFINE_TYPE_INFO();
		cFAIStatusChangeByProbability(TiXmlElement*e_pTiXmlElement);
        cFAIStatusChangeByProbability(cFAIStatusChangeByProbability*e_pFAIPathBehavior);
		CLONE_MYSELF(cFAIStatusChangeByProbability);
        virtual ~cFAIStatusChangeByProbability();
    };
}

#endif
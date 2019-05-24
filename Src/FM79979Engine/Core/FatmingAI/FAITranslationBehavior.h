#ifndef _FAI_TRANSLATION_BEHAVIOR_H_
#define _FAI_TRANSLATION_BEHAVIOR_H_

namespace FATMING_AI
{
	//<cFAIWalkToDestinationAIBehavior Name="1" Probability="10" NextAIName="Waddle" DestPos="500,500,0" Speed="100" />
    class   cFAIWalkToDestinationAIBehavior:public cFAIBaseBehave
    {
		cCurveWithTime		m_CurveWithTime;
        GET_SET_DEC(Vector3,m_vDestPos,GetDestPos,SetDestPos);
        //
        GET_SET_DEC(float,m_fCharacterSpeed,GetCharacterSpeed,SetCharacterSpeed);
		virtual void    InternalInit()override;
		virtual void    InternalUpdate(float e_fElpaseTime)override;
    public:
        DEFINE_TYPE_INFO();
		cFAIWalkToDestinationAIBehavior(TiXmlElement*e_pTiXmlElement);
        cFAIWalkToDestinationAIBehavior(cFAIWalkToDestinationAIBehavior*e_pWalkToDestinationAIBehavior);
		CLONE_MYSELF(cFAIWalkToDestinationAIBehavior);
        virtual ~cFAIWalkToDestinationAIBehavior();
    };
	//<cFAIIdleAIBehavior Name="Idle" Probability="10" NextAIName="Waddle" Time="2" />
    class   cFAIIdleAIBehavior:public cFAIBaseBehave
    {
        sTimeCounter    m_IdleTimeCounter;
		virtual void    InternalInit()override
		{
		    m_IdleTimeCounter.Start();
		}
		virtual void    InternalUpdate(float e_fElpaseTime)override
		{
		    m_IdleTimeCounter.Update(e_fElpaseTime);
		    if(m_IdleTimeCounter.bTragetTimrReached)
		    {
		        m_bSatisfiedCondition = true;
		    }
		}
    public:
        DEFINE_TYPE_INFO();
		cFAIIdleAIBehavior(TiXmlElement*e_pTiXmlElement);
        cFAIIdleAIBehavior(cFAIIdleAIBehavior*e_pIdleAIBehavior);
		CLONE_MYSELF(cFAIIdleAIBehavior);
        virtual ~cFAIIdleAIBehavior(){}
        sTimeCounter*    GetIdleTimeCounter(){ return &m_IdleTimeCounter; }
    };

	//<cFAIIdleAIBehavior Name="DirectionMoving" Probability="10" Direction="0,1,0" />
  //  class   cFAIDirectionMovingAIBehavior:public cFAIBaseBehave
  //  {
		//GET_SET_DEC(Vector3,m_vDirection,GetDirection,SetDirection);
		//virtual void    InternalInit();
		//virtual void    InternalUpdate(float e_fElpaseTime);
  //  public:
  //      DEFINE_TYPE_INFO();
		//cFAIWalkToDestinationAIBehavior(TiXmlElement*e_pTiXmlElement);
  //      cFAIWalkToDestinationAIBehavior(cFAIWalkToDestinationAIBehavior*e_pWalkToDestinationAIBehavior);
		//CLONE_MYSELF(cFAIWalkToDestinationAIBehavior);
  //      virtual ~cFAIWalkToDestinationAIBehavior();
  //  };
}

#endif
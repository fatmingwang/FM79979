#ifndef _AI_MACHINE_H_
#define _AI_MACHINE_H_

namespace FATMING_AI
{
	#define AI_PARSE_NAME_VALUE_START	TiXmlAttribute*	l_pAttribute = e_pTiXmlElement->FirstAttribute();\
                                        const WCHAR*	l_strValue = 0;\
                                        const WCHAR*	l_strName = 0;\
                                        while(l_pAttribute)\
                                        {\
	                                        l_strValue = l_pAttribute->Value();\
	                                        l_strName = l_pAttribute->Name();\
                                        COMPARE_NAME("Name")\
                                        {\
                                            SetName(l_strValue);\
                                        }\
                                        else							    \
                                        COMPARE_NAME("Probability")\
                                        {\
                                            SetProbability(VALUE_TO_INT);\
                                        }\
                                        else\
                                        COMPARE_NAME("NextAIName")\
                                        {\
                                            SetNextAIBehaviorName(l_strValue);\
                                        }\
                                        else\
                                        COMPARE_NAME("KillAfterDone")\
                                        {\
                                            SetKillAfterDone(VALUE_TO_BOOLEAN);\
                                        }\
                                        else
	//a interface for AI behavior change,if AIbehavior condition is filled,it will change autoly,and decide what is next behavior
    //it also have to implement some function to input behavior data for start,so the class which inhierent cFAIMaintainMachine,
    //must override InternalChangeAIBehavior
    //makje sure same type behavior can't add again
    class   cFAIMachine:public cFMWorkingObjectChanger<cFAIBaseBehave>,public cFAIBaseBehave
    {
		std::wstring	m_strStartBehaviorName;
		//all behavior's probability
		int				m_iTotalProbability;
        cFAIBaseBehave* GetRandomAIBehavior();
        //cFAIBaseBehave*m_pCurrentAIBehave;
        //we will update this every frame,if condition is filled,we will force to change m_pCurrentAIBehave.
        //ex: attack,hurt,collision and else...
        //strongly suggest do this override in the game project!.
        //normally we will delete this after AIMachine is delete
        //GET_SET_DEC(cFAIBaseBehave*,m_pTopRequestBehavior,GetTopRequestBehavior,SetTopRequestBehavior);
        //the behavior is done of last one
        //we could get this behavior to decide what it happen at last movent
        //its possible be killed whild AI is done
        cFAIBaseBehave* m_pLastConditionDoneBehavior;
        //input index to active AI,behavior.
		virtual void    InternalInit();
		virtual void    InternalUpdate(float e_fElpaseTime);
        //{
        //  if( e_pTargetAIBehavior->Type() == XXXX::TypeID )
        //  {
        //      return (void*)xxxxx;
        //  }
        //}
		//
		bool			m_bStopWhileNoAI;
    public:
        DEFINE_TYPE_INFO();
        //e_pAICharacterInterface could be null,and set it late
		cFAIMachine(TiXmlElement*e_pTiXmlElement);
		cFAIMachine(cFAIMachine*e_pAIMachine);
        virtual ~cFAIMachine();
        //AI_CLONE_AND_CONVERT_TYPE(cFAIBaseBehave,cFAIMachine)
		CLONE_MYSELF(cFAIMachine);
        //not necessary,it might helpful for debug mode
        virtual void    Render();
		virtual void    DebugRender();
        //after add all Object call this one will set probability and some data.
        virtual void    InitProbability();
		virtual	void    Destroy(){}
		virtual	void    Init();
		virtual	void    Update(float e_fElpaseTime){ cFAIBaseBehave::Update(e_fElpaseTime); }
		virtual void	SetSrcCharacter(cFAICharacterInterface*e_pAICharacterInterface);
		bool			IsDone();
		bool			IsBehaviorChanged();
    };
//end namesapce
}
#endif
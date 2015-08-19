#ifndef _FAI_MULTI_TASK_BEHAVIOR_H_
#define _FAI_MULTI_TASK_BEHAVIOR_H_
namespace FATMING_AI
{
//more than AI at same time
//ex walking WalkToDestinationAIBehavior with cFAIMPDIAnimationChangeAIBehavior
//go to the destination and loop animation.
//if one of them  or allAI behavior condition is done go to next AI
//if the target AI behavior index,all behavior has to be done
//or just the index indicate done this AI behavior is done
    //<cFAIMultiAITaskBehavior Name="Walk" Probability="10" TargetAIBehavior="1" NextAIName="Idle">
    //	<cFAIWalkToDestinationAIBehavior Name="1" DestPos="500,500,0" Speed="100" />
    //	<cFAIMPDIAnimationChangeAIBehavior AnimationName="Moving" />
    //</cFAIMultiAITaskBehavior>
    class   cFAIMultiAITaskBehavior:public cFAIBaseBehave,public cNamedTypedObjectVector<cFAIBaseBehave>
    {
        //see above comment
        GET_SET_DEC(int,m_iTargetAIBehavior,GetTargetAIBehavior,SetTargetAIBehavior);
        //seprate data,because the data might not we expected,so override this function to convert data and input it to expeted
        //data is a vector,std::vector<void*>
        //or override what u want.
	    virtual void    InternalInit();
	    virtual void    InternalUpdate(float e_fElpaseTime);    
    public:
        DEFINE_TYPE_INFO();
		cFAIMultiAITaskBehavior(TiXmlElement*e_pTiXmlElement);
        cFAIMultiAITaskBehavior(cFAIMultiAITaskBehavior*e_pMultiAITaskBehavior);
        virtual ~cFAIMultiAITaskBehavior();
		CLONE_MYSELF(cFAIMultiAITaskBehavior);
	    virtual void    SetSrcCharacter(cFAICharacterInterface*e_pAICharacterInterface);
		inline  virtual	void    Update(float e_fElpaseTime){ cFAIBaseBehave::Update(e_fElpaseTime);}
	    virtual inline  void    Render()
	    {
	        int l_iCount = this->Count();
	        for( int i=0;i<l_iCount;++i )
	        {
	            this->m_ObjectList[i]->Render();
	        }	    
	    }
    };
//==============================
//for animation with a state
//if state is done and animation is done ,it might is a attack or a idle or else...
//if state is walk and animation is loop,we onl expect walk to the destination and condition is done!.
//==============================
}
#endif
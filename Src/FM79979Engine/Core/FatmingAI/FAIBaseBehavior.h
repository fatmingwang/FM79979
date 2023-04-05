#pragma once
#include "FAICharacterInterface.h"
#include <functional>
namespace FATMING_AI
{
	typedef std::function<Vector4()>	f_MyGameViewRect;
    //base AI behavior,ex walk,Idle,Jump,followe....else
	class	cFAIBaseBehave :virtual public NamedTypedObject
	{
	protected:
	    GET_SET_DEC(bool,m_bKillAfterDone,IsKillAfterDone,SetKillAfterDone);
	    //if enable is false,skip this AI go to next behavior
	    GET_SET_DEC(bool,m_bEnable,IsEnable,SetEnable);
	    //the data we need to process,call SetData then Start will convert the data to what we expected,implement this by each Behavior Type!
		//each time ai behavior is asaked,it might has new condition,so here we might have to input some data for this behavior
		//ex: Idle behavior need a time to idle,walk need a idrection and speed,and destination...else
		//no warranty ensure this data will or won't delete,so each Behavior which inhirent cFAIBaseBehave do delete or not delete by itself.
	    GET_SET_DEC(void*,m_pData,GetData,SetData);
		//so we could know character's staus ex:pos frame..and else...it's quite possible convert type to target.
		//the character we control
		//do not selete this one because it come from AIMachine
		cFAICharacterInterface*  m_pSrcCharacter;
	    //
		std::wstring	m_strNextAIBehaviorName;
		//is this AIBehave done
		GET_SET_DEC(bool,m_bSatisfiedCondition,IsSatisfiedCondition,SetSatisfiedCondition);
		////if condition is filled,return next AI behavior name
		//std::wstring    m_strTargetAIName;		
		////it usually come from character data,or event or AI,it's possible to use this data while update,to dectec condition
		//GET_SET_DEC(void*,m_pSourceData,GetSourceData,SetSourceData);
		////it usually from character data,or event or AI
		//GET_SET_DEC(void*,m_pDestData,GetDestData,SetDestData);
		//this one usually do for m_pDestData,not nexessary
		//virtual void    InternalUpdateForConditionFilled(){}
		//the percentage for possible to active.
		//virtual	void	DoAIBehave(ATG::Frame*,void*e_pData = 0){ assert(0&&"this one AIBehave is empty"); }
		GET_SET_DEC(int,m_iProbabilityToDo,GetProbability,SetProbability);
		//
		virtual void    InternalInit() = 0;
		virtual void    InternalUpdate(float e_fElpaseTime) = 0;
	public:
	    DEFINE_TYPE_INFO();
		cFAIBaseBehave();
		//please call SetSrcCharacter aftern clone let all behavior has same character resource
		cFAIBaseBehave(cFAIBaseBehave*e_pBaseAIBehave);
		virtual ~cFAIBaseBehave();
		cFAICharacterInterface*	GetSrcCharacter(){ return m_pSrcCharacter; };
		//virtual for cFAIMultiAITaskBehavior.
		virtual void			SetSrcCharacter(cFAICharacterInterface*e_pAICharacterInterface);
        //input a data if we need
		inline  void			Init(){ m_bSatisfiedCondition = false; InternalInit(); }
        inline  virtual	void    Update(float e_fElpaseTime){ if( !m_bEnable ) m_bSatisfiedCondition = true; else if( m_pSrcCharacter&&!m_bSatisfiedCondition )InternalUpdate(e_fElpaseTime); }
        //not necessary
        virtual void			Render(){}
		virtual void			Destroy(){}
		//target AI name,if return 0,random to set next ai behavior
		virtual const wchar_t*	GetNextAIBehaviorName(){  if( wcslen(m_strNextAIBehaviorName.c_str()) )return m_strNextAIBehaviorName.c_str();return 0; }
		virtual void			SetNextAIBehaviorName(const wchar_t*e_strName);


		static f_MyGameViewRect	m_sfGetGameViewRect;
	};
}
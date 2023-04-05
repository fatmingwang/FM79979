#pragma once

#include "FAIStatus.h"
namespace FATMING_AI
{
    //basic character let AI control position and some info.
    class   cFAICharacterInterface:public Frame
    {
        GET_SET_DEC(float,m_fSpeed,GetSpeed,SetSpeed);
		//if AI is dying,speed is posible let AI run faster
		GET_SET_DEC(float,m_fRunSpeed,GetRunSpeed,SetRunSpeed);
		GET_SET_DEC(float,m_fAnimationSpeed,GetAnimationSpeed,SetAnimationSpeed);
        GET_SET_DEC(cFAICharacterStatus*,m_pAICharacterStatus,GetStatus,SetStatus);
		//for 2D angle
		GETP_SET_DEC(Vector3,m_v2DAngle,GetAngle,SetAngle);
    public:
        DEFINE_TYPE_INFO();
		cFAICharacterInterface();
		cFAICharacterInterface(TiXmlElement*e_pElement);
		cFAICharacterInterface(cFAICharacterInterface*e_pAICharacterInterface);
        virtual ~cFAICharacterInterface();
		CLONE_MYSELF(cFAICharacterInterface);
		void						ProcessXmlData(TiXmlElement*e_pelement);
        //below function let AIBehavior to fetch the data as needed.
        Vector3*					GetPosition();
		virtual	void				DebugRender();
		//
	    virtual void*				GetAnimationData(){ return 0; }
		virtual const wchar_t*		GetAnimationDataType(){ return 0; }
		//
	    virtual void*				GetWeaponData(){ return 0; }
		virtual	bool				IsCharacterBehaviorStatusLock();
		virtual	bool				IsCharacterBehaviorStatusLock(eFAICharacterBehaviorStatus	e_eFAICharacterBehaviorStatus);
    };

//end namespace
}
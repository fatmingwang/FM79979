#ifndef _FAI_WALL_REFELECT_MOVING_BEHAVIOR_H_
#define _FAI_WALL_REFELECT_MOVING_BEHAVIOR_H_

namespace FATMING_AI
{
    class   cFAIWallReflectMovingBehavior:public cFAIBaseBehave
    {
        GET_SET_DEC(Vector4,m_vWall,GetWall,SetWall);
		UT::sMinMaxData<float>	m_fSpeedRestrict;
		float					m_fSpeedScale;
		Vector3					m_vDirection;
		UT::sMinMaxData<float>	m_WallHitRetrict;
		int						m_iNumWallHit;
		UT::sTimeCounter		m_CurrentWallHitTC;

		virtual void    InternalInit()override;
		virtual void    InternalUpdate(float e_fElpaseTime)override;
		GET_SET_DEC(DoButtonGoal_Callback,m_pHittWallCallback,GetHittWallCallback,SetHittWallCallback);
    public:
        DEFINE_TYPE_INFO();
		cFAIWallReflectMovingBehavior(TiXmlElement*e_pTiXmlElement);
        cFAIWallReflectMovingBehavior(cFAIWallReflectMovingBehavior*e_pFAIWallReflectMovingBehavior);
		CLONE_MYSELF(cFAIWallReflectMovingBehavior);
        virtual ~cFAIWallReflectMovingBehavior();
    };
}

#endif
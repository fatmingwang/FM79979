#ifndef _FAI_FILED_PATH_BEHAVIOR_H_
#define _FAI_FILED_PATH_BEHAVIOR_H_

namespace FATMING_AI
{
	//<cFAIFiledPathBehavior Wall="0,0,1280,720" MinPointDistance="300" MaxPointDistance="300" MinTime="10" MaxTime="" MinPoint="" MaxPoint="" LOD="3" />
	//time,distance,point,collision Wall.
    class   cFAIFiledPathBehavior:public cFAIBaseBehave
    {
		sMinMaxData<float>	m_fDirectionOffsetX;
		sMinMaxData<float>	m_fDirectionOffsetY;
		Vector4				m_vWall;
		sMinMaxData<float>	m_fTime;
		sMinMaxData<int>	m_iPoint;
		sMinMaxData<float>	m_fXDistance;
		sMinMaxData<float>	m_fYDistance;
		float				m_fLeaveAwayProbability;
		sMinMaxData<int>	m_iDistanceChangePoint;//avoid m_fXDistance,m_fYDistance change too fast
		virtual void    InternalInit();
		virtual void    InternalUpdate(float e_fElpaseTime);
		//GETP_SET_DEC(cCurveWithTime,m_CurveWithTime,GetPath,SetPath);
		cCurveWithTime		m_CurveWithTime;
    public:
        DEFINE_TYPE_INFO();
		cFAIFiledPathBehavior(TiXmlElement*e_pTiXmlElement);
        cFAIFiledPathBehavior(cFAIFiledPathBehavior*e_pFAIPathBehavior);
		CLONE_MYSELF(cFAIFiledPathBehavior);
        virtual ~cFAIFiledPathBehavior();
		virtual void Render()override;
		cCurveWithTime*		GetPath() { return &m_CurveWithTime; }
		void				SetPath(cCurveWithTime*e_CurveWithTime) {m_CurveWithTime = *e_CurveWithTime;}
    };
}

#endif
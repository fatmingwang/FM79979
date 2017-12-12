#ifndef _CURVE_CHANGER_H
#define _CURVE_CHANGER_H
namespace FATMING_CORE
{
//=============================
//change curve as need and make it smmoth to change curve
//basicly all the curve in the m_CurveWithTimeVector has same count.
//but we still accept different count of curve but make sure u are calling carefully.
//=============================
	class	cCurveChanger:public NamedTypedObject
	{
		//for change path use
		cCurveWithTime			m_CurveWithTime;
		//
		void					AssignCurveChangeData(float e_fSpeed,int e_iTargetFinalPointIndex,cCurveWithTime*e_pTargetCurve);
		//for change curve,if it's true do not update the path we are using,until current position reach destination
		bool					m_bChangeCurve;
		Vector3					m_vChangeCurvePos;
		float					m_fChangeCurveLastAngle;
		float					m_fChangeCurveTargetAngle;
		float					m_fToTargetPositionAngle;
		//get all possible nex points data
		cNamedTypedObjectVector<cCurveWithTime>	m_CurveWithTimeVector;
	public:
		DEFINE_TYPE_INFO()
		//current curve data
		Vector3					m_vCurrentPos;
		float					m_fCurrentAngle;
		int						m_iCurrentCurveIndex;
		cCurveWithTime*			m_pCurrentCurveWithTime;
		//if e_bCloneData is false the data in m_CurveWithTimeVector would be a reference pointer
		cCurveChanger(cNamedTypedObjectVector<cCurveWithTime>* e_pCurveWithTimeVector);
		//clone a new one
		cCurveChanger(cCurveChanger*e_pCurveChanger);
		virtual ~cCurveChanger();
		void					Start(int e_iStartCurveIndex = 0,float e_fStartTime = 0.f);
		void					Update(float e_fElpaseTime);
		void					Render();
		void					DebugRender();
		//cCurveWithTime::CalculateCurrentSpeed
		//if e_iCurveIndex is not legal return false and do notthing,and if e_bLoopPointIndex is true,it will force to fetch a accetable index
		bool					ChangeCurve(int e_iCurveIndex,int e_iTargetOriginalPointIndex,float e_fSpeed,bool e_bLoopPointIndex = true);
		//it will calculate the final list count and find the index for TargetPointIndexPercentage
		bool					ChangeCurve(int e_iCurveIndex,float e_fTargetOriginalPointIndexPercentage,float e_fSpeed);
		int						GetCurrentCurveIndex();
		cCurveWithTime*			GetCurrentCurve(){ return m_pCurrentCurveWithTime; }
	};
}
#endif
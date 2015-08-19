#include "../stdafx.h"
#include "CurveChanger.h"
#include "../Image/GlyphFontRender.h"
#include "../GLSL/Shader.h"
#include "../Image/SimplePrimitive.h"
namespace FATMING_CORE
{
	const wchar_t*         cCurveChanger::TypeID( L"cCurveChanger" );

	cCurveChanger::cCurveChanger(cNamedTypedObjectVector<cCurveWithTime> *e_pCurveWithTimeVector)
	{
		m_bChangeCurve = false;
		m_iCurrentCurveIndex = 0;
		m_pCurrentCurveWithTime = 0;
		CLONE_VECTOR_TO_NEW_VECTOR(cCurveWithTime,e_pCurveWithTimeVector,((cNamedTypedObjectVector<cCurveWithTime>*)&m_CurveWithTimeVector));
		//or instead set data is loop from path file
		for( int i=0;i<m_CurveWithTimeVector.Count();++i )
		{
			m_CurveWithTimeVector[i]->SetCurveLoop(true);
		}
	}

	cCurveChanger::cCurveChanger(cCurveChanger*e_pCurveChanger)
	{
		SetName(e_pCurveChanger->GetName());
		m_bChangeCurve = e_pCurveChanger->m_bChangeCurve;
		m_iCurrentCurveIndex = e_pCurveChanger->m_iCurrentCurveIndex;
		m_pCurrentCurveWithTime = e_pCurveChanger->m_pCurrentCurveWithTime;
		CLONE_VECTOR_TO_NEW_VECTOR(cCurveWithTime,((cNamedTypedObjectVector<cCurveWithTime>*)&e_pCurveChanger->m_CurveWithTimeVector),((cNamedTypedObjectVector<cCurveWithTime>*)&m_CurveWithTimeVector));
		//or instead set data is loop from path file
		for( int i=0;i<m_CurveWithTimeVector.Count();++i )
		{
			m_CurveWithTimeVector[i]->SetCurveLoop(true);
		}
	}

	cCurveChanger::~cCurveChanger()
	{
	
	}

	void	cCurveChanger::Start(int e_iStartCurveIndex,float e_fStartTime)
	{
		m_pCurrentCurveWithTime = this->m_CurveWithTimeVector[e_iStartCurveIndex];
		m_pCurrentCurveWithTime->Init();
	}

	void	cCurveChanger::Update(float e_fElpaseTime)
	{
//=================================
//two part first one for steering to the target curve
//sceond part is steering to the nextpoint direction
//assume half way of those points is a seprate point!
//=================================
		if(m_bChangeCurve)
		{
			m_CurveWithTime.Update(e_fElpaseTime);
			float	l_fLerP = m_CurveWithTime.GetCurrentLERPTime();
			if( l_fLerP>0.7f )
			{
				//m_fCurrentAngle = m_CurveWithTime.GetCurrentPosToNextPointAngle();
				float	l_fDiffAngle = CalculateDifferenceBetweenRadius(m_fToTargetPositionAngle,m_fChangeCurveTargetAngle);
				m_fCurrentAngle = m_fToTargetPositionAngle+(l_fDiffAngle*(l_fLerP-0.7f)*2);
				//m_fCurrentAngle = m_fChangeCurveTargetAngle;
				//m_fCurrentAngle = m_CurveWithTime.GetCurrentPosToNextPointAngle();
			}
			else
			{
				Vector3	l_vTargetPos = m_CurveWithTime.GetOriginalPointList()[2];
				float	l_fAngle = D3DXToRadian(180.f-UT::GetAngleBy2Point(this->m_vChangeCurvePos,l_vTargetPos));
				float	l_fDiffAngle = CalculateDifferenceBetweenRadius(m_fChangeCurveLastAngle,l_fAngle);
				m_fCurrentAngle = this->m_fChangeCurveLastAngle+(l_fDiffAngle*l_fLerP*2);
				m_fToTargetPositionAngle = m_fCurrentAngle;
			}
			m_vCurrentPos = m_CurveWithTime.GetCurrentPosition();
			if( m_CurveWithTime.IsCurveMoveDone() )
				m_bChangeCurve = false;
		}
		else
		if( m_pCurrentCurveWithTime )
		{
			m_pCurrentCurveWithTime->Update(e_fElpaseTime);
			m_vCurrentPos = m_pCurrentCurveWithTime->GetCurrentPosition();
			m_fCurrentAngle = m_pCurrentCurveWithTime->GetCurrentPosToNextPointAngle();
		}
	}

	void	cCurveChanger::Render()
	{
	
	}

	void	cCurveChanger::DebugRender()
	{
		m_pCurrentCurveWithTime->RenderCurve(Vector4(1,0,0,1));
		//m_pCurrentCurveWithTime->RenderPointIndex();
		if(m_bChangeCurve)
		{
			if( m_CurveWithTime.GetPointList().size() )
			{
				//GLRender::RenderPoint(m_CurveWithTime.GetPointList()[m_CurveWithTime.GetPointList().size()-1],15,Vector4(1,1,0,1));
				GLRender::RenderPoint(m_CurveWithTime.GetPointList()[0],20,Vector4(1,0,0,1));
				GLRender::RenderPoint(m_CurveWithTime.GetPointList()[1],20,Vector4(0,1,0,1));
				GLRender::RenderPoint(m_CurveWithTime.GetPointList()[2],20,Vector4(0,0,1,1));
			}
		}
	}

	void	cCurveChanger::AssignCurveChangeData(float e_fSpeed,int e_iTargetOriginalPointIndex,cCurveWithTime*e_pTargetCurve)
	{
		//avoid over size
		e_iTargetOriginalPointIndex = UT::GetLoopIndex(e_iTargetOriginalPointIndex,e_pTargetCurve->GetOriginalPointList().size());
		//here we add a fake curve
		//first add start point as current curve's pervious position
		//sedcond add current position
		//third add destination position
		m_CurveWithTime.Destroy();
		int	l_iIndex1 = e_iTargetOriginalPointIndex;
		int	l_iPrevioudIndex = e_iTargetOriginalPointIndex-1;
		std::vector<Vector3>	l_PointList = e_pTargetCurve->GetOriginalPointList();
		l_iIndex1 = UT::GetLoopIndex<Vector3>(l_iIndex1,&l_PointList);
		l_iPrevioudIndex = UT::GetLoopIndex<Vector3>(l_iPrevioudIndex,&l_PointList);
		m_CurveWithTime.AddPoint(e_pTargetCurve->GetOriginalPointList()[l_iPrevioudIndex],0.f);
		m_CurveWithTime.AddPoint(e_pTargetCurve->GetOriginalPointList()[l_iIndex1],1.f);
		m_CurveWithTime.Init();
		m_CurveWithTime.Update(0.999f);
		m_fChangeCurveTargetAngle = m_CurveWithTime.GetCurrentPosToNextPointAngle();
		m_CurveWithTime.Destroy();


		Vector3 l_NextPointPos;		Vector3	l_CurrentPosition;
		e_pTargetCurve->SetCalAngle(true);
		std::vector<Vector3>	l_PointList2 = m_pCurrentCurveWithTime->GetOriginalPointList();
		int	l_iStartPointIndex = UT::GetLoopIndex<Vector3>(m_pCurrentCurveWithTime->GetCurrentWorkingOriginalIndex()-1,&l_PointList2);
		Vector3	l_vStartPos = m_pCurrentCurveWithTime->GetOriginalPointList()[l_iStartPointIndex];
		int	l_iTargetPointIndex = UT::GetLoopIndex<Vector3>(e_iTargetOriginalPointIndex,&l_PointList);
		l_NextPointPos = e_pTargetCurve->GetOriginalPointList()[l_iTargetPointIndex];
		m_CurveWithTime.AddPoint(l_vStartPos,0.f);
		m_CurveWithTime.AddPoint(this->m_vCurrentPos,0.5f);
		m_CurveWithTime.AddPoint(l_NextPointPos,1.f);
		m_CurveWithTime.SetCalAngle(true);
		m_CurveWithTime.Init();
		m_CurveWithTime.Update(0.50000f+EPSIONAL);


		m_bChangeCurve = true;
		m_fChangeCurveLastAngle = m_pCurrentCurveWithTime->GetCurrentPosToNextPointAngle();
		m_vChangeCurvePos = m_vCurrentPos;
		//calculate current angle?
		bool	l_b = m_pCurrentCurveWithTime->IsCalAngle();
		//e_pTargetCurve->Start(m_pCurrentCurveWithTime->GetCurrentTime());
		float	l_fTargetStartTime = m_pCurrentCurveWithTime->GetOriginalTimeList()[l_iPrevioudIndex];
		float	l_fStepTime = m_pCurrentCurveWithTime->GetOriginalTimeList()[l_iIndex1]-l_fTargetStartTime;
		//
		if( l_fStepTime<0.f )
			l_fStepTime = 0.0001f;
		//if( l_fTargetStartTime>0.01f )
		//	l_fTargetStartTime -= 0.001f;
		e_pTargetCurve->Init();
		e_pTargetCurve->Update(l_fStepTime+l_fTargetStartTime);

		this->m_pCurrentCurveWithTime = e_pTargetCurve;
		m_pCurrentCurveWithTime->SetCalAngle(l_b);
	}

	bool	cCurveChanger::ChangeCurve(int e_iCurveIndex,int e_iTargetOriginalPointIndex,float e_fSpeed,bool e_bLoopPointIndex)
	{
		if(!IsLegalIndexInVector<cCurveWithTime*>(m_CurveWithTimeVector.GetList(),e_iCurveIndex))
			return false;
		cCurveWithTime*l_pTargetCurve = this->m_CurveWithTimeVector[e_iCurveIndex];
		if( l_pTargetCurve->GetOriginalTimeList().size() == 0 )
			return false;
		if( e_bLoopPointIndex )
		{
			std::vector<Vector3>	l_vPointList = l_pTargetCurve->GetPointList();
			e_iTargetOriginalPointIndex = GetLoopIndex<Vector3>(e_iTargetOriginalPointIndex,&l_vPointList);
			//e_iTargetOriginalPointIndex = GetLoopIndex<Vector3>(e_iTargetOriginalPointIndex,);
		}
		else
		{
			std::vector<Vector3>l_vPointList = l_pTargetCurve->GetPointList();
			if(!IsLegalIndexInVector<Vector3>(&l_vPointList,e_iTargetOriginalPointIndex))
				return false;
		}
		AssignCurveChangeData(e_fSpeed,e_iTargetOriginalPointIndex,l_pTargetCurve);
		return true;
	}

	bool	cCurveChanger::ChangeCurve(int e_iCurveIndex,float e_fTargetOriginalPointIndexPercentage,float e_fSpeed)
	{
		cCurveWithTime*l_pTargetCurve = this->m_CurveWithTimeVector[e_iCurveIndex];
		if( e_iCurveIndex+1 >= (int)l_pTargetCurve->GetPointList().size() )
			return false;
		//calculate current angle?
		int	l_iTargetIndex = (int)(e_fTargetOriginalPointIndexPercentage*l_pTargetCurve->GetOriginalPointList().size());
		AssignCurveChangeData(e_fSpeed,l_iTargetIndex,l_pTargetCurve);
		return true;
	}

	int		cCurveChanger::GetCurrentCurveIndex()
	{
		if(!this->m_pCurrentCurveWithTime)
			return -1;
		return this->m_CurveWithTimeVector.GetObjectIndexByPointer(m_pCurrentCurveWithTime);
	}
}
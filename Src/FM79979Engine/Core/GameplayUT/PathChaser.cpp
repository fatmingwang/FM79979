#include "../stdafx.h"
#include "../Math/Curve.h"
#include "PathChaser.h"
#include "../Image/SimplePrimitive.h"
#include "../GLSL/Shader.h"
namespace FATMING_CORE
{

	cPathChaser::cPathChaser(float e_fTolerationErrorDistance,float e_fReachedLength)
	{
		m_bTouched = false;
		m_iNextGoalIndex = 0;
		m_fTotalErrorDistance = 0.f;
		m_fTolerationErrorDistance = e_fTolerationErrorDistance;
		m_fCurrentErrorDistance = 0.f;
		m_fReachedLength = e_fReachedLength;
		m_fTotalLength = 0.f;
		m_bTouchProgressSuccess = true;
		m_bAllPointsTouchSuccess = false;
		m_bCurveLoop = true;
		m_pvAllLinePositionInfo = 0;
		//Start();
	}

	cPathChaser::~cPathChaser()
	{
		SAFE_DELETE(m_pvAllLinePositionInfo);
	}
	void	cPathChaser::AssignPathData(cCurveWithTime *e_pCurvesWithTime)
	{
		this->m_OriginalPointList = e_pCurvesWithTime->GetOriginalPointList();
		this->m_OriginalTimeList = e_pCurvesWithTime->GetOriginalTimeList();
		bool	l_b = this->SetLOD(e_pCurvesWithTime->GetLOD());
		assert(l_b);
		m_bCurveLoop = true;
		Init();
		int	l_uiSize = (int)this->m_FinallyPointList.size();
		m_pvAllLinePositionInfo = new Vector2[l_uiSize*2];
		if( m_OriginalPointList.size() )
			m_vCurrentTouchedPosition = this->m_OriginalPointList[0];

	}
	void	cPathChaser::CalculateLength()
	{
		UINT	l_uiSize = (int)this->m_OriginalPointList.size();
		Vector3	l_vPrevoisPos = m_OriginalPointList[0];
		for( UINT i=1;i<l_uiSize;++i )
		{
			Vector3	l_vPos = m_OriginalPointList[i];
			m_fTotalLength+=(l_vPos-l_vPrevoisPos).Length();
			l_vPrevoisPos = l_vPos;
		}
	}

	//=================
	//first time touch check if it close start point
	//=================
	void	cPathChaser::MouseDown(int e_iPosX,int e_iPosY)
	{
		m_bTouched = true;
		assert(m_OriginalPointList.size()&&"there is no data");
		Vector3	l_vPos = this->m_OriginalPointList[0];
		m_vCurrentTouchedPosition = Vector2((float)e_iPosX,(float)e_iPosY);
		float	l_fLength = (l_vPos-m_vCurrentTouchedPosition).Length();
		if( l_fLength>this->m_fTolerationErrorDistance )
		{
			m_bTouchProgressSuccess = false;
			return;
		}
		m_fCurrentErrorDistance = 0;
		m_iNextGoalIndex = 1;
		this->m_fTotalErrorDistance = m_fTolerationErrorDistance-l_fLength;
		m_fLastLengthToGoalPoint = (this->m_OriginalPointList[m_iNextGoalIndex]-m_vCurrentTouchedPosition).Length();
		if( m_OriginalPointList.size() )
			m_vCurrentTouchedPosition = this->m_OriginalPointList[0];
		this->m_bAllPointsTouchSuccess = false;
		m_bTouchProgressSuccess = true;
	}

	void	cPathChaser::MouseUp(int e_iPosX,int e_iPosY)
	{
		m_bTouched = false;
		if(!m_bTouchProgressSuccess)
			return;
		if( m_iNextGoalIndex == this->m_OriginalPointList.size()-1 )
		{
			if( m_fLastLengthToGoalPoint <= this->m_fTolerationErrorDistance )
			{
				this->m_bAllPointsTouchSuccess = true;
				m_bTouchProgressSuccess = true;
			}
		}
		m_iNextGoalIndex = 0;
	}

	void	cPathChaser::MouseMove(int e_iPosX,int e_iPosY)
	{
		if(!m_bTouchProgressSuccess)
			return;
		Vector2	l_vGoalPos = Vector2(this->m_OriginalPointList[m_iNextGoalIndex].x,this->m_OriginalPointList[m_iNextGoalIndex].y);
		Vector2	l_vCurrentPos = Vector2((float)e_iPosX,(float)e_iPosY);
		m_vCurrentTouchedPosition = Vector2((float)e_iPosX,(float)e_iPosY);
		float	l_fCurrentDistanceToGoalPoint = (l_vGoalPos-l_vCurrentPos).Length();
		float	l_fToGoalDistanceChangedValue = l_fCurrentDistanceToGoalPoint-m_fLastLengthToGoalPoint;
		if( l_fToGoalDistanceChangedValue>0 )//it go far.....
		{
			m_fCurrentErrorDistance+=l_fToGoalDistanceChangedValue;
			if( m_fCurrentErrorDistance>=this->m_fTolerationErrorDistance )
			{
				this->m_bTouchProgressSuccess = false;
				return;
			}
			this->m_fTotalErrorDistance+=l_fToGoalDistanceChangedValue;
		}
		else//it's closer,check if is it time to next point?
		{
			if(this->m_fReachedLength>=l_fCurrentDistanceToGoalPoint)//reached goal point
			{
				if( m_iNextGoalIndex<(int)this->m_OriginalPointList.size()-1 )
				{
					++m_iNextGoalIndex;
					m_fCurrentErrorDistance = 0;
					m_fLastLengthToGoalPoint = (this->m_OriginalPointList[m_iNextGoalIndex]-this->m_vCurrentTouchedPosition).Length();
					return;
				}
			}
		}
		m_fLastLengthToGoalPoint = l_fCurrentDistanceToGoalPoint;
	}

	//void	cPathChaser::Update(float e_fElpaseTime)
	//{
	//	cCurveWithTime::Update(e_fElpaseTime);
	//}
	void	cPathChaser::RenderCurve(Vector4 e_vColor)
	{
		cCurveWithTime::RenderCurve(e_vColor);
		if( m_OriginalPointList.size() < 1 )
			return;
		int	l_iSize = m_iCurrentPointIndex+1;
		if( m_bTouched )
		{
			l_iSize = this->m_iNextGoalIndex;
		}
		for( int i=0;i<l_iSize;++i )
		{
			m_pvAllLinePositionInfo[i*2] = m_FinallyPointList[i];
			if( i+1 == l_iSize )
			{
				if( m_bTouched )
					m_pvAllLinePositionInfo[i*2+1] = this->m_vCurrentTouchedPosition;
				else
					m_pvAllLinePositionInfo[i*2+1] = this->m_vCurrentPosition;
			}
			else
				m_pvAllLinePositionInfo[i*2+1] = m_FinallyPointList[i+1];
		}
		myGlVertexPointer(2, m_pvAllLinePositionInfo);
		FATMING_CORE::SetupShaderColor(e_vColor);
		MY_GLDRAW_ARRAYS(GL_LINES, 0, l_iSize*2);
		//draw each point
		float	l_fPoints[4] = { m_pvAllLinePositionInfo[0].x,m_pvAllLinePositionInfo[0].y,
								!m_bTouched?m_vCurrentPosition.x:this->m_vCurrentTouchedPosition.x,
								!m_bTouched?m_vCurrentPosition.y:m_vCurrentTouchedPosition.y };
		myGlVertexPointer(2, l_fPoints);
		MY_GLDRAW_ARRAYS(GL_POINTS, 0, 2);
	}
}
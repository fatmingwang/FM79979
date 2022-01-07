#pragma once
#include "../Render/Curve/Curve.h"
namespace FATMING_CORE
{
	//===================================
	//my thoughts
	//first u have a start point,while the touch point is quite close goal index's position(a small toleration distance),
	//change current goal index to next,if fTotalErrorDistance is over toleration error distance make condition is fail.
	//set goal index as 0 last one.
	//=================
	enum eTracePathToleranceType
	{
		//basicly no error if you move or touch close to next point,change target next point(game play for time limition)
		eTPTT_ONLY_CHECK_POINTS = 0,
		//if moving out of line(2 points) get failed
		eTPTT_CHECK_2_POINTS_MOVING,
		eTPTT_,
		eTPTT_MAX,
	};
	class	cPathChaser:public cCurveWithTime
	{
		//the point index for reached the target 
		int						m_iNextGoalIndex;
		//current mouse or finger position
		Vector2					m_vCurrentTouchedPosition;
		//the length to goal point
		float					m_fLastLengthToGoalPoint;
		//it may to know the correction ratio,each 2 point has a length and it could be a little error so this is it
		float					m_fTotalErrorDistance;
		//if error length is bigger than this,it's failed
		float					m_fTolerationErrorDistance;
		//if the position is smaller than the length of position of goal's index point
		float					m_fReachedLength;
		//the error distance between 2 points,if this value is bigger than m_fTolerationErrorDistance,m_bTouchProgressSuccess is false
		float					m_fCurrentErrorDistance;
		//all line between points's length
		float					m_fTotalLength;
		//while the touch is failed it's false,else it's true
		bool					m_bTouchProgressSuccess;
		//if all point is touched it's true.
		bool					m_bAllPointsTouchSuccess;
		//while state is touched
		bool					m_bTouched;
		//a line is construct by 2 points,we could not direct to fetch path data,so here to store the line position
		//if render is not coorectly it's possible SetLod is called
		Vector2*					m_pvAllLinePositionInfo;
	public:
		DEFINE_TYPE_INFO();
		cPathChaser(float e_fTolerationErrorDistance = 20.f,float e_fReachedLength = 20.f);
		virtual ~cPathChaser();
		void					AssignPathData(cCurveWithTime *e_pCurvesWithTime);
		void					CalculateLength();
		float					GetLength(){ return m_fTotalLength; }
		void					MouseDown(int e_iPosX,int e_iPosY);
		void					MouseUp(int e_iPosX,int e_iPosY);
		void					MouseMove(int e_iPosX,int e_iPosY);
		float					GetTotalErrorRation(){ return 1-(m_fTolerationErrorDistance/m_fTotalLength); }
		//while update it have to test IsTouchProgressSuccess is not false or it failed,
		//if IsTouchProgressSuccess is true and IsAllPointsTouchSuccess is true,it done well
		bool					IsTouchProgressSuccess(){ return m_bTouchProgressSuccess; }
		bool					IsAllPointsTouchSuccess(){ return m_bAllPointsTouchSuccess; }
		//void	Update(float e_fElpaseTime);
		void					RenderCurve(Vector4 e_vColor = Vector4(0,1,0,1));
	};

}
#ifndef _CURVE_H_
#define _CURVE_H_
#include "Vector3.h"
#include "Quaternion.h"
#include "MathUtility.h"
#include "../Image/FMAnimationRule.h"
//http://www.gamedev.net/reference/articles/article1808.asp
//The simplest form is a straight line from a control point A, to a control point B. 
//
//I will use Ax to denote the x-coordinate of control point A, and Ay to denote the y-coordinate. The same goes for B. 
//
//I am introducing another variable, b, just to make it a bit more simple to read the functions. However, the variable b is only a in disguise, b is always equal to 1-a. So if a = 0.2, then b = 1-0.2 = 0.8. So when you see the variable b, think: (1-a). 
//
//This parametric curve describes a line that goes from point A, to point B when the variable a goes from 1.0 to 0.0: 
//
//X(a) = Ax¡Pa + Bx¡Pb
//Y(a) = Ay¡Pa + By¡Pb
//Z(a) = Az¡Pa + Bz¡Pb
//
//If you set a = 0.5 (and thus also b = 0.5, since b = 1.0-a = 1.0-0.5 = 0.5) then X(a), Y(a) and Z(a) will give you the 3D coordinate of the point on the middle of the line from point A to point B. 
//
//Note that the reason this works is because a + b is always equal to one, and when you multiply something with one, it will stay unchanged. This makes the curve behave predictably and lets you place the control points anywhere in the coordinate system, since when a = 1.0 then b = 0.0 thus making the point equal to one of the control points, and completely ignoring the other one. 
//
//Quadratic


//http://www.experts-exchange.com/Programming/Game_Development/Game_Graphics/OpenGL/Q_20809629.html
//Chaikin_Curve
//=====================
//actually working curve is m_pFinallyPointList
//m_OriginalPointList is a originale data,we via fix it to change m_pFinallyPointList
//=====================
namespace FATMING_CORE
{
	class cCurve
	{

		void				IncreaseLod();
		void				DecreaseLod();
		//finally pointlist
		GET_SET_DEC(std::vector<Vector3>,m_FinallyPointList,GetPointList,SetPointList);	
		//original point,
		GET_SET_DEC(std::vector<Vector3>,m_OriginalPointList,GetOriginalPointList,SetOriginalPointList);
		//how detail is line become
		int 				m_iLOD;
		//rotate the object head to next point
		//GET_SET_DEC(bool,m_bRotationObject,IsRotationObject,SetRotationObject);
		virtual	void		DoLOD();//while point is changed call this
	public:
		virtual const wchar_t*        Type() const{ return TypeID; }
		static const  wchar_t*        TypeID;
		cCurve();
		cCurve(cCurve*e_pCurve);
		//CLONE_MYSELF(cCurve);
		virtual~cCurve();
		//set m_OriginalPointList = m_FinallyPointList,and set lod = 1
		virtual	void		SetOriginalToFinal();
		//while LOD is changing the line data have to change too
		int					GetLOD(){ return m_iLOD; }
		virtual	bool		SetLOD(int e_iLOD);
		//parse current data replace point to new data and let LOD to be 1,the m_OriginalPointList's count would be same as e_iNumPoint
		//fuck lazy to finish it
		//virtual	void	ReplacePointsByCount(int e_iNumPoint);
		//do not recommand call this at all,because I have not optmize it at all,it do reallocate memory often,so it sux :(
		//u have to apply matrix by your onw!if u want to show it at the position u want!
		void				RenderCurve(Vector4 e_vColor = Vector4::One,cMatrix44 e_mat = cMatrix44::Identity,float e_fLineWidth = 2.f);
		//
		void				RenderPoint(float e_fSize = 5,int e_iColorR = 255,int e_iColorG = 255,int e_iColorB = 255);
		//only for debug or edit mode
		void				RenderPointIndex();
		//
		void				AddPoint(Vector3 e_vPos);
		//
		void				InsertPoint(Vector3 e_vPos,int e_iIndex);
		//
		virtual	void		DelPoint(int e_iIndex);
		//erase old point replace new one
		void				FixPoint(int e_iIndex,Vector3 e_vPos);
		//return closest one point,if point with desire position is smaller than offset valuse 
		int					GetClosetPointIndex(int e_x,int e_y,int e_z);
		//it could be add a offset to all points
		void				AddOffsetToAllPoint(Vector3 e_vOffset);
		//
		void				Destroy();
		//if the distance of tje closest point is bigger than e_fOffsetDis,it will return -1
		int					GetClosestPointIndex(Vector3 e_vPos,float e_fOffsetDis = 20.f);
		//if points data is zero return vector::zero
		Vector3				GetCurveCenter(float*e_pfRightDownToLeftUpperLength = 0);
		//scale all curve as vector(length as 1),then resize curve
		void				Scale(float e_fScale);
		//
		void				ScaleByDistance(float e_fDistance);
		//
		int					FinalListIndexToOriginalIndex(int e_iFinalListIndex);
		//
		void				TransformCurve(cMatrix44 e_mat);
		void				Translate(Vector3 e_vPos);
		void				RotateCurveWithCurveCenter(cMatrix44 e_mat);
		void				RotateCurveWithSpecificCenterPosition(Vector3 e_vCenterPos,Quaternion e_Quaternion);
		float				GetTotalDistance();
		Vector3				GetLastPoint();
		Vector3				GetInterplotValue(float e_fInterpolationValue);
		//could reuse cCurve to get interpolation value
		static	cCurve		GetInterplotValue(Vector3 e_vStart,Vector3 e_vEnd,int e_iLOD,float e_fInterpolationValue,Vector3*e_vResult);
	};
	//=====================
	//each point has one time line,and the point is next to pervious one the time of next point is always bigger than pervious one
	//ex::POINT1( XY(0,0) ,Time(0)  ),POINT2( XY(10,10) ,Time(0.3)  ),POINT3( XY(30,30) ,Time(0.9)  ).......
	//the next point always has bigger time value than pervious,or it will occur some sort error while using GetCurrentPosition
	//=====================
	class	cCurveWithTime:public cCurve,public cFMTimeLineAnimationRule
	{
	protected:
		//inherit from cFMTimeLineAnimationRule
		virtual	void	InternalInit()override;
		virtual	void	InternalUpdate(float e_fElpaseTime)override;
		virtual	void	InternalRender()override;
		//
		Vector3		m_vMovedCoordinate;
		//correspond LOD for finalpointlist
		void	IncreaseTimeLod();
		//correspond LOD for finalpointlist
		void	DecreaseTimeLod();
		//the size as many as m_OriginalPointList.size,or it could be occur error
		GET_SET_DEC(std::vector<float>,m_OriginalTimeList,GetOriginalTimeList,SetOriginalTimeList);
		//the size as many as m_FinalPointList.size,or it could be occur error
		GET_SET_DEC(std::vector<float>,m_FinalTimeList,GetTimeList,SetTimeList);
		//current time trace step
		float	m_fCurrentTime;
		//new position by update		
		Vector3	m_vCurrentPosition;
		Vector3	m_vPreviousPosition;
		//if curent time is bigger than last point's time it's done
		//GET_SET_DEC(bool,m_bCurveMoveDone,IsCurveMoveDone,SetCurveMoveDone);
		//quickly to search index
		int m_iCurrentPointIndex;
		//let us know is it reach a new point index
		int	m_iPerviousPointIndex;
		//true for loopable show line
		GET_SET_DEC(bool,m_bCurveLoop,IsCurveLoop,SetCurveLoop);
		//while time is changed call this
		virtual	void	DoLOD()override;
		Vector3	m_vCurrentDirection;
		//if we need to calculate angle set it to true
		GET_SET_DEC(bool,m_bCalculateAngle,IsCalAngle,SetCalAngle);
		//previous angle so we could do interlopte angle let calculate angle smmoth.
		float	m_fPreviousPosToNextPointAngle;
		//angle in radian
		GET_SET_DEC(float,m_fCurrentPosToNextPointAngle,GetCurrentPosToNextPointAngle,SetCurrentPosToNextPointAngle);
		//m_fCurrentLERPTime = the interval time to next point/2 point time gap.
		GET_SET_DEC(float,m_fCurrentLERPTime,GetCurrentLERPTime,SetCurrentLERPTime);
		//
		void		MovedDistanceCalculate(float e_fElpaseTime);

	public:
		//
		DEFINE_TYPE_INFO()
		//======================
		cCurveWithTime();
		cCurveWithTime(cCurveWithTime*e_pCurveWithTime);
		cCurveWithTime(TiXmlElement*e_pTiXmlElement);
		CLONE_MYSELF(cCurveWithTime);
		virtual ~cCurveWithTime();
		virtual	TiXmlElement*		ToTiXmlElement()override;
		virtual bool				ProcessTiXmlElement(TiXmlElement*e_pTiXmlElement);
		//inherit from cFMTimeLineAnimationRule
		virtual	void	RearrangeTime(float e_fNewMaxTime)override;
		virtual	void	RearrangeTimeByPercent(float e_fPercenttage)override;
		virtual	void	UpdateByGlobalTime(float e_fGlobalTime)override;
		virtual	void	RenderByGlobalTime(float e_fTime)override;
		virtual	void	InvertOrder()override;
		virtual	float	GetEndTime()override;//float			GetLastTime();
		virtual	void	Destroy()override;//void	Clear();
		//
		//add point by speficif time and position
		virtual	void	AddPoint(Vector3 e_vPos,float e_fTime);
		virtual	void	DelPoint(int e_iIndex)override;
		void			InsertPoint(Vector3 e_vPos,float e_fTime,int e_iIndex);
		//erase old point replace new one
		void			FixPoint(int e_iIndex,Vector3 e_vPos,float e_fTime);
		void			FixPoint(int e_iIndex,Vector3 e_vPos);
		virtual bool	SetLOD(int e_iLOD)override;
		//while data is modified without function(FixPoint,AddPoint),call this to refresh data		
		void			Refresh(){ DoLOD(); }
		//let origina time and original position become as final data,
		virtual	void	SetOriginalToFinal()override;
		//
		//virtual	void	ReplacePointsByCount(int e_iNumPoint);
		//performance is very very bad...it should only running under win32,not retail version
		virtual	void	Render(Vector4 e_vColor = Vector4::One,cMatrix44 e_mat = cMatrix44::Identity);
		//
		float			GetCurrentTime(){ return this->m_fCurrentTime; }
		//while immediately to change the current time is could possible change the current point index,so we have st current point index to 0,to research close one point
		void			SetCurrentTime(float e_fCurrentTime);
		//get position by path
		Vector3			GetClosePoint();
		//Vector3			GetClosePointByLODList():
		//assume there is no LOD or set lod as original,because I am lazy to to that.
		Vector3			GetPositionByTime(float e_fTime);
		bool			IsReachNewIndex(){ return this->m_iPerviousPointIndex!=this->m_iCurrentPointIndex; }
		//averagely to assign the target time
		void			AverageTimeAssign(float e_fTime);
		int				GetCurrentWorkingFinalListIndex(){return m_iCurrentPointIndex;}
		//the current direciton 
		Vector3			GetCurrentDirection(){return m_vCurrentDirection;}
		//
		//
		//THE FUNCTION BLOEW ARE NOT OPTMIZE MAKE USRE U JUST CALL ONCE EACH FRAME
		//
		//
		//make sure the data has sort from small to big.
		//OriginalTimeList not FinalTimeList
		//-1 is out of range
		int				GetTimeRelativeIndex(float e_fTargetTime);
		//if update is called we have a m_iCurrentPointIndex,but it's working for final list but we want original index instead
		int				GetCurrentWorkingOriginalIndex();
		//
		//
		//BELOW FUNCTION ONLY WORKS WHILE U ARE CALLING UPDATE CORRECTLY
		//
		//get the distance we have moved,if u like to get the speed u have to do as this:  GetMovedDistance()/elpaseTime
		float			GetMovedDistance();
		Vector3			GetMovedCoordinate();
		//get last time move's speed
		float			GetSpeedBySecond(float e_fTime);
		float			GetSpeedByMinute(float e_fTime);
		float			GetSpeedByHour(float e_fTime);
		//a vector where a vector present by target index position and next position
		float			GetAngleByFinalListIndex(int e_iIndex);
		float			GetVelocity(float e_fTargetTime);
		//input length from current position to relative current direction
		//if 0 there is no offsrt
		cMatrix44		GetTransformWithOffsetPos(Vector3 e_vOffsetPos,float e_fLength);
		float			GetRestTime();
		//current path position
		Vector3			GetCurrentPosition(){ return m_vCurrentPosition; }
		void			SetCurrentPosition(Vector3 e_vPos);
		cCurveWithTime	GetOffsetCurve(float e_fStartTimeOffset,UT::sMinMaxData<float> e_fDirectionOffsetX,UT::sMinMaxData<float> e_fDirectionOffsetY,int e_iLOD);
	};
}
#endif
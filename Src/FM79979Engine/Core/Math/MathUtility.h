#ifndef _MATH_UTILITY_H_
#define _MATH_UTILITY_H_

#ifdef WIN32
#pragma warning( disable : 4996 )
#endif

#include <map>
#include <vector>
#include "../Common/VS_TO_GNN.h"
#include "../Common/CommonDefine.h"
#include "Vector4.h"
#include "Matrix44.h"
#include "MathTemplateUtility.h"
//#include "LinerTemplateDataProcess.h"
namespace UT
{
	//get a distance releste from now to start between a range.
	//situation1.
	//    Start      TargetPos   CurrentPos       End
	//situation2.
	//    Start		  CurrentPos  TargetPos	      End
	inline	float		GetDistanceFromCurrentToStartOfLoop(float e_fPosStart,float e_fPosEnd,float e_fCurrentPos,float e_fTarget){if( e_fTarget>e_fCurrentPos ){return e_fTarget-e_fCurrentPos;}return (e_fPosEnd-e_fCurrentPos)+(e_fTarget-e_fPosStart);}
//#endif
	//Modulus should change to remainder.
	inline	float		GetFloatModulus(float e_fValue,float e_fDivision){int	l_iModulus = (int)(e_fValue/e_fDivision);return e_fValue-((float)l_iModulus*e_fDivision);}
	inline	int			ValueDivideAddOneIfModulusIsNotZero(int e_iValue,int e_iDividValue)
	{
		int l_iResult = e_iValue/e_iDividValue;
		if(e_iValue%e_iDividValue)
			l_iResult += 1;
		return l_iResult;
	}
	//count for how	 many do we want?the max value for random range,
	//below is a sample to generate a 01234 random table
	//vector<int>	l_uiAllMagicIDVector1 = GenerateRandomTable(5,5);
	std::vector<int>	GenerateRandomTable(int e_iCount,int e_iMaxValue);
	//the number could be same
	std::vector<int>	GenerateOverlapableRandomTable(int e_iCount,int e_iMaxValue);
	Vector3				AngleToDirection(float e_fAngle);
	float				GetRadianBy2Point(float e_SrcX,float e_SrcY,float e_DestX,float e_DestY);
	//Y up			is 0,RightX is -90,return value in degree
	float				GetAngleBy2Point(float e_SrcX,float e_SrcY,float e_DestX,float e_DestY);
	//only			compare xy.,return value in degree
	float				GetAngleBy2Point(Vector3 e_vSrc,Vector3 e_vDest);

	//to fi			nd out the closest point from a vector,if the distance is smaller than e_fOffsetDis,the condition is success
	int					GetClosestPointIndex(Vector2 e_vPos,std::vector<Vector2>*e_pPosVector,float e_fOffsetDis = 20.f);
	int					GetClosestPointIndex(Vector3 e_vPos,std::vector<Vector3>*e_pPosVector,float e_fOffsetDis = 20.f);
	int					GetClosestPointIndex(float e_fValue,std::vector<float>*e_pPosVector,float e_fOffsetDis = 20.f);
	int					GetClosestPointIndex(Vector4 e_fValue,std::vector<Vector4 >*e_pPosVector,float e_fOffsetDis = 20.f);

	Vector2				GetCurveCenter(std::vector<Vector2>*e_pDataVector,float*e_pfRightDownToLeftUpperLength = 0);
	Vector3				GetCurveCenter(std::vector<Vector3>*e_pDataVector,float*e_pfRightDownToLeftUpperLength = 0);
	Vector4				GetCurveCenter(std::vector<Vector4>*e_pDataVector,float*e_pfRightDownToLeftUpperLength = 0);
	Vector4				GetBiggerRect(Vector4 e_vOld,Vector4 e_vNew);

	inline	POINT		operator+=(POINT e_Point1,POINT e_Point2){e_Point1.x+=e_Point2.x;e_Point1.y+=e_Point2.y;return e_Point1;}
	inline	POINT		operator-=(POINT e_Point1,POINT e_Point2){e_Point1.x-=e_Point2.x;e_Point1.y-=e_Point2.y;return e_Point1;}
	inline	POINT		operator+(POINT e_Point1,POINT e_Point2){e_Point1.x+=e_Point2.x;e_Point1.y+=e_Point2.y;return e_Point1;}
	inline	POINT		operator-(POINT e_Point1,POINT e_Point2){e_Point1.x-=e_Point2.x;e_Point1.y-=e_Point2.y;return e_Point1;}
	//hexadecimal
	//multiply
	inline	int64		BitMoveLeft(int64 e_iValue,int ShiftCount){ShiftCount+=1;while(--ShiftCount){e_iValue *= 2;}return e_iValue;}
	//divide
	inline	int64		BitMoveRight(int64 e_iValue,int ShiftCount){ShiftCount+=1;while(--ShiftCount){e_iValue /= 2;}return e_iValue;}

	inline	int			power_of_two(int input){int value = 1;while ( value < input )	value <<= 1;return value;}

	float				CalculateDifferenceBetweenRadius(float e_fSrc,float e_fTarget);

	float				CalculateDifferenceBetweenAngles(float e_fSrc,float e_fTarget);
	//get th			e percetange has been past between 2 value
	float				GetTimeGapLERP( float e_fStart,float e_fEnd,float e_fCurrent );
	Vector3				rand(Vector3 e_vMin,Vector3 e_vMax);
	Vector2				rand(Vector2 e_vMin,Vector2 e_vMax);
	int					rand(int	e_iMin,	int		e_iMax);
	float				rand(float	e_fMin,	float	e_fMax);
	POINT				rand(POINT	e_Min,	POINT	e_Max);
#ifndef RAND_MAX
#define RAND_MAX 0x7fff
#endif
	float				frand( float fMin, float fMax );
//end namespace UT
}

#endif
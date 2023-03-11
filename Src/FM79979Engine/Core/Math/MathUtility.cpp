#include "MathUtility.h"
#include "assert.h"
#include "MathTemplateUtility.h"
namespace UT
{
	std::vector<int>	GenerateRandomTable(int e_iCount,int e_iMaxValue)
	{
		vector<int>	l_uiAllMagicIDVector;
		l_uiAllMagicIDVector.reserve(e_iCount);
		if( e_iMaxValue == e_iCount || e_iMaxValue/2 <= e_iCount )
		{
			vector<int>	l_uiAllValueVector;
			l_uiAllValueVector.reserve(e_iMaxValue);
			for( int i=0;i<e_iMaxValue;++i )
			{
				l_uiAllValueVector.push_back(i);
			}
			while( (int)l_uiAllMagicIDVector.size() != e_iCount )
			{
				int	l_iIndex = rand(0,(int)l_uiAllValueVector.size()-1);
				l_uiAllMagicIDVector.push_back(l_uiAllValueVector[l_iIndex]);
				l_uiAllValueVector.erase(l_uiAllValueVector.begin()+l_iIndex);
			}
		}
		else
		{
			while((int)l_uiAllMagicIDVector.size() != e_iCount )
			{
				int	l_iMagicID = ::rand()%(e_iMaxValue);
				bool	l_bSameMagicID = false;
				for( UINT j=0;j<l_uiAllMagicIDVector.size();j++ )
				{
					if( l_uiAllMagicIDVector[j] == l_iMagicID )
					{					
						l_bSameMagicID = true;
						break;
					}
				}
				if( !l_bSameMagicID )
					l_uiAllMagicIDVector.push_back(l_iMagicID);
			}
		}
		return l_uiAllMagicIDVector;
	}

	std::vector<int>	GenerateOverlapableRandomTable(int e_iCount,int e_iMaxValue)
	{
		vector<int>	l_uiAllMagicIDVector;
		l_uiAllMagicIDVector.reserve(e_iCount);
		while((int)l_uiAllMagicIDVector.size() != e_iCount )
		{
			int	l_iMagicID = ::rand()%(e_iMaxValue);
			l_uiAllMagicIDVector.push_back(l_iMagicID);
		}
		return l_uiAllMagicIDVector;
	}

	float	GetRadianBy2Point(float e_SrcX,float e_SrcY,float e_DestX,float e_DestY)
	{
		float l_fX = e_DestX - e_SrcX;
		float l_fY = e_DestY - e_SrcY;
		return (float)atan2(l_fX,l_fY);
		//just using atan2 instead below code

		//if( abs(l_fX)<1E-10 && l_fY<1E-10 )
		//	return 0;
		//if( l_fY>l_fX )
		//{
		//	if( l_fY>0 )
		//		return D3DXToDegree(atan(l_fX/l_fY));
		//	return D3DXToDegree(atan(l_fX/l_fY)+D3DX_PI);
		//}
		//if( l_fX>0 )
		//	return D3DXToDegree(0.5f*D3DX_PI-atan(l_fY/l_fX));
		//return D3DXToDegree(-0.5f*D3DX_PI-atan(l_fY/l_fX));
	}

	float	GetAngleBy2Point(Vector3 e_vSrc,Vector3 e_vDest)
	{
		//180 for Z invert
		//return 180-D3DXToDegree(atan2(l_fX,l_fY));
		return D3DXToDegree(GetRadianBy2Point(e_vSrc.x,e_vSrc.y,e_vDest.x,e_vDest.y));
	}

	float GetAngleBy2Point(float e_SrcX,float e_SrcY,float e_DestX,float e_DestY)
	{
		return D3DXToDegree(GetRadianBy2Point(e_SrcX,e_SrcY,e_DestX,e_DestY));
	}

	Vector3	AngleToDirection(float e_fAngle)
	{
		cMatrix44	l_mat = cMatrix44::ZAxisRotationMatrix(D3DXToRadian(e_fAngle));
		return l_mat.GetAxis(MyMath::X);
	}

	int		GetClosestPointIndex(Vector2 e_vPos,std::vector<Vector2>*e_pPosVector,float e_fOffsetDis)
	{
		int	l_iFocusPoint = -1;
		Vector2	l_vMousePos = e_vPos;
		std::vector<Vector2> l_vPointList = *e_pPosVector;
		float	l_fClosetDistance = e_fOffsetDis;
		int	l_iLength = (int)l_vPointList.size();
		for( int i=0;i<l_iLength;++i )
		{
			Vector2 l_vPos = l_vPointList[i];
			Vector2	l_vDis = l_vPos-l_vMousePos;
			float	l_fDis = l_vDis.Length();
			if( l_fClosetDistance>l_fDis )
			{
				l_fClosetDistance = l_fDis;
				l_iFocusPoint = i;
			}
		}
		return l_iFocusPoint;
	}

	int		GetClosestPointIndex(Vector3 e_vPos,std::vector<Vector3>*e_pPosVector,float e_fOffsetDis)
	{
		int	l_iFocusPoint = -1;
		Vector3	l_vMousePos = e_vPos;
		std::vector<Vector3> l_vPointList = *e_pPosVector;
		float	l_fClosetDistance = e_fOffsetDis;
		int	l_iLength = (int)l_vPointList.size();
		for( int i=0;i<l_iLength;++i )
		{
			Vector3 l_vPos = l_vPointList[i];
			Vector3	l_vDis = l_vPos-l_vMousePos;
			float	l_fDis = l_vDis.Length();
			if( l_fClosetDistance>l_fDis )
			{
				l_fClosetDistance = l_fDis;
				l_iFocusPoint = i;
			}
		}
		return l_iFocusPoint;
	}

	int		GetClosestPointIndex(float e_fValue,std::vector<float>*e_pPosVector,float e_fOffsetDis)
	{
		int	l_iFocusPoint = -1;
		float	l_vMousePos = e_fValue;
		std::vector<float> l_vPointList = *e_pPosVector;
		float	l_fClosetDistance = e_fOffsetDis;
		int	l_iLength = (int)l_vPointList.size();
		for( int i=0;i<l_iLength;++i )
		{
			float	 l_vPos = l_vPointList[i];
			float	l_vDis = l_vPos-l_vMousePos;
			float	l_fDis = l_vDis;
			if( l_fClosetDistance>l_fDis )
			{
				l_fClosetDistance = l_fDis;
				l_iFocusPoint = i;
			}
		}
		return l_iFocusPoint;
	}
	int		GetClosestPointIndex(Vector4 e_fValue,std::vector<Vector4 >*e_pPosVector,float e_fOffsetDis)
	{
		int	l_iFocusPoint = -1;
		Vector4	l_vMousePos = e_fValue;
		std::vector<Vector4> l_vPointList = *e_pPosVector;
		float	l_fClosetDistance = e_fOffsetDis;
		int	l_iLength = (int)l_vPointList.size();
		for( int i=0;i<l_iLength;++i )
		{
			Vector4	l_vPos = l_vPointList[i];
			Vector4	l_vDis = l_vPos-l_vMousePos;
			float	l_fDis = Vector4Length(l_vDis).x;
			if( l_fClosetDistance>l_fDis )
			{
				l_fClosetDistance = l_fDis;
				l_iFocusPoint = i;
			}
		}
		return l_iFocusPoint;
	}
	Vector2				GetCurveCenter(std::vector<Vector2>*e_pDataVector,float*e_pfRightDownToLeftUpperLength)
	{
		int	l_iNum = (int)e_pDataVector->size();
		if( l_iNum)
		{
			Vector2	l_veLeftUp = (*e_pDataVector)[0];
			Vector2	l_veRightDown = l_veLeftUp;
			for( int i=1;i<l_iNum;++i )
			{
				Vector2	l_vPos = (*e_pDataVector)[i];
				if( l_veLeftUp.x>l_vPos.x )
					l_veLeftUp.x = l_vPos.x;
				if( l_veLeftUp.y>l_vPos.y )//2d so y up is getting small and small
					l_veLeftUp.y = l_vPos.y;
				if( l_veRightDown.x<l_vPos.x )
					l_veRightDown.x = l_vPos.x;
				if( l_veRightDown.y<l_vPos.y )
					l_veRightDown.y = l_vPos.y;
			}
			if( e_pfRightDownToLeftUpperLength )
				*e_pfRightDownToLeftUpperLength = (l_veRightDown-l_veLeftUp).Length();
			return (l_veLeftUp+l_veRightDown)/2.f;
		}
		return Vector2::Zero;
	}
	Vector3				GetCurveCenter(std::vector<Vector3>*e_pDataVector,float*e_pfRightDownToLeftUpperLength)
	{
		int	l_iNum = (int)e_pDataVector->size();
		if( l_iNum)
		{
			Vector3	l_veLeftUp = (*e_pDataVector)[0];
			Vector3	l_veRightDown = l_veLeftUp;
			for( int i=1;i<l_iNum;++i )
			{
				Vector3	l_vPos = (*e_pDataVector)[i];
				if( l_veLeftUp.x>l_vPos.x )
					l_veLeftUp.x = l_vPos.x;
				if( l_veLeftUp.y>l_vPos.y )//2d so y up is getting small and small
					l_veLeftUp.y = l_vPos.y;
				if( l_veLeftUp.z>l_vPos.z )
					l_veLeftUp.z = l_vPos.z;
				if( l_veRightDown.x<l_vPos.x )
					l_veRightDown.x = l_vPos.x;
				if( l_veRightDown.y<l_vPos.y )
					l_veRightDown.y = l_vPos.y;
				if( l_veRightDown.z<l_vPos.z )
					l_veRightDown.z = l_vPos.z;
			}
			if( e_pfRightDownToLeftUpperLength )
				*e_pfRightDownToLeftUpperLength = (l_veRightDown-l_veLeftUp).Length();
			return (l_veLeftUp+l_veRightDown)/2.f;
		}
		return Vector3::Zero;
	}

	Vector4				GetCurveCenter(std::vector<Vector4>*e_pDataVector,float*e_pfRightDownToLeftUpperLength)
	{
		int	l_iNum = (int)e_pDataVector->size();
		if( l_iNum)
		{
			Vector4	l_veLeftUp = (*e_pDataVector)[0];
			Vector4	l_veRightDown = l_veLeftUp;
			for( int i=1;i<l_iNum;++i )
			{
				Vector4	l_vPos = (*e_pDataVector)[i];
				if( l_veLeftUp.x>l_vPos.x )
					l_veLeftUp.x = l_vPos.x;
				if( l_veLeftUp.y>l_vPos.y )//2d so y up is getting small and small
					l_veLeftUp.y = l_vPos.y;
				if( l_veLeftUp.z>l_vPos.z )
					l_veLeftUp.z = l_vPos.z;
				if( l_veLeftUp.w>l_vPos.w )
					l_veLeftUp.w = l_vPos.w;
				if( l_veRightDown.x<l_vPos.x )
					l_veRightDown.x = l_vPos.x;
				if( l_veRightDown.y<l_vPos.y )
					l_veRightDown.y = l_vPos.y;
				if( l_veRightDown.z<l_vPos.z )
					l_veRightDown.z = l_vPos.z;
				if( l_veRightDown.w<l_vPos.w )
					l_veRightDown.w = l_vPos.w;
			}
			if( e_pfRightDownToLeftUpperLength )
				*e_pfRightDownToLeftUpperLength = (l_veRightDown-l_veLeftUp).Length();
			return (l_veLeftUp+l_veRightDown)/2.f;
		}
		return Vector4::Zero;
	}

	Vector4	GetBiggerRect(Vector4 e_vOld,Vector4 e_vNew)
	{
		if( e_vOld.x > e_vNew.x )			e_vOld.x = e_vNew.x;
		if( e_vOld.y > e_vNew.y )			e_vOld.y = e_vNew.y;
		if( e_vOld.z < e_vNew.z )			e_vOld.z = e_vNew.z;
		if( e_vOld.w < e_vNew.w )			e_vOld.w = e_vNew.w;
		return e_vOld;
	}

	float	CalculateDifferenceBetweenRadius(float e_fSrc,float e_fTarget)
	{
		e_fSrc = GetFloatModulus(e_fSrc,D3DX_PI*2);
		e_fTarget = GetFloatModulus(e_fTarget,D3DX_PI*2);
		float	l_fDifferenceRadian = e_fTarget-e_fSrc;
		while (l_fDifferenceRadian < -D3DX_PI)
			l_fDifferenceRadian += D3DX_PI*2;
		while (l_fDifferenceRadian > D3DX_PI)
			l_fDifferenceRadian -= D3DX_PI*2;
		return l_fDifferenceRadian;
	}

	float	CalculateDifferenceBetweenAngles(float e_fSrc,float e_fTarget)
	{
		float	l_fDifferenceAngle = e_fTarget-e_fSrc;
		while (l_fDifferenceAngle < -180)
			l_fDifferenceAngle += 360;
		while (l_fDifferenceAngle > 180)
			l_fDifferenceAngle -= 360;
		return l_fDifferenceAngle;
	}
	//get the percetange has been past between 2 value
	float	GetTimeGapLERP( float e_fStart,float e_fEnd,float e_fCurrent )
	{
		float	l_fGap = e_fEnd-e_fStart;
		assert(l_fGap>0.f&&"soory I am lazy to do safe function here...");
		float	l_fPast = e_fCurrent-e_fStart;
		//it's possible over than 1
		return l_fPast/l_fGap;
	}

	Vector3	rand(Vector3 e_vMin,Vector3 e_vMax)
	{
		return Vector3(frand(e_vMin.x,e_vMax.x),
		frand(e_vMin.y,e_vMax.y),
		frand(e_vMin.z,e_vMax.z));
	}

	Vector2	rand(Vector2 e_vMin,Vector2 e_vMax)
	{
		return Vector2(frand(e_vMin.x,e_vMax.x),frand(e_vMin.y,e_vMax.y));
	}
	int	rand(long	e_iMin,	long	e_iMax)
	{
		return rand( (int)e_iMin,(int) e_iMax );
	}
	int rand( int e_iMin,int e_iMax )
	{
		int	l_iRandValue = (e_iMax-e_iMin);
		if( l_iRandValue == 0 )
			return e_iMin;
		int l_iVal = ::rand()%l_iRandValue;
		return e_iMin+l_iVal;
	}

	float rand( float fMin, float fMax )
	{
		return frand(fMin,fMax);
	}

	POINT	rand(POINT	e_Min,	POINT	e_Max)
	{
		POINT	l_Point = {rand((int)e_Min.x,(int)e_Max.x),rand((int)e_Min.y,(int)e_Max.y)};
		return l_Point;
	}
	float frand( float fMin, float fMax )
	{
		double fRange = fMax - fMin;
		double fVal = ( (double)::rand() * fRange ) / (double)RAND_MAX;
		return (float)fVal + fMin;
	}
//end namespace UT
}
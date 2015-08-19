#include "../stdafx.h"
#include "PuzzleImage.h"
#include "SubMPDIRenderPointData.h"

namespace FATMING_CORE
{
	Vector2 sTexBehaviorDataWithImageIndexData::GetScale()
	{
        sPuzzleData*l_pPIData = pPI->GetPuzzleData()[iImageIndex];
        return Vector2(Size.x/l_pPIData->Size.x,Size.y/l_pPIData->Size.y);
	}
	sTexBehaviorDataWithImageIndexData::sTexBehaviorDataWithImageIndexData()
	{	
		Size.x = Size.y = 0.f;
		vColor = Vector4::One;
		iImageIndex = -1;
		bMirror = false;
		pPI = 0;
		vAngle = Vector3::Zero;
	}
	sTexBehaviorDataWithImageIndexData::sTexBehaviorDataWithImageIndexData(sTexBehaviorDataWithImageIndexData*e_pTexBehaviorDataWithImageIndexData)
	{
		Size = e_pTexBehaviorDataWithImageIndexData->Size;
		vAngle = e_pTexBehaviorDataWithImageIndexData->vAngle;
		vColor = e_pTexBehaviorDataWithImageIndexData->vColor;
		iImageIndex = e_pTexBehaviorDataWithImageIndexData->iImageIndex;
		bMirror = e_pTexBehaviorDataWithImageIndexData->bMirror;
		pPI = e_pTexBehaviorDataWithImageIndexData->pPI;
	}
	sTexBehaviorDataWithImageIndexData::sTexBehaviorDataWithImageIndexData(Vector2 e_Size,Vector3 e_vAngle,Vector4 e_vColor,int e_iImageIndex,bool e_bMirror,cPuzzleImage*e_pPI)
	{
		Size = e_Size;
		vAngle = e_vAngle;
		vColor = e_vColor;
		iImageIndex = e_iImageIndex;
		bMirror = e_bMirror;
		pPI = e_pPI;
	}

	sPuzzleData*        sTexBehaviorDataWithImageIndexData::GetPuzzleData()
	{
		if( pPI->Count() > iImageIndex )
			return pPI->GetPuzzleData()[iImageIndex];
		return 0;
	}

	//Vector2	sTexBehaviorDataWithImageIndexData::GetSize()
	//{
	//	sPuzzleData*l_psPuzzleData = this->GetPuzzleData();
	//	return Vector2(l_psPuzzleData->Size.x*Scale.x,l_psPuzzleData->Size.y*Scale.y);
	//}

    void    sTexBehaviorDataWithImageIndexData::DumpData(sTexBehaviorDataWithImageIndexData*e_pDest)
    {
		e_pDest->Size = Size;
		e_pDest->vAngle = vAngle;
		e_pDest->vColor = vColor;
		e_pDest->iImageIndex = iImageIndex;
		e_pDest->bMirror = bMirror;
		e_pDest->pPI = pPI;
		//ensure the hit point data is exists!
		e_pDest->bHintPoint = bHintPoint;
    }


	wchar_t*	ValueToStringW(eImageAnchorType e_eAnchorType)
	{
		switch(e_eAnchorType)
		{
			case eIAT_LEFT_UP:
				return L"LeftUp";
			case eIAT_CENTER_UP:
				return L"CenterUp";
			case eIAT_RIGHT_UP:
				return L"RightUp";
			case eIAT_LEFT_CENTER:
				return L"LeftCenter";
			case eIAT_CENTER_CENTER:
				return L"CenterCenter";
			case eIAT_RIGHT_CENTER:
				return L"RightCenter";
			case eIAT_LEFT_DOWN:
				return L"LeftDown";
			case eIAT_CENTER_DOWN:
				return L"CenterDown";
			case eIAT_RIGHT_DOWN:
				return L"RightDown";
		}
		return L"unknow";
	}

	#define	STRING_COMPARE(p,q) 	if( !wcscmp(p,q) )

	eImageAnchorType	GetAnchorType(const wchar_t*e_str)
	{
		STRING_COMPARE(e_str,L"LeftUp")
			return eIAT_LEFT_UP;
		STRING_COMPARE(e_str,L"CenterUp")
			return eIAT_CENTER_UP;
		STRING_COMPARE(e_str,L"RightUp")
			return eIAT_RIGHT_UP;
		STRING_COMPARE(e_str,L"LeftCenter")
			return eIAT_LEFT_CENTER;
		STRING_COMPARE(e_str,L"CenterCenter")
			return eIAT_CENTER_CENTER;
		STRING_COMPARE(e_str,L"RightCenter")
			return eIAT_RIGHT_CENTER;
		STRING_COMPARE(e_str,L"LeftDown")
			return eIAT_LEFT_DOWN;
		STRING_COMPARE(e_str,L"CenterDown")
			return eIAT_CENTER_DOWN;
		STRING_COMPARE(e_str,L"RightDown")
			return eIAT_RIGHT_DOWN;
		return eIAT_MAX;
	}
	Vector4				GetExtendImagePoint(eImageAnchorType e_eImageAnchorType,Vector2 e_vHalfImageSize,Vector2 e_vImageSize,float e_fScale)
	{
		//assume left up potin is anchor point to assign data.
		Vector4 l_vEntendImagePoint(0,0,0,0);
		switch(e_eImageAnchorType)
		{
			case eIAT_LEFT_UP:
				l_vEntendImagePoint.x = 0.f;
				l_vEntendImagePoint.y = 0.f;
				l_vEntendImagePoint.z = e_vImageSize.x;
				l_vEntendImagePoint.w = e_vImageSize.y;
				break;
			case eIAT_CENTER_UP:
				l_vEntendImagePoint.x = -e_vHalfImageSize.x;
				l_vEntendImagePoint.y = 0.f;
				l_vEntendImagePoint.z = e_vHalfImageSize.x;
				l_vEntendImagePoint.w = e_vImageSize.y;
				break;
			case eIAT_RIGHT_UP:
				l_vEntendImagePoint.x = -e_vImageSize.x;
				l_vEntendImagePoint.y = 0.f;
				l_vEntendImagePoint.z = 0;
				l_vEntendImagePoint.w = e_vImageSize.y;
				break;
			case eIAT_LEFT_CENTER:
				l_vEntendImagePoint.x = 0.f;
				l_vEntendImagePoint.y = -e_vHalfImageSize.y;
				l_vEntendImagePoint.z = e_vImageSize.x;
				l_vEntendImagePoint.w = e_vHalfImageSize.y;
				break;
			case eIAT_CENTER_CENTER:
				l_vEntendImagePoint.x = -e_vHalfImageSize.x;
				l_vEntendImagePoint.y = -e_vHalfImageSize.y;
				l_vEntendImagePoint.z = e_vHalfImageSize.x;
				l_vEntendImagePoint.w = e_vHalfImageSize.y;
				break;
			case eIAT_RIGHT_CENTER:
				l_vEntendImagePoint.x = -e_vImageSize.x;
				l_vEntendImagePoint.y = -e_vHalfImageSize.y;
				l_vEntendImagePoint.z = 0;
				l_vEntendImagePoint.w = e_vHalfImageSize.y;
				break;
			case eIAT_LEFT_DOWN:
				l_vEntendImagePoint.x = 0;
				l_vEntendImagePoint.y = -e_vImageSize.y;
				l_vEntendImagePoint.z = e_vImageSize.x;
				l_vEntendImagePoint.w = 0;
				break;
			case eIAT_CENTER_DOWN:
				l_vEntendImagePoint.x = -e_vHalfImageSize.x;
				l_vEntendImagePoint.y = -e_vImageSize.y;
				l_vEntendImagePoint.z = e_vHalfImageSize.x;
				l_vEntendImagePoint.w = 0;
				break;
			case eIAT_RIGHT_DOWN:
				l_vEntendImagePoint.x = -e_vImageSize.x;
				l_vEntendImagePoint.y = -e_vImageSize.y;
				l_vEntendImagePoint.z = 0;
				l_vEntendImagePoint.w = 0;
				break;
		}
		return l_vEntendImagePoint;
	}
}
#pragma once
#include "assert.h"
#include "../PuzzleImage/PuzzleImage.h"
namespace FATMING_CORE
{
	//==========
	//each point could has below data to do something
	//==========
	struct	sTexBehaviorDataWithImageIndexData
	{
		Vector2	Size;			//size
		Vector3	vAngle;			//angle
		Vector4	vColor;			//color
		int		iImageIndex;	//image index
		bool	bMirror;		//mirror
		bool	bHintPoint;		//is this point a cue?
		cPuzzleImage*pPI;       //the pi it used,related of iImageIndex
		cPuzzleImageUnit*   GetPuzzleImageUnit(){ return (*pPI)[iImageIndex]; }
		sPuzzleData*        GetPuzzleData();
		Vector2				GetScale();
		sTexBehaviorDataWithImageIndexData();
		sTexBehaviorDataWithImageIndexData(sTexBehaviorDataWithImageIndexData*e_pTexBehaviorDataWithImageIndexData);
		sTexBehaviorDataWithImageIndexData(Vector2 e_Size,Vector3 e_vAngle,Vector4 e_vColor,int e_iImageIndex,bool e_bMirror,cPuzzleImage*e_pPI);
        void    DumpData(sTexBehaviorDataWithImageIndexData*e_pDest);
		//Vector2	GetSize();
		inline	void	ToLerp(sTexBehaviorDataWithImageIndexData*e_pDest,float e_fLERP,sTexBehaviorDataWithImageIndexData*e_pSaveTo)
		{
			//because fucking 1 pixel problem,so we force it to be a intger value!
			e_pSaveTo->Size.x = (float)((int)LERP<float>(Size.x,e_pDest->Size.x,e_fLERP));
			e_pSaveTo->Size.y = (float)((int)LERP<float>(Size.y,e_pDest->Size.y,e_fLERP));
			e_pSaveTo->vAngle = LERP<Vector3>(vAngle,e_pDest->vAngle,e_fLERP);
			e_pSaveTo->vColor = LERP<Vector4>(vColor,e_pDest->vColor,e_fLERP);
			e_pSaveTo->iImageIndex = iImageIndex;
#ifdef DEBUG
			if( iImageIndex == -1 )
			{
				int a=0;
			}
#endif
			e_pSaveTo->bMirror = e_pDest->bMirror;
			e_pSaveTo->pPI = e_pDest->pPI;
		}
	};

	enum	eAnimationEvent
	{
		eAE_NONE = 0,//just a cue,let user detect and using.
		eAE_PARTICLE,
		eAE_SOUND,
		eAE_MAX,
	};

	struct	sHintPoint
	{
		int		iPointIndex;
		wchar_t	strEventName[MAX_PATH];//ensure all the size is n*MAX_PATH
		eAnimationEvent	AnimationEvent;
		sHintPoint(int e_iIndex, const wchar_t*e_str, eAnimationEvent e_eAnimationEvent);
	};

	enum	eImageAnchorType
	{
		eIAT_LEFT_UP = 0,
		eIAT_CENTER_UP,
		eIAT_RIGHT_UP,
		eIAT_LEFT_CENTER,
		eIAT_CENTER_CENTER,
		eIAT_RIGHT_CENTER,
		eIAT_LEFT_DOWN,
		eIAT_CENTER_DOWN,
		eIAT_RIGHT_DOWN,
		eIAT_MAX,
	};

	wchar_t*	ValueToStringW(eImageAnchorType e_eAnchorType);
	eImageAnchorType	GetAnchorType(const wchar_t*e_str);
	Vector4				GetExtendImagePoint(eImageAnchorType e_eImageAnchorType,Vector2 e_vImageSize,float e_fScale);
}

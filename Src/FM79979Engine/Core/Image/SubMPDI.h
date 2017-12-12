#ifndef SUB_MPDI
#define SUB_MPDI
#include "FMAnimationRule.h"
#include "PuzzleImage.h"
#include "BaseUVImageData.h"
#include "SubMPDIRenderPointData.h"
namespace FATMING_CORE
{
	class	cMultiPathDynamicImage;
	//================
	//cCurveWithTime with a new sTexBehaviorDataWithImageIndexData list vector,
	//bad design here because cCurveWithTime and cFMTimeLineAnimationRule both has a virtual function Start and update
	//if it occur ambigous make sure what function u real want
	//ex:((cFMTimeLineAnimationRule*)(l_p))->Update or ((cCurveWithTime*)(l_p))->Update
	//================
	class	cCueToStartCurveWithTime:public cCurveWithTime,public cMulti_PI_Image
	{
		struct	s2DVertex
		{
			Vector3		vPos[4];
			Vector2		fUV[4];
			//Vector4		vColor[4];
		};
		s2DVertex			m_2DVertices;
		//
		//cMatrix44			m_matRenderMatrix;
		friend class		cMultiPathDynamicImage;
		//the pos to anchor image size change
		GET_SET_DEC(eImageAnchorType,m_eAnchorType,GetAnchorType,SetAnchorType);
		GETP_SET_DEC(Vector3,m_vRotationAnglePosOffset,GetRotationAnglePosOffset,SetRotationAnglePosOffset);
		//GET_SET_DEC(float,m_fScale,GetScale,SetScale);
		//blending data,if all cCueToStartCurveWithTime under its parent MPDI,it could optmize to speed up,but I am not doing it right now,fatming,fuck
		GET_SET_DEC(bool,m_bColorBlending,IsColorBlending,SetColorBlending);
		GET_SET_DEC(GLenum,m_SrcBlendingMode,GetSrcBlendingMode,SetSrcBlendingMode);
		GET_SET_DEC(GLenum,m_DestBlendingMode,GetDestBlendingMode,SetDestBlendingMode);
		//some times we do not need offset position,
		//because the points is moving and offset position let image animation become weired.
		//GET_SET_DEC(bool,m_bAvtivePosOffset,IsAvtivePosOffset,SetAvtivePosOffset);
		//we not actually need to change size color or .....so we bkeep this flag to determinal update size or else
		GET_SET_DEC(bool,m_bBehaviorUpdateActive,IsBehaviorUpdateActive,SetBehaviorUpdateActive);
		GETP_SET_DEC(std::vector<sTexBehaviorDataWithImageIndexData*>,m_PointDataList,GetPointDataList,SetPointDataList);		//the image pading on,size as same as cCurveWithTime's original data,here using LERP to find out correspnd data
		GET_SET_DEC(sTexBehaviorDataWithImageIndexData*,m_pCurrentPointData,GetCurrentPointData,SetCurrentPointData);			//curren data we are going to using
		//for hint point
		bool				m_bHintPoint;
		int					m_iCurrentHintPointDataIndex;//if is true,we would go to set correspond index for m_iCurrentHintPointData,let user fetch it quickly
		//each node in the path is possible has been assign the hint point,ensure it has sorted,in the game,because we return the count that related current hint point
		GETP_SET_DEC(std::vector<sHintPoint>,m_HintPointVector,GetHintPointVector,SetHintPointVector);
		//find out the image index by CurrentPositionIndex
		int					CalculateCurrentPointIndex();
		virtual		void	UpdateData();
		virtual		void	InternalInit();
		virtual		void	InternalUpdate(float e_fElpaseTime);
		virtual		void	InternalRender();
		void  				GetRenderPuzzleDataAndMatrix(sTexBehaviorDataWithImageIndexData*e_pImageData,int e_iStride,float*e_pOutVertices,float*e_pOutTextureUV,sPuzzleData*e_pPuzzleData);
		bool				AssignColorAndTransformToVertices(Vector4 e_vInputColor,float*e_pfVertices, float*e_pfColor);
		//force to render,input the position u want to show,default m_vCurrentPos and m_vPos is working inside
		void				RenderPuzzleData(sPuzzleData*e_pPuzzleData);
		cMatrix44			GetConvertedWorldTransformIfParentRequireDoPositionOffsetToCenter();
	public:
		DEFINE_TYPE_INFO()
		CLONE_MYSELF(cCueToStartCurveWithTime);
		cCueToStartCurveWithTime();
		cCueToStartCurveWithTime(cMulti_PI_Image*e_pMulti_PI_Image);
		cCueToStartCurveWithTime(cCueToStartCurveWithTime*e_pCueToStartCurvesWithTime);
		virtual ~cCueToStartCurveWithTime();
		int	GetCurrentImageIndex() { return this->m_pCurrentPointData ? this->m_pCurrentPointData->iImageIndex : -1; }
		//prevent ambiguous
		inline	virtual void	Init(){ cFMTimeLineAnimationRule::Init(); }
		//prevent ambiguous
		inline	virtual void	Render(){ cFMTimeLineAnimationRule::Render(); }
		//prevent ambiguous
		inline	virtual void	Update(float e_fElpaseTime){ cFMTimeLineAnimationRule::Update(e_fElpaseTime); }
		virtual void			RenderFirsttFrameImage(bool e_bBlending,Vector4 e_vColor);
		virtual	Vector3			GetPositionByTime(float e_fGlobalTime);
		//virtual	void ReplacePointsByCount(int e_iNumPoint);
		//virtual	void	Render(Vector3 e_vRotateCenter = Vector3(0.f,0.f,0.f));
		virtual	void			InvertOrder();
		//most time we do not expect a specular angle to rotate,but programmer might need to do rotate if we want.
		virtual	void			RenderByGlobalTime(float e_fTime);
		virtual	void			RenderLastFrame();
		virtual	void			SetAnimationLoop(bool e_bLoop);
		//
		//virtual bool			IsAnimationDone(){ return cFMTimeLineAnimationRule::IsAnimationDone()?false:this->IsCurveMoveDone(); }
		//because RearrangeTime Function so I do not override it,so ensure u get last time as u expect
		//virtual	float	GetLastTime(){ if( m_fOriinalTimeList.size() )return m_fStartTime+m_fOriinalTimeList[m_fOriinalTimeList.size()-1]; return 0.f; }
		//example
		//int	l_iNumRelated;
		//sHintPoint*	l_pHintPoint = GetCurrentHintPointData(&l_iNumRelated);
		//for( int i=0;i<l_iNumRelated;++i )
		//{
		//	l_pHintPoint;//do something about
		//	l_pHintPoint++;//go to next sHintPoint,because it's fetch from vector so it's continue array;
		//}
		sTexBehaviorDataWithImageIndexData*GetPointData(int e_iIndex);
		cBaseImage*				PointDataToBaseImage(int e_iIndex);
		void					AddPoint(Vector3 e_vPos,float e_fTime,Vector2 e_Size,Vector3 e_vAngle,Vector4 e_vColor,int e_iImageIndex,bool e_bMirror,cPuzzleImage*e_pPI,bool e_bCheckOptmize = false);
		void					AddPoint(Vector3 e_vPos,float e_fTime,Vector2 e_Size,float e_fAngle,Vector4 e_vColor,int e_iImageIndex,bool e_bMirror,cPuzzleImage*e_pPI,bool e_bCheckOptmize = false);
		void					DelPoint(int e_iIndex);
		void					ClearPoint();
		void					InsertPoint(Vector3 e_Pos,float e_fTime,sTexBehaviorDataWithImageIndexData*e_pTexBehaviorDataWithImageIndexData,int e_iIndex);
		//let the path and time store list be same,
		void					SetOriginalToFinal();
		//do not call this every frame because it reset all color.
		void					SetColor(Vector4 e_vColor);
		void					SetColorScale(Vector4 e_vColor);
		void					SetCurrentColor(Vector4 e_vColor);
		//it could be none if it doesn't occur any hitpoint
		sHintPoint*				GetCurrentHintPointData(int *e_piNumRelated);
		//if the image is different,and you wanna sacle image too set e_bScalingImage as true.
		void					ChangeAllImageByImageIndex(int e_iIndex,bool e_bScalingImage = false);
		void					Render(sPuzzleData*e_pPuzzleData);
		//======================================================
		//below function is change point data,do not call those function at game,peformance is bad!
		//======================================================		
		//set point data rotation,rotate by curve's center
		void					RotateCurveWithCurveCenter(Quaternion e_Quaternion,bool e_bRotateImage);
		//rotate with a specific center position
		void					RotateCurveWithSpecificCenterPosition(Vector3 e_vCenterPos,Quaternion e_Quaternion,bool e_bRotateImage);
		void					SetScaleForPointData(float e_fScale);
		//set first image as e_fAngle and minus the offset for each point
		void					ImageRotation(Vector3 e_vAngle);
		void					ImageRelativeRotation(Vector3 e_vAngle);
		//to collide current data,usually all the data is same.
		//only support non scale collide,because I am lazy to do this.
		virtual	bool			Collide(int e_iPosX,int e_iPosY);
		virtual	bool			Collide(Vector4 e_vViewRect);
		//for collide debug
		void					RenderCollide();
		void					RenderByCollide(Vector4 e_vViewRect);
		//for optmize render,ensure all pi is same texture,its triangles not triangle strip
		//force to fetch mean hwile time is over progress it still get last frame's data
		//2 triangles 6 points(xyz),total size 18 float
		bool					GetTransformedTrianglesVertices(float*e_pfVertices,float*e_pfUV,float*e_pfColor,bool e_bForceToFetch = true);
		//old GetRenderDataByIndex
		bool					GetTransformedVerticesByIndex(float*e_pfVertices,float*e_pfUV,float*e_pfColor,int e_iIndex);
		Vector4					GetCollideRectByIndex(int e_iIndex);
		//for editor if this PI is not in using return false
		bool					IsUsingPuzzleImage(cPuzzleImage*e_pPI);
		//
		//cMatrix44				GetRenderMatrix(){return m_matRenderMatrix;}
		static					cCueToStartCurveWithTime*		GetMe(TiXmlElement*e_pElement,bool e_bClone = true);
	};

	typedef cCueToStartCurveWithTime	cSubMPDI;
}
#endif
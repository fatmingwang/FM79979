#ifndef CURVE_MANAGER
#define CURVE_MANAGER
#include "../XML/XMLLoader.h"
#include "Curve.h"
#include "../Image/GlyphFontRender.h"
#include "../TemplateClass.h"
namespace FATMING_CORE
{
	//<Curve count ="3" />
	//<CurvePoint  P1="0,0" P2="10,20" P3="20,20"  />
	//<CurveTime   />
	enum	ePathMode
	{
		ePM_EDIT = 0,
		ePM_FIX,					//while mode is this change the point select
		ePM_FIX_WITH_MOUSE_DRAG,	//while mode is this,do not change the m_iFocusPoint
		//
		ePM_PATH_MOVING,	
		ePM_PATH_WITH_MOUSE_DRAG,				//move path by mouse
		//
		ePM_PATH_MOVING_ALL,
		ePM_ALL_PATH_WITH_MOUSE_DRAG,			//move all path by mouse
		//
		ePM_PATH_ROTATION,
		ePM_PATH_ROTATION_DRAG,
		ePM_ALL_PATH_ROTATION,					//rotate all path by mouse
		ePM_ALL_PATH_ROTATION_DRAG,
		//
		ePM_ADD_POINT,
		ePM_NONE,
	};

	class	cCurveManager:public cNamedTypedObjectVector<cCurveWithTime>,public ISAXCallback
	{
		//<CurveData Name="79979" Count ="3" LOD="1" />
		void	ProcessCurveData();
		//<CurvePoint  P1="0,0" P2="10,20" P3="20,20"  />
		void	ProcessCurvePoint();
		//<CurveTime   />
		void	ProcessCurveTime();
		//<PIFile FileName=""   />
		void	ProcessPIFileName();
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
		int	m_iCurrentSelectedIndex;	//which curve is selected
		GET_SET_DEC(ePathMode,m_ePathMode,GetPathMode,SetPathMode);
		//for roataion
		void	FinoutCurrentPointsCenter();
		void	FinoutAllPointsCenter();
		Vector2	m_vRotationCenter;
		std::vector<Vector3>	m_fOriginalPosForRotatePointAroundRotationCenter;
	public:
		DEFINE_TYPE_INFO()
		DEFINE_FILE_EXTENSION_NAME_INFO()
		cCurveManager();
		virtual ~cCurveManager();
		bool	m_bBaseImageFromResource;
		cBaseImage*	m_pBasemageForShow;
		cCurveWithTime*m_pCurrentCurve;//depend on m_iCurrentSelectedIndex,if m_iCurrentSelectedIndex = -1,null
		void	SetCurrentSelectedIndex(int e_iIndex);
		int		GetCurrentSelectedIndex(){ return m_iCurrentSelectedIndex; }

		std::vector<int>		GetCurrentSelectedIndices(){ return m_SelectedIndicesToMovingOrRotating; }
		void	Render(Vector4 e_vColor = Vector4(0.f,1.f,0.f,0.3f));
		void	RenderALLCurve();
		void	Update(float e_fElpase);
		void	LoopUpdate(float e_fElpase);
		void	RenderWithImage();
		//for selected point moving 
		Vector3	m_vMouseClickedPosition;
		std::vector<int>	m_SelectedNodes;
		void	PathMoviing(bool e_bClicked,POINT e_MousePos);
		//for fix mode using,if not in fix moede ensure m_iFocusPoint is -1,to avoid show closet point
		int		m_iFocusPoint;				//which one point is closest point
		void	MouseMove(Vector3 e_vPos,MyMath::AXIS e_Axis = MyMath::W,float*e_pvProjectionMatrix = 0,float*e_pfViewMatrix = 0,int *e_piViewPort = 0);//xyz for orthognal,w for pespective
		void	MouseDown(Vector3 e_vPos,MyMath::AXIS e_Axis = MyMath::W,float*e_pvProjectionMatrix = 0,float*e_pfViewMatrix = 0,int *e_piViewPort = 0);//xyz for orthognal,w for pespective
		void	MouseUp(Vector3 e_vPos,MyMath::AXIS e_Axis = MyMath::W,float*e_pvProjectionMatrix = 0,float*e_pfViewMatrix = 0,int *e_piViewPort = 0);//xyz for orthognal,w for pespective
		//e_strForCollidedpiFile for edit collision data,it need a pi file.
		void	ExportData(const char*e_strFileName, const char*e_strForCollidedpiFile = 0, const char*e_pstrAutorData = 0);
		//if the file name is existed,it's for collision data use
		//make sure the data u have get while u keep parse collision data
		std::wstring m_strForCollidedpiFile;
		//while ePM_PATH_MOVING_ALL or ePM_ALL_PATH_ROTATION we could detected the index we wanna to process.
		std::vector<int>		m_SelectedIndicesToMovingOrRotating;
		//
		void	DoScale(Vector2 e_vScale);
	};

	Vector3	SwapPosAsAxisView(Vector3 e_vPos,MyMath::AXIS e_Axis);
	Vector3	RetainPosAxisAsAxisView(Vector3 e_vSrcPos,Vector3 e_vDestSrc,MyMath::AXIS e_Axis);
}
#endif
#ifndef _MPDI_FOR_EDITOR_H_
#define _MPDI_FOR_EDITOR_H_

#include "MPDI.h"

namespace FATMING_CORE
{
	//=======================
	//copy cCurveManager,let editor code more easy to maintain
	//=======================
	class	cMPDIForEditor:public cMultiPathDynamicImage
	{
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
		CLONE_MYSELF(cMPDIForEditor);
		//for create
		cMPDIForEditor(cMulti_PI_Image*e_pMulti_PI_Image);
		//for clone
		cMPDIForEditor(cMultiPathDynamicImage*e_pMultiPathDynamicImage);
		virtual ~cMPDIForEditor();
		cBaseImage*	m_pBasemageForShow;
		cCueToStartCurveWithTime*m_pCurrentCurve;//depend on m_iCurrentSelectedIndex,if m_iCurrentSelectedIndex = -1,null
		void	SetCurrentSelectedIndex(int e_iIndex);
		int		GetCurrentSelectedIndex(){ return m_iCurrentSelectedIndex; }

		std::vector<int>		GetCurrentSelectedIndices(){ return m_SelectedIndicesToMovingOrRotating; }
		void	MPDIRender(){ cMultiPathDynamicImage::Render(); }
		void	Render(Vector4 e_vColor = Vector4(0.f,1.f,0.f,0.3f));
		void	RenderALLCurve();
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
		void	ExportData(char*e_strFileName,char*e_strForCollidedpiFile = 0,char*e_pstrAutorData = 0);
		//if the file name is existed,it's for collision data use
		//make sure the data u have get while u keep parse collision data
		wchar_t	m_strForCollidedpiFile[MAX_PATH];
		//while ePM_PATH_MOVING_ALL or ePM_ALL_PATH_ROTATION we could detected the index we wanna to process.
		std::vector<int>		m_SelectedIndicesToMovingOrRotating;
	};
//end namespace FATMING_CORE
}
#endif
#ifndef _SPLIT_SCREEN_CAMERA_H_
#define _SPLIT_SCREEN_CAMERA_H_

#include "SimpleCamera.h"
#include "OrthogonalCamera.h"

namespace FATMING_CORE
{
//=================================
//split for four screen ,XYZ View and a camera view
//2Camera Type
//=================================
	//typedef	void	(*RenderCallback)();
	class	cSplitScreenCamera
	{
		void						DrawAxisPlane(POINT e_Viewport,cFrameCamera* e_pFrameCamera,cOrthogonalCamera* e_pOrthogonalCamera);
		void						RenderViewPortRect(POINT e_Viewport,MyMath::AXIS e_Axis);
		//0 for X
		//1 for Y
		//2 For Z
		//3 For Original
		int							FocusPanel(int e_iMouseX,int e_iMouseY,Vector2 e_vViewport);
		int							m_iFocusPanel;
		Vector3						m_vMousePosToWorldPos;
	public:
		Projection::ProjectionType	m_eProjectionType;
		bool						m_bShowSplitScreen;
		bool						m_bShowPlane;
		//cFrameCamera				m_FrameCamera;
		//cCameraBehaveByMouseBehave*	m_pCameraBehaveByMouseBehave;//for perspective camera
		struct	sAxisCamera
		{
			cFrameCamera			FrameCamera;
			cCameraBehaveByMouseBehave*	pCameraBehaveByMouseBehave;
			cOrthogonalCamera		OrthogonalCamera;
			sAxisCamera(){ pCameraBehaveByMouseBehave = new cCameraBehaveByMouseBehave(&FrameCamera); }
			~sAxisCamera(){ SAFE_DELETE(pCameraBehaveByMouseBehave); }
		};
		sAxisCamera					m_sAxisCamera[4];//xyz,original
		//RenderCallback*			m_pRenderFunction;			   //fuck I am lazy do implement this
		float						m_fDistanceToOriginalPoint;
	public:
		cSplitScreenCamera();
		virtual ~cSplitScreenCamera();
		//indicate the View port position,it's possible to render something after split screen so we have to let it called by 4 times
		//ensure u have call no texture shader if it's using shader
		void						RenderSplitScreen(POINT e_ViewPort,MyMath::AXIS e_Axis);
		void						Render(POINT e_ViewPort);
		void						Reset();
		//for mouse move mouse down mouse up
		void						MouseEvent(bool e_bLeftButton,bool e_bRightButton,int e_iWheelDelta,int e_iMouseX,int e_iMouseY,Vector2 e_vViewport,float e_fElpaseTime);
		//if m_bShowSplitScreen is true it's allow to move XYZ view by mouse
		//void	MouseMove(int e_iMouseX,int e_iMouseY,bool e_bMouseRightButton);
		int							GetFocusPanelIndex(){ return m_iFocusPanel; }
		Vector3						GetCurrentMousePos();
	};
//end namespace 
}

#endif
#include "../stdafx.h"
#include "SimpleCamera.h"
#include "MouseCamera.h"
#include "../GameplayUT/GameApp.h"
namespace FATMING_CORE
{
	void	cCameraBehaveByMouseBehave::Update(float e_fElpaseTime,bool e_bDoGameAppMouseStatusUpdate)
	{
		if(!m_pCamera)
			return;
		if( e_bDoGameAppMouseStatusUpdate )
		{
			CameraUpDateByMouse(cGameApp::m_sbMouseClickStatus[0],
				cGameApp::m_sbMouseClickStatus[1],
				cGameApp::m_sbMouseClickStatus[2]?cGameApp::m_sMouseWhellDelta:0,
				cGameApp::m_sMousePosition.x,
				cGameApp::m_sMousePosition.y,
				e_fElpaseTime);
		}
		if(this->m_MouseBehave.GetMouseState().bMidButton)//go ahead with camera direction
		{
			Vector3	l_vCameraDir = m_pCamera->GetLocalDirection();
			float	l_fMovePositiveOrNegative = 1.5f;
			m_MouseBehave.GetMouseState().iMoveZ>0 ?l_fMovePositiveOrNegative = -1.5f:l_fMovePositiveOrNegative = 1.5f;
			l_vCameraDir.x = l_vCameraDir.x*l_fMovePositiveOrNegative*m_fMoveSpeed;
			l_vCameraDir.y = l_vCameraDir.y*l_fMovePositiveOrNegative*m_fMoveSpeed;
			l_vCameraDir.z = l_vCameraDir.z*l_fMovePositiveOrNegative*m_fMoveSpeed;
			//l_vCameraDir.w = 1.0f;
			Vector3	l_vOriginalPos = this->m_pCamera->GetLocalPosition();
			l_vOriginalPos.x+=l_vCameraDir.x;
			l_vOriginalPos.y+=l_vCameraDir.y;
			l_vOriginalPos.z+=l_vCameraDir.z;
			this->m_pCamera->SetLocalPosition(l_vOriginalPos);
		}

		if(this->m_MouseBehave.GetMouseState().bLefButton)//Rotate camera
		{
			Vector3	l_cameraPos = m_pCamera->GetLocalPosition();
			float	l_fRotateX = 0,l_fRotateY = 0;
			if( m_MouseBehave.GetMouseState().iMoveX >2 || m_MouseBehave.GetMouseState().iMoveX <-2 )
			{
				l_fRotateY = -m_MouseBehave.GetMouseState().iMoveX/100.0f;
				m_fAngelY += l_fRotateY;
			}
			if( m_MouseBehave.GetMouseState().iMoveY >2 || m_MouseBehave.GetMouseState().iMoveY <-2 )
			{
				l_fRotateX = -m_MouseBehave.GetMouseState().iMoveY/100.0f;
				m_fAngelX += l_fRotateX;
			}
			fabs(l_fRotateX)>=fabs(l_fRotateY)?l_fRotateY = 0:l_fRotateX=0;
			//this one for local X and Y rotate
			//m_pCamera->SetLocalTransform(XMMatrixRotationX( l_fRotateX)*XMMatrixRotationY(l_fRotateY)*m_pCamera->GetLocalTransform());

			//this one for game rotate
			//remember original position
			Vector3	l_vOriginalPos = m_pCamera->GetWorldPosition();
			//rotate camera with the real world coordinate Y axis,this one will let camera rotate with the (0,1,0) so position will get
			//different to wanted,so we need to set worldposition to original position.
			m_pCamera->SetLocalTransform(cMatrix44::YAxisRotationMatrix(l_fRotateY*m_fRotationSpeed)*m_pCamera->GetLocalTransform()*cMatrix44::XAxisRotationMatrix(l_fRotateX*m_fRotationSpeed));
			//set to original position
			m_pCamera->SetWorldPosition(l_vOriginalPos);
		}

		if(this->m_MouseBehave.GetMouseState().bRigButton)
		{
			Vector3 l_vDirection1 = Vector3(1,0,0);
			Vector3 l_vDirection2 = Vector3(0,1,0);
			Vector3	l_vMovePos = l_vDirection1*(float)m_MouseBehave.GetMouseState().iMoveX/5.0f +
				l_vDirection2*((float)-m_MouseBehave.GetMouseState().iMoveY/5.0f);
			l_vMovePos*=m_fMoveSpeed;
			this->m_pCamera->SetWorldTransform(m_pCamera->GetWorldTransform()*cMatrix44::TranslationMatrix(l_vMovePos));
		}
	//	this->m_MouseBehave.RestButton();
	}

	void	cCameraBehaveByMouseBehave::CameraUpDateByMouse(bool e_bLeftButton,bool e_bRightButton,int e_iWheelDelta,int e_iMouseX,int e_iMouseY,float e_fElpaseTime)
	{
		sMouseState	l_sMouseState;
		memset(&l_sMouseState,0,sizeof(sMouseState));

		if( e_bLeftButton )
			l_sMouseState.bLefButton = true;
		else
		if ( e_bRightButton )
			l_sMouseState.bRigButton = true;
		if ( e_iWheelDelta )
		{
			l_sMouseState.iMoveZ = e_iWheelDelta;
			l_sMouseState.bMidButton = true;
		}
		l_sMouseState.iMoveX = e_iMouseX - m_MouseBehave.GetMouseState().iCurrentX;
		l_sMouseState.iMoveY = e_iMouseY - m_MouseBehave.GetMouseState().iCurrentY;
		l_sMouseState.iCurrentX = e_iMouseX;
		l_sMouseState.iCurrentY = e_iMouseY;
		m_MouseBehave.Update(l_sMouseState);
		if( l_sMouseState.bLefButton || l_sMouseState.bRigButton || l_sMouseState.bMidButton)
		{
			Update(e_fElpaseTime);
		}
	}

	void	cCameraBehaveByMouseBehave::CameraUpDateByMouse(sMouseState	e_MouseState,float e_fElpaseTime)
	{
		m_MouseBehave.Update(e_MouseState);
		if( e_MouseState.bLefButton || e_MouseState.bRigButton || e_MouseState.bMidButton)
		{
			Update(e_fElpaseTime);
		}	
	}
}
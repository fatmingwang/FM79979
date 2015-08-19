#ifndef MOUSE_CAMERA_H
#define MOUSE_CAMERA_H

namespace FATMING_CORE
{
	class cFrameCamera;

	struct sMouseState
	{
		bool	bMidButton;
		bool	bLefButton;
		bool	bRigButton;
		int		iMoveX;
		int		iMoveY;
		int		iMoveZ;	//360 not use it's with MidButton
		int		iCurrentX;
		int		iCurrentY;
	};

	class cMouseBehave
	{
	public:
		sMouseState	m_sMouseState;
		cMouseBehave(){ ResetMouseState();}
		virtual ~cMouseBehave(){}
		sMouseState	GetMouseState(){ return m_sMouseState; }
		void					Init(int e_StartPosX,int e_StartPosY){ m_sMouseState.iCurrentX = e_StartPosX; m_sMouseState.iCurrentY = e_StartPosY; }
		void					Update(bool	e_bLefButton,bool e_bRigButton,bool e_bMidButton,int e_iCurrentMousePosX,int e_iCurrentMousePosY,int e_iCurrentMousePosZ)
		{
			m_sMouseState.bLefButton = e_bLefButton; m_sMouseState.bRigButton = e_bRigButton; m_sMouseState.bMidButton = e_bMidButton;
			m_sMouseState.iMoveX = m_sMouseState.iCurrentX - e_iCurrentMousePosX;
			m_sMouseState.iMoveY = m_sMouseState.iCurrentX - e_iCurrentMousePosY;
			m_sMouseState.iCurrentX = e_iCurrentMousePosX;
			m_sMouseState.iCurrentX = e_iCurrentMousePosY;
		}
		void					Update(sMouseState e_sMouseState){	m_sMouseState = e_sMouseState; }
		void					ResetMouseState(){ memset(&m_sMouseState,0,sizeof(sMouseState)); }
		void					RestButton(){ m_sMouseState.bLefButton = false; m_sMouseState.bRigButton = false; m_sMouseState.bMidButton = false;}
	};
	//if u like to use mouse wheel event in the dot net,
	//make sure you have call Focus function in the mouse move envet to the panel,if u would like to use!!
	//
	//if u like to set camera at specific angle do it like below
	//if(GCFORM::Keys::R == e->KeyCode)//set camera not look at sky
	//{
	//	sMouseState	l_sMouseState;
	//	memset(&l_sMouseState,0,sizeof(sMouseState));
	//	l_sMouseState.bLefButton = true;
	//	l_sMouseState.iMoveY = g_pCameraBehaveByMouseBehave->GetRotateX()*100;
	//	g_pCameraBehaveByMouseBehave->GetMouseBehave()->Update(l_sMouseState);
	//	g_pCameraBehaveByMouseBehave->Update();
	//}
	class cCameraBehaveByMouseBehave
	{
		cMouseBehave			m_MouseBehave;
		//for 2 points touch.
		cMouseBehave			m_MouseBehave2;
		cFrameCamera*			m_pCamera;
		float					m_fAngelX,m_fAngelY;
		float					m_fMoveSpeed;
		float					m_fRotationSpeed;
	public:	
		cCameraBehaveByMouseBehave(cFrameCamera*e_pCamera){ m_pCamera = e_pCamera; m_fAngelX = 0.0f; m_fAngelY = 0.0f; m_fRotationSpeed = m_fMoveSpeed = 1.f; /*m_bFirstClick = false;*/ }
		virtual ~cCameraBehaveByMouseBehave(){ m_pCamera = nullptr; }

		cMouseBehave*			GetMouseBehave(){ return &m_MouseBehave; }
		float					GetRotateX(){ return m_fAngelX; }
		float					GetRotateY(){ return m_fAngelY; }
		void					SetCamera(cFrameCamera*	e_pCamera){ m_pCamera = e_pCamera; }
		//old fashion
		void					Update(float e_fElpaseTime);
		void					SetMouseMoveSpeed(float e_fSpeed){ m_fMoveSpeed = e_fSpeed; }
		float					GetMouseMoveSpeed(){ return m_fMoveSpeed; }
		void					SetMouseRotationSpeed(float e_fSpeed){ m_fRotationSpeed = e_fSpeed; }
		float					GetMouseRotationSpeed(){ return m_fRotationSpeed; }
		void					SetAngleX(float e_fAngleX){ m_fAngelX = e_fAngleX; }
		void					SetAngleY(float e_fAngleY){ m_fAngelY = e_fAngleY; }
		void					CameraUpDateByMouse(bool e_bLeftButton,bool e_bRightButton,int e_iWheelDelta,int e_iMouseX,int e_iMouseY,float e_fElpaseTime);
		void					CameraUpDateByMouse(sMouseState	e_MouseState,float e_fElpaseTime);
	};

//end FATMING_CORE namespace
}
#endif
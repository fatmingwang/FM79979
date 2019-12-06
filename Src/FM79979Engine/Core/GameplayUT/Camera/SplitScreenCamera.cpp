#include "SplitScreenCamera.h"
namespace FATMING_CORE
{
	cSplitScreenCamera::cSplitScreenCamera()
	{
		m_fDistanceToOriginalPoint = 100.f;
		m_eProjectionType = Projection::Orthographic;
		m_bShowSplitScreen = false;
		m_bShowPlane = true;
		m_iFocusPanel = 0;
		Vector2	l_v(1920.f,1080.f);
		for(int i=0;i<4;++i)
			this->m_sAxisCamera[i].OrthogonalCamera.CameraUpdateByMouse(false,false,0,0,0,l_v);
		Reset();
	}

	cSplitScreenCamera::~cSplitScreenCamera()
	{
	
	}

	void	cSplitScreenCamera::DrawAxisPlane(POINT e_Viewport,cFrameCamera* e_pFrameCamera,cOrthogonalCamera* e_pOrthogonalCamera)
	{
		if( m_eProjectionType == Projection::Perspective )
		{
			e_pFrameCamera->Render();
			RenderPlane(cMatrix44::XAxisRotationMatrix(0),10,10,1,1,4,Vector4(1,0,0,1));
			RenderPlane(cMatrix44::XAxisRotationMatrix(D3DXToRadian(90)),10,10,1,1,4,Vector4(0,1,0,1));
			RenderPlane(cMatrix44::YAxisRotationMatrix(D3DXToRadian(90)),10,10,1,1,4,Vector4(0,0,1,1));
			RenderPoint(Vector3(0,0,0),5,Vector4(1,1,1,1));
		}
		else
		{
			e_pOrthogonalCamera->Render();
			RenderPlane(cMatrix44::Identity,1000,1000,100,100,4,Vector4(1,0,0,1));
			RenderPlane(cMatrix44::XAxisRotationMatrix(D3DXToRadian(90)),1000,1000,100,100,4,Vector4(0,1,0,1));
			RenderPlane(cMatrix44::YAxisRotationMatrix(D3DXToRadian(90)),1000,1000,100,100,4,Vector4(0,0,1,1));
			RenderPoint(Vector2(0,0),5,Vector4(1,1,1,1));
		}
	}
	void	cSplitScreenCamera::RenderViewPortRect(POINT e_Viewport,MyMath::AXIS e_Axis)
	{
		Vector4	l_vRect = this->m_sAxisCamera[3].OrthogonalCamera.GetViewRect();
		Vector2	l_vSize = this->m_sAxisCamera[3].OrthogonalCamera.GetScreenViewPortSize();
		this->m_sAxisCamera[3].OrthogonalCamera.Render();
		POINT	l_Pos = {(int)l_vRect.x,(int)l_vRect.y};
		RenderRectangle(l_Pos,(int)l_vSize.x,(int)l_vSize.y,Vector4(0,1,1,0.8f),0,10);
		cOrthogonalCamera l_OrthogonalCamera;
		l_OrthogonalCamera.Reset();
		l_OrthogonalCamera.Render();
		switch( e_Axis )
		{
			case MyMath::X:
			{
				Vector2	l_vX[4] = {Vector2(0,1080),Vector2(100,1000),Vector2(100,1080),Vector2(0,1000)};
				RenderLine((float*)l_vX,2,Vector4(1.f,1.f,1,1.f),2);
				RenderLine((float*)l_vX[2],2,Vector4(1.f,1.f,1,1.f),2);
			}
			break;
			case MyMath::Y:
			{
				Vector2	l_vY[6] = {Vector2(50,1080),Vector2(50,1040),Vector2(0,1000),Vector2(50,1040),Vector2(100,1000),Vector2(50,1040)};
				RenderLine((float*)l_vY,2,Vector4(1.f,1.f,1,1.f),2);
				RenderLine((float*)l_vY[2],2,Vector4(1.f,1.f,1,1.f),2);
				RenderLine((float*)l_vY[4],2,Vector4(1.f,1.f,1,1.f),2);
			}
			break;
			case MyMath::Z:
			{
				Vector2	l_vY[6] = {Vector2(0,1000),Vector2(100,1000),Vector2(100,1000),Vector2(0,1080),Vector2(0,1080),Vector2(100,1080)};
				RenderLine((float*)l_vY,2,Vector4(1.f,1.f,1,1.f),2);
				RenderLine((float*)l_vY[2],2,Vector4(1.f,1.f,1,1.f),2);
				RenderLine((float*)l_vY[4],2,Vector4(1.f,1.f,1,1.f),2);
			}
			break;
			case MyMath::W:
			break;			
		}
	}

	//indicate the View port position,it's possible to render something after split screen so we have to let it called by 4 times
	//ensure u have call no texture shader if it's using shader
	void	cSplitScreenCamera::RenderSplitScreen(POINT e_ViewPort,MyMath::AXIS e_Axis)
	{
		int		l_iWidth = e_ViewPort.x;
		int		l_iHeight = e_ViewPort.y;
		//Vector3	l_vOriginalPos = Vector3::Zero;//m_FrameCamera.GetWorldPosition();
		//opengl glViewport left lower corrner is 0,0,right upper is max wisth max height
		cFrameCamera	*l_pFrameCamera = 0;
		cOrthogonalCamera*l_pOrthogonalCamera = 0;
		int i = (int)e_Axis;
		{
			l_pFrameCamera = &this->m_sAxisCamera[i].FrameCamera;
			l_pOrthogonalCamera = &this->m_sAxisCamera[i].OrthogonalCamera;
			switch(e_Axis)
			{
				case MyMath::X://no x
				{
					//first Left up,XView
					glViewport(0,l_iHeight,l_iWidth,l_iHeight);
				}
				break;
				case MyMath::Y://no y
				{
					//second Right Up,YView
					glViewport(l_iWidth,l_iHeight,l_iWidth,l_iHeight);
				}
				break;
				case MyMath::Z://no z
				{
					//Third Left Down,ZView
					glViewport(0,0,l_iWidth,l_iHeight);
				}
				break;
				case MyMath::W:
				{
					//Fourth Right Down
					glViewport(l_iWidth,0,l_iWidth,l_iHeight);
				}
				break;
			}
			RenderViewPortRect(e_ViewPort,e_Axis);
			if( i == MyMath::W &&this->m_eProjectionType == Projection::Orthographic )
			{
				l_pOrthogonalCamera->Render();
				l_pOrthogonalCamera->DrawGrid(0,0,Vector4(1.f,1.f,0.f,1.f),3);
			}
			if( this->m_bShowPlane )
				DrawAxisPlane(e_ViewPort,l_pFrameCamera,l_pOrthogonalCamera);
		}
	}

	void	cSplitScreenCamera::Render(POINT e_ViewPort)
	{
		if( m_eProjectionType == Projection::Perspective )
		{
			this->m_sAxisCamera[3].FrameCamera.Render();
		}
		else
		{
			this->m_sAxisCamera[3].OrthogonalCamera.Render();
			this->m_sAxisCamera[3].OrthogonalCamera.DrawGrid(0,0,Vector4(1.f,1.f,0.f,1.f),3);
		}
		if( this->m_bShowPlane )
			DrawAxisPlane(e_ViewPort,&this->m_sAxisCamera[3].FrameCamera,&this->m_sAxisCamera[3].OrthogonalCamera);
	}

	void	cSplitScreenCamera::Reset()
	{
		Vector3	l_vOriginalPos(Vector3::Zero);
		float	l_fOffsetPosition = m_fDistanceToOriginalPoint;
		//first Left up,XView
		Vector3	l_vXPos = l_vOriginalPos;	l_vXPos.x -= l_fOffsetPosition;
		cMatrix44	l_matX = cMatrix44::TranslationMatrix(l_vXPos)*cMatrix44::YAxisRotationMatrix(D3DXToRadian(-90));
		this->m_sAxisCamera[0].FrameCamera.SetWorldTransform(l_matX);
		this->m_sAxisCamera[0].OrthogonalCamera.Reset();
		this->m_sAxisCamera[0].OrthogonalCamera.SetWorldTransform(cMatrix44::YAxisRotationMatrix(D3DXToRadian(90)));
		//this->m_sAxisCamera[0].OrthogonalCamera.SetWorldTransform(l_matX);
		//second Right Up,YView
		Vector3	l_vYPos = l_vOriginalPos;	l_vYPos.y += l_fOffsetPosition;
		cMatrix44	l_matY = cMatrix44::TranslationMatrix(l_vYPos)*cMatrix44::XAxisRotationMatrix(D3DXToRadian(-90));
		this->m_sAxisCamera[1].FrameCamera.SetWorldTransform(l_matY);
		this->m_sAxisCamera[1].OrthogonalCamera.Reset();
		this->m_sAxisCamera[1].OrthogonalCamera.SetWorldTransform(cMatrix44::XAxisRotationMatrix(D3DXToRadian(-90)));
		//Third Left Down,ZView
		 Vector3	l_vZPos = l_vOriginalPos;	l_vZPos.z += l_fOffsetPosition;
		cMatrix44	l_matZ = cMatrix44::TranslationMatrix(l_vZPos);		
		this->m_sAxisCamera[2].FrameCamera.SetWorldTransform(l_matZ);
		this->m_sAxisCamera[2].OrthogonalCamera.Reset();
		this->m_sAxisCamera[2].OrthogonalCamera.SetWorldTransform(cMatrix44::Identity);
		//Fourth Right Down
		this->m_sAxisCamera[3].FrameCamera.SetLocalTransform(cMatrix44::Identity);
		this->m_sAxisCamera[3].OrthogonalCamera.Reset();
		this->m_sAxisCamera[3].OrthogonalCamera.SetLocalTransform(cMatrix44::Identity);
	}
	//0 for X
	//1 for Y
	//2 For Z
	//3 For Original
	int	cSplitScreenCamera::FocusPanel(int e_iMouseX,int e_iMouseY,Vector2 e_vViewport)
	{
		if( !m_bShowSplitScreen )
		{
			return 3;
		}
		else
		{
			Vector2	l_vSplitPanelSize(e_vViewport.x/2.f,e_vViewport.y/2.f);
			if( e_iMouseX>(int)l_vSplitPanelSize.x )//1,3
			{
				if( e_iMouseY>l_vSplitPanelSize.y )
					return 3;
				return 1;
			}
			else
			{
				if( e_iMouseY> (int)l_vSplitPanelSize.y )
					return 2;
				return 0;				
			}
		}
	}
	//for mouse move mouse down mouse up
	void	cSplitScreenCamera::MouseEvent(bool e_bLeftButton,bool e_bRightButton,int e_iWheelDelta,int e_iMouseX,int e_iMouseY,Vector2 e_vViewport,float e_fElpaseTime)
	{
		if( !m_bShowSplitScreen )
		{
			m_iFocusPanel = 3;//for original
			if( m_eProjectionType == Projection::Perspective )
			{
				this->m_sAxisCamera[3].pCameraBehaveByMouseBehave->CameraUpDateByMouse(e_bLeftButton,e_bRightButton,e_iWheelDelta,e_iMouseX,e_iMouseY,e_fElpaseTime);
				//just return 2D position
				m_vMousePosToWorldPos.x = (float)e_iMouseX;
				m_vMousePosToWorldPos.y = (float)e_iMouseY;
				m_vMousePosToWorldPos.z = 0.f;
			}
			else
			if( m_eProjectionType == Projection::Orthographic )
			{
				this->m_sAxisCamera[3].OrthogonalCamera.CameraUpdateByMouse(e_bLeftButton,e_bRightButton,e_iWheelDelta,e_iMouseX,e_iMouseY,e_vViewport);
				m_vMousePosToWorldPos.x = m_sAxisCamera[3].OrthogonalCamera.GetMouseWorldPos().x;
				m_vMousePosToWorldPos.y = m_sAxisCamera[3].OrthogonalCamera.GetMouseWorldPos().y;
				m_vMousePosToWorldPos.z = m_sAxisCamera[0].OrthogonalCamera.GetMouseWorldPos().x;
			}
		}
		else//find the current focus panel
		{
			m_iFocusPanel = FocusPanel(e_iMouseX,e_iMouseY,e_vViewport);
			e_vViewport *= 0.5;
			if( m_iFocusPanel%2 )
			{//convert X
				e_iMouseX -= (int)e_vViewport.x;
			}
			if( m_iFocusPanel>1 )
			{//convert Y
				e_iMouseY -= (int)e_vViewport.y;
			}
			if( m_eProjectionType == Projection::Perspective )
			{
				//float	l_fZ = l_pCamera->GetWorldPosition().z/l_pCamera->GetProjection().GetZFar();
				//float	l_fZ = 1.f;
				//m_vMousePosToWorldPos = UT::ScreenToWorld(Vector3(e_iMouseX,e_iMouseY,l_fZ),l_pCamera->GetProjection().GetMatrix()*l_pCamera->GetWorldTransform(),e_vViewport);
				switch( m_iFocusPanel )
				{
					case MyMath::X://no x
					case MyMath::Y://no y
					case MyMath::Z://no z
					{
						cCameraBehaveByMouseBehave*l_pCameraBehaveByMouseBehave = this->m_sAxisCamera[m_iFocusPanel].pCameraBehaveByMouseBehave;
						l_pCameraBehaveByMouseBehave->CameraUpDateByMouse(false,e_bRightButton,e_iWheelDelta,e_iMouseX,e_iMouseY,e_fElpaseTime);
					}
						break;
					case MyMath::W:
						m_bShowSplitScreen = false;
							MouseEvent(e_bLeftButton,e_bRightButton,e_iWheelDelta,e_iMouseX,e_iMouseY,e_vViewport,e_fElpaseTime);
						m_bShowSplitScreen = true;
						break;
				}
				//just return 2D position
				m_vMousePosToWorldPos.x = (float)e_iMouseX;
				m_vMousePosToWorldPos.y = (float)e_iMouseY;
				m_vMousePosToWorldPos.z = 0.f;
			}
			else
			if( m_eProjectionType == Projection::Orthographic )
			{
				this->m_sAxisCamera[m_iFocusPanel].OrthogonalCamera.CameraUpdateByMouse(e_bLeftButton,e_bRightButton,e_iWheelDelta,e_iMouseX,e_iMouseY,e_vViewport);
				switch(m_iFocusPanel)
				{
					case 0://X
						m_vMousePosToWorldPos.x = m_sAxisCamera[3].OrthogonalCamera.GetMouseWorldPos().x;
						m_vMousePosToWorldPos.y = m_sAxisCamera[0].OrthogonalCamera.GetMouseWorldPos().y;
						m_vMousePosToWorldPos.z = m_sAxisCamera[0].OrthogonalCamera.GetMouseWorldPos().x;
						break;
					case 1://y
						m_vMousePosToWorldPos.x = m_sAxisCamera[1].OrthogonalCamera.GetMouseWorldPos().x;
						m_vMousePosToWorldPos.y = m_sAxisCamera[0].OrthogonalCamera.GetMouseWorldPos().y;
						m_vMousePosToWorldPos.z = m_sAxisCamera[1].OrthogonalCamera.GetMouseWorldPos().y;
						break;
					case 2://z
						m_vMousePosToWorldPos.x = m_sAxisCamera[2].OrthogonalCamera.GetMouseWorldPos().x;
						m_vMousePosToWorldPos.y = m_sAxisCamera[2].OrthogonalCamera.GetMouseWorldPos().y;
						m_vMousePosToWorldPos.z = m_sAxisCamera[0].OrthogonalCamera.GetMouseWorldPos().x;
						break;
					case 3://w
						m_vMousePosToWorldPos.x = m_sAxisCamera[3].OrthogonalCamera.GetMouseWorldPos().x;
						m_vMousePosToWorldPos.y = m_sAxisCamera[3].OrthogonalCamera.GetMouseWorldPos().y;
						m_vMousePosToWorldPos.z = m_sAxisCamera[0].OrthogonalCamera.GetMouseWorldPos().x;
						break;
				}
			}
		}
	} 
	Vector3	cSplitScreenCamera::GetCurrentMousePos()
	{
		return m_vMousePosToWorldPos;	
	}
//end namespace
}
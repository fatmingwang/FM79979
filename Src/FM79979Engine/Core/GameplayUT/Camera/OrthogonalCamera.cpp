#include "OrthogonalCamera.h"
#include "../OpenGL/GLSL/Shader.h"
#include "SimpleCamera.h"
#include "../Render/CommonRender/SimplePrimitive.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cOrthogonalCamera);

	cOrthogonalCamera::cOrthogonalCamera(Vector2 e_vResolution)
	{
		m_fScale = 1.f;
		m_iTotalGridVertexSize = -1;
		m_pDrawGridVertex = nullptr;
		m_vCameraAngle = Vector3::Zero;
		m_vResolution = e_vResolution;
		m_fGridWidth = 100.f;
		m_fGridHeight = 100.f;
		Reset();
		glhOrthof2(m_ProjectionMatrix,m_vViewRect.x,m_vViewRect.z,m_vViewRect.w,m_vViewRect.y, -10000, 10000);
	}

	cOrthogonalCamera::cOrthogonalCamera(cOrthogonalCamera*e_pOrthogonalCamera):Frame(e_pOrthogonalCamera)
	{
		m_fScale = 1.f;
		m_iTotalGridVertexSize = -1;
		m_pDrawGridVertex = nullptr;
		m_vCameraAngle = e_pOrthogonalCamera->m_vCameraAngle;
		m_vResolution = e_pOrthogonalCamera->m_vResolution;
		m_fGridWidth = e_pOrthogonalCamera->m_fGridWidth;
		m_fGridHeight = e_pOrthogonalCamera->m_fGridHeight;
		glhOrthof2(m_ProjectionMatrix,m_vViewRect.x,m_vViewRect.z,m_vViewRect.w,m_vViewRect.y, -10000, 10000);	
	}

	cOrthogonalCamera::~cOrthogonalCamera()
	{
		SAFE_DELETE(m_pDrawGridVertex);
	}

	void	cOrthogonalCamera::SetViewRect(Vector4 e_vViewRect)
	{
		m_fScale = 1.f;
		this->m_vViewRect = e_vViewRect;
	}

	Vector2	cOrthogonalCamera::ConvertMousePositionToWorldPosition(Vector2 e_MousePosition,Vector2 e_ViewportSize)
	{
		return ConvertMousePositionToWorldPosition(e_MousePosition,e_ViewportSize,m_vViewRect);
	}

	Vector2	cOrthogonalCamera::ConvertMousePositionToWorldPosition(Vector2 e_MousePosition,Vector2 e_ViewportSize,Vector4 e_vCameraViewRect)
	{
		float	l_fScaleX = e_MousePosition.x/e_ViewportSize.x;
		float	l_fScaleY = e_MousePosition.y/e_ViewportSize.y;
		//transform mouse positio into game's resolution

		e_MousePosition.x = l_fScaleX*e_vCameraViewRect.Width()+e_vCameraViewRect.x;
		e_MousePosition.y = l_fScaleY*e_vCameraViewRect.Height()+e_vCameraViewRect.y;
		return e_MousePosition;	
	}
    //void    cOrthogonalCamera::MoveViewRect(Vector2 e_vDirection)
    //{
		//m_CameraPos.x += e_vDirection.x;
		//m_CameraPos.y += e_vDirection.y;
		//this->m_vViewRect.x+=e_vDirection.x;
		//this->m_vViewRect.y+=e_vDirection.y;
		//this->m_vViewRect.z+=e_vDirection.x;
		//this->m_vViewRect.w+=e_vDirection.y;
    //}

	void	cOrthogonalCamera::Render(Vector2 e_vCameraPos)
	{
		MyGLDisable(GL_DEPTH_TEST);
		MyGLDisable(GL_CULL_FACE);
		glhOrthof2(m_ProjectionMatrix,m_vViewRect.x+e_vCameraPos.x,m_vViewRect.z+e_vCameraPos.x,m_vViewRect.w+e_vCameraPos.y,m_vViewRect.y+e_vCameraPos.y, -10000, 10000);
		m_ProjectionMatrix *= cMatrix44::RotationMatrix(m_vCameraAngle);
		FATMING_CORE::SetupShaderViewProjectionMatrix( m_ProjectionMatrix,true );
	}

	void	cOrthogonalCamera::Render(bool e_bYInvert)
	{
		//push attribute
		MyGLDisable(GL_DEPTH_TEST);
		MyGLDisable(GL_CULL_FACE);
		if( e_bYInvert )
			glhOrthof2(m_ProjectionMatrix,m_vViewRect.x,m_vViewRect.z,m_vViewRect.y,m_vViewRect.w, -10000, 10000);
		else
			glhOrthof2(m_ProjectionMatrix,m_vViewRect.x,m_vViewRect.z,m_vViewRect.w,m_vViewRect.y, -10000, 10000);
		//because I invert Y.
		//cMatrix44	l_mat = this->GetWorldTransform();
		//l_mat.SetTranslation(-l_mat.GetTranslation());
		m_ProjectionMatrix *= cMatrix44::RotationMatrix(m_vCameraAngle);
		FATMING_CORE::SetupShaderViewProjectionMatrix(m_ProjectionMatrix,true);
	}

	void	cOrthogonalCamera::DrawGrid(float e_fXOffset,float e_fYOffset,Vector4 e_vColor,float e_fLineWidth)
	{
		//just want a interge value for 100
		//int		l_iLeft = (int)m_vViewRect.x/100*100;
		//int		l_iRight = (int)m_vViewRect.z/100*100;
		//int		l_iUp = (int)m_vViewRect.y/100*100;
		//int		l_iDown = (int)m_vViewRect.w/100*100;

		float	l_fResolutionWidth = m_vViewRect.z-m_vViewRect.x;
		float	l_fResolutionHeight = m_vViewRect.w-m_vViewRect.y;
		float	l_fRow = l_fResolutionWidth/m_fGridWidth;
		float	l_fColumn = l_fResolutionHeight/m_fGridHeight;
		int	l_iRow = (int)l_fRow;
		int	l_iColumn = (int)l_fColumn;
		l_iRow = l_fRow-l_iRow>0.f?l_iRow+1:(l_iRow);
		l_iColumn = l_fColumn-l_iColumn>0.f?l_iColumn+1:(l_iColumn);
		float	l_fPosX = (m_vViewRect.x)/m_fGridWidth;
		float	l_fPosY = (m_vViewRect.y)/m_fGridHeight;
		float	l_fXOffset = (l_fPosX-(int)(l_fPosX))*m_fGridWidth;
		float	l_fYOffset = (l_fPosY-(int)(l_fPosY))*m_fGridHeight;
		int	l_iTotalCount = (l_iRow+2+l_iColumn+2)*2;


		if( l_iRow<=0||l_iColumn<=0 )
			return;
		//glTranslatef(-l_fXOffset,-l_fYOffset,0.f);
		glLineWidth(e_fLineWidth);
		if( m_iTotalGridVertexSize != l_iTotalCount )
		{
			m_iTotalGridVertexSize = l_iTotalCount;
			SAFE_DELETE(m_pDrawGridVertex);
			m_pDrawGridVertex = new Vector2[m_iTotalGridVertexSize];
		}
		Vector2*l_vAllVertices = m_pDrawGridVertex;
		float	l_fStartUp = m_vViewRect.y-m_fGridHeight;
		float	l_fEndDown = m_vViewRect.w+m_fGridHeight;
		float	l_fStartLeft = m_vViewRect.x-m_fGridWidth;
		float	l_fEndRight = m_vViewRect.z+m_fGridWidth;
		//+2 from l_iTotalCount
		for( int i=0;i<l_iRow+2;++i )
		{
			l_vAllVertices[i*2] = Vector2(l_fStartLeft+i*m_fGridWidth,l_fStartUp);
			l_vAllVertices[i*2+1] = Vector2(l_fStartLeft+i*m_fGridWidth,l_fEndDown);
			l_vAllVertices[i*2].x+=e_fXOffset;
			l_vAllVertices[i*2+1].x+=e_fXOffset;
		}
		//+2 from l_iTotalCount
		int	l_iTotalRow = (l_iRow+2)*2;
		for( int j=0;j<l_iColumn+2;++j )
		{
			l_vAllVertices[l_iTotalRow+j*2]		=	Vector2(l_fStartLeft,l_fStartUp+j*m_fGridHeight);
			l_vAllVertices[l_iTotalRow+j*2+1]	=	Vector2(l_fEndRight,l_fStartUp+j*m_fGridHeight);
			l_vAllVertices[l_iTotalRow+j*2].y	+=	e_fYOffset;
			l_vAllVertices[l_iTotalRow+j*2+1].y	+=	e_fYOffset;
		}
		cMatrix44	l_mat = cMatrix44::TranslationMatrix(Vector3(-l_fXOffset,-l_fYOffset,0.f));
		RenderLine((float*)l_vAllVertices, l_iTotalCount, e_vColor, 2, l_mat);
	}

	void	cOrthogonalCamera::DrawSelectFrame()
	{
		if( m_bDrawSelectFrame )
			m_DrawSelectRectangleByMouse.Render();
	}

	void	cOrthogonalCamera::Reset()
	{
		m_CameraPos.x = m_vResolution.x/2.f;m_CameraPos.y = m_vResolution.y/2.f; m_fScale = 1.f;
		m_vViewRect.x = m_vViewRect.z = m_vViewRect.y = m_vViewRect.w = 0.f;
		m_bDrawSelectFrame = true;
		m_vViewRect.x = m_CameraPos.x-m_vResolution.x/2.f;
		m_vViewRect.z = m_vViewRect.x+m_vResolution.x;
		m_vViewRect.w = m_CameraPos.y+m_vResolution.y/2.f;
		m_vViewRect.y = m_vViewRect.w-m_vResolution.y;
		this->SetLocalTransform(cMatrix44::Identity);
	}

	void	cOrthogonalCamera::ViewportChangeApplyNewResolution(int e_iViewportWidth,int e_iViewportHeight)
	{
		POINT l_ViewportSize = { e_iViewportWidth,e_iViewportHeight };
		Vector2	l_vOriginalResolution = GetResolution();
		Vector2	l_vOriginalLeftUpPos = GetCameraPos()-(l_vOriginalResolution/2.f);
		SetResolution(Vector2((float)l_ViewportSize.x,(float)l_ViewportSize.y));
		SetCameraPos(l_vOriginalLeftUpPos+GetResolution()/2.f);
		CameraUpdateByMouse(false,false,0,0,0,Vector2((float)e_iViewportWidth,(float)e_iViewportHeight));
	}

	void	cOrthogonalCamera::CameraUpdateByMouse(bool e_bLeftButton,bool e_bRightButton,int e_iWheelDelta,int e_iMouseX,int e_iMouseY,Vector2 e_vViewport, float e_fMoveSpeed)
	{
		 sMouseState	l_sMouseState;
		 memset(&l_sMouseState,0,sizeof(sMouseState));
		 l_sMouseState.iMoveX = e_iMouseX - m_MouseBehave.GetMouseState().iCurrentX;
		 l_sMouseState.iMoveY = e_iMouseY - m_MouseBehave.GetMouseState().iCurrentY;
		 l_sMouseState.iCurrentX = e_iMouseX;
		 l_sMouseState.iCurrentY = e_iMouseY;
		 if(e_iWheelDelta != 0 )
		 //if(e->Button == System::Windows::Forms::MouseButtons::Middle)
		 {
			 l_sMouseState.iMoveZ = (int)(e_iWheelDelta/120* e_fMoveSpeed);
			 l_sMouseState.bMidButton = true;
		 }
		 else
		 if ( e_bLeftButton )
			 l_sMouseState.bLefButton = true;
		 else
		 if ( e_bRightButton )
			 l_sMouseState.bRigButton = true;
		 m_MouseBehave.Update(l_sMouseState);
		 //POINT l_ViewportSize = { (int)e_vViewport.x,(int)e_vViewport.y };
		//if scale is active we have to know old position and caompare to new mouse position.
		//to get old width and height,so we know the percentage
		//float	l_fOldWidth;
		//float	l_fOldHeight;
		if(this->m_MouseBehave.GetMouseState().bMidButton)//go ahead with camera direction
		{
			Vector2	l_PerviousWorldMousePosition = m_MouseWorldPosition;
			float l_fNewScale = ((float)m_MouseBehave.GetMouseState().iMoveZ / 100.f);
			m_fScale += l_fNewScale;
			float	l_fNewWidth = m_vResolution.x * m_fScale;
			float	l_fNewHeight = m_vResolution.y * m_fScale;
			Vector2 l_vMousePosScale(e_iMouseX / e_vViewport.x, e_iMouseY / e_vViewport.y);
			m_vViewRect.x = m_MouseWorldPosition.x-(l_vMousePosScale.x* l_fNewWidth);
			m_vViewRect.z = m_vViewRect.x + l_fNewWidth;
			m_vViewRect.y = m_MouseWorldPosition.y - (l_vMousePosScale.y * l_fNewHeight);
			m_vViewRect.w = m_vViewRect.y + l_fNewHeight;
			m_CameraPos.x = (m_vViewRect.z - m_vViewRect.x) / 2+ m_vViewRect.x;
			m_CameraPos.y = (m_vViewRect.w - m_vViewRect.y) / 2+ m_vViewRect.y;
		}
		else
		if(this->m_MouseBehave.GetMouseState().bRigButton)
		{		
			m_CameraPos.x += ((float)m_MouseBehave.GetMouseState().iMoveX*e_fMoveSpeed);
			m_CameraPos.y += ((float)m_MouseBehave.GetMouseState().iMoveY*e_fMoveSpeed);
			float	l_fNewWidth = m_vResolution.x * m_fScale;
			float	l_fNewHeight = m_vResolution.y * m_fScale;
			m_vViewRect.x = m_CameraPos.x - (l_fNewWidth / 2.f);
			m_vViewRect.z = m_vViewRect.x + l_fNewWidth;
			m_vViewRect.y = m_CameraPos.y - (l_fNewHeight / 2.f);
			m_vViewRect.w = m_vViewRect.y + l_fNewHeight;
		}
		else
		{
			float	l_fNewWidth = m_vResolution.x * m_fScale;
			float	l_fNewHeight = m_vResolution.y * m_fScale;
			m_vViewRect.x = m_CameraPos.x - (l_fNewWidth / 2.f);
			m_vViewRect.z = m_vViewRect.x + l_fNewWidth;
			m_vViewRect.y = m_CameraPos.y - (l_fNewHeight / 2.f);
			m_vViewRect.w = m_vViewRect.y + l_fNewHeight;
			m_CameraPos.x = (m_vViewRect.z - m_vViewRect.x) / 2 + m_vViewRect.x;
			m_CameraPos.y = (m_vViewRect.w - m_vViewRect.y) / 2 + m_vViewRect.y;
		}
		m_MouseWorldPosition = ConvertMousePositionToWorldPosition(Vector2((float)m_MouseBehave.GetMouseState().iCurrentX, (float)m_MouseBehave.GetMouseState().iCurrentY), Vector2((float)e_vViewport.x, (float)e_vViewport.y));
		//below is not necessary,but it's helpful while we want to draw rectange to show position info on the screen
		POINT	l_MouseWorldPosition = {(int)m_MouseWorldPosition.x,(int)m_MouseWorldPosition.y};
		m_DrawSelectRectangleByMouse.Update(this->m_MouseBehave.GetMouseState().bLefButton,l_MouseWorldPosition);
		l_sMouseState.bLefButton = false;
		l_sMouseState.bMidButton = false;;
		l_sMouseState.bRigButton = false;;
		m_MouseBehave.Update(l_sMouseState);
	}

	void	cOrthogonalCamera::CameraUpdateByMouse(MyMath::AXIS e_Axis,bool e_bLeftButton,bool e_bRightButton,int e_iWheelDelta,int e_iMouseX,int e_iMouseY,Vector2 e_vViewport)
	{
		cOrthogonalCamera	l_OrthogonalCamera;
		l_OrthogonalCamera.CameraUpdateByMouse(e_bLeftButton,e_bRightButton,e_iWheelDelta,e_iMouseX,e_iMouseY,e_vViewport);
		if( e_Axis == MyMath::W )
			return;
		switch(  e_Axis )
		{
			case MyMath::X:
				l_OrthogonalCamera.SetLocalTransform(*cMatrix44::X_TO_Z*l_OrthogonalCamera.GetLocalTransform());
				break;
			case MyMath::Y:
				l_OrthogonalCamera.SetLocalTransform(*cMatrix44::Y_TO_Z*l_OrthogonalCamera.GetLocalTransform());
				break;
			case MyMath::Z:
				break;
			default:
				break;
		}
		this->SetLocalTransform(l_OrthogonalCamera.GetLocalTransform());
	}

	bool		cOrthogonalCamera::ViewportConvert(Vector4 e_vShowRect,Vector2 e_vRealViewPort,Vector4*e_pResult)
	{
		if(!IsVisible(e_vShowRect))
			return false;
		if( e_vShowRect.Width() == 0 || e_vShowRect.Height() == 0 )
			return false;
		if( e_pResult )
		{
			Vector4	l_vLocalSpaceShowRect(e_vShowRect.x-m_vViewRect.x,e_vShowRect.y-m_vViewRect.y,
				e_vShowRect.z-m_vViewRect.x,e_vShowRect.w-m_vViewRect.y);
			if( e_pResult )
				*e_pResult = ViewRectToOpenGLScissor(l_vLocalSpaceShowRect);
		}
		return true;
	}

	bool		cOrthogonalCamera::IsVisible(Vector4 e_vShowRect)
	{
		return this->m_vViewRect.Collide(e_vShowRect);
	}

	cZoomableCamera::cZoomableCamera()
	{
		m_vMinViewRange = Vector2(240,135);
		m_vMaxViewRange = Vector4(0,0,1920,1080);
		m_vViewRect = Vector4(0,0,1920,1080);
		glhOrthof2(m_ProjectionMatrix,m_vViewRect.x,m_vViewRect.z,m_vViewRect.w,m_vViewRect.y, -10000, 10000);	
		m_bDoCenterBack = false;
		m_bMouseUp = false;
		m_BackToCenterTimer.SetTargetTime(0.1f);
	}

	cZoomableCamera::~cZoomableCamera()
	{
		
	}


	void	cZoomableCamera::MakeViewRectLegal(Vector4 e_vViewRect,bool e_bScale)
	{
		m_bDoCenterBack = false;
		bool	l_b = m_vMaxViewRange.IsOutOfRange(e_vViewRect);
		//check out of range
		if(l_b)
		{
			if( e_bScale )
			{
				m_vNewTargetCenter = m_vOldTargetCenter = this->m_vViewRect.GetCenter();
				//for horizontal
				float	l_fOffset = 0.f;
				if( e_vViewRect.x < m_vMaxViewRange.x && e_vViewRect.z > m_vMaxViewRange.z )
				{
					m_vNewTargetCenter.x = m_vMaxViewRange.GetCenter().x;
				}
				else
				if( e_vViewRect.x < m_vMaxViewRange.x )
				{
					l_fOffset = e_vViewRect.x-m_vMaxViewRange.x;
					m_vNewTargetCenter.x -= l_fOffset;
				}
				else
				if( e_vViewRect.z > m_vMaxViewRange.z )
				{
					l_fOffset = e_vViewRect.z-m_vMaxViewRange.z;
					m_vNewTargetCenter.x -= l_fOffset;
				}
				//for vertical
				if( e_vViewRect.y < m_vMaxViewRange.y && e_vViewRect.w > m_vMaxViewRange.w )
				{
					m_vNewTargetCenter.y = m_vMaxViewRange.GetCenter().y;
				}
				else
				if( e_vViewRect.y < m_vMaxViewRange.y )
				{
					l_fOffset = e_vViewRect.y-m_vMaxViewRange.y;
					m_vNewTargetCenter.y -= l_fOffset;
				}
				else
				if( e_vViewRect.w > m_vMaxViewRange.w )
				{
					l_fOffset = e_vViewRect.w-m_vMaxViewRange.w;
					m_vNewTargetCenter.y -= l_fOffset;
				}
				if( m_bDoCenterBack == false )
				{
					//check use max or min rect
					if(e_vViewRect.IsContainRect(this->m_vMaxViewRange))
					{//this is all out of expect
						m_vLastLegalSize = m_vMaxViewRange.Size();
					}
					else
					{//a part is out of
						m_vLastLegalSize = this->m_vViewRect.Size();
					}
				}
				Vector2	l_vNewCenter = m_vNewTargetCenter;
				if( l_vNewCenter.x-m_vLastLegalSize.x/2 < this->m_vMaxViewRange.x || 
					l_vNewCenter.y-m_vLastLegalSize.y/2 < this->m_vMaxViewRange.y ||
					l_vNewCenter.x+m_vLastLegalSize.x/2 > this->m_vMaxViewRange.z ||
					l_vNewCenter.y+m_vLastLegalSize.y/2 > this->m_vMaxViewRange.w)
				{
					m_vLastLegalSize = m_vMaxViewRange.Size();
					m_vNewTargetCenter = m_vMaxViewRange.GetCenter();
				}
				m_BackToCenterTimer.Start();
				m_bDoCenterBack = true;
			}
			else
			{
				Vector2	l_vSize = e_vViewRect.Size();
				//e_vViewRect.Clamp(m_vMaxViewRange);
				if(e_vViewRect.x < this->m_vMaxViewRange.x &&
					e_vViewRect.z > this->m_vMaxViewRange.z)
				{
					e_vViewRect.x = m_vMaxViewRange.x;
					e_vViewRect.z = m_vMaxViewRange.x+m_vMaxViewRange.Width();
					e_vViewRect.y = m_vMaxViewRange.y;
					e_vViewRect.w = m_vMaxViewRange.y+m_vMaxViewRange.Height();
				}
				else
				if( e_vViewRect.x < this->m_vMaxViewRange.x )
				{
					e_vViewRect.x = m_vMaxViewRange.x;
					e_vViewRect.z = m_vMaxViewRange.x+l_vSize.x;
				}
				else
				if( e_vViewRect.z > this->m_vMaxViewRange.z )
				{
					e_vViewRect.x = m_vMaxViewRange.z-l_vSize.x;
					e_vViewRect.z = m_vMaxViewRange.z;	
				}

				if(e_vViewRect.y < this->m_vMaxViewRange.y &&
					e_vViewRect.w > this->m_vMaxViewRange.w)
				{
					e_vViewRect.x = m_vMaxViewRange.x;
					e_vViewRect.z = m_vMaxViewRange.x+m_vMaxViewRange.Width();
					e_vViewRect.y = m_vMaxViewRange.y;
					e_vViewRect.w = m_vMaxViewRange.y+m_vMaxViewRange.Height();
				}
				else
				if( e_vViewRect.y < this->m_vMaxViewRange.y )
				{
					e_vViewRect.y = m_vMaxViewRange.y;
					e_vViewRect.w = m_vMaxViewRange.y+l_vSize.y;
				}
				else
				if( e_vViewRect.w > this->m_vMaxViewRange.w )
				{
					e_vViewRect.y = m_vMaxViewRange.w-l_vSize.y;
					e_vViewRect.w = m_vMaxViewRange.w;
				}
			}
		}
		else
		{
			Vector2	l_vCenter = e_vViewRect.GetCenter();
			Vector2	l_vSize = e_vViewRect.Size();
			if( e_bScale )
			{
				if( (int)l_vSize.x < (int)m_vMinViewRange.x )
				{
					e_vViewRect.x = l_vCenter.x-m_vMinViewRange.x/2;
					e_vViewRect.z = l_vCenter.x+m_vMinViewRange.x/2;
				}

				if( (int)l_vSize.y < (int)m_vMinViewRange.y )
				{
					e_vViewRect.y = l_vCenter.y-m_vMinViewRange.y/2;
					e_vViewRect.w = l_vCenter.y+m_vMinViewRange.y/2;
				}
			}

		}
		m_vViewRect = e_vViewRect;
	}

	void	cZoomableCamera::Zoom(Vector2 e_vCenter,float e_fScale)
	{
		if( m_bMouseUp )
			return;
		//first find the percentage to left up right down by e_vCenter
		//second get new height and width
		//third set percentage as first and the center still at sam position.
		float	l_fWidth = m_vViewRect.Width();
		float	l_fHeight = m_vViewRect.Height();
		float	l_fWidthPercentage = (e_vCenter.x-m_vViewRect.x)/l_fWidth;
		float	l_fHeightPercentage = (e_vCenter.y-m_vViewRect.y)/l_fHeight;
		float	l_fNewWidth = l_fWidth*e_fScale;
		float	l_fNewHeight = l_fHeight*e_fScale;
		float	l_fLeftLength = l_fNewWidth * l_fWidthPercentage;
		float	l_fRightLength = l_fNewWidth - l_fLeftLength;
		float	l_fUpLength = l_fNewHeight*l_fHeightPercentage;
		float	l_fDownLength = l_fNewHeight-l_fUpLength;

		Vector4	l_vViewRect;
		l_vViewRect.x = e_vCenter.x-l_fLeftLength;
		l_vViewRect.y = e_vCenter.y-l_fUpLength;
		l_vViewRect.z = e_vCenter.x+l_fRightLength;
		l_vViewRect.w = e_vCenter.y+l_fDownLength;
		MakeViewRectLegal(l_vViewRect,true);
	}
	//
	void	cZoomableCamera::Move(Vector2 e_vDirection)
	{
		Vector4 l_vMovedViewrect = m_vViewRect;
		l_vMovedViewrect.Move(e_vDirection);
		MakeViewRectLegal(l_vMovedViewrect,false);
	}
	//
	float*	cZoomableCamera::GetProjection()
	{
		glhOrthof2(m_ProjectionMatrix,m_vViewRect.x,m_vViewRect.z,m_vViewRect.w,m_vViewRect.y, -10000, 10000);
		FATMING_CORE::SetupShaderViewProjectionMatrix( m_ProjectionMatrix,true );
		return m_ProjectionMatrix;
	}

	void	cZoomableCamera::Update(float e_fElpaseTime)
	{
		if( m_bDoCenterBack && m_bMouseUp )
		{
			//if( e_fElpaseTime > 0.016f )
			//	e_fElpaseTime = 0.016f;
			m_BackToCenterTimer.Update(e_fElpaseTime);
			float	l_fLerp = m_BackToCenterTimer.GetLERP();
			Vector2	l_vCenter = UT::LERP<Vector2>(m_vOldTargetCenter,m_vNewTargetCenter,l_fLerp);
			//Vector2	l_vMaxSize = this->m_vMaxViewRange.Size();
			Vector2		l_MouseUpViewRectSize = m_vMouseUpViewRect.Size();
			Vector2	l_vSize = UT::LERP<Vector2>(l_MouseUpViewRectSize,m_vLastLegalSize,l_fLerp)/2;
			this->m_vViewRect.x = l_vCenter.x-l_vSize.x;
			this->m_vViewRect.y = l_vCenter.y-l_vSize.y;
			this->m_vViewRect.z = l_vCenter.x+l_vSize.x;
			this->m_vViewRect.w = l_vCenter.y+l_vSize.y;
			if( m_BackToCenterTimer.bTragetTimrReached )
			{
				m_bDoCenterBack = false;
				m_bMouseUp = false;
				m_vViewRect.x = m_vNewTargetCenter.x-m_vLastLegalSize.x/2;
				m_vViewRect.z = m_vNewTargetCenter.x+m_vLastLegalSize.x/2;
				m_vViewRect.y = m_vNewTargetCenter.y-m_vLastLegalSize.y/2;
				m_vViewRect.w = m_vNewTargetCenter.y+m_vLastLegalSize.y/2;
				//it should not happen,but I am lazy to avoid this
				if( m_vViewRect.x < this->m_vMaxViewRange.x || 
					m_vViewRect.y < this->m_vMaxViewRange.y ||
					m_vViewRect.z > this->m_vMaxViewRange.z ||
					m_vViewRect.w > this->m_vMaxViewRange.w)
				{
					m_vViewRect = m_vMaxViewRange;
				}
			}
		}
	}

	void	cZoomableCamera::SetViewData(Vector2 e_vMinViewRange,Vector4	e_vMaxViewRange,Vector4	e_vViewRect)
	{
		m_vMinViewRange = e_vMinViewRange;
		m_vMaxViewRange = e_vMaxViewRange;
		m_vViewRect = e_vViewRect;
	}

	void	cZoomableCamera::SetMouseUp(bool e_b)
	{
		m_bMouseUp = e_b;
		if( m_bMouseUp )
			m_vMouseUpViewRect = this->m_vViewRect;
	}

//end namespace FATMING_CORE
}
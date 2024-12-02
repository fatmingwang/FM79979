#include "OpenGLCommon.h"
#include "../GameApp/GameApp.h"

Vector3	WorldToScreen(float objX, float objY, float objZ, float *e_pfProjectionMatrix, float *e_pfModelViewMatrix)
{
	GLint realy;
	float win_x, win_y, win_z;
	int viewport[4];
	MyGLGetIntegerv(GL_VIEWPORT, viewport);
	gluProject(objX, objY, objZ, e_pfModelViewMatrix, e_pfProjectionMatrix, viewport, &win_x, &win_y, &win_z);
	realy = viewport[3] - (GLint)win_y - 1;

	Vector3 temp((float)win_x, (float)realy, (float)win_z);
	return temp;
}

Vector3	WorldToScreen(float objX, float objY, float objZ, float*e_pmatProjection, float*e_pmatMV, int *e_piViewPort)
{
	GLint realy;
	float win_x, win_y, win_z;
	gluProject(objX, objY, objZ, e_pmatMV, e_pmatProjection, e_piViewPort, &win_x, &win_y, &win_z);
	realy = e_piViewPort[3] - (GLint)win_y - 1;

	Vector3 temp((float)win_x, (float)realy, (float)win_z);
	return temp;
}

Vector3	ScreenToWorld(Vector3 e_vScreenPos, cMatrix44 e_matVP, Vector2 e_vViewPortSize)
{
	//the z indicate how far will it be
	Vector4	l_vWolrdPos = Vector4((e_vScreenPos.x / e_vViewPortSize.x - 0.5f) * 2, (-e_vScreenPos.y / e_vViewPortSize.y + 0.5f) * 2, e_vScreenPos.z, 1);
	l_vWolrdPos = e_matVP.Inverted().TransformCoordinate(l_vWolrdPos);
	float	l_iInverseW = 1 / l_vWolrdPos.w;
	l_vWolrdPos = l_vWolrdPos * l_iInverseW;
	return *(Vector3*)&l_vWolrdPos;
}

Vector4	ViewRectToOpenGLScissor(Vector4 e_v2DViewRange,Vector4 e_vViewPort,Vector2 e_vGameResolution,eDeviceDirection e_eDeviceDirection)
{
	Vector2 l_vViewSize(e_vViewPort.Width(),e_vViewPort.Height());
	Vector2	l_vScale(1.f,1.f);
	switch(e_eDeviceDirection)
	{
		case eDD_PORTRAIT:
		case eDD_UPSIDE_DOWN:
			l_vScale = Vector2(e_vGameResolution.x/l_vViewSize.x,e_vGameResolution.y/l_vViewSize.y);
			break;
		case eDD_LANDSCAPE_LEFT:
		case eDD_LANDSCAPE_RIGHT:
			l_vScale = Vector2(e_vGameResolution.x/l_vViewSize.y,e_vGameResolution.y/l_vViewSize.x);
			break;
		default:
			break;
	}
	e_v2DViewRange.x /= l_vScale.x;
	e_v2DViewRange.y /= l_vScale.y;
	e_v2DViewRange.z /= l_vScale.x;
	e_v2DViewRange.w /= l_vScale.y;
	//add offset
	if(cGameApp::m_spOpenGLRender->m_vViewPortSize.x != 0)
	{
		e_v2DViewRange.x += cGameApp::m_spOpenGLRender->m_vViewPortSize.x;
		e_v2DViewRange.z += cGameApp::m_spOpenGLRender->m_vViewPortSize.x;
	}
	//add offset
	if(cGameApp::m_spOpenGLRender->m_vViewPortSize.y != 0)
	{
		e_v2DViewRange.y -= cGameApp::m_spOpenGLRender->m_vViewPortSize.y;
		e_v2DViewRange.w -= cGameApp::m_spOpenGLRender->m_vViewPortSize.y;
	}
	Vector4	l_vScissor;
	float	l_fWidth = (e_v2DViewRange.z-e_v2DViewRange.x);
	float	l_fHeight = (e_v2DViewRange.w-e_v2DViewRange.y);
	
	switch(e_eDeviceDirection)
	{
		case eDD_PORTRAIT:
			l_vScissor.x = e_v2DViewRange.x;
			l_vScissor.y = l_vViewSize.y-(e_v2DViewRange.y+l_fHeight);
			break;
		case eDD_UPSIDE_DOWN:
			l_vScissor.x = l_vViewSize.x-(e_v2DViewRange.x+l_fWidth);
			l_vScissor.y = e_v2DViewRange.y;
			break;
		case eDD_LANDSCAPE_LEFT:
		{
			l_vScissor.x = e_v2DViewRange.y;
			l_vScissor.y = e_v2DViewRange.x;
		}
			break;
		case eDD_LANDSCAPE_RIGHT:
			l_vScissor.x = l_vViewSize.x-e_v2DViewRange.w;
			l_vScissor.y = l_vViewSize.y-e_v2DViewRange.z;
			break;
		default:
			break;
	}
	switch(e_eDeviceDirection)
	{
		case eDD_PORTRAIT:
		case eDD_UPSIDE_DOWN:
			l_vScissor.z = l_fWidth;
			l_vScissor.w = l_fHeight;
			break;
		case eDD_LANDSCAPE_LEFT:
		case eDD_LANDSCAPE_RIGHT:
			l_vScissor.w = l_fWidth;
			l_vScissor.z = l_fHeight;
			break;
		default:
			break;
	}
	return l_vScissor;
}

Vector4	ViewRectToOpenGLScissor(Vector4 e_v2DViewRange)
{
	return ViewRectToOpenGLScissor(e_v2DViewRange,cGameApp::m_spOpenGLRender->m_vViewPortSize,FATMING_CORE::cGameApp::m_spOpenGLRender->m_vGameResolution,FATMING_CORE::cGameApp::m_spOpenGLRender->m_eDeviceDirection);
}
#include "stdafx.h"
#include "GameApp.h"
#include "SceneControl.h"
#include "MainCharacterBehavior.h"
#include "UIInfo.h"
UT::sTimeAndFPS g_TimeAndFPS;
cSceneControl*					cMagicTowerApp::m_spSceneControl = 0;
cOrthogonalCamera*				cMagicTowerApp::m_sp2DCamera = 0;
cMainRoleData*					cMagicTowerApp::m_spMainRoleData = 0;;
cMagicTowerApp*					g_pMagicTowerApp = 0;
bool							g_bGameLeave = false;
#if defined(ANDROID)
cMagicTowerApp::cMagicTowerApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject* e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize ):cGameApp(e_pActivity,e_pThreadEnv,e_pAppThreadThis,e_vGameResolution,e_vViewportSize )
#elif defined(WIN32)
cMagicTowerApp::cMagicTowerApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
#endif
{
	g_pMagicTowerApp = this;
	m_spSceneControl = 0;
	m_sp2DCamera = 0;
	m_pUIInfo = 0;
}

cMagicTowerApp::~cMagicTowerApp()
{
	g_bGameLeave = true;
	SAFE_DELETE(m_pUIInfo);
	SAFE_DELETE(m_spSceneControl);
	SAFE_DELETE(m_sp2DCamera);
	SAFE_DELETE(m_spMainRoleData);
}

void	cMagicTowerApp::Init()
{
	cGameApp::Init();
	cNodeISAX	l_cNodeISAX;
	if(l_cNodeISAX.ParseDataIntoXMLNode(MAIN_ROLE_SETUP_DATA))
	{
		m_spMainRoleData = new cMainRoleData(l_cNodeISAX.GetRootElement());
	}
	m_sp2DCamera = new cOrthogonalCamera(Vector2(480,320));
	m_spSceneControl = new cSceneControl();
	POINT	l_ViewSize = {480,320};
	m_sp2DCamera->SetResolution(Vector2(480,320));
	//m_sp2DCamera->Update(l_ViewSize);
	if( m_spSceneControl )
		m_spSceneControl->Load();
	m_pUIInfo = new cUIInfo();
	//g_pDebugFont = new cGlyphFontRender("MagicTower/Font");
}

void	cMagicTowerApp::Destory()
{

}

void	cMagicTowerApp::Update(float e_fElpaseTime)
{
	if( g_bGameLeave )
		return;
	cGameApp::Update(e_fElpaseTime);
	if( m_spSceneControl )
		m_spSceneControl->Update(e_fElpaseTime);
	if(m_pUIInfo)
		m_pUIInfo->Update(e_fElpaseTime);
}

void	cMagicTowerApp::Render()
{
	if( g_bGameLeave )
		return;
	cGameApp::Render();
	//UseShaderProgram();
	//glViewport(0,0,(int)this->m_svViewPortSize.x,(int)this->m_svViewPortSize.y);
	//glEnable(GL_SCISSOR_TEST);
	//Vector4	l_vViewRect(0,0,cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
	//l_vViewRect = ViewRectToOpenGLScissor(l_vViewRect);
	//glScissor((int)l_vViewRect.x,(int)l_vViewRect.y,(int)l_vViewRect.z,(int)l_vViewRect.w);
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_ALPHA_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	if(m_pUIInfo)
		m_pUIInfo->Render();
	m_sp2DCamera->Render();
	if( m_spSceneControl )
		m_spSceneControl->Render();
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_COLOR_ARRAY);
	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_ALPHA_TEST);
	if( m_spSceneControl )
		m_spSceneControl->DebugRender();
	cGameApp::RenderFont(0,0,UT::ComposeMsgByFormat(L"Mouse:%d,%d",cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y).c_str());
	GLRender::glDisable2D();
	SwapBuffers (this->m_sHdc);
}

//void	cMagicTowerApp::Run()
//{
//	g_TimeAndFPS.Update();
//	Update(g_TimeAndFPS.fElpaseTime);
//	Render();
//}

void	cMagicTowerApp::MouseDown(int e_iPosX,int e_iPosY)
{
	if( g_bGameLeave )
		return;
	//this->m_sMousePosition.x = e_iPosX;
	//this->m_sMousePosition.y = e_iPosY;
	cGameApp::MouseDown(e_iPosX,e_iPosY);
	if( m_spSceneControl )
		this->m_spSceneControl->MouseDown(this->m_sMousePosition.x,this->m_sMousePosition.y);
}

void	cMagicTowerApp::MouseMove(int e_iPosX,int e_iPosY)
{
	if( g_bGameLeave )
		return;
	//this->m_sMousePosition.x = e_iPosX;
	//this->m_sMousePosition.y = e_iPosY;
	cGameApp::MouseMove(e_iPosX,e_iPosY);
		if( m_spSceneControl )
	this->m_spSceneControl->MouseMove(this->m_sMousePosition.x,this->m_sMousePosition.y);
}

void	cMagicTowerApp::MouseUp(int e_iPosX,int e_iPosY)
{
	if( g_bGameLeave )
		return;
	//this->m_sMousePosition.x = e_iPosX;
	//this->m_sMousePosition.y = e_iPosY;
	cGameApp::MouseUp(e_iPosX,e_iPosY);
		if( m_spSceneControl )
	this->m_spSceneControl->MouseUp(this->m_sMousePosition.x,this->m_sMousePosition.y);
}

void	cMagicTowerApp::KeyDown(char e_char)
{
	if( g_bGameLeave )
		return;
	if( m_spSceneControl )
		m_spSceneControl->KeyDown(e_char);
}

void	cMagicTowerApp::ChangeWalkingView()
{
	if( g_bGameLeave )
		return;
	if( m_spSceneControl )
	{
		cWalkingInMapPhase*l_pWalkingInMapPhase = (cWalkingInMapPhase*)cMagicTowerApp::m_spSceneControl->GetObject(WALKING_IN_MAP_PHASE);
		l_pWalkingInMapPhase->SetWalkingModelToFix(!l_pWalkingInMapPhase->IsWalkingModeIsFix());
	}
}
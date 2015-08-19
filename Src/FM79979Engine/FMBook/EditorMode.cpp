#include "stdafx.h"
#include "EditorMode.h"

//for editor,event object status
cEventObject*							g_pEditorEventObject = 0;

cEventManager*							cEditorMode::m_spEventManager = 0;
cEventPackageRunner*					cEditorMode::m_spEventPackageRunner = 0;
sFMBookEditorData*						g_pFMBookEditorData = 0;

cEditorMode::cEditorMode()
{
	m_spEventManager = new cEventManager();
	m_spEventManager->m_pEventFont = new cGlyphFontRender("Font");
	if( !m_spEventManager->m_pEventFont->GetFontImage() )
	{
		SAFE_DELETE(m_spEventManager->m_pEventFont);
	}
	m_spEventPackageRunner = m_spEventManager->m_pEventPackageRunner;
	g_pFMBookEditorData = new sFMBookEditorData();
}
cEditorMode::~cEditorMode()
{
	SAFE_DELETE(g_pFMBookEditorData);
	SAFE_DELETE(m_spEventManager);
}
void	cEditorMode::Init()
{

}
void	cEditorMode::Destroy()
{

}
void	cEditorMode::Update(float e_fElpaseTime)
{
	if( g_pEditorEventObject )
	{
		g_pEditorEventObject->Update(e_fElpaseTime);
	}
	else
	if( m_spEventPackageRunner )
		m_spEventPackageRunner->Update(e_fElpaseTime);
}
void	cEditorMode::Render()
{
	//glViewport(0,0,(int)this->m_svViewPortSize.x,(int)m_svViewPortSize.y);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	//Vector4 l_vBGColor = g_pCurrentFMPageDataList->GetBGColor();
	//glClearColor( l_vBGColor.x,l_vBGColor.y,l_vBGColor.z,l_vBGColor.w );
	glClearColor( 0,0,0,1 );
	glClearDepth(1.0f);	
	//glAlphaFunc(GL_GREATER,0.001f);
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
	if( g_pEditorEventObject )
	{
		g_pEditorEventObject->Render();
	}
	else
	if( m_spEventPackageRunner )
		m_spEventPackageRunner->Render();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	if( cGameApp::m_spGlyphFontRender )
	{
		WCHAR   l_str[MAX_PATH];
		swprintf(l_str,MAX_PATH,L"MousePos:%d,%d",cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
		cGameApp::m_spGlyphFontRender->RenderFont(0,0,l_str);
		cGameApp::m_spGlyphFontRender->RenderFont(0,150,UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()));
	}
	if( g_pEditorEventObject )
	{
		g_pEditorEventObject->DebugRender();
	}	
	glDisable2D();
#ifdef WIN32
	SwapBuffers(cGameApp::m_sHdc);
#endif
}
void	cEditorMode::MouseDown(int e_iPosX,int e_iPosY)
{
	if( this->m_spEventPackageRunner )
	{
		this->m_spEventPackageRunner->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	}
	if( g_pEditorEventObject )
	{
		g_pEditorEventObject->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	}
}
void	cEditorMode::MouseMove(int e_iPosX,int e_iPosY)
{
	if( this->m_spEventPackageRunner )
	{
		this->m_spEventPackageRunner->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	}
	if( g_pEditorEventObject )
	{
		g_pEditorEventObject->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	}
}
void	cEditorMode::MouseUp(int e_iPosX,int e_iPosY)
{
	if( this->m_spEventPackageRunner )
	{
		this->m_spEventPackageRunner->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	}
	if( g_pEditorEventObject )
	{
		g_pEditorEventObject->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	}
}
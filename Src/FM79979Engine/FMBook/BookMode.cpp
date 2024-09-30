#include "stdafx.h"
#include "BookMode.h"

cBookMode::cBookMode()
{
	m_bPickupBooks = true;
	m_bInitOk = false;
	m_pEventManager = 0;
	m_pEventPackageRunner = 0;
	m_pScroller = 0;
	m_pBackToPickupBooksButton = 0;
}
cBookMode::~cBookMode()
{
	SAFE_DELETE(m_pBackToPickupBooksButton);
	SAFE_DELETE(m_pEventManager);
	SAFE_DELETE(m_pScroller);
}
void	cBookMode::Init()
{
	cGameApp::m_spOpenGLRender->m_vGameResolution = Vector2(720,1280);
	m_bInitOk = false;
	SAFE_DELETE(m_pEventManager);
	m_pEventManager = new cEventManager();
	m_pEventPackageRunner = m_pEventManager->m_pEventPackageRunner;
	m_bPickupBooks = true;
	m_BookDataVector.clear();
	SAFE_DELETE(m_pScroller);
	m_pScroller = new cScroller((cPuzzleImage*)0,Vector4(50,50,cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y),0,false);		
	m_pScroller->SetName(L"BookModeScroller");
	cNodeISAX	l_cNodeISAX;
	if(l_cNodeISAX.ParseDataIntoXMLNode("BookList.xml"))
	{
		//open booklist.xml
		//<Root>
		//	<Book Name="" FileName="abc/EventPackage.xml">
		//	<Book Name="" FileName="abc/EventPackage.xml">
		//	<Book Name="" FileName="abc/EventPackage.xml">
		//	<Book Name="" FileName="abc/EventPackage.xml">
		//</Root>
		TiXmlElement*l_pElement = l_cNodeISAX.GetRootElement();
		Vector2	l_vStartPos(100,100);
		float	l_fHeightGap = 30.f;
		if( l_pElement )
		{
			const WCHAR*l_strStartPos = l_pElement->Attribute(L"StartPos");
			if( l_strStartPos )
			{
				l_vStartPos = GetVector2(l_strStartPos);
			}
			const WCHAR*l_strHeightGap = l_pElement->Attribute(L"HeightGap");
			if( l_strHeightGap )
			{
				l_fHeightGap = GetFloat(l_strHeightGap);
				m_pScroller->SetObjectGap(l_fHeightGap);
			}
			l_pElement = l_pElement->FirstChildElement();
			while( l_pElement )
			{
				sBookData	l_sBookData;
				const WCHAR*l_strName = l_pElement->Attribute(L"Name");
				const WCHAR*l_strFileName = l_pElement->Attribute(L"FileName");
				const WCHAR*l_strResolution = l_pElement->Attribute(L"Resolution");
				l_sBookData.strPageFileName = UT::WcharToChar(l_strFileName);
				l_sBookData.strPageName = l_strName;
				if( l_strResolution )
					l_sBookData.vResolution = GetVector2(l_strResolution);
				m_BookDataVector.push_back(l_sBookData);
				l_pElement = l_pElement->NextSiblingElement();
				cTextButton*l_pTextButton = new cTextButton(l_vStartPos,cGameApp::m_spGlyphFontRender,Vector4(0.5,0.5,0.5,1),l_strName);
				l_pTextButton->SetName(l_strName);
				m_pScroller->AddObject(l_pTextButton);
				l_vStartPos.y += l_fHeightGap;
			}
		}
		m_pScroller->Init();
	}
	if( !m_pBackToPickupBooksButton )
		m_pBackToPickupBooksButton = new cTextButton(Vector2(0,cGameApp::m_spOpenGLRender->m_vGameResolution.y-100),cGameApp::m_spGlyphFontRender,Vector4(0,1,1,1),L"<---");
	m_pBackToPickupBooksButton->Init();
	m_bInitOk = true;
}
void	cBookMode::Destroy()
{

}
void	cBookMode::Update(float e_fElpaseTime)
{
	if( !m_bInitOk )
		return;
	if( m_bPickupBooks )
	{
		m_pScroller->Update(e_fElpaseTime);
		if( m_pScroller->IsSatisfiedCondition() )
		{
			int	l_iSelectedIndex = m_pScroller->GetCurrentWorkingObjectIndex();
			if( l_iSelectedIndex == -1 )
			{
				m_pScroller->Init();
				return;
			}
			const char*l_strFileName = m_BookDataVector[l_iSelectedIndex].strPageFileName.c_str();
			cGameApp::m_spOpenGLRender->m_vGameResolution = m_BookDataVector[l_iSelectedIndex].vResolution;
			m_pBackToPickupBooksButton->SetPos(Vector3(0,cGameApp::m_spOpenGLRender->m_vGameResolution.y-30,0));
			m_pScroller->Init();
			SAFE_DELETE(m_pEventManager);
			m_pEventManager = new cEventManager();
			m_pEventPackageRunner = m_pEventManager->m_pEventPackageRunner;
			m_bPickupBooks = true;
			if(m_pEventManager->ParseDataIntoXMLNode(l_strFileName))
			{
				m_pEventPackageRunner->LoadPackage();
				m_bPickupBooks = false;
			}
		}
	}
	else
	{
		m_pEventPackageRunner->Update(e_fElpaseTime);
		m_pBackToPickupBooksButton->Update(e_fElpaseTime);
		if( m_pBackToPickupBooksButton->IsSatisfiedCondition() )
		{
			m_pBackToPickupBooksButton->Init();
			m_bPickupBooks = true;
			cGameApp::m_spOpenGLRender->m_vGameResolution = Vector2(720,1280);
			SAFE_DELETE(m_pEventManager);
			cGameApp::m_spAnimationParser->Destroy();
			cGameApp::m_spImageParser->Destroy();
			cGameApp::m_spBehaviorPaticleManager->Destroy();
			cGameApp::m_spPathFileList->Destroy();
			//cGameApp::m_spPhaseManager->Destroy();
			cGameApp::m_spSoundParser->Destroy();
		}
	}
}
void	cBookMode::Render()
{
	if( !m_bInitOk )
		return;
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	//Vector4 l_vBGColor = g_pCurrentFMPageDataList->GetBGColor();
	//glClearColor( l_vBGColor.x,l_vBGColor.y,l_vBGColor.z,l_vBGColor.w );
	glClearColor( 0,0,0,1 );
	glClearDepth(1.0f);	
	//glAlphaFunc(GL_GREATER,0.001f);
	glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	if( m_bPickupBooks )
	{
		m_pScroller->Render();
	}
	else
	{
		m_pEventPackageRunner->Render();
		m_pBackToPickupBooksButton->Render();
		cEventVariable*l_pEventVariable = this->m_pEventManager->m_pEventVariableManager->GetObject(L"CurrentPackageName");
		const WCHAR*l_strCurrentPackageName = l_pEventVariable->GetWString()->c_str();
		cGameApp::m_spGlyphFontRender->RenderFont(cGameApp::m_spOpenGLRender->m_vGameResolution.x/2.f,0.f,l_strCurrentPackageName);
	}
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
		//cGameApp::m_spGlyphFontRender->RenderFont(0,150,UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()));
	}
	glDisable2D();
#ifdef WIN32
	SwapBuffers(cGameApp::m_spOpenGLRender->m_Hdc);
#endif
}
void	cBookMode::MouseDown(int e_iPosX,int e_iPosY)
{
	if( !m_bInitOk )
		return;
	if( m_bPickupBooks )
	{
		m_pScroller->MouseDown(e_iPosX,e_iPosY);
	}
	else
	{
		m_pBackToPickupBooksButton->MouseDown(e_iPosX,e_iPosY);
		m_pEventPackageRunner->MouseDown(e_iPosX,e_iPosY);
	}
}
void	cBookMode::MouseMove(int e_iPosX,int e_iPosY)
{
	if( !m_bInitOk )
		return;
	if( m_bPickupBooks )
	{
		m_pScroller->MouseMove(e_iPosX,e_iPosY);
	}
	else
	{
		m_pBackToPickupBooksButton->MouseMove(e_iPosX,e_iPosY);
		m_pEventPackageRunner->MouseMove(e_iPosX,e_iPosY);
	}
}
void	cBookMode::MouseUp(int e_iPosX,int e_iPosY)
{
	if( !m_bInitOk )
		return;
	if( m_bPickupBooks )
	{
		m_pScroller->MouseUp(e_iPosX,e_iPosY);
	}
	else
	{
		m_pBackToPickupBooksButton->MouseUp(e_iPosX,e_iPosY);
		m_pEventPackageRunner->MouseUp(e_iPosX,e_iPosY);
	}
}
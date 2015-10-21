#include "stdafx.h"
#include "GameApp.h"
#include "FishingPhase.h"
#include "StageSelectPhase.h"
#include "SelectFishRodFishThreadPhase.h"
#include "StorePhase.h"
#include "SetupPhase.h"
#include "ShowFishingResultPhase.h"
#include "ShowFishCollectionPhase.h"
#include "Math\MathFormulaXML.h"
//#include "TestPhase.h"

cFishRodAndFishThreadData*						cFishingFunApp::m_spFishRodAndFishThreadData = 0;
//cNamedTypedObjectVector<cColladaParser>*		cFishingFunApp::m_spAllColladaData = 0;
cPlayerData*									cFishingFunApp::m_spPlayerData = 0;


#if defined(ANDROID)
cFishingFunApp::cFishingFunApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize,NvEGLUtil*e_pNvEGLUtil ):cGameApp(e_pActivity,e_pThreadEnv,e_pAppThreadThis,e_vGameResolution,e_vViewportSize,e_pNvEGLUtil)
#elif defined(WIN32)
cFishingFunApp::cFishingFunApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
#else
cFishingFunApp::cFishingFunApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_vGameResolution,e_vViewportSize)
#endif
{
	cMathFormulaXML l_;
	l_.SetExpr(L"(a/b)/(1+(((60-20)/20*0.1)))");
	//l_.SetExpr(L"2");
	l_.AddDefineVariable(L"a",1);
	l_.AddDefineVariable(L"b",2);
	double aa = l_.GetResult();
	m_bLeave = false;
	m_pPhaseManager = new cPhaseManager();	
#ifdef DEBUG
	this->m_sbSpeedControl = true;
#endif
	m_spFishRodAndFishThreadData = new cFishRodAndFishThreadData();
	//m_spAllColladaData = new cNamedTypedObjectVector<cColladaParser>();
	m_spPlayerData = new cPlayerData();
	cNamedTypedObjectVector<cColladaParser>*l_pColladaParserVector = new cNamedTypedObjectVector<cColladaParser>();
	m_spColladaParserVector = (cNamedTypedObjectVector<NamedTypedObject>*)l_pColladaParserVector;
}

cFishingFunApp::~cFishingFunApp()
{
	SAFE_DELETE(m_spFishRodAndFishThreadData);
	SAFE_DELETE(m_pPhaseManager);
	//SAFE_DELETE(m_spAllColladaData);
	SAFE_DELETE(m_spPlayerData);
	Destroy();
}

void	cFishingFunApp::Init()
{
	cGameApp::Init();
	if( m_spFishRodAndFishThreadData )
	{
		PARSE_FAILED_MESSAGE_BOX(m_spFishRodAndFishThreadData,"Data/FishRodAndFishThreadData.xml");
	}
	cFishingPhase*l_pFishingPhase = new cFishingPhase();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pFishingPhase);
	cStageSelectPhase*l_pStageSelectPhase = new cStageSelectPhase();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pStageSelectPhase);
	cSelectFishRodFishThreadPhase*l_pSelectFishRodFishThreadPhase = new cSelectFishRodFishThreadPhase();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pSelectFishRodFishThreadPhase);
	cStorePhase*l_pStorePhase = new cStorePhase();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pStorePhase);
	cSetupPhase*l_pSetupPhase = new cSetupPhase();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pSetupPhase);
	cShowFishingResultPhase*l_pShowFishingResultPhase = new cShowFishingResultPhase();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pShowFishingResultPhase);
	cShowFishCollectionPhase*l_pShowFishCollectionPhase = new cShowFishCollectionPhase();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pShowFishCollectionPhase);
	//cGamePhase*l_pGamePhase = new cGamePhase();
	//l_pGamePhase->SetName(L"Test");
	//bool	l_b = this->m_pPhaseManager->AddObject(l_pGamePhase);
	//if( !l_b )
	//{
		//assert(0&&"this phase has been added!");
	//}
	//this->m_pPhaseManager->SetCurrentCurrentPhase(l_pShowFishCollectionPhase->GetName());
	this->m_pPhaseManager->SetCurrentCurrentPhase(l_pStageSelectPhase->GetName());
	//this->m_pPhaseManager->SetCurrentCurrentPhase(l_pFishingPhase->GetName());
	m_spPlayerData->ParsePlayerData();
	//let first update is not too big
	this->m_sTimeAndFPS.Update();
}

void	cFishingFunApp::Update(float e_fElpaseTime)
{
    cGameApp::Update(e_fElpaseTime);
	this->m_pPhaseManager->Update(e_fElpaseTime);
}

void	cFishingFunApp::Render()
{
	//glViewport(0,0,(int)m_svViewPortSize.Width(),(int)m_svViewPortSize.Height());
	//glScissor(0,0,(int)m_svViewPortSize.x,(int)m_svViewPortSize.y);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glClearColor( 0,0.5,0.5,1 );
	glClearDepth(1.0f);	
#ifndef ANDROID
	glEnable(GL_TEXTURE_2D);
#endif
	float	l_fVP[16];
	//UseShaderProgram();
	//glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,l_fVP);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glAlphaFunc(GL_GREATER,0.001f);	
	UseShaderProgram();
	cGameApp::Render();
	this->m_pPhaseManager->Render();
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,l_fVP);
	if( cGameApp::m_spGlyphFontRender )
	{
	    WCHAR   l_str[MAX_PATH];
	    swprintf(l_str,MAX_PATH,L"MousePos:%d,%d",cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	    cGameApp::m_spGlyphFontRender->RenderFont(0,0,l_str);
	    cGameApp::m_spGlyphFontRender->RenderFont(0,20,UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()));
	}
	cGameApp::RenderPause();
	m_spPlayerData->Render();
	glDisable(GL_TEXTURE_2D);
	if( this->m_spMultiTouchPoints->TouchPointWorkingIndex.size() == 4 )
		m_sbDebugFunctionWorking = true;
	else
	if( this->m_spMultiTouchPoints->TouchPointWorkingIndex.size() == 3 )
		m_sbDebugFunctionWorking = false;
	if( m_sbDebugFunctionWorking )
	{
		this->m_pPhaseManager->DebugRender();
#ifdef	ANDROID
		//fuck here occur glerror and I am lazy to fix it.
		UseShaderProgram();
		glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,l_fVP);
		std::wstring	l_str = L"Xaxis:";
		l_str += ValueToStringW(cGameApp::m_svAccelerometer.x);
		l_str += L",Yaxis:";
		l_str += ValueToStringW(m_svAccelerometer.y);
		cGameApp::RenderFont(0,700,l_str);
		cGameApp::m_spMultiTouchPoints->DebugRender(20);
#endif
	}
	glDisable2D();
#ifdef WIN32
	SwapBuffers(cGameApp::m_sHdc);
#endif
}

void	cFishingFunApp::MouseDown(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseDown(e_iPosX,e_iPosY);
	this->m_pPhaseManager->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
}

void	cFishingFunApp::MouseMove(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseMove(e_iPosX,e_iPosY);
	this->m_pPhaseManager->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
}

void	cFishingFunApp::MouseUp(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseUp(e_iPosX,e_iPosY);
	this->m_pPhaseManager->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
}

void	cFishingFunApp::KeyDown(char e_char)
{
	 cGameApp::KeyDown(e_char);
}
void	cFishingFunApp::KeyUp(char e_char)
{
	cGameApp::KeyUp(e_char);
	if( e_char == 68 )
		this->m_sbDebugFunctionWorking = !m_sbDebugFunctionWorking;
}

cColladaParser*	cFishingFunApp::ProcessDaeFile(TiXmlElement*e_pElement)
{
	std::wstring	l_strFileName = e_pElement->Attribute(L"FileName");
	if( l_strFileName.length() )
	{
		cNamedTypedObjectVector<cColladaParser>*l_pColladaParserVector = (cNamedTypedObjectVector<cColladaParser>*)cGameApp::m_spColladaParserVector;
		cColladaParser*l_pColladaParser = l_pColladaParserVector->GetObject(l_strFileName);
		if( l_pColladaParser )
			return l_pColladaParser;
		l_pColladaParser = new cColladaParser();
		if(l_pColladaParser->ParseDataIntoXMLNode(UT::WcharToChar(l_strFileName).c_str(),true))
		{
			l_pColladaParserVector->AddObjectNeglectExist(l_pColladaParser);
			return l_pColladaParser;
		}
		SAFE_DELETE(l_pColladaParser);
	}
	return 0;
}
#include "stdafx.h"
#include "TitlePhase.h"
#include "AllPhaseDefine.h"
#include "BluffingGirlApp.h"
#include "GameNetwork.h"
#ifdef WIN32
//VS2012
#include "shellapi.h"
#endif
cTitlePhase::cTitlePhase()
{
	m_pOpeningMPDI = 0;
	m_pClickMouseBehaviorVector = 0;
	this->SetName(TITLE_PHASE_NAME);
	m_strTutorialVideoLink = "https://www.youtube.com/watch?v=rXgENISqKmc";
	m_strFansLink = "https://www.facebook.com/dicegirl";
	//m_pOpeningMPDI = new cMPDI::GetMe();
}

cTitlePhase::~cTitlePhase()
{
	Destroy();
}

void	cTitlePhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("cClickMouseBehaviorVector")
	{
		m_pClickMouseBehaviorVector = GetClickMouseBehaviorList(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("OPeningMPDI")
	{
		m_pOpeningMPDI = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
		if(m_pOpeningMPDI)
			m_pOpeningMPDI->SetStayAtLastFrame(true);
	}
	else
	COMPARE_VALUE("Link")
	{
		const WCHAR*l_strTutorialVideo = e_pTiXmlElement->Attribute(L"TutorialVideo");
		const WCHAR*l_strFans = e_pTiXmlElement->Attribute(L"Fans");
		m_strTutorialVideoLink = UT::WcharToChar(l_strTutorialVideo);
		m_strFansLink = UT::WcharToChar(l_strFans);
	}
}

void	cTitlePhase::Init()
{
	Destroy();
	PARSE_FAILED_MESSAGE_BOX(this,"BluffingGirl/PhaseData/TitlePhase.xml");
	if( m_pOpeningMPDI )
		m_pOpeningMPDI->Init();
	if( m_pClickMouseBehaviorVector )
		m_pClickMouseBehaviorVector->Init();
	int	l_iADHeight = 50;
	//ShowAd("ca-app-pub-5585058356130084/5814745857",(int)cGameApp::m_svViewPortSize.Width()-20,l_iADHeight,Vector2(0,cGameApp::m_svViewPortSize.w-l_iADHeight-30));
	//ShowAd("ca-app-pub-5585058356130084/2063713857",320,50,Vector2(0,cGameApp::m_svViewPortSize.w-l_iADHeight-30));
#ifdef ANDROID
	ShowIntersitialAd("ca-app-pub-5585058356130084/8796088253");
#endif
}

void	cTitlePhase::Destroy()
{
	if( cBluffingGirlApp::m_spGameNetwork )
		cBluffingGirlApp::m_spGameNetwork->Destroy();
	SAFE_DELETE(m_pClickMouseBehaviorVector);
#ifdef ANDROID
	jclass	l_Class = GetCustomJavaClass("util/AdMobUtility");
	jmethodID l_Method = g_pMainThreadJNIUtilData->pJNIEnv->GetStaticMethodID(l_Class,"Close","()V");
	g_pMainThreadJNIUtilData->pJNIEnv->CallStaticVoidMethod(l_Class, l_Method);
	g_pMainThreadJNIUtilData->pJNIEnv->DeleteLocalRef(l_Class);
#endif
	
}

void	cTitlePhase::Update(float e_fElpaseTime)
{
	if( m_pOpeningMPDI && !m_pOpeningMPDI->IsAnimationDone() )
		m_pOpeningMPDI->Update(e_fElpaseTime);
	else
	if( m_pClickMouseBehaviorVector )
	{
		m_pClickMouseBehaviorVector->Update(e_fElpaseTime);
		if( m_pClickMouseBehaviorVector->IsSatisfiedCondition() )
		{
			int	l_iIndex = m_pClickMouseBehaviorVector->GetCurrentWorkingObjectIndex();
#ifdef WASM//no thread for wasm now!?
			if( l_iIndex == 0 )
#else
			if (l_iIndex < 3)
#endif
			{
				this->m_strNextPhaseName = m_pClickMouseBehaviorVector->GetCurrentWorkingObjectName();
				if(m_pClickMouseBehaviorVector->GetCurrentWorkingObjectIndex() != 2)
					Destroy();
				else
					m_pClickMouseBehaviorVector->Init();
				this->m_bSatisfiedCondition = true;
			}
			else
			{
				m_pClickMouseBehaviorVector->Init();

#if defined(ANDROID)
				if( l_iIndex == 3 )
					OpenURL(m_strTutorialVideoLink.c_str());
				else
					OpenURL(m_strFansLink.c_str());
#elif defined(WIN32)
				if( l_iIndex == 3 )
					ShellExecute(NULL, L"open", L"https://www.youtube.com/watch?v=rXgENISqKmc", NULL, NULL, SW_SHOWNORMAL);
				else
					ShellExecute(NULL, L"open", L"https://www.facebook.com/dicegirl", NULL, NULL, SW_SHOWNORMAL);
#endif
			}
		}
	}
}

void	cTitlePhase::Render()
{
	if( m_pOpeningMPDI )
		m_pOpeningMPDI->Render();
	if( m_pClickMouseBehaviorVector )
	{
		m_pClickMouseBehaviorVector->Render();
	}
}

void	cTitlePhase::DebugRender()
{

}

void	cTitlePhase::MouseDown(int e_iX,int e_iY)
{
	if( !m_pOpeningMPDI || m_pOpeningMPDI->IsAnimationDone() )
	{
		if( m_pClickMouseBehaviorVector )
			m_pClickMouseBehaviorVector->MouseDown(e_iX,e_iY);
	}
}

void	cTitlePhase::MouseUp(int e_iX,int e_iY)
{
	//jclass	l_Class = GetCustomJavaClass("util/AdMobUtility");
	//jmethodID l_Method = g_pMainThreadJNIUtilData->pJNIEnv->GetStaticMethodID(l_Class,"Refresh","()V");
	//g_pMainThreadJNIUtilData->pJNIEnv->CallStaticVoidMethod(l_Class, l_Method);
	//g_pMainThreadJNIUtilData->pJNIEnv->DeleteLocalRef(l_Class);

	if( !m_pOpeningMPDI || m_pOpeningMPDI->IsAnimationDone() )
	{
		if( m_pClickMouseBehaviorVector )
			m_pClickMouseBehaviorVector->MouseUp(e_iX,e_iY);
	}
}

void	cTitlePhase::MouseMove(int e_iX,int e_iY)
{
	if( !m_pOpeningMPDI || m_pOpeningMPDI->IsAnimationDone() )
	{
		if( m_pClickMouseBehaviorVector )
			m_pClickMouseBehaviorVector->MouseMove(e_iX,e_iY);
	}
}
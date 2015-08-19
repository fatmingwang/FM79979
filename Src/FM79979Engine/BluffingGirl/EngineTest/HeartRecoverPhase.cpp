#include "stdafx.h"
#include "../../Core/Terrain/ShockBoobsPainter.h"
#include "HeartRecoverPhase.h"
#include "AllPhaseDefine.h"
#include "BluffingGirlApp.h"
#include "PlayerData.h"
#include "HeartRecoverSelectGirlPhase.h"


cHeartRecoverPhase::cHeartRecoverPhase()
{
	this->SetName(HEART_RECOVER_PHASE_NAME);
	m_pCurrentMessenger = 0;
	m_pSuccessMessenger = 0;
	m_pFailedMessenger = 0;
	m_pTraningMessenger = 0;
	m_pShowMPDI = 0;
	m_pStartConfirmButton = 0;
	m_pCountDownNumeralImage = 0;
	m_pShockBoobsPainter = 0;	
	m_vLastTimeDirection.x = cGameApp::m_svAccelerometer.x;
	m_vLastTimeDirection.y = cGameApp::m_svAccelerometer.y;
	m_NexShackTime.SetTargetTime(0.1f);
	m_fLastBigStregth = 0.f;
	m_fCurrentAddedStregth = 0.f;
	m_fTargetStregth = 10.f;
	m_fTimeReduceStrgeth = 5.f;
	m_fShockAddStrgeth = 1.f;
	m_pBackImageButton = 0;
	m_pShockDirectionValue = 0;
	m_pShockForceValue = 0;
	m_pHeartRecoverMPDI = 0;
	m_RestTimetoshockTC.SetTargetTime(60.f);
}

cHeartRecoverPhase::~cHeartRecoverPhase()
{
	Destroy();
}


void	cHeartRecoverPhase::Destroy()
{
	m_pShowMPDI = 0;
	SAFE_DELETE(m_pSuccessMessenger);
	SAFE_DELETE(m_pFailedMessenger);
	SAFE_DELETE(m_pTraningMessenger);
	SAFE_DELETE(m_pStartConfirmButton);
	SAFE_DELETE(m_pCountDownNumeralImage);
	SAFE_DELETE(m_pBackImageButton);
	SAFE_DELETE(m_pShockBoobsPainter);
	SAFE_DELETE(m_pShockForceValue);
	SAFE_DELETE(m_pShockDirectionValue);
	
	m_ResourceStamp.ReleaseNewResource();
	cGameApp::m_spSoundParser->RemoveAllBGM();
}

//<cHeartRecoverPhase ShockAddValue="" ReduceStrgethValue="" TargetValue="">
//</cHeartRecoverPhase>
void	cHeartRecoverPhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("cHeartRecoverPhase")
	{
		ProcessShockParameterData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("BackButton")
	{
		m_pBackImageButton = cImageButton::GetMe(e_pTiXmlElement->FirstChildElement());
		if( m_pBackImageButton )
			m_pBackImageButton->Init();
	}
	else
	COMPARE_VALUE("HeartRecover")
	{
		m_pHeartRecoverMPDI = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
		if( m_pHeartRecoverMPDI )
		{
			m_pHeartRecoverMPDI->Init();
			m_pHeartRecoverMPDI->UpdateByGlobalTime(0.0016f);
		}
	}
	else
	COMPARE_VALUE("cNumeralImage")
	{
		m_pCountDownNumeralImage = cNumeralImage::GetMe(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("ShowMPDI")
	{
		m_pShowMPDI = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
		if(m_pShowMPDI)
			m_pShowMPDI->Init();
	}
	else
	COMPARE_VALUE("StartButton")
	{
		m_pStartConfirmButton = dynamic_cast<cImageButton*>(GetClickMouseBehavior(e_pTiXmlElement->FirstChildElement()));
		if(m_pStartConfirmButton)
			m_pStartConfirmButton->Init();
	}
	else
	COMPARE_VALUE("cPhasePopUpMessager")
	{
		cPhasePopUpMessager*l_pMessenger = cPhasePopUpMessager::GetMe(e_pTiXmlElement);
		if( l_pMessenger )
		{
			l_pMessenger->Init();
			if(!wcscmp(l_pMessenger->GetName(),L"Success"))
			{
				m_pSuccessMessenger = l_pMessenger;
			}
			else
			if(!wcscmp(l_pMessenger->GetName(),L"Failed"))
			{
				m_pFailedMessenger = l_pMessenger;
			}
			else
			if(!wcscmp(l_pMessenger->GetName(),L"FinishTraning"))
			{
				m_pTraningMessenger = l_pMessenger;
			}
			else
			{
				UT::ErrorMsg(l_pMessenger->GetName(),L"not support name");
				SAFE_DELETE(l_pMessenger);

			}
		}
	}
}

void	cHeartRecoverPhase::ProcessShockParameterData(TiXmlElement*e_pTiXmlElement)
{
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("ShockAddValue")
		{
			m_fShockAddStrgeth = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("ReduceStrgethValue")
		{
			m_fTimeReduceStrgeth = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("TargetValue")
		{
			m_fTargetStregth = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("Time")
		{
			m_RestTimetoshockTC.SetTargetTime(VALUE_TO_FLOAT);
		}
	PARSE_NAME_VALUE_END
}

namespace FATMING_CORE
{
	extern cBaseShader*g_pCurrentShader;
}
void	cHeartRecoverPhase::Init()
{
	Destroy();
	m_pCurrentMessenger = 0;
	cBasicSound*l_pSound = cGameApp::GetSoundByFileName(L"Sound/ShakeBG.ogg");
	if(l_pSound)
	{
		l_pSound->Play(true);
		l_pSound->SetLoop(true);
	}
	m_ResourceStamp.StampResource1();
	m_pHeartRecoverMPDI = 0;

	m_pShockForceValue = new cValueChangeFilter<float>(&cGameApp::m_sfForce,0.1f);
	m_pShockDirectionValue = new cValueChangeFilter<Vector3>(&cGameApp::m_svAccelerometer,0.1f);

	m_pShockBoobsPainter = new cShockBoobsPainter(0,ePM_GAME_MODE);
	const char*l_strFileDirectory = "ShockingBoobs/";
	std::string	l_strFileName = l_strFileDirectory;
	l_strFileName += m_strSelectedGirlName;
	l_strFileName += ".xml";
	PARSE_FAILED_MESSAGE_BOX(m_pShockBoobsPainter,l_strFileName.c_str());
	PARSE_FAILED_MESSAGE_BOX(this,"PhaseData/HeartRecoverPhase.xml");
	m_RestTimetoshockTC.Start();
	m_ResourceStamp.StampResource2();
	g_pCurrentShader = 0;
	cGameApp::m_sTimeAndFPS.Update();
}

void	cHeartRecoverPhase::Update(float e_fElpaseTime)
{
	if(m_pShowMPDI && !m_pShowMPDI->IsAnimationDone() )
	{
		m_pShowMPDI->Update(e_fElpaseTime);
	}
	else
	{
		if( !m_pCurrentMessenger )
		{
			if( m_pStartConfirmButton && !m_pStartConfirmButton->IsSatisfiedCondition() )
			{
				m_pStartConfirmButton->Update(e_fElpaseTime);
			}
			else
			{
				m_RestTimetoshockTC.Update(e_fElpaseTime);
				if( m_pCountDownNumeralImage )
					m_pCountDownNumeralImage->SetValue((int)m_RestTimetoshockTC.fRestTime);
				if(  m_RestTimetoshockTC.bTragetTimrReached )
				{
					m_pCurrentMessenger = m_pFailedMessenger;
					if( !m_pCurrentMessenger )
						this->m_bSatisfiedCondition = true;
					//this->m_bSatisfiedCondition = true;
				}
				if( m_pShockBoobsPainter )
					m_pShockBoobsPainter->Update(e_fElpaseTime);
	#ifndef WIN32
				m_pShockDirectionValue->Update(e_fElpaseTime);
				static float	l_fPreviousForce = 0.f;
				if( l_fPreviousForce != cGameApp::m_sfForce )
				{
					l_fPreviousForce = cGameApp::m_sfForce;
					if( m_pShockDirectionValue && cGameApp::m_sfForce > 10.f )
					{
						if( m_pShockDirectionValue->IsReachCheckPoint() )
						{
							//Vector3	l_vChangeDirection = m_pShockValue->GetChangedValue();
							Vector2	l_vNewDirection(cGameApp::m_svAccelerometer.x,cGameApp::m_svAccelerometer.y);
							l_vNewDirection.NormalizeIt();
							l_vNewDirection.x *= cGameApp::m_sfForce*30;
							l_vNewDirection.y *= cGameApp::m_sfForce*30;
							POINT	l_QuaterResolution = {(int)cGameApp::m_svGameResolution.x/4,(int)cGameApp::m_svGameResolution.y/4};
							POINT	l_OldPos = {(int)(l_QuaterResolution.x*m_vLastTimeDirection.x),(int)(l_QuaterResolution.y*m_vLastTimeDirection.y)};
							POINT	l_NewPos = {(int)(l_vNewDirection.x*l_QuaterResolution.x),(int)(l_vNewDirection.y*l_QuaterResolution.y)};
							DoShock(l_OldPos.x,l_OldPos.y,l_NewPos.x,l_NewPos.y);
							m_vLastTimeDirection = l_vNewDirection;
							cGameApp::m_sfForce = 0.f;
						}
					}
				}
	#endif
				if( m_pHeartRecoverMPDI )
				{
					float	l_fTotalPlayTime = m_pHeartRecoverMPDI->GetEndTime();
					float	l_fTime = l_fTotalPlayTime*(m_fCurrentAddedStregth/m_fTargetStregth);
					m_pHeartRecoverMPDI->UpdateByGlobalTime(l_fTime);
				}
				m_NexShackTime.Update(e_fElpaseTime);
				float	l_fReduceValue = m_fTimeReduceStrgeth * e_fElpaseTime;
				m_fCurrentAddedStregth -= l_fReduceValue;
				if( m_fCurrentAddedStregth < 0 )
					m_fCurrentAddedStregth = 0;
				if( m_fCurrentAddedStregth >= m_fTargetStregth )
				{
					m_pHeartRecoverMPDI->UpdateByGlobalTime(m_pHeartRecoverMPDI->GetEndTime());
					m_fCurrentAddedStregth = 0.f;
					//play sound
					cHeartRecoverSelectGirlPhase*l_pPhase = (cHeartRecoverSelectGirlPhase*)cBluffingGirlApp::GetPhase(HEART_RECOVER_SELECTGIRL_PHASE_NAME);
					if(l_pPhase->IsShake())
					{
						//add heart
						cBluffingGirlApp::m_spPlayerData->HeartAdd(1,false);
						m_pCurrentMessenger = m_pSuccessMessenger;
					}
					else
					{
						m_pCurrentMessenger = m_pTraningMessenger;
					}
					if( !m_pCurrentMessenger )
						this->m_bSatisfiedCondition = true;
				}
				if( m_pBackImageButton )
				{
					m_pBackImageButton->Update(e_fElpaseTime);
					if( m_pBackImageButton->IsSatisfiedCondition() )
					{
						this->m_bSatisfiedCondition  =true;
					}
				}
			}
		}
		else
		{
			m_pCurrentMessenger->Update(e_fElpaseTime);
			m_pHeartRecoverMPDI->UpdateByGlobalTime(m_pHeartRecoverMPDI->GetEndTime());
			if( m_pCurrentMessenger->IsSatisfiedCondition() )
			{
				this->m_bSatisfiedCondition  = true;			
			}
		}
	}

	if(m_bSatisfiedCondition)
		this->Destroy();
}

void	cHeartRecoverPhase::Render()
{
	m_pShockBoobsPainter->RenderTerrian();
	if(m_pShowMPDI && !m_pShowMPDI->IsAnimationDone() )
		m_pShowMPDI->Render();
	else
	{
		if(!m_pCurrentMessenger)
		{
			if( m_pStartConfirmButton && !m_pStartConfirmButton->IsSatisfiedCondition() )
			{
				m_pStartConfirmButton->Render();
			}
		}
		else
			m_pCurrentMessenger->Render();
	}
	//m_pShockBoobsPainter->DebugRenderTerrian();
	if( m_pBackImageButton )
		m_pBackImageButton->Render();
	if( m_pHeartRecoverMPDI )
		m_pHeartRecoverMPDI->RenderByGlobalTime();
	if( m_pCountDownNumeralImage )
		m_pCountDownNumeralImage->Render();
}

void	cHeartRecoverPhase::DebugRender()
{
	//m_pShockBoobsPainter->DebugRenderTerrian();
	cGameApp::RenderFont(400,300,ValueToStringW(m_fCurrentAddedStregth).c_str());
	char	l_strrr[MAX_PATH];
	sprintf(l_strrr,"Force:%f",cGameApp::m_sfForce);
	cGameApp::m_spGlyphFontRender->RenderFont(0,200,UT::CharToWchar(l_strrr));
}

void	cHeartRecoverPhase::MouseDown(int e_iX,int e_iY)
{
	//m_pShockBoobsPainter->MouseDown(e_iX,e_iY);
	m_TouchDownPoint.x = e_iX;
	m_TouchDownPoint.y = e_iY;
	if( !m_pCurrentMessenger )
	{
		if( m_pShowMPDI && m_pShowMPDI->IsAnimationDone() && m_pStartConfirmButton && !m_pStartConfirmButton->IsSatisfiedCondition() )
		{
			m_pStartConfirmButton->MouseDown(e_iX,e_iY);
		}
		else
		if( m_pBackImageButton )
			m_pBackImageButton->MouseDown(e_iX,e_iY);
	}
	else
		m_pCurrentMessenger->MouseDown(e_iX,e_iY);
}

void	cHeartRecoverPhase::MouseMove(int e_iX,int e_iY)
{
	//m_pShockBoobsPainter->MouseMove(e_iX,e_iY);
	if( !m_pCurrentMessenger )
	{
		if( m_pShowMPDI && m_pShowMPDI->IsAnimationDone() && m_pStartConfirmButton && !m_pStartConfirmButton->IsSatisfiedCondition() )
		{
			m_pStartConfirmButton->MouseMove(e_iX,e_iY);
		}
		else
		if( m_pBackImageButton )
			m_pBackImageButton->MouseMove(e_iX,e_iY);
	}
	else
		m_pCurrentMessenger->MouseMove(e_iX,e_iY);
}

void	cHeartRecoverPhase::MouseUp(int e_iX,int e_iY)
{
	//m_pShockBoobsPainter->MouseUp(e_iX,e_iY);
	if( !m_pCurrentMessenger )
	{
	DoShock(m_TouchDownPoint.x,m_TouchDownPoint.y,e_iX,e_iY);
	if( m_pShowMPDI && m_pShowMPDI->IsAnimationDone() && m_pStartConfirmButton && !m_pStartConfirmButton->IsSatisfiedCondition() )
	{
		m_pStartConfirmButton->MouseUp(e_iX,e_iY);
	}
	else
	if( m_pBackImageButton )
		m_pBackImageButton->MouseUp(e_iX,e_iY);
	}
	else
		m_pCurrentMessenger->MouseUp(e_iX,e_iY);
}

void	cHeartRecoverPhase::KeyDown(char e_char)
{

}

const   WCHAR*	cHeartRecoverPhase::GetNextPhaseName()
{
	return HEART_RECOVER_SELECTGIRL_PHASE_NAME;
}

void*	cHeartRecoverPhase::GetData()
{
	cBasicSound*l_pSound = cGameApp::GetSoundByFileName(L"Sound/MainBG.ogg");
	if(l_pSound)
	{
		l_pSound->Play(true);
		l_pSound->SetLoop(true);
	}
	return 0;
}

void	cHeartRecoverPhase::FetchData(const WCHAR*e_strPhaseName,void*e_pData)
{
	const WCHAR*l_str = (const WCHAR*)e_pData;
	m_strSelectedGirlName = UT::WcharToChar(l_str);
}

void	cHeartRecoverPhase::DoShock(int e_iPointX1,int e_iPointY1,int e_iPointX2,int e_iPointY2)
{
	int		l_iMultiplyValue = 1;
	POINT	l_TouchUpPos = {e_iPointX2-e_iPointX1,e_iPointY2-e_iPointY1};
	Vector2	l_vNewDirection(e_iPointX2,e_iPointY2);
	Vector2	l_vMoveDirection = l_vNewDirection-Vector2(e_iPointX1,e_iPointY1);
	Vector2	l_vMaximum = cGameApp::m_svGameResolution;
	float	l_fMoveLength = l_vMoveDirection.Length();
#ifdef ANDROID
	//if( l_fMoveLength > l_vMaximum.Length()/100.f)
#endif
	{
		//if( m_NexShackTime.bTragetTimrReached && (m_fLastBigStregth < l_fMoveLength || m_pShockBoobsPainter->IsShockDone()) )
		if( m_fLastBigStregth < l_fMoveLength || m_pShockBoobsPainter->IsShockDone() )
		{
			if( m_fLastBigStregth < l_fMoveLength )
				m_fLastBigStregth = l_fMoveLength;
			else
			if( m_pShockBoobsPainter->IsShockDone() )
				m_fLastBigStregth = 0.f;
			if( l_vMoveDirection.x > cGameApp::m_svGameResolution.x/2 )				l_vMoveDirection.x = cGameApp::m_svGameResolution.x/2;
			if( l_vMoveDirection.x < -cGameApp::m_svGameResolution.x/2 )				l_vMoveDirection.x = -cGameApp::m_svGameResolution.x/2;
			if( l_vMoveDirection.y > cGameApp::m_svGameResolution.y/2 )				l_vMoveDirection.y = cGameApp::m_svGameResolution.y/2;
			if( l_vMoveDirection.y < -cGameApp::m_svGameResolution.y/2 )				l_vMoveDirection.y = -cGameApp::m_svGameResolution.y/2;
			m_pShockBoobsPainter->MouseDown((int)cGameApp::m_svGameResolution.x/2,(int)cGameApp::m_svGameResolution.y/2);
			m_pShockBoobsPainter->MouseUp((int)(cGameApp::m_svGameResolution.x/2+l_vMoveDirection.x*l_iMultiplyValue),(int)(cGameApp::m_svGameResolution.y/2+l_vMoveDirection.y*l_iMultiplyValue));
			m_NexShackTime.Start();
			l_fMoveLength/=l_vMaximum.Length();
			float	l_fAddValue = l_fMoveLength*m_fShockAddStrgeth;
			if( l_fAddValue > this->m_fTargetStregth / 5 )
				l_fAddValue = this->m_fTargetStregth / 5;
			m_fCurrentAddedStregth += l_fAddValue;

		}
		else
		{
			m_pShockBoobsPainter->SetShockDirection(m_pShockBoobsPainter->GetShockDirection());
		}
	}
}
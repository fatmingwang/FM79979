#include "stdafx.h"
#include "SetupPhase.h"
#include "GameDefine.h"

cSetupPhase::cSetupPhase()
{
	m_pBGMPDI = 0;
	m_AllButton.SetFromResource(true);
	this->SetName(SETUP_PHASE_NAME);
	m_pBGMVolumeSlider = 0;
	m_pEffectVolumeSlider = 0;
	m_fOriginalEffectVolume = 100.f;
	m_fOriginalBGMVolume = 100.f;
	m_pApplyButton = 0;
	m_pChancelButton = 0;
	m_pMuteButton = 0;
	m_pSoundEffect = 0;
	m_pSoundBGM = 0;
	if(m_SoundVolumeFile.Openfile("Sound.xml"))
	{
		float*l_pfData = (float*)m_SoundVolumeFile.GetDataFile(0);
		if( l_pfData )
		{
			m_fOriginalEffectVolume = *l_pfData;
			++l_pfData;
			m_fOriginalBGMVolume = *l_pfData;
			m_SoundVolumeFile.CloseFile();
		}
		else
		{
			m_fOriginalEffectVolume = 1.f;
			m_fOriginalBGMVolume = 1.f;		
		}
	}
	else
	{
		m_fOriginalEffectVolume = 1.f;
		m_fOriginalBGMVolume = 1.f;
	}
	
	cSoundParser::m_sfSoundEffectVolume = m_fOriginalEffectVolume;
	cSoundParser::m_sfBGMVolume = m_fOriginalBGMVolume;
}

cSetupPhase::~cSetupPhase()
{
	SAFE_DELETE(m_pBGMVolumeSlider);
	SAFE_DELETE(m_pEffectVolumeSlider);
	SAFE_DELETE(m_pApplyButton);
	SAFE_DELETE(m_pChancelButton);
	SAFE_DELETE(m_pMuteButton);
}

void	cSetupPhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("cSliderBar")
	{
		cSliderBar*l_pSliderBar = cSliderBar::GetMe(e_pTiXmlElement);
		if( l_pSliderBar )
		{
			
			if( l_pSliderBar->IsSameName(L"BGMVolume") )
			{
				m_pBGMVolumeSlider = l_pSliderBar;
			}
			else
			if( l_pSliderBar->IsSameName(L"EffectVolume") )
				m_pEffectVolumeSlider = l_pSliderBar;
			else
			{
				UT::ErrorMsg(l_pSliderBar->GetName(),L"what is this slider!?");
				SAFE_DELETE(l_pSliderBar);
			}
		}
	}
	else
	COMPARE_VALUE("cImageButton")
	{
		cImageButton*l_pButton = cImageButton::GetMe(e_pTiXmlElement);
		if( l_pButton )
		{
			m_AllButton.AddObjectNeglectExist(l_pButton);
			if( l_pButton->IsSameName(L"Apply") )
				m_pApplyButton = l_pButton;
			else
			if( l_pButton->IsSameName(L"Cancel") )
				m_pChancelButton = l_pButton;
			else
			if( l_pButton->IsSameName(L"Mute") )
				m_pMuteButton = l_pButton;
			else
			{
				UT::ErrorMsg(l_pButton->GetName(),L"what is this button!?");
				SAFE_DELETE(l_pButton);
			}
		}
		else
		{
			UT::ErrorMsg(e_pTiXmlElement->Value(),L"what is this button!?");
			SAFE_DELETE(l_pButton);
		}
	}
	else
	COMPARE_VALUE("Sound")
	{
		const WCHAR*l_strBGM = e_pTiXmlElement->Attribute(L"BGM");
		const WCHAR*l_strEffect = e_pTiXmlElement->Attribute(L"Effect");
		if( l_strBGM )
		{
			m_pSoundBGM = cGameApp::m_spSoundParser->GetObject(UT::GetFileNameWithoutFullPath(l_strBGM));
			if( !m_pSoundBGM && cGameApp::m_spSoundParser->AddStreamingSound(cGameApp::m_spSoundParser,UT::WcharToChar(l_strBGM).c_str()))
			{
				m_pSoundBGM = cGameApp::m_spSoundParser->GetLastObject();
			}
			if( m_pSoundBGM )
			{
				m_pSoundBGM->SetLoop(true);
				m_pSoundBGM->Play(true);
			}
		}
		if( l_strEffect )
		{
			m_pSoundEffect = cGameApp::m_spSoundParser->GetObject(UT::GetFileNameWithoutFullPath(l_strEffect));
			if(!m_pSoundEffect && cGameApp::m_spSoundParser->AddStaticSound(cGameApp::m_spSoundParser,UT::WcharToChar(l_strEffect).c_str()))
			{
				m_pSoundEffect = cGameApp::m_spSoundParser->GetLastObject();
				if( m_pSoundEffect )
					m_pSoundEffect->SetLoop(true);
			}
		}
	}
	else
	COMPARE_VALUE("BG")
	{
		m_pBGMPDI = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
		if( m_pBGMPDI )
			m_pBGMPDI->Init();
	}
}

void	cSetupPhase::Destroy()
{
	m_pBGMPDI = 0;
	this->m_AllButton.Destroy();
	SAFE_DELETE(m_pBGMVolumeSlider);
	SAFE_DELETE(m_pEffectVolumeSlider);
	SAFE_DELETE(m_pApplyButton);
	SAFE_DELETE(m_pChancelButton);
	SAFE_DELETE(m_pMuteButton);
	m_ResourceStamp.ReleaseNewResource();
}

void	cSetupPhase::Init()
{
	m_AllButton.Destroy();
	m_ResourceStamp.StampResource1();
	PARSE_FAILED_MESSAGE_BOX(this,"Data/Phase/SetupPhase.xml")
	if(m_SoundVolumeFile.Openfile("Sound.xml"))
	{
		float*l_pfData = (float*)m_SoundVolumeFile.GetDataFile(0);
		if( l_pfData )
		{
			m_fOriginalEffectVolume = *l_pfData;
			++l_pfData;
			m_fOriginalBGMVolume = *l_pfData;
			m_SoundVolumeFile.CloseFile();
		}
		else
		{
			m_fOriginalEffectVolume = 1.f;
			m_fOriginalBGMVolume = 1.f;		
		}
	}
	else
	{
		m_fOriginalEffectVolume = 1.f;
		m_fOriginalBGMVolume = 1.f;
	}
	if( m_pBGMVolumeSlider )
		m_pBGMVolumeSlider->SetValue(m_fOriginalBGMVolume*100);
	if(m_pEffectVolumeSlider)
		m_pEffectVolumeSlider->SetValue(m_fOriginalEffectVolume*100);
	if( m_pSoundBGM )
		m_pSoundBGM->SetVolume(m_fOriginalBGMVolume);
	if( m_pSoundEffect )
		m_pSoundEffect->SetVolume(m_fOriginalEffectVolume);
	m_AllButton.Init();
	m_ResourceStamp.StampResource2();
}

void	cSetupPhase::Update(float e_fElpaseTime)
{
	if( m_pBGMPDI )
		m_pBGMPDI->Update(e_fElpaseTime);
	if( m_pBGMVolumeSlider )
		m_pBGMVolumeSlider->Update(e_fElpaseTime);
	if( m_pEffectVolumeSlider )
		m_pEffectVolumeSlider->Update(e_fElpaseTime);
	m_AllButton.Update(e_fElpaseTime);
	if( m_AllButton.IsSatisfiedCondition() )
	{
		if( m_AllButton.GetCurrentWorkingObject() == m_pApplyButton )
		{
			if(m_SoundVolumeFile.Writefile("Sound.xml",true,true))
			{
				m_SoundVolumeFile.WriteToFile(cSoundParser::m_sfSoundEffectVolume);
				m_SoundVolumeFile.WriteToFile(cSoundParser::m_sfBGMVolume);
				m_SoundVolumeFile.CloseFile();
			}
			//write file
			this->m_bSatisfiedCondition = true;
		}
		else
		if( m_AllButton.GetCurrentWorkingObject() == m_pChancelButton )
		{
			cSoundParser::m_sfSoundEffectVolume = m_fOriginalEffectVolume;
			cSoundParser::m_sfBGMVolume = m_fOriginalBGMVolume;
			this->m_bSatisfiedCondition = true;
		}
		else
		if( m_AllButton.GetCurrentWorkingObject() == m_pMuteButton )
		{
			m_pBGMVolumeSlider->SetValue(0);
			m_pEffectVolumeSlider->SetValue(0);
			cSoundParser::m_sfSoundEffectVolume = 0.f;
			cSoundParser::m_sfBGMVolume = 0.f;
			if( m_pSoundEffect )
			{
				if( m_pSoundEffect->IsUsing() )
					m_pSoundEffect->SetVolume(1.f,true);
			}
			if( m_pSoundBGM )
				m_pSoundBGM->SetVolume(1.f);
		}
		m_AllButton.Init();
		if( this->m_bSatisfiedCondition == true )
			this->Destroy();
	}
}

void	cSetupPhase::Render()
{
	if( m_pBGMPDI )
		m_pBGMPDI->Render();
	if( m_pBGMVolumeSlider )
		m_pBGMVolumeSlider->Render();
	if( m_pEffectVolumeSlider )
		m_pEffectVolumeSlider->Render();
	m_AllButton.Render();
}

void	cSetupPhase::DebugRender()
{
	if( m_pBGMVolumeSlider )
		m_pBGMVolumeSlider->DebugRender();
	if( m_pEffectVolumeSlider )
		m_pEffectVolumeSlider->DebugRender();
	m_AllButton.DebugRender();
}

void	cSetupPhase::MouseDown(int e_iX,int e_iY)
{
	if( m_pBGMVolumeSlider )
		m_pBGMVolumeSlider->MouseDown(e_iX,e_iY);
	if( m_pEffectVolumeSlider )
	{
		m_pEffectVolumeSlider->MouseDown(e_iX,e_iY);
		if(m_pSoundEffect && m_pEffectVolumeSlider->IsCollided())
		{
			m_pSoundEffect->Play(true);
		}
	}
	m_AllButton.MouseDown(e_iX,e_iY);
}

void	cSetupPhase::MouseUp(int e_iX,int e_iY)
{
	if( m_pBGMVolumeSlider )
	{
		m_pBGMVolumeSlider->MouseUp(e_iX,e_iY);
		float	l_fValue = *m_pBGMVolumeSlider->GetValue()/100.f;
		cSoundParser::m_sfBGMVolume = l_fValue;
	}
	if( m_pEffectVolumeSlider )
	{
		m_pEffectVolumeSlider->MouseUp(e_iX,e_iY);
		float	l_fValue = *m_pEffectVolumeSlider->GetValue()/100.f;
		cSoundParser::m_sfSoundEffectVolume = l_fValue;
	}
	m_AllButton.MouseUp(e_iX,e_iY);
	if( m_pSoundEffect )
		m_pSoundEffect->Play(false);
}

void	cSetupPhase::MouseMove(int e_iX,int e_iY)
{
	if( m_pBGMVolumeSlider )
	{
		m_pBGMVolumeSlider->MouseMove(e_iX,e_iY);
		if(m_pBGMVolumeSlider->IsCollided(e_iX,e_iY))
		{
			if(m_pBGMVolumeSlider->GetMouseBehavior() == eOMB_FIRST_TIME_INTO ||  m_pBGMVolumeSlider->GetMouseBehavior() == eOMB_HORVER)
			{
				float	l_fValue = *m_pBGMVolumeSlider->GetValue()/100.f;
				cSoundParser::m_sfBGMVolume = l_fValue;
				if( m_pSoundBGM )
					m_pSoundBGM->SetVolume(1.f);
			}
		}
	}
	if( m_pEffectVolumeSlider )
	{
		m_pEffectVolumeSlider->MouseMove(e_iX,e_iY);
		if(m_pEffectVolumeSlider->IsCollided(e_iX,e_iY))
		{
			if( m_pEffectVolumeSlider->GetMouseBehavior() == eOMB_FIRST_TIME_INTO ||  m_pEffectVolumeSlider->GetMouseBehavior() == eOMB_HORVER)
			{
				float	l_fValue = *m_pEffectVolumeSlider->GetValue()/100.f;
				cSoundParser::m_sfSoundEffectVolume = l_fValue;
				if( m_pSoundEffect )
					m_pSoundEffect->SetVolume(1.f,true);
			}
		}
	}
	m_AllButton.MouseMove(e_iX,e_iY);
}

const   WCHAR*	cSetupPhase::GetNextPhaseName()
{
	return m_strLastPhaseName.c_str();
}

void	cSetupPhase::FetchData(const WCHAR*e_strPhaseName,void*e_pData)
{
	m_strLastPhaseName = e_strPhaseName;
}
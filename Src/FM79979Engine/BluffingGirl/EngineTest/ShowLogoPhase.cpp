#include "stdafx.h"
#include "ShowLogoPhase.h"
#include "AllPhaseDefine.h"
#ifndef WIN32
#include "unistd.h"
#endif
#include "UnpackExpansionPack.h"
cShowLogoPhase::cShowLogoPhase()
{
	this->SetName(SHOWLOGO_PHASE_NAME);
	m_pBGMPDI = 0;
	m_bGotoMainPhase = false;
	m_pUnpackExpansionPack = 0;
}

cShowLogoPhase::~cShowLogoPhase()
{
	Destroy();
}

bool	g_bWait = true;

cBasicSound*g_pLogoMusic = 0;
void	ShowLogoBGM(size_t _workParameter, size_t _pUri)
{
	UT::sTimeAndFPS	l_TC;
	l_TC.Update();
	while( g_bWait )
	{
		l_TC.Update();
		if(!cGameApp::m_sbGamePause)
			g_pLogoMusic->Update(l_TC.fElpaseTime);
		Sleep(160);
	}
}
void	ShowLogoBGMDone(size_t _workParameter, size_t _pUri)
{

}


void	cShowLogoPhase::Init()
{
	m_ResourceStamp.StampResource1();
	SAFE_DELETE(m_pUnpackExpansionPack);
	m_pUnpackExpansionPack = new cUnpackExpansionPack();
	cGameApp::OutputDebugInfoString(L"cShowLogoPhase Init() 4!");
	m_bGotoMainPhase = false;
	cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(L"BluffingGirl/Image/Title.mpdi");
	cGameApp::OutputDebugInfoString(L"cShowLogoPhase Init() 5!");
	if( l_pMPDIList )
	{
		m_pBGMPDI = l_pMPDIList->GetObject(L"Title");
		if( m_pBGMPDI )
		{
			m_pBGMPDI->Init();
			m_pBGMPDI->SetAnimationLoop(false);
			m_pBGMPDI->Update(0.5f);
		}
	}
	g_pLogoMusic = new cOpanalOgg(this,"BluffingGirl/Sound/Opening.ogg",true);
	cGameApp::OutputDebugInfoString(L"cShowLogoPhase Init() 6!");
	if(g_pLogoMusic)
	{
		g_pLogoMusic->Play(true);
		g_pLogoMusic->SetLoop(true);
		FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&ShowLogoBGM);
		FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&ShowLogoBGMDone);
		this->m_ThreadPool.ExecuteWork(workFunctor,doneFunctor,(size_t)this,0);
	}
	m_pUnpackExpansionPack->IsInUnpackProgress();
	m_ResourceStamp.StampResource2();
	cGameApp::OutputDebugInfoString(L"cShowLogoPhase Init() Finish!");
}
void	cShowLogoPhase::Destroy()
{
	g_bWait = false;
	m_pBGMPDI = 0;
	m_ThreadPool.Clear();
	if( g_pLogoMusic )
		g_pLogoMusic->Release(this);
	g_pLogoMusic = 0;
	SAFE_DELETE(m_pUnpackExpansionPack);
	m_ResourceStamp.ReleaseNewResource();
}

void	cShowLogoPhase::Update(float e_fElpaseTime)
{
	if(m_pBGMPDI)
	{
		if( m_pBGMPDI->IsAnimationDone())
		{
			if( m_bGotoMainPhase )
			{
				if( m_pUnpackExpansionPack->IsInUnpackProgress() )
				{
					m_pUnpackExpansionPack->Update(e_fElpaseTime);
				}
				else
				{
					g_bWait = false;
					this->m_bSatisfiedCondition = true;
					this->Destroy();
					//
					cGameApp::m_spSoundParser->RemoveAllBGM();
					cBasicSound*l_pSound = cGameApp::GetSoundByFileName(L"BluffingGirl/Sound/MainBG.ogg");
					if(l_pSound)
					{
						l_pSound->Play(true);
						l_pSound->SetLoop(true);
					}
				}
			}
		}
		else
		{
			m_pBGMPDI->Update(e_fElpaseTime);
		}
	}
}
//it would be called if the stage is changed.
void	cShowLogoPhase::Render()
{
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
	if( m_pBGMPDI )
	{
		m_pBGMPDI->Render();
		if( m_pUnpackExpansionPack->IsInUnpackProgress() )
		{
			m_pUnpackExpansionPack->Render();
		}
	}

}

void	cShowLogoPhase::DebugRender()
{

}

void	cShowLogoPhase::MouseDown(int e_iX,int e_iY)
{

}

void	cShowLogoPhase::MouseUp(int e_iX,int e_iY)
{

}

void	cShowLogoPhase::MouseMove(int e_iX,int e_iY)
{

}

const   WCHAR*	cShowLogoPhase::GetNextPhaseName()
{
	return TITLE_PHASE_NAME;
}

void	cShowLogoPhase::KeyDown(char e_char)
{
	switch(e_char)
	{
		case 37://left

			break;
		case 38://up

			break;
		case 39://right

			break;
		case 40://down

			break;
	}

}
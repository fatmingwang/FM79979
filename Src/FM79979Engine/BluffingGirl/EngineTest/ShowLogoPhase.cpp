#include "stdafx.h"
#include "ShowLogoPhase.h"
#include "AllPhaseDefine.h"
#ifndef WIN32
#include "unistd.h"
#endif
#include "UnpackExpansionPack.h"
#include <thread>
#include <mutex>
bool	g_bWaitThreadFinish = true;
bool	g_bThreadFinish = false;
cShowLogoPhase::cShowLogoPhase()
{
	this->SetName(SHOWLOGO_PHASE_NAME);
	m_pBGMPDI = 0;
	m_bGotoMainPhase = false;
	m_pUnpackExpansionPack = 0;
	g_bWaitThreadFinish = true;
	g_bThreadFinish = false;

}

cShowLogoPhase::~cShowLogoPhase()
{
	Destroy();
}

cBasicSound*g_pLogoMusic = 0;
void	ShowLogoBGM(size_t _workParameter, size_t _pUri)
{
	UT::sTimeAndFPS	l_TC;
	l_TC.Update();
	while(g_bWaitThreadFinish && g_pLogoMusic)
	{
		l_TC.Update();
		if(!cGameApp::m_sbGamePause)
			g_pLogoMusic->Update(l_TC.fElpaseTime);
		Sleep(10);
	}
	g_bThreadFinish = true;
}
void	ShowLogoBGMDone(size_t _workParameter, size_t _pUri)
{

}


void	cShowLogoPhase::Init()
{
	g_bWaitThreadFinish = true;
	m_ResourceStamp.StampResource1();
	SAFE_DELETE(m_pUnpackExpansionPack);
	//m_pUnpackExpansionPack = new cUnpackExpansionPack();
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
#ifndef WASM
	g_pLogoMusic = new cOpanalOgg(this,"BluffingGirl/Sound/Opening.ogg",true);
	if(g_pLogoMusic)
	{
		g_pLogoMusic->Play(true);
		g_pLogoMusic->SetLoop(true);
		std::thread l_ShowLogoBGMThread(ShowLogoBGM,(size_t)this, 0);
		l_ShowLogoBGMThread.detach();
		//l_ShowLogoBGMThread.join();
	}
#else
	//g_pLogoMusic = new cOpanalOgg(this, "BluffingGirl/Sound/Opening.ogg", false);
	if (g_pLogoMusic)
		g_pLogoMusic->Play(true);
#endif
	m_ResourceStamp.StampResource2();
	cGameApp::OutputDebugInfoString(L"cShowLogoPhase Init() Finish!");
}
void	cShowLogoPhase::Destroy()
{
	g_bWaitThreadFinish = false;
	m_pBGMPDI = 0;
	//m_ThreadPool.Clear();
	while (!g_bThreadFinish && g_pLogoMusic)
	{
		Sleep(100);
		cGameApp::OutputDebugInfoString(L"wait g_pLogoMusic thread finish");
	}
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
				//some show main thread can't wait java thread in NDK 16c version
				//I am lazy to fix this.
				if(m_pUnpackExpansionPack && m_pUnpackExpansionPack->IsInUnpackProgress() )
				{
					m_pUnpackExpansionPack->Update(e_fElpaseTime);
				}
				else
				{
					g_bWaitThreadFinish = false;
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
	glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	if( m_pBGMPDI )
	{
		m_pBGMPDI->Render();
		if(m_pUnpackExpansionPack && m_pUnpackExpansionPack->IsInUnpackProgress() )
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
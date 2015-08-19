#include "../stdafx.h"
#include "../AVIPlayer/AVIPlayer.h"
#include "VideoSubMPDI.h"
#ifdef WIN32

namespace FATMING_CORE
{
	cVideoSubMPDI::cVideoSubMPDI()
	{
		m_pAVIPlayer = new cAVIPlayer;
		m_fFilmStartTime = 0.f;
		m_fFilmEndTime = 0.f;
	}

	cVideoSubMPDI::~cVideoSubMPDI()
	{
		SAFE_DELETE(m_pAVIPlayer);
	}


	void	cVideoSubMPDI::InternalInit()
	{
		if(m_pAVIPlayer->IsFileOpen())
		{
			m_pAVIPlayer->SetLoop(this->IsAnimationLoop());
			m_pAVIPlayer->SetCurrentFrameByTime(this->m_fStartTime);
		}
		m_vCurrentPosition = Vector3::Zero;
		this->SetLocalPosition(Vector3::Zero);
	}

	void	cVideoSubMPDI::InternalUpdate(float e_fElpaseTime)
	{
		if(this->IsAnimationDone())
			return;
		this->m_fCurrentTime += e_fElpaseTime;
		float	l_fTargetTime = m_fCurrentTime-this->m_fStartTime;
		if( l_fTargetTime<=0.f )
			return;
		if( l_fTargetTime > this->m_fFilmEndTime )
		{
			if( !this->IsAnimationLoop() )
				return;
			l_fTargetTime = UT::GetFloatModulus(l_fTargetTime,m_fFilmEndTime);
		}
		//m_pAVIPlayer->SetCurrentFrameByTime(l_fTargetTime);
		m_pAVIPlayer->Update(e_fElpaseTime);
		//m_pAVIPlayer->Update(e_fElpaseTime);
	}

	void	cVideoSubMPDI::InternalRender()
	{
		float	l_fPosX = this->m_vCurrentPosition.x+this->GetPos().x;
		float	l_fPosY = this->m_vCurrentPosition.y+this->GetPos().y;
		POINT	l_Size = {(int)this->m_pCurrentPointData->Size.x,(int)this->m_pCurrentPointData->Size.y};
		m_pAVIPlayer->Render(l_fPosX,l_fPosY,l_Size);
	}

	bool	cVideoSubMPDI::OpenFile(char*e_strFileName)
	{
		bool	l_b = m_pAVIPlayer->OpenFile(UT::CharToWchar(e_strFileName).c_str());
		if( l_b )
		{
			m_fFilmEndTime = (float)m_pAVIPlayer->GetFileTimeLength();
			this->m_pCurrentPointData->Size.x = (float)m_pAVIPlayer->GetWidth();
			this->m_pCurrentPointData->Size.y = (float)m_pAVIPlayer->GetHeight();
		}
		else
			m_fFilmEndTime = 0.f;
		return l_b;
	}

	void    cVideoSubMPDI::RenderFirsttFrameImage(bool e_bBlending,Vector4 e_vColor)
	{
		m_pAVIPlayer->SetCurrentFrameByTime(this->m_fStartTime);
		InternalRender();
	}

	void	cVideoSubMPDI::RenderByGlobalTime(float e_fTime)
	{
		float	l_fTargetTime = e_fTime-this->m_fStartTime;
		m_pAVIPlayer->SetCurrentFrameByTime(l_fTargetTime);
		InternalRender();
	}

	void	cVideoSubMPDI::RenderLastFrame()
	{
		m_pAVIPlayer->SetCurrentFrame(m_pAVIPlayer->GetLastframe());
		InternalRender();	
	}
}

#endif
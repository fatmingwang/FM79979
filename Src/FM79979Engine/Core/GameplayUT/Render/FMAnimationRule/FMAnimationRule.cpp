//#include "../stdafx.h"
#include "FMAnimationRule.h"
namespace FATMING_CORE
{
	cFMTimeLineAnimationRule::cFMTimeLineAnimationRule()
    {
		m_pUserData = 0;
		m_fCurrentProgress = 0.f;
	    m_fStartTime = 0.f;
	    m_bStart = false;
	    m_fTotalPlayTime = m_fPastTime = 0.f; 
		m_iPlayCount = 1;
	    m_iCurrentPlayCount = 0;
		m_bStayAtLastFrame = false;
		m_bInitCalled = false;
    }
	cFMTimeLineAnimationRule::cFMTimeLineAnimationRule(cFMTimeLineAnimationRule*e_pFatmingAnimationRule):cRenderObject(e_pFatmingAnimationRule)
    {
		m_pUserData = 0;
		m_fCurrentProgress = 0.f;
		m_iCurrentPlayCount = 0;
		m_bStayAtLastFrame = e_pFatmingAnimationRule->m_bStayAtLastFrame;
	    m_fStartTime = e_pFatmingAnimationRule->m_fStartTime;
	    m_bStart = e_pFatmingAnimationRule->m_bStart;
	    m_fPastTime = e_pFatmingAnimationRule->m_fPastTime;
	    m_iPlayCount  = e_pFatmingAnimationRule->m_iPlayCount ;
		m_fTotalPlayTime = e_pFatmingAnimationRule->m_fTotalPlayTime;
		m_bInitCalled = false;
    }

    cFMTimeLineAnimationRule::~cFMTimeLineAnimationRule()
	{
		//OutputDebugString(L"cFMTimeLineAnimationRule\n");
	}


	float*	cFMTimeLineAnimationRule::GetStartTimePointer()
	{
		return &m_fStartTime;
	}
	void	cFMTimeLineAnimationRule::SetPlayCount(int e_iPlayCount)
	{
		m_iPlayCount = e_iPlayCount;
	}

	int		cFMTimeLineAnimationRule::GetCurrentPlayCount()
	{
		return m_iCurrentPlayCount;
	}
	void	cFMTimeLineAnimationRule::Stop()
	{
		m_fPastTime = m_fTotalPlayTime;
		m_iCurrentPlayCount = 0;
		m_fCurrentProgress = 1.f;
		SetAnimationDone(true); 
	}

	void	cFMTimeLineAnimationRule::SetData(void*e_pData,bool e_bForceReplace)
	{
		if( m_pUserData == nullptr || e_bForceReplace)
		{
			this->m_pUserData = (char*)e_pData;
		}
	}

	void	cFMTimeLineAnimationRule::ForAllNodeUpdateByGlobalTime(float e_fGlobalTime)
	{
		UpdateByGlobalTime(e_fGlobalTime);
		cFMTimeLineAnimationRule*l_pNextSibling = dynamic_cast<cFMTimeLineAnimationRule*>(this->GetNextSibling());
		while( l_pNextSibling )
		{
			l_pNextSibling->UpdateByGlobalTime(e_fGlobalTime);
			l_pNextSibling = dynamic_cast<cFMTimeLineAnimationRule*>(l_pNextSibling->GetNextSibling());
		}
		cFMTimeLineAnimationRule*l_pFirstChild = dynamic_cast<cFMTimeLineAnimationRule*>(this->GetFirstChild());
		if(l_pFirstChild)
			l_pFirstChild->ForAllNodeUpdateByGlobalTime(e_fGlobalTime);
	}


	void	cFMTimeLineAnimationRule::ForAllNodeRenderByGlobalTime(float e_fGlobalTime)
	{
		RenderByGlobalTime(e_fGlobalTime);
		cFMTimeLineAnimationRule*l_pNextSibling = dynamic_cast<cFMTimeLineAnimationRule*>(this->GetNextSibling());
		while( l_pNextSibling )
		{
			l_pNextSibling->RenderByGlobalTime(e_fGlobalTime);
			l_pNextSibling = dynamic_cast<cFMTimeLineAnimationRule*>(l_pNextSibling->GetNextSibling());
		}
		cFMTimeLineAnimationRule*l_pFirstChild = dynamic_cast<cFMTimeLineAnimationRule*>(this->GetFirstChild());	
		if(l_pFirstChild)
			l_pFirstChild->ForAllNodeRenderByGlobalTime(e_fGlobalTime);
	}
    void	cFMTimeLineAnimationRule::Init()
    {
		m_fCurrentProgress = 0.f;
		m_bInitCalled = true;
	    m_bStart = false;
	    m_fPastTime = 0.f;
		m_iCurrentPlayCount = 0;
		if( m_fTotalPlayTime == 0.f )
		{
			m_fTotalPlayTime = this->GetEndTime();
		}
		if( m_fTotalPlayTime == 0.f )
			m_iCurrentPlayCount = 0;
	    InternalInit();
	    //if( e_fElpaseTime !=0.f )
		  //  Update(e_fElpaseTime);
    }

    void	cFMTimeLineAnimationRule::Update(float e_fElpaseTime)
    {
		if( !m_bInitCalled )
			return;
		if( m_iCurrentPlayCount == this->m_iPlayCount)
		{
			m_fCurrentProgress = 1.f;
		    return;
		}
		float	l_fPreviousPastTime = m_fPastTime;
		float	l_fEndTime = this->GetEndTime();
	    m_fPastTime += e_fElpaseTime;
		if (m_fPastTime >= (l_fEndTime - EPSIONAL))
		{
			//seems use m_iCurrentPlayCount += m_fPastTime/l_fEndTime; is better but I do care.
			m_iCurrentPlayCount++;
			if( m_iCurrentPlayCount  != m_iPlayCount )
			{
				int l_iCurrentPlayTime = m_iCurrentPlayCount+1;
				float l_fPastTime = m_fPastTime;
				m_fPastTime = GetFloatModulus(m_fPastTime,m_fTotalPlayTime);
				e_fElpaseTime = m_fPastTime;
				this->Init();
				m_iCurrentPlayCount = l_iCurrentPlayTime;
				//if(this->IsAnimationLoop())
				{
					m_fPastTime = this->m_fStartTime+0.0000001f;
				}
			}
			else
			{
				if( m_bStayAtLastFrame )
				{
					float	l_fRestTimeToEnd = l_fEndTime - l_fPreviousPastTime + EPSIONAL;
					InternalUpdate(l_fRestTimeToEnd);
					//m_fPastTime += e_fElpaseTime;
					//if(m_fCurrentProgress != 1.f)
					//{
					//	this->Init();
					//	InternalUpdate(m_fPastTime-m_fStartTime);
					//	InternalUpdate(l_fEndTime);
					//}
				}
				m_fCurrentProgress = 1.f;
			}
		}
		if( !this->IsAnimationDone() )
		{
			if( !m_bStart )
			{
				if(m_fPastTime>=m_fStartTime)
				{
					m_bStart = true;
					InternalUpdate(m_fPastTime-m_fStartTime);
				}
			}
			else
			{
				InternalUpdate(e_fElpaseTime);
			}
			m_fCurrentProgress = this->m_fPastTime/this->m_fTotalPlayTime;
		}
    }

    void	cFMTimeLineAnimationRule::Render()
    {
		if( !m_bStart || !this->m_bVisible)
			return;
		if( !this->IsAnimationDone() || m_bStayAtLastFrame )
			InternalRender();
    }
    bool	cFMTimeLineAnimationRule::IsAnimationLoop(){ return m_iPlayCount == -1?true:false;}
	bool	cFMTimeLineAnimationRule::IsAnimationDone()
	{
		if( m_iPlayCount == -1 || m_iCurrentPlayCount != this->m_iPlayCount )
			return false;
		return true;
	}
    float	cFMTimeLineAnimationRule::GetPastTime(){ return m_fPastTime; }
    void	cFMTimeLineAnimationRule::SetAnimationLoop(bool e_bLoop){ m_iPlayCount = e_bLoop?-1:1; }
    void	cFMTimeLineAnimationRule::SetAnimationDone(bool e_bAnimationDone)
	{
		if( e_bAnimationDone )
		{
			m_iCurrentPlayCount = m_iPlayCount;
			m_fPastTime = this->GetEndTime();
		}
		else
		{
			m_iCurrentPlayCount = 0;
		}
	}
//end namespace FATMING_CORE
}
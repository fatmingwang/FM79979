#include "../stdafx.h"
#include "FMTimelineAnimationObject.h"
namespace FATMING_CORE
{
	cFMTimelineAnimationObject::cFMTimelineAnimationObject()
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
	cFMTimelineAnimationObject::cFMTimelineAnimationObject(cFMTimelineAnimationObject*e_pFatmingAnimationRule):cRenderNode(e_pFatmingAnimationRule)
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

    cFMTimelineAnimationObject::~cFMTimelineAnimationObject()
	{
		//OutputDebugString(L"cFMTimelineAnimationObject\n");
	}


	float*	cFMTimelineAnimationObject::GetStartTimePointer()
	{
		return &m_fStartTime;
	}
	void	cFMTimelineAnimationObject::SetPlayCount(int e_iPlayCount)
	{
		m_iPlayCount = e_iPlayCount;
	}

	int		cFMTimelineAnimationObject::GetCurrentPlayCount()
	{
		return m_iCurrentPlayCount;
	}
	void	cFMTimelineAnimationObject::Stop()
	{
		m_fPastTime = m_fTotalPlayTime;
		m_iCurrentPlayCount = 0;
		m_fCurrentProgress = 1.f;
		SetAnimationDone(true); 
	}

	void	cFMTimelineAnimationObject::SetData(void*e_pData,bool e_bForceReplace)
	{
		if( m_pUserData == nullptr || e_bForceReplace)
		{
			this->m_pUserData = e_pData;
		}
	}

	void	cFMTimelineAnimationObject::UpdateByGlobalTime(float e_fGlobalTime)
	{
		InternalUpdateByGlobalTime(e_fGlobalTime);
		cFMTimelineAnimationObject*l_pFirstChild = (cFMTimelineAnimationObject*)this->GetFirstChild();
		while( l_pFirstChild )
		{
			l_pFirstChild->UpdateByGlobalTime(e_fGlobalTime);
			l_pFirstChild = (cFMTimelineAnimationObject*)l_pFirstChild->GetNextSibling();
		}
	}


    void	cFMTimelineAnimationObject::Init()
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
	    cRenderNode::Init();
    }

    void	cFMTimelineAnimationObject::Update(float e_fElpaseTime)
    {
		if( m_bInitCalled )
		{
			if( m_iCurrentPlayCount != this->m_iPlayCount)
			{
				m_fPastTime += e_fElpaseTime;
				float	l_fEndTime = this->GetEndTime();
				if( m_fPastTime >= l_fEndTime )
				{
					//seems use m_iCurrentPlayCount += m_fPastTime/l_fEndTime; is better but I do care.
					m_iCurrentPlayCount++;
					if( m_iCurrentPlayCount  != m_iPlayCount )
					{
						int l_iCurrentPlayTime = m_iCurrentPlayCount;
						m_fPastTime = GetFloatModulus(m_fPastTime,m_fTotalPlayTime);
						InternalInit();
						e_fElpaseTime = m_fPastTime;
						m_iCurrentPlayCount = l_iCurrentPlayTime;
					}
					else
					{
						if( m_bStayAtLastFrame )
						{
							if(m_fCurrentProgress != 1.f)
							{
								InternalInit();
								InternalUpdate(m_fPastTime-m_fStartTime);
							}
						}
						m_fCurrentProgress = 1.f;
					}
				}
				if( !this->IsAnimationDone() )
				{
					if( !m_bStart )
					{
						if(m_fPastTime>m_fStartTime)
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
			else
			if( m_iCurrentPlayCount == this->m_iPlayCount)
			{
				m_fCurrentProgress = 1.f;
			}
		}
		DoChildUpdate(e_fElpaseTime);
    }

    void	cFMTimelineAnimationObject::Render()
    {
		if( m_bStart )
		{
			if( !this->IsAnimationDone() || m_bStayAtLastFrame )
				InternalRender();
		}
		DoChildRender();
    }
    bool	cFMTimelineAnimationObject::IsAnimationLoop(){ return m_iPlayCount == -1?true:false;}
	bool	cFMTimelineAnimationObject::IsAnimationDone()
	{
		if( m_iPlayCount == -1 || m_iCurrentPlayCount != this->m_iPlayCount )
			return false;
		return true;
	}
    float	cFMTimelineAnimationObject::GetPastTime(){ return m_fPastTime; }
    void	cFMTimelineAnimationObject::SetAnimationLoop(bool e_bLoop){ m_iPlayCount = e_bLoop?-1:m_iPlayCount; }
    void	cFMTimelineAnimationObject::SetAnimationDone(bool e_bAnimationDone)
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
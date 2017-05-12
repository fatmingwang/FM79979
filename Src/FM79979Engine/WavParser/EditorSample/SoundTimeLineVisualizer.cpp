#include "stdafx.h"
#include "SoundTimeLineVisualizer.h"
#include "SoundTimeLineData.h"
cChartBasicInfo::cChartBasicInfo()
{
	m_vShowPos = Vector2::Zero;
	m_vResolution = Vector2(1,1);
}
cChartBasicInfo::~cChartBasicInfo()
{
	
}

void	cChartBasicInfo::RenderEdgeLines()
{
	GLRender::RenderRectangle(this->m_vShowPos,this->m_vResolution.x,this->m_vResolution.y,Vector4::One);
}

float		cChartBasicInfo::GetHeightGapByPoints(int e_iPoint)
{
	return m_vResolution.y/e_iPoint;
}

float		cChartBasicInfo::GetWidthGapByPoints(int e_iPoint)
{
	return m_vResolution.x/e_iPoint;
}

Vector2		cChartBasicInfo::GetGapByPoints(POINT e_ResolutionPoints)
{
	Vector2 l_v = Vector2(m_vResolution.x/e_ResolutionPoints.x,m_vResolution.y/e_ResolutionPoints.y);
	return l_v;
}



cTimeLineRange::cTimeLineRange(float e_fEndTime,float e_fBeforeCurrentTimeViewRange,float e_fAfterCurrentTimeViewRange)
{
	m_fCurrentTime = 0.f;
	m_fBeforeCurrentTimeViewRange = e_fBeforeCurrentTimeViewRange;
	m_fAfterCurrentTimeViewRange = e_fAfterCurrentTimeViewRange;
	//m_fStartTimne = 0.f;
	m_fEndTime = e_fEndTime;//
	SetCurrentTimeViewRange(m_fBeforeCurrentTimeViewRange,m_fAfterCurrentTimeViewRange);
}

cTimeLineRange::~cTimeLineRange()
{
	
}

void	cTimeLineRange::SetCurrentTimeViewRange(float e_fBefore,float e_fAfter)
{
	this->m_vCurrentTimeViewRange.x = m_fCurrentTime-e_fBefore;
	this->m_vCurrentTimeViewRange.y = m_fCurrentTime+e_fAfter;
}

void	cTimeLineRange::Init()
{
	JumpTo(0.f);
}

void	cTimeLineRange::Update(float e_fElpaseTime)
{
	if( !IsEnd() )
		this->m_fCurrentTime += e_fElpaseTime;
}
//
void	cTimeLineRange::JumpTo(float e_fTime)
{
	m_fCurrentTime = 0.f;
	SetCurrentTimeViewRange(m_fBeforeCurrentTimeViewRange,m_fAfterCurrentTimeViewRange);	
}
//
bool	cTimeLineRange::IsEnd()
{
	if( m_fCurrentTime >= this->m_fEndTime )
		return true;
	return true;
}
//
Vector2	cTimeLineRange::GetCurrentTimeViewRange()
{
	return m_vCurrentTimeViewRange;	
}



cTimeLineRangeChart::cTimeLineRangeChart(cSoundTimeLineDataCollection*e_pSoundTimeLineDataCollection)
{
	m_pSoundTimeLineDataCollection = e_pSoundTimeLineDataCollection;
	m_iLastToneDataObjectIndex = -1;
}

cTimeLineRangeChart::~cTimeLineRangeChart()
{

}

void	cTimeLineRangeChart::Init()
{
	cTimeLineRange::Init();
	m_iLastToneDataObjectIndex = 0;
}

void	cTimeLineRangeChart::CollectToneDataByTimeRange()
{
	m_iCountInCompareTime = 0;
	if( m_pSoundTimeLineDataCollection && m_iLastToneDataObjectIndex != -1 )
	{
		int		l_iCount = m_pSoundTimeLineDataCollection->Count();
		if( l_iCount == 0 )
			return;
		//find statr and end index
		float	l_fStartTime = this->m_fCurrentTime-this->m_fBeforeCurrentTimeViewRange;
		float	l_fCompareEndTime = this->m_fCurrentTime-this->m_fAfterCurrentTimeViewRange;
		float	l_fEndTime = this->m_fEndTime-this->m_fAfterCurrentTimeViewRange;
		if(l_fEndTime >= m_pSoundTimeLineDataCollection->GetLastObjectCompareEndTime() )
		{//finish
			m_iLastToneDataObjectIndex = -1;
			return;
		}
		cSoundTimeLineData*l_pFirstToneDataObject = m_pSoundTimeLineDataCollection->GetObject(0);
		if( l_pFirstToneDataObject && l_pFirstToneDataObject->GetCompareTime() <= l_fStartTime )
		{//wait start
			return;
		}
		Vector2 l_vCurrentTimeViewRange = this->GetCurrentTimeViewRange();
		cSoundTimeLineData*l_pSoundTimeLineData = nullptr;
		int l_iCount = m_pSoundTimeLineDataCollection->Count();
		while( !l_pSoundTimeLineData && m_iLastToneDataObjectIndex < l_iCount )
		{
			l_pSoundTimeLineData = m_pSoundTimeLineDataCollection->GetObject(m_iLastToneDataObjectIndex);
			if( l_pSoundTimeLineData && !l_pSoundTimeLineData->IsStillInCompareTime(l_fStartTime) )
			{
				l_pSoundTimeLineData = nullptr;
				m_iLastToneDataObjectIndex++;
			}
		}
		m_iCountInCompareTime = 1;
		for(int i=m_iLastToneDataObjectIndex;i<l_iCount;++i)
		{
			if( l_pSoundTimeLineData && !l_pSoundTimeLineData->IsStillInCompareTime(l_fStartTime) )
			{
				++m_iCountInCompareTime;
			}
			else
			{
				break;
			}
		}
	}
}

void	cTimeLineRangeChart::Update(float e_fElpaseTime)
{
	cTimeLineRange::Update(e_fElpaseTime);
	if( !m_pSoundTimeLineDataCollection )
		return;
	CollectToneDataByTimeRange();
}

void	cTimeLineRangeChart::Render()
{
	cChartBasicInfo::RenderEdgeLines();
	if( m_iCountInCompareTime < 1 )
		return;
	for(int i=0;i<this->m_iLastToneDataObjectIndex;this->m_iCountInCompareTime)
	{
	
	}
}
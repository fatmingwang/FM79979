#include "stdafx.h"
#include "TimelineBasicInfo.h"
#include "Parameters.h"
cChartBasicInfo::cChartBasicInfo()
{
	m_vShowPos = cSoundCompareParameter::m_vTimelineShowPos;
	m_vResolution = cSoundCompareParameter::m_vTimelineResolution;
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

float	cChartBasicInfo::GetHeightGapByPoints(float e_fPoint)
{
	return m_vResolution.x/e_fPoint;
}

float	cChartBasicInfo::GetWidthGapByPoints(float e_fPoint)
{
	return m_vResolution.y/e_fPoint;
}

Vector2		cChartBasicInfo::GetGapByPoints(POINT e_ResolutionPoints)
{
	Vector2 l_v = Vector2(m_vResolution.x/e_ResolutionPoints.x,m_vResolution.y/e_ResolutionPoints.y);
	return l_v;
}

void		cChartBasicInfo::RenderWithData(Vector2 e_vShowPos,Vector2 e_vResolution,int*e_piData,int e_iCount,Vector2*e_pvPointsData,Vector4 e_vColor,float e_fYScale)
{
	if( !e_pvPointsData )
		return;
	float l_fXGap = e_vResolution.x/e_iCount;
	Vector2 l_vShowPos = e_vShowPos;
	std::vector<Vector2>	l_vBlueLinePos;
	for( int i=0;i<e_iCount;++i )
	{
		e_pvPointsData[i*2].x = l_vShowPos.x;
		e_pvPointsData[i*2+1].x = l_vShowPos.x;
		e_pvPointsData[i*2].y = l_vShowPos.y;
		e_pvPointsData[i*2+1].y = e_piData[i]*e_fYScale+l_vShowPos.y;
		if( i % 100 == 0 )
		{
			l_vBlueLinePos.push_back(e_pvPointsData[i*2]);
			Vector2 l_vPosYIncrease = e_pvPointsData[i*2+1];
			l_vPosYIncrease.y = l_vShowPos.y+20;
			l_vBlueLinePos.push_back(l_vPosYIncrease);
		}
		l_vShowPos.x += l_fXGap;
	}
	RenderLine((float*)e_pvPointsData,e_iCount*2,e_vColor,2);
	RenderLine((float*)&l_vBlueLinePos[0],l_vBlueLinePos.size(),Vector4::Blue,2);
}



cTimeLineRange::cTimeLineRange(float e_fBeforeCurrentTimeViewRange,float e_fAfterCurrentTimeViewRange)
{
	m_fCurrentTime = 0.f;
	m_fBeforeCurrentTimeViewRange = e_fBeforeCurrentTimeViewRange;
	m_fAfterCurrentTimeViewRange = e_fAfterCurrentTimeViewRange;
	//m_fStartTimne = 0.f;
	m_fEndTime = -1.f;//
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
	//JumpTo(-(this->m_fBeforeCurrentTimeViewRange+this->m_fAfterCurrentTimeViewRange)/2);
	JumpTo(-0.5f);
}

void	cTimeLineRange::Update(float e_fElpaseTime)
{
	//if( !IsEnd() )
	{
		this->m_fCurrentTime += e_fElpaseTime;
	}
}
//
void	cTimeLineRange::JumpTo(float e_fTime)
{
	m_fCurrentTime = e_fTime;
	SetCurrentTimeViewRange(m_fBeforeCurrentTimeViewRange,m_fAfterCurrentTimeViewRange);
}
//
bool	cTimeLineRange::IsEnd(bool e_bCalculateWithBeforeTime)
{
	if( e_bCalculateWithBeforeTime )
	{
		if( m_fCurrentTime >= this->m_fEndTime+this->m_fBeforeCurrentTimeViewRange )
			return true;
	}
	else
	{
		if( m_fCurrentTime >= this->m_fEndTime )
			return true;
	}
	return false;
}
//
Vector2	cTimeLineRange::GetCurrentTimeViewRange()
{
	return m_vCurrentTimeViewRange;	
}

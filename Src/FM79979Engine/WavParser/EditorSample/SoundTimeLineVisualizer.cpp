#include "stdafx.h"
#include "SoundTimeLineVisualizer.h"
#include "SoundTimeLineData.h"
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

Vector2		cChartBasicInfo::GetGapByPoints(POINT e_ResolutionPoints)
{
	Vector2 l_v = Vector2(m_vResolution.x/e_ResolutionPoints.x,m_vResolution.y/e_ResolutionPoints.y);
	return l_v;
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
	JumpTo(-this->m_fAfterCurrentTimeViewRange);
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
bool	cTimeLineRange::IsEnd()
{
	if( m_fCurrentTime >= this->m_fEndTime )
		return true;
	return false;
}
//
Vector2	cTimeLineRange::GetCurrentTimeViewRange()
{
	return m_vCurrentTimeViewRange;	
}



cTimeLineRangeChart::cTimeLineRangeChart(float e_fBeforeCurrentTimeViewRange,float e_fAfterCurrentTimeViewRange)
	:cTimeLineRange(e_fBeforeCurrentTimeViewRange,e_fAfterCurrentTimeViewRange)
{
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

bool	cTimeLineRangeChart::MyParse(TiXmlElement*e_pRoot)
{
	if(cSoundTimeLineDataCollection::MyParse(e_pRoot))
	{
		this->SetEndTime(this->GetLastObjectCompareEndTime());
		return true;
	}
	UT::ErrorMsg("something wrong,cTimeLineRangeChart::MyParse",this->m_strFileName);
	return false;
}

void	cTimeLineRangeChart::CollectToneDataByTimeRange()
{
	m_iCountInCompareTime = 0;
	int		l_iCount = Count();
	if( m_iLastToneDataObjectIndex != -1 && l_iCount > 0 )
	{
		//find statr and end index
		float	l_fCurrentStartTime = m_vCurrentTimeViewRange.x;
		float	l_fCurrentEndTime = m_vCurrentTimeViewRange.y;
		float	l_fEndTime = this->m_fEndTime+this->m_fBeforeCurrentTimeViewRange;

		if( this->m_fCurrentTime > l_fEndTime )
		{//finish
			m_iLastToneDataObjectIndex = -1;
			return;
		}
		cSoundTimeLineData*l_pFirstToneDataObject = GetObject(0);
		if( l_pFirstToneDataObject && l_pFirstToneDataObject->GetCompareTime() > l_fCurrentEndTime )
		{//wait start
			return;
		}
		Vector2 l_vCurrentTimeViewRange = this->GetCurrentTimeViewRange();
		cSoundTimeLineData*l_pSoundTimeLineData = nullptr;
		int l_iLastToneDataObjectIndex = m_iLastToneDataObjectIndex;
		while( l_iLastToneDataObjectIndex < l_iCount )
		{
			l_pSoundTimeLineData = GetObject(l_iLastToneDataObjectIndex);
			if( l_pSoundTimeLineData && l_pSoundTimeLineData->GetCompareTime() >= l_fCurrentStartTime  )
			{
				m_iLastToneDataObjectIndex = l_iLastToneDataObjectIndex;
				break;
			}
			++l_iLastToneDataObjectIndex;
		}
		m_iCountInCompareTime = 0;
		for(int i=m_iLastToneDataObjectIndex;i<l_iCount;++i)
		{
			l_pSoundTimeLineData = GetObject(i);
			if( l_pSoundTimeLineData && l_pSoundTimeLineData->GetCompareTime() <= l_fCurrentEndTime )
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
	SetCurrentTimeViewRange(m_fBeforeCurrentTimeViewRange,m_fAfterCurrentTimeViewRange);
	CollectToneDataByTimeRange();
	int l_iCount = m_iLastToneDataObjectIndex+this->m_iCountInCompareTime;
	for(int i=m_iLastToneDataObjectIndex;i<l_iCount;++i)
	//for(int i=0;i<this->m_iLastToneDataObjectIndex;this->m_iCountInCompareTime)
	{
		auto l_pData = this->GetObject(i);
		l_pData->Update(this->m_fCurrentTime);
	}
}

void	cTimeLineRangeChart::Render()
{
	//for edges
	cChartBasicInfo::RenderEdgeLines();
	float l_fTotalTime = m_fBeforeCurrentTimeViewRange+m_fAfterCurrentTimeViewRange;
	float l_fShowCurrentTimePosX = m_fBeforeCurrentTimeViewRange/l_fTotalTime*this->m_vResolution.x+this->m_vShowPos.x;
	const int l_iCurrentLineWidth = 5;
	const int l_iNameYOffset = 10;
	GLRender::RenderRectangle(Vector2(l_fShowCurrentTimePosX,m_vShowPos.y),(float)l_iCurrentLineWidth,this->m_vResolution.y,Vector4::One);
	//
	if( m_iCountInCompareTime < 1 )
		return;
	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::Red);
	int l_iCount = m_iLastToneDataObjectIndex+this->m_iCountInCompareTime;
	for(int i=m_iLastToneDataObjectIndex;i<l_iCount;++i)
	//for(int i=0;i<this->m_iLastToneDataObjectIndex;this->m_iCountInCompareTime)
	{
		auto l_pData = this->GetObject(i);
		//get percentage
		float l_fCompareTime = l_pData->GetCompareTime();
		//float l_fLocalTime = this->m_fCurrentTime-l_fCompareTime;
		float l_fLERP = GetTimeGapLERP(this->m_vCurrentTimeViewRange.x,this->m_vCurrentTimeViewRange.y,l_fCompareTime);
		float l_fShowPosX = this->m_vResolution.x*l_fLERP+this->m_vShowPos.x;
		Vector2 l_vShowPos = Vector2(l_fShowPosX,m_vShowPos.y);
		GLRender::RenderRectangle(l_vShowPos,(float)l_iCurrentLineWidth,this->m_vResolution.y,Vector4::Green);
		l_vShowPos.y -= l_iNameYOffset;
		std::wstring l_strDebugInfo = l_pData->GetName();
		l_strDebugInfo += L",";
		l_strDebugInfo += ValueToStringW((int)l_pData->GetCompareTime());
		cGameApp::RenderFont(l_vShowPos,l_strDebugInfo.c_str());
	}
	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
	std::wstring l_strCurrentTime = ValueToStringW(this->m_fCurrentTime);
	Vector2 l_vFontPos = this->m_vShowPos;
	l_vFontPos.y -= 50;
	cGameApp::RenderFont(l_vFontPos,l_strCurrentTime.c_str());
}
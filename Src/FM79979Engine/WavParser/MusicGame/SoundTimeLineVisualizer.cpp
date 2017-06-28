#include "stdafx.h"
#include "SoundTimeLineVisualizer.h"
#include "SoundTimeLineData.h"
#include "ToneData.h"
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



cTimeLineRangeChart::cTimeLineRangeChart(cMPDI*e_pTimeLineMPDI,float e_fBeforeCurrentTimeViewRange,float e_fAfterCurrentTimeViewRange,eMoveDirection e_eMoveDirection)
	:cTimeLineRange(e_fBeforeCurrentTimeViewRange,e_fAfterCurrentTimeViewRange)
{
	m_pTimeLineMPDI = e_pTimeLineMPDI;
	m_NoteInTimelineImage = nullptr;
	m_pTickImage = nullptr;
	m_pTimelineNoteRoot = nullptr;
	m_WhiteKeyImage = nullptr;;
	m_BlackKeyImage = nullptr;
	m_eMoveDirection = e_eMoveDirection;
	//m_eMoveDirection = eMD_LEFT_TO_RIGHT;
	//m_eMoveDirection = eMD_RIGHT_TO_LEFT;
	m_eMoveDirection = eMD_UP_TO_DOWN;
	//m_eMoveDirection = eMD_DOWN_TO_UP;
	m_iLastToneDataObjectIndex = -1;
	m_fEndTimeWithBeforTime = 0.f;
	m_SoundTimeLineDataTimeline.SetShowPos(Vector2(0,860));
	m_SoundTimeLineDataTimeline.SetResolution(Vector2(1920,220));
}

cTimeLineRangeChart::~cTimeLineRangeChart()
{
	if( m_pTimelineNoteRoot )
		Frame::DestoryWithChildren(m_pTimelineNoteRoot);
	SAFE_DELETE(m_NoteInTimelineImage);
	SAFE_DELETE(m_WhiteKeyImage);
	SAFE_DELETE(m_BlackKeyImage);
	SAFE_DELETE(m_pTickImage);
}

void	cTimeLineRangeChart::Init()
{
	cTimeLineRange::Init();
	cSoundTimeLineDataCollection::Init();
	m_iLastToneDataObjectIndex = 0;
	m_fEndTimeWithBeforTime = this->m_fEndTime+this->m_fBeforeCurrentTimeViewRange;
	if( !m_pTimelineNoteRoot && m_pTimeLineMPDI )
	{
		auto l_pTimer = m_pTimeLineMPDI->GetObject(L"timer");
		auto l_pNote = m_pTimeLineMPDI->GetObject(L"Note");

		auto l_pWhiteKey = m_pTimeLineMPDI->GetObject(L"WhiteKeySymbol");
		auto l_pBlackKey = m_pTimeLineMPDI->GetObject(L"BlackKeySymbol");
		if( l_pWhiteKey )
		{
			m_WhiteKeyImage = l_pWhiteKey->PointDataToBaseImage(0);
		}
		if( l_pBlackKey )
		{
			m_BlackKeyImage = l_pBlackKey->PointDataToBaseImage(0);
		}
		if( !m_NoteInTimelineImage )
		{
			m_NoteInTimelineImage = l_pNote->PointDataToBaseImage(0);
			if( m_NoteInTimelineImage )
			{
				m_NoteInTimelineImage->SetColor(Vector4(1.2f,1.1f,1.9f,1.f));
				m_NoteInTimelineImage->SetWidth((int)(m_NoteInTimelineImage->GetWidth()*0.5));
				m_NoteInTimelineImage->SetHeight((int)(m_NoteInTimelineImage->GetHeight()*0.5));
			}
		}
		
		m_pTimelineNoteRoot = new cRenderObject();
		if( !m_pTickImage )
		{
			m_pTickImage = l_pTimer->PointDataToBaseImage(0);
			m_SoundTimeLineDataTimeline.SetShowPos(this->m_vShowPos);
			m_SoundTimeLineDataTimeline.SetResolution(this->m_vResolution);
			auto l_pLocalBount = m_pTickImage->GenerateBound();
			if( l_pLocalBount )
			{
				RECT l_Rect = l_pLocalBount->GetRect();
				POINT l_Size = {l_Rect.left-l_Rect.left,l_Rect.bottom-l_Rect.top};
				l_Rect.left -= l_Size.x;
				l_Rect.right += l_Size.x;
				l_Rect.top -= l_Size.y;
				l_Rect.bottom += l_Size.y;
				cBound l_Bound(l_Rect);
				m_pTickImage->SetLocalBound(&l_Bound);
			}
		}
	}

	if( !m_pTickImage )
	{
		m_pTickImage = new cBaseImage("MusicGame/Image/Tick.png");
		m_pTickImage->GenerateBound();
	}
	if( m_pTickImage )
	{
		//this->m_vShowPos.x = m_pTickImage->GetPos().x;
		//this->m_vShowPos.y = m_pTickImage->GetPos().y;
		//this->m_vResolution.x = cGameApp::m_svGameResolution.x;
		//this->m_vResolution.y = cGameApp::m_svGameResolution.y-m_pTickImage->GetPos().y;
	}
	SetupSoundTimeLineDataObject(m_SoundTimeLineDataTimeline.GetShowPos(),m_SoundTimeLineDataTimeline.GetResolution(),m_fEndTimeWithBeforTime);
}

bool	cTimeLineRangeChart::MyParse(TiXmlElement*e_pRoot)
{
	this->Destroy();
	PARSE_ELEMENT_START(e_pRoot)
		COMPARE_ASSIGN_VECTOR2("ChartResolution",m_vResolution)
		else
		COMPARE_ASSIGN_VECTOR2("ChartShowPos",m_vShowPos)
		else
		COMPARE_NAME("MoveDirection")
		{
			m_eMoveDirection = StringToMoveDirection(l_strValue);
		}
		else
		COMPARE_ASSIGN_FLOAT("BeforeTime",this->m_fBeforeCurrentTimeViewRange)
		else
		COMPARE_ASSIGN_FLOAT("AfterTime",this->m_fAfterCurrentTimeViewRange)
	TO_NEXT_VALUE
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
		int l_iLastToneDataObjectIndexTemp = m_iLastToneDataObjectIndex;
		//find statr and end index
		float	l_fCurrentStartTime = m_vCurrentTimeViewRange.x;
		float	l_fCurrentEndTime = m_vCurrentTimeViewRange.y;
		float	l_fEndTime = m_fEndTimeWithBeforTime;

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
			if( l_pSoundTimeLineData && l_pSoundTimeLineData->GetCompareTime()+l_pSoundTimeLineData->GetTuneKeepTime() >= l_fCurrentStartTime  )
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

void	cTimeLineRangeChart::RenderHorizontal()
{

}

void	cTimeLineRangeChart::RenderVertical()
{
	float	l_fTotalTime = m_fBeforeCurrentTimeViewRange+m_fAfterCurrentTimeViewRange;
	float	l_fOneSecondLength = this->m_vResolution.y/l_fTotalTime;
	int l_iCount = m_iLastToneDataObjectIndex+this->m_iCountInCompareTime;
	for(int i=m_iLastToneDataObjectIndex;i<l_iCount;++i)
	//for(int i=0;i<this->m_iLastToneDataObjectIndex;this->m_iCountInCompareTime)
	{
		auto l_pData = this->GetObject(i);
		if( !l_pData )
			break;
		//get percentage
		float l_fCompareTime = l_pData->GetCompareTime();
		//float l_fLocalTime = this->m_fCurrentTime-l_fCompareTime;
		float l_fLERP = GetTimeGapLERP(this->m_vCurrentTimeViewRange.x,this->m_vCurrentTimeViewRange.y,l_fCompareTime);
		if(m_eMoveDirection == eMD_UP_TO_DOWN)
		{
			l_fLERP = 1-l_fLERP;
		}
		if( l_fLERP >1.f )
			continue;
		float l_fShowPosY = this->m_vResolution.y*l_fLERP;//+this->m_vShowPos.y;
		//fuck...
		float l_fTuneKeepTime = l_pData->GetTuneKeepTime()*l_fOneSecondLength/4;
		Vector2 l_vShowPos = Vector2(m_vShowPos.x,l_fShowPosY);
		auto l_pToneData = l_pData->GetToneData();
		l_vShowPos.x += l_pToneData->GetWorldPosition().x;
		l_vShowPos.y += l_pToneData->GetWorldPosition().y;
		Vector2 l_vTonePos = l_vShowPos;
		if( l_pData->IsTuneMatched() )
			continue;
		//l_vTonePos.y -= l_fTuneKeepTime;
		if(l_pData->IsTuneMatched())
		{//play effect!?

		}
		else
		{
			if(l_pData->GetToneData()->IsBlackKey())
			{
				if(m_BlackKeyImage)
				{
					l_vTonePos.x -= m_BlackKeyImage->GetWidth()/2.f;
					l_vTonePos.y -= m_BlackKeyImage->GetHeight()/2.f;
					m_BlackKeyImage->Render(l_vTonePos);
				}
			}
			else
			{
				if(m_WhiteKeyImage)
				{
					l_vTonePos.x -= m_WhiteKeyImage->GetWidth()/2.f;
					l_vTonePos.y -= m_WhiteKeyImage->GetHeight()/2.f;
					m_WhiteKeyImage->Render(l_vTonePos);
				}
			}
		}
	}
}


void	cTimeLineRangeChart::RenderDebugHorizontal()
{
	const float l_fCurrentLineHeight = 5.f;
	const int l_iNameYOffset = 10;
	float	l_fTotalTime = m_fBeforeCurrentTimeViewRange+m_fAfterCurrentTimeViewRange;
	float	l_fOneSecondLength = this->m_vResolution.x/l_fTotalTime;
	float	l_fShowCurrentTimePosX = m_fAfterCurrentTimeViewRange;
	float	l_fPerformanceRange = cSoundCompareParameter::m_sfCompareTuneTolerateTime*2/l_fTotalTime;
	//for edges
	cChartBasicInfo::RenderEdgeLines();
	if( m_eMoveDirection == eMD_RIGHT_TO_LEFT )
		l_fShowCurrentTimePosX = m_fBeforeCurrentTimeViewRange;
	l_fShowCurrentTimePosX = l_fShowCurrentTimePosX/l_fTotalTime*this->m_vResolution.x+this->m_vShowPos.x;
	l_fPerformanceRange = this->m_vResolution.x*l_fPerformanceRange;
	GLRender::RenderRectangle(Vector2(l_fShowCurrentTimePosX,m_vShowPos.y),l_fPerformanceRange,this->m_vResolution.y,Vector4::Red);

	GLRender::RenderRectangle(Vector2(l_fShowCurrentTimePosX-l_fPerformanceRange/2,m_vShowPos.y),1,this->m_vResolution.y,Vector4::Blue);

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
		if(m_eMoveDirection == eMD_LEFT_TO_RIGHT)
		{
			l_fLERP = 1-l_fLERP;
		}
		float l_fShowPosX = this->m_vResolution.x*l_fLERP+this->m_vShowPos.x;
		Vector2 l_vShowPos = Vector2(l_fShowPosX,m_vShowPos.y);
		auto l_pToneData = l_pData->GetToneData();
		l_vShowPos.x += l_pToneData->GetWorldPosition().x;
		l_vShowPos.y += l_pToneData->GetWorldPosition().y;

		float l_fTuneKeepTime = l_pData->GetTuneKeepTime()*l_fOneSecondLength;
		if(l_pData->IsTuneMatched())
			GLRender::RenderRectangle(l_vShowPos,l_fTuneKeepTime,l_fCurrentLineHeight,Vector4::Red);
		else
		{
			if(l_pData->GetToneData()->IsBlackKey())
				GLRender::RenderRectangle(l_vShowPos,l_fTuneKeepTime,l_fCurrentLineHeight,Vector4::Green);
			else
				GLRender::RenderRectangle(l_vShowPos,l_fTuneKeepTime,l_fCurrentLineHeight,Vector4::Blue);
		}
		
		//l_vShowPos.y -= l_iNameYOffset;
		//std::wstring l_strDebugInfo = l_pData->GetName();
		//l_strDebugInfo += L",";
		//l_strDebugInfo += ValueToStringW((int)l_pData->GetCompareTime());
		//cGameApp::RenderFont(l_vShowPos,l_strDebugInfo.c_str());
	}
	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
	std::wstring l_strCurrentTime = ValueToStringW(this->m_fCurrentTime);
	Vector2 l_vFontPos = this->m_vShowPos;
	l_vFontPos.y -= 50;
	cGameApp::RenderFont(l_vFontPos,l_strCurrentTime.c_str());
}

void	cTimeLineRangeChart::RenderDebugVertical()
{
	const float l_fCurrentLineWidth = 2.f;
	const int	l_iNameYOffset = 10;
	float	l_fTotalTime = m_fBeforeCurrentTimeViewRange+m_fAfterCurrentTimeViewRange;
	float	l_fShowCurrentTimePosY = m_fBeforeCurrentTimeViewRange;
	float	l_fOneSecondLength = this->m_vResolution.y/l_fTotalTime;
	float	l_fPerformanceRange = cSoundCompareParameter::m_sfCompareTuneTolerateTime*2/l_fTotalTime;
	//for edges
	cChartBasicInfo::RenderEdgeLines();
	if( m_eMoveDirection == eMD_UP_TO_DOWN )
	{
		l_fShowCurrentTimePosY = m_fAfterCurrentTimeViewRange;
	}
	l_fShowCurrentTimePosY = l_fShowCurrentTimePosY/l_fTotalTime*this->m_vResolution.y+this->m_vShowPos.y;								


	l_fPerformanceRange = this->m_vResolution.y*l_fPerformanceRange;

	GLRender::RenderRectangle(Vector2(m_vShowPos.x,l_fShowCurrentTimePosY-l_fPerformanceRange/2),this->m_vResolution.x,l_fPerformanceRange,Vector4::Red);
	GLRender::RenderRectangle(Vector2(m_vShowPos.x,l_fShowCurrentTimePosY),this->m_vResolution.x,1,Vector4::Blue);

	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::Red);
	cGameApp::m_spGlyphFontRender->SetScale(2.f);
	int l_iCount = m_iLastToneDataObjectIndex+this->m_iCountInCompareTime;
	for(int i=m_iLastToneDataObjectIndex;i<l_iCount;++i)
	//for(int i=0;i<this->m_iLastToneDataObjectIndex;this->m_iCountInCompareTime)
	{
		auto l_pData = this->GetObject(i);
		if( !l_pData )
			break;
		//get percentage
		float l_fCompareTime = l_pData->GetCompareTime();
		//float l_fLocalTime = this->m_fCurrentTime-l_fCompareTime;
		float l_fLERP = GetTimeGapLERP(this->m_vCurrentTimeViewRange.x,this->m_vCurrentTimeViewRange.y,l_fCompareTime);
		if(m_eMoveDirection == eMD_UP_TO_DOWN)
		{
			l_fLERP = 1-l_fLERP;
		}
		float l_fShowPosY = this->m_vResolution.y*l_fLERP+this->m_vShowPos.y;
		//fuck...
		float l_fTuneKeepTime = l_pData->GetTuneKeepTime()*l_fOneSecondLength/4;
		Vector2 l_vShowPos = Vector2(m_vShowPos.x,l_fShowPosY);
		auto l_pToneData = l_pData->GetToneData();
		l_vShowPos.x += l_pToneData->GetWorldPosition().x;
		l_vShowPos.y += l_pToneData->GetWorldPosition().y;
		Vector2 l_vTonePos = l_vShowPos;
		l_vTonePos.y -= l_fTuneKeepTime;
		if(l_pData->IsTuneMatched())
			GLRender::RenderRectangle(l_vTonePos,l_fCurrentLineWidth,l_fTuneKeepTime,Vector4::Red);
		else
		{
			if(l_pData->GetToneData()->IsBlackKey())
				GLRender::RenderRectangle(l_vTonePos,l_fCurrentLineWidth,l_fTuneKeepTime,Vector4::Green);
			else
				GLRender::RenderRectangle(l_vTonePos,l_fCurrentLineWidth,l_fTuneKeepTime,Vector4::Blue);
		}
		//l_vShowPos.x -= l_iNameYOffset;
		//std::wstring l_strDebugInfo = l_pData->GetName();
		//l_strDebugInfo += L",";
		//l_strDebugInfo += ValueToStringW((int)l_pData->GetCompareTime());
		//cGameApp::RenderFont(l_vShowPos,l_strDebugInfo.c_str());
	}
	cGameApp::m_spGlyphFontRender->SetScale(1.f);
	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
	std::wstring l_strCurrentTime = ValueToStringW(this->m_fCurrentTime);
	Vector2 l_vFontPos = this->m_vShowPos;
	l_vFontPos.y -= 50;
	cGameApp::RenderFont(l_vFontPos,l_strCurrentTime.c_str());
}

void	cTimeLineRangeChart::RenderTimeLine()
{
	
}

void	cTimeLineRangeChart::DebugRender()
{
	if( m_iCountInCompareTime < 1 )
		return;
	if( m_eMoveDirection == eMD_LEFT_TO_RIGHT || m_eMoveDirection == eMD_RIGHT_TO_LEFT )
		RenderDebugHorizontal();
	else
		RenderDebugVertical();

	DebugRenderTimeLineData(m_SoundTimeLineDataTimeline.GetShowPos(),m_SoundTimeLineDataTimeline.GetResolution(),this->m_fCurrentTime,this->m_fEndTimeWithBeforTime);
}

void	cTimeLineRangeChart::Render()
{
	if( m_iCountInCompareTime < 1 )
		return;
	if( m_eMoveDirection == eMD_LEFT_TO_RIGHT || m_eMoveDirection == eMD_RIGHT_TO_LEFT )
		RenderHorizontal();
	else
		RenderVertical();
	RenderTimeLineData(m_SoundTimeLineDataTimeline.GetShowPos(),m_SoundTimeLineDataTimeline.GetResolution(),this->m_fCurrentTime,this->m_fEndTimeWithBeforTime);
}

bool	cTimeLineRangeChart::Compare(float e_fElpaseTime,cQuickFFTDataFrequencyFinder*e_pQuickFFTDataFrequencyFinder)
{
	int l_iCount = m_iLastToneDataObjectIndex+this->m_iCountInCompareTime;
	for(int i=m_iLastToneDataObjectIndex;i<l_iCount;++i)
	//for(int i=0;i<this->m_iLastToneDataObjectIndex;this->m_iCountInCompareTime)
	{
		auto l_pData = this->GetObject(i);
		l_pData->Compare(e_fElpaseTime,this->m_fCurrentTime,e_pQuickFFTDataFrequencyFinder);
	}
	return false;	
}

//
void	cTimeLineRangeChart::SetupSoundTimeLineDataObject(Vector2 e_vShowPos,Vector2 e_vResolution,float e_fTotalTime)
{
	m_SoundTimeLineDataObjectSmallTimelinePosVector.clear();
	float l_PosStepByTimeGap = this->m_vResolution.x/e_fTotalTime;
	size_t l_uiSize = this->m_ObjectList.size();
	for(size_t i=0;i<l_uiSize;++i)
	{
		auto l_pData  = this->m_ObjectList[i];
		float l_fTime = l_pData->GetCompareTime();
		float l_fPos = l_PosStepByTimeGap*l_fTime;
		Vector2 l_vShowPos = Vector2(l_fPos,e_vShowPos.y);
		m_SoundTimeLineDataObjectSmallTimelinePosVector.push_back(l_vShowPos);
	}
}

Vector2	cTimeLineRangeChart::GetCurrenTimeControlPos(Vector2 e_vShowPos,Vector2 e_vResolution,float e_fCurrentTime,float e_fTotalTime)
{
	float l_PosStepByTimeGap = e_vResolution.x/e_fTotalTime;
	Vector2 l_vCurrentTimePos = Vector2(l_PosStepByTimeGap*e_fCurrentTime,e_vShowPos.y);
	return l_vCurrentTimePos;
}

float	cTimeLineRangeChart::MousePositionToTime(int e_iMousePositionX,int e_iMousePositionY,Vector2 e_vShowPos,Vector2 e_vResolution,float e_fTotalTime)
{
	float	l_fEndPos = e_vShowPos.x+e_vResolution.x;
	float	l_Percentage = GetPosPercentageInRange<float>((float)e_iMousePositionX,e_vShowPos.x,l_fEndPos);
	float	l_fCurrentTime = l_Percentage*e_fTotalTime;
	return l_fCurrentTime;
}

void	cTimeLineRangeChart::DebugRenderTimeLineData(Vector2 e_vShowPos,Vector2 e_vResolution,float e_fCurrentTime,float e_fTotalTime)
{
	GLRender::RenderFilledRectangle(e_vShowPos,e_vResolution.x,e_vResolution.y,Vector4(1,1,0,1),0);
	//float l_PosStepByTimeGap = e_vResolution.x/e_fTotalTime;
	//Vector2 l_vCurrentTimePos = Vector2(l_PosStepByTimeGap*e_fCurrentTime,e_vShowPos.y);
	Vector2 l_vCurrentTimePos = GetCurrenTimeControlPos(e_vShowPos,e_vResolution,e_fCurrentTime,e_fTotalTime);
	if( m_pTickImage )
	{
		m_pTickImage->SetPos(l_vCurrentTimePos);
		m_pTickImage->Render();
	}
	else
		GLRender::RenderRectangle(l_vCurrentTimePos,1,e_vResolution.y,Vector4(1,0,1,1));
	size_t l_uiSize = m_SoundTimeLineDataObjectSmallTimelinePosVector.size();
	if( l_uiSize > 0 )
	{
		for(size_t i=0;i<l_uiSize;++i)
		{
			auto l_Data = this->m_ObjectList[i];
			Vector4 l_vColor = WAIT_TIME_TUNE;
			if( l_Data->IsTuneMatched() )
			{
				l_vColor = CORRECT_TUNE_COLOR;
			}
			else
			if( l_Data->IsTimeOver() )
			{
				l_vColor = TUNE_TIME_PAST;
			}
			GLRender::RenderRectangle(this->m_SoundTimeLineDataObjectSmallTimelinePosVector[i],1,e_vResolution.y,l_vColor);
		}
	}
}

void	cTimeLineRangeChart::RenderTimeLineData(Vector2 e_vShowPos,Vector2 e_vResolution,float e_fCurrentTime,float e_fTotalTime)
{
	Vector2 l_vCurrentTimePos = GetCurrenTimeControlPos(e_vShowPos,e_vResolution,e_fCurrentTime,e_fTotalTime);
	if( m_pTickImage )
	{
		l_vCurrentTimePos.y = m_pTickImage->GetPos().y;
		m_pTickImage->SetPos(l_vCurrentTimePos);
		m_pTickImage->Render();
	}
	else
		GLRender::RenderRectangle(l_vCurrentTimePos,1,e_vResolution.y,Vector4(1,0,1,1));
	if( m_pTimelineNoteRoot )
	{
		m_pTimelineNoteRoot->RenderNodes();
	}
	size_t l_uiSize = m_SoundTimeLineDataObjectSmallTimelinePosVector.size();
	if( l_uiSize > 0 )
	{
		for(size_t i=0;i<l_uiSize;++i)
		{
			auto l_Data = this->m_ObjectList[i];
			Vector2 l_vPos = this->m_SoundTimeLineDataObjectSmallTimelinePosVector[i];
			l_vPos.y = l_vCurrentTimePos.y;
			//l_vPos.y += m_NoteInTimelineImage->GetHeight();
			l_vPos.y += 36;
			m_NoteInTimelineImage->Render(l_vPos);
		}
	}

}

cBaseImage*		cTimeLineRangeChart::GetTimeControlImage()
{
	return m_pTickImage;
}

bool	cTimeLineRangeChart::TimeControlCollision(int e_iPosX,int e_iPosY)
{
	//Vector2 l_vPos = GetCurrenTimeControlPos(m_SoundTimeLineDataTimeline.GetShowPos(),m_SoundTimeLineDataTimeline.GetResolution(),this->m_fCurrentTime,this->m_fEndTimeWithBeforTime);
	if(this->m_pTickImage)
	{
		auto l_pBound = m_pTickImage->GetWorldBound();
		if( l_pBound )
		{
			return l_pBound->Collide(e_iPosX,e_iPosY);
		}
	}
	return false;
}

void	cTimeLineRangeChart::TimeControlMouseDown(int e_iPosX,int e_iPosY,cClickBehavior*e_pClickBehavior)
{
	int a=0;
}

void	cTimeLineRangeChart::TimeControlMouseMove(int e_iPosX,int e_iPosY,cClickBehavior*e_pClickBehavior)
{
	this->m_fCurrentTime = MousePositionToTime(e_iPosX,e_iPosY,m_SoundTimeLineDataTimeline.GetShowPos(),m_SoundTimeLineDataTimeline.GetResolution(),this->m_fEndTimeWithBeforTime);
	m_iLastToneDataObjectIndex = 0;
	//int l_iCount = this->Count();
	//for( int i=l_iCount-1;i>-1;--i )
	//{
	//	auto l_pData = this->GetObject(i);
	//	if(l_pData->GetCompareTime()+cSoundCompareParameter::m_sfCompareTuneTolerateTime >= m_fCurrentTime)
	//	{
	//		l_pData->SetTimeOver(false);
	//		l_pData->SetTuneMatched(false);
	//	}
	//	if( l_pData->IsTimeOver() || l_pData->IsTuneMatched() )
	//		break;
	//}
}

void	cTimeLineRangeChart::TimeControlMouseUp(int e_iPosX,int e_iPosY,cClickBehavior*e_pClickBehavior)
{
	this->m_fCurrentTime = MousePositionToTime(e_iPosX,e_iPosY,m_SoundTimeLineDataTimeline.GetShowPos(),m_SoundTimeLineDataTimeline.GetResolution(),this->m_fEndTimeWithBeforTime);
	m_iLastToneDataObjectIndex = 0;
	int l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		auto l_pData = this->GetObject(i);
		if(l_pData->GetCompareTime()+cSoundCompareParameter::m_sfCompareTuneTolerateTime >= m_fCurrentTime)
		{
			l_pData->SetTimeOver(false);
			l_pData->SetTuneMatched(false);
		}
		else
		{
			if( !l_pData->IsTuneMatched() )
				l_pData->SetTimeOver(true);
		}
	}
}
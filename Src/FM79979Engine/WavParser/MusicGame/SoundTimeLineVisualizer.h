#pragma once

#include "SoundTimeLineData.h"
#include "Parameters.h"


class cChartBasicInfo
{
protected:
	GET_SET_DEC(Vector2,m_vShowPos,GetShowPos,SetShowPos);
	GET_SET_DEC(Vector2,m_vResolution,GetResolution,SetResolution);
	//
	void	RenderEdgeLines();
public:
	cChartBasicInfo();
	virtual ~cChartBasicInfo();
	float		GetHeightGapByPoints(int e_iPoint);
	float		GetWidthGapByPoints(int e_iPoint);
	float		GetHeightGapByPoints(float e_fPoint);
	float		GetWidthGapByPoints(float e_fPoint);
	Vector2		GetGapByPoints(POINT e_ResolutionPoints);
};

//
//a canvans has a time range(time range)
//it will show the tone data ID on the timeline
//the tond data ID is moving from right to left
//there is a hit point,while hit point is reached show the result
//


class cTimeLineRange
{
protected:
	float	m_fCurrentTime;
	//keep going for a while make it smooth
	GET_SET_DEC(float,m_fBeforeCurrentTimeViewRange,GetBeforeCurrentTimeViewRange,SetBeforeCurrentTimeViewRange);
	//before start need a empty gap.
	GET_SET_DEC(float,m_fAfterCurrentTimeViewRange,GetAfterCurrentTimeViewRange,SetAfterCurrentTimeViewRange);
	//
	//float	m_fStartTimne;
	//
	GET_SET_DEC(float,m_fEndTime,GetEndTime,SetEndTime);
	//
	void	SetCurrentTimeViewRange(float e_fBefore,float e_fAfter);
	Vector2	m_vCurrentTimeViewRange;
	//
public:
	cTimeLineRange(float e_fBeforeCurrentTimeViewRange = 2.f,float e_fAfterCurrentTimeViewRange = 10.f);
	virtual ~cTimeLineRange();
	virtual void	Init();
	virtual void	Update(float e_fElpaseTime);
	//void	Render();
	//void	DebugRender();
	//
	void	JumpTo(float e_fTime);
	//
	bool	IsEnd(bool e_bCalculateWithBeforeTime = true);
	//
	Vector2	GetCurrentTimeViewRange();
};



//class cSoundTimeLineDataCollection;

class cTimeLineRangeChart:public cChartBasicInfo,public cTimeLineRange,public cSoundTimeLineDataCollection
{
	cBaseImage*						m_pTickImage;
	//
	int								m_iLastToneDataObjectIndex;
	int								m_iCountInCompareTime;
	float							m_fEndTimeWithBeforTime;
	void							CollectToneDataByTimeRange();
	//
	virtual	bool					MyParse(TiXmlElement*e_pRoot);
	//
	cChartBasicInfo					m_SoundTimeLineDataTimeline;
	std::vector<Vector2>			m_SoundTimeLineDataObjectSmallTimelinePosVector;
	void							SetupSoundTimeLineDataObject(Vector2 e_vShowPos,Vector2 e_vResolution,float e_fTotalTime);
	Vector2							GetCurrenTimeControlPos(Vector2 e_vShowPos,Vector2 e_vResolution,float e_fCurrentTime,float e_fTotalTime);
	float							MousePositionToTime(int e_iMousePositionX,int e_iMousePositionY,Vector2 e_vShowPos,Vector2 e_vResolution,float e_fTotalTime);
	void							RenderTimeLineData(Vector2 e_vShowPos,Vector2 e_vResolution,float e_fCurrentTime,float e_fTotalTime);
	//
	eMoveDirection					m_eMoveDirection;
	void							RenderHorizontal();
	void							RenderVertical();
	void							RenderTimeLine();
public:
	cTimeLineRangeChart(float e_fBeforeCurrentTimeViewRange = cSoundCompareParameter::m_sfBeforeCurrentTimeViewRange,float e_fAfterCurrentTimeViewRange = cSoundCompareParameter::m_sfAfterCurrentTimeViewRange,eMoveDirection e_eMoveDirection = eMD_RIGHT_TO_LEFT);
	virtual ~cTimeLineRangeChart();
	//
	virtual void	Init();
	virtual void	Update(float e_fElpaseTime);
	virtual void	Render();
	virtual void	DebugRender();
	bool			Compare(float e_fElpaseTime,cQuickFFTDataFrequencyFinder*e_pQuickFFTDataFrequencyFinder);
	cBaseImage*		GetTimeControlImage();
	bool			TimeControlCollision(int e_iPosX,int e_iPosY);
	void			TimeControlMouseDown(int e_iPosX,int e_iPosY,cClickBehavior*e_pClickBehavior);
	void			TimeControlMouseMove(int e_iPosX,int e_iPosY,cClickBehavior*e_pClickBehavior);
	void			TimeControlMouseUp(int e_iPosX,int e_iPosY,cClickBehavior*e_pClickBehavior);
};
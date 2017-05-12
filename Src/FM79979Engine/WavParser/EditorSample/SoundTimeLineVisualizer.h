#pragma once

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
	cTimeLineRange(float e_fEndTime,float e_fBeforeCurrentTimeViewRange = 2.f,float e_fAfterCurrentTimeViewRange = 10.f);
	virtual ~cTimeLineRange();
	virtual void	Init();
	virtual void	Update(float e_fElpaseTime);
	//void	Render();
	//void	DebugRender();
	//
	void	JumpTo(float e_fTime);
	//
	bool	IsEnd();
	//
	Vector2	GetCurrentTimeViewRange();
};



class cSoundTimeLineDataCollection;

class cTimeLineRangeChart:public cChartBasicInfo,public cTimeLineRange
{
	cSoundTimeLineDataCollection*	m_pSoundTimeLineDataCollection;
	int								m_iLastToneDataObjectIndex;
	int								m_iCountInCompareTime;
	void							CollectToneDataByTimeRange();
public:
	cTimeLineRangeChart(cSoundTimeLineDataCollection*e_pSoundTimeLineDataCollection);
	~cTimeLineRangeChart();
	//
	virtual void	Init();
	virtual void	Update(float e_fElpaseTime);
	virtual void	Render();
};
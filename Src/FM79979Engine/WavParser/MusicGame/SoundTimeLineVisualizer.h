#pragma once

#include "SoundTimeLineData.h"
#include "Parameters.h"
#include "TimelineBasicInfo.h"


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
	void							DebugRenderTimeLineData(Vector2 e_vShowPos,Vector2 e_vResolution,float e_fCurrentTime,float e_fTotalTime);
	void							RenderTimeLineData(Vector2 e_vShowPos,Vector2 e_vResolution,float e_fCurrentTime,float e_fTotalTime);
	//
	eMoveDirection					m_eMoveDirection;
	void							RenderDebugHorizontal();
	void							RenderDebugVertical();
	void							RenderHorizontal();
	void							RenderVertical();
	cRenderObject*					m_pTimelineNoteRoot;
	cMPDI*							m_pTimeLineMPDI;
	cBaseImage*						m_WhiteKeyImage;
	cBaseImage*						m_BlackKeyImage;
	cBaseImage*						m_NoteInTimelineImage;
public:
	cTimeLineRangeChart(cMPDI*e_pTimeLineMPDI,float e_fBeforeCurrentTimeViewRange = cSoundCompareParameter::m_sfBeforeCurrentTimeViewRange,float e_fAfterCurrentTimeViewRange = cSoundCompareParameter::m_sfAfterCurrentTimeViewRange,eMoveDirection e_eMoveDirection = eMD_RIGHT_TO_LEFT);
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
#include "stdafx.h"
#include "Parameters.h"

float	cSoundCompareParameter::m_sfTolerateTime = 0.32f;


int		cSoundCompareParameter::m_siAmplitudeOffset = 30;
float	cSoundCompareParameter::m_sfBeforeCurrentTimeViewRange = 3.f;
float	cSoundCompareParameter::m_sfAfterCurrentTimeViewRange = 7.f;

Vector2	cSoundCompareParameter::m_vTimelineShowPos = Vector2(50,250);
//Vector2	cSoundCompareParameter::m_vTimelineResolution = Vector2(1800,300);
Vector2	cSoundCompareParameter::m_vTimelineResolution = Vector2(1800,800);

bool	cSoundCompareParameter::m_sbAutoPlaySoundForDebugTest = true;
//int		cSoundCompareParameter::m_siRecordFrequency = SOUND_CAPTURE_FREQUENCE/2;
int		cSoundCompareParameter::m_siRecordFrequency = SOUND_CAPTURE_FREQUENCE*2;
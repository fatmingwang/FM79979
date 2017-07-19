#include "stdafx.h"
#include "Parameters.h"

int				ONE_FRAME_NEED_NUM_FFT_DATA_COUNT = 10;

float			cSoundCompareParameter::m_sfCompareTuneTolerateTime = 0.3f;


int				cSoundCompareParameter::m_siAmplitudeOffset = 20;
float			cSoundCompareParameter::m_sfBeforeCurrentTimeViewRange = 3.f;
float			cSoundCompareParameter::m_sfAfterCurrentTimeViewRange = 7.f;

Vector2			cSoundCompareParameter::m_vTimelineShowPos = Vector2(50,250);
//Vector2	cSoundCompareParameter::m_vTimelineResolution = Vector2(1800,300);
Vector2			cSoundCompareParameter::m_vTimelineResolution = Vector2(1800,800);

bool			cSoundCompareParameter::m_sbAutoPlaySoundForDebugTest = true;
//intcSoundCompareParameter::m_siRecordFrequency = SOUND_CAPTURE_FREQUENCE/2;
//int				cSoundCompareParameter::m_siRecordFrequency = SOUND_CAPTURE_FREQUENCE*2;

int				cSoundCompareParameter::m_siDebugAmplitudeValue = 80;
int				cSoundCompareParameter::m_siFFTStoreThresholeValue = 15;

const int		cSoundCompareParameter::CAPTURE_FREQUENCY = 44100;
const int		cSoundCompareParameter::FFT_DATA_LINE_POINTS_COUNT = cSoundCompareParameter::CAPTURE_FREQUENCY/6*2;
const float		cSoundCompareParameter::m_sfDebugAlphaValue = 0.5f;

float			cSoundCompareParameter::m_sfAcceptableNoteMatchValue = 0.98f;


eMUSIC_GAME_MODE cSoundCompareParameter::m_seMusicGameMode = eMGM_GAME;
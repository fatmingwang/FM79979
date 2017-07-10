#pragma once

#define	PCM_SWAP_BUFFER_COUNT	20
#define	FFT_DATA_SWAP_BUFFER_COUNT	20
//sometimes I need to test so just make this big
//#define	FFT_DATA_LINE_POINTS_COUNT	44100/ONE_FRAME_NEED_NUM_FFT_DATA_COUNT*2//2 for lin need 2 points

//#define	FFT_DATA_LINE_POINTS_COUNT	44100/6*2//2 for lin need 2 points

//default we expect 60fps
extern int	ONE_FRAME_NEED_NUM_FFT_DATA_COUNT;

#define	WINDOWN_FUNCTION_FRUSTRUM	2

//FOR DEBUG vision
#define	AFTER_FILTER_MIN_VALUE	3

class cSoundCompareParameter
{
public:
	//time line
	static float		m_sfBeforeCurrentTimeViewRange;
	static float		m_sfAfterCurrentTimeViewRange;
	//
	static float		m_sfCompareTuneTolerateTime;
	//if the frequency amplitude close enought
	static int			m_siAmplitudeOffset;

	static Vector2		m_vTimelineShowPos;
	static Vector2		m_vTimelineResolution;

	static bool			m_sbAutoPlaySoundForDebugTest;
	static int			m_siRecordFrequency;

	static int			m_siDebugAmplitudeValue;
	static int			m_siFFTStoreThresholeValue;

	static const int	CAPTURE_FREQUENCY;	//44100
	static const int	FFT_DATA_LINE_POINTS_COUNT;//	44100/6*2//2 for lin need 2 points
};
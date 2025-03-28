#pragma once

#define	FREQUENCY_AND_DEIBELS_EXTENSION_FILE_NAME	"FreqDeci"

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
//assume sound input is 441000 and need 30 fps(iOS limit 30 fps)
//44100/30 = 1470,fft need power of two,most close 1470 is 2048
//and
//44100/21 = 2100
//44100/22 = 2004
//so fps 21 is the best
#define	BEST_RECORD_FPS 21

enum eMUSIC_GAME_MODE
{
	eMGM_EDITOR = 0,
	eMGM_GAME,
};

class cSoundCompareParameter
{
public:
	//time line
	static float					m_sfBeforeCurrentTimeViewRange;
	static float					m_sfAfterCurrentTimeViewRange;
	//
	static float					m_sfCompareTuneTolerateTime;
	//if the frequency amplitude close enought
	static int						m_siAmplitudeOffset;

	static Vector2					m_vTimelineShowPos;
	static Vector2					m_vTimelineResolution;

	static bool						m_sbAutoPlaySoundForDebugTest;
	static int						m_siRecordFrequency;

	static int						m_siDebugAmplitudeValue;
	static int						m_siFFTStoreThresholeValue;

	static const int				CAPTURE_FREQUENCY;	//44100
	static const int				FFT_DATA_LINE_POINTS_COUNT;//	44100/6*2//2 for lin need 2 points
	static eMUSIC_GAME_MODE			m_seMusicGameMode;
	static const float				m_sfDebugAlphaValue;
	static float					m_sfAcceptableNoteMatchValue;

	static float					m_sfDecibelsMatchDivideValue;
};
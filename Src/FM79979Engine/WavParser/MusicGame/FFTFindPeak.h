#pragma once


//class cFFTFindPeak
//{
//	//while increas and decrease are very close then it could be a peak
//	int			m_iNumIncrease;
//	int			m_iNumIncreaseSequence;
//	int64		m_i64TotalIncreaseValue;
//	int			m_iNumDecreaseSequence;
//	int64		m_i64TotalDecreaseValue;
//	int			m_iBiggestPeak;
//public:
//	cFFTFindPeak();
//	~cFFTFindPeak();
//};
#define	MOVE_DIRECTION_HISTORY_COUNT	5

#define	FFT_BIN_AMPLITUDE_MOVE_DIRECTION_MIN_ALLOW_CHANGE_VALUE	20

class cFFTBinStatus
{
public:
	//static int			m_siOffsetAmplitudeToPreventBounce;
	//
	struct sFFTBinData
	{
		//this one will be reset once move direction is change.

		int				iTempAmplitudeDiffForMoveDirection;
		int				iPriorAmplitude;
		int				iCurrentAmplitude;
		//thi sone will compare recent amplitude,once the big change is happen this one will change to the current amplitude.
		int				iLastAcceptableAmplitude;
		float			fLastAcceptableAmplitudeKeepTime;
		//
		float			fCurrentDirectionKeepTime;
		eMoveDirection	eCurrentMoveDirection;
		//assume fft fps is 25,5/25 = 0.2,0.2 second test to find out current is increase or decrease.
		//get from iCurrentAmplitude to get 
		int				iAmplitudeHistory[MOVE_DIRECTION_HISTORY_COUNT];
		bool			bFrequencyStrong;
		sFFTBinData();
		~sFFTBinData(){}
		eMoveDirection	GetMoveDirectionByMoveDirectionHistory(int e_iMoveDirectionHistoryIndex);
	};
	int					m_iNumFFTBin;
	sFFTBinData*		m_pFFTBinData;
	//a reference
	int*				m_pFFTSource;
	//
	int					m_iNumFrame;
public:
	cFFTBinStatus(int e_iNumFFTBins,int*e_pFFTSource);
	~cFFTBinStatus();
	void				Update(float e_fTime);
};
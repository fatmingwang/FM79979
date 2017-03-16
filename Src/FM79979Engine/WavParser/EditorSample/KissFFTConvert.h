#pragma once

//http://stackoverflow.com/questions/14536950/applying-kiss-fft-on-audio-samples-and-getting-nan-output
class cKissFFTConvert:public NamedTypedObject
{
	cOpanalWAV*							m_pTestSound;
	std::vector<std::vector<int>* >		m_FFTDataVectorChannelVector;
	int									m_iCurrentFFTDataLineCount;
	std::vector<std::vector<Vector2>*>	m_FFTDataLinePointVectorVector;
	//
	FATMING_CORE::cSoundFile*m_pSoundFile;
	float	m_fCurrentTime;
	bool	FetchSoundData(int e_iStartDataIndex,int e_iCount);
	bool	FetchSoundDataByTimeRange(float e_fStartTime,float e_fDuring = 0.16f);
	//time for estimate PreProcessedAllData
	int		m_iTargetFPS;
	UT::sTimeAndFPS	m_Timer;
	void	PreProcessedAllData();
	//ex:frequence is 44100 and fps is 60,count is 44100/60 = 735.
	int		m_iFPSDataCount;
	//count is m_i60FPSDataCount
	char*	m_pSampleDataArray;
	float	m_fScale;
	float	m_fXGap;
	float	m_fYGap;
	void	Destroy();
public:
	cKissFFTConvert();
	~cKissFFTConvert();
	//now only support wav
	bool	FetchSoundDataStart(const char*e_strFileName);
	//need another thread to do this?
	void	Update(float e_fElpaseTime);
	void	Render();
	//
	FATMING_CORE::cSoundFile*GetSoundFile(){return m_pSoundFile;}
	float	GetCurrentTimePercentage();
};
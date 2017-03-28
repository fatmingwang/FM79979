#pragma once
//default we expect 60fps
#define	ONE_FRAME_NEED_NUM_FFT_DATA_COUNT	60
#define	WINDOWN_FUNCTION_FRUSTRUM	2
class cKissFFTConvertBase:public NamedTypedObject
{
	virtual void	Destroy() = 0;
protected:
	UT::sTimeCounter	m_TimeToUpdateFFTData;
	//
	float	m_fCurrentTime;	
	//this value will dive data into parts,if value is frequence/m_iPerFrameFFTDataCount,value gigher performance higer but low accurance
	int		m_iDivideFFTDataToNFrame;
	//ex:frequence is 44100 and fps is 60,count is 44100/60 = 735.
	int		m_iNFrameFFTDataCount;
	//for chart
	Vector2	m_vChartResolution;
	Vector2	m_vChartShowPos;
	float	m_fScale;
	float	m_fNextChannelYGap;
	bool	m_bPause;
public:
	cKissFFTConvertBase();
	~cKissFFTConvertBase();
	//now only support wav
	virtual bool	FetchSoundDataStart(const char*e_strFileName){return false;}
	//need another thread to do this?
	virtual void	Update(float e_fElpaseTime) = 0;
	virtual void	Render() = 0;
	void			SetFFTDataUpdateTime(float e_fTime);
	void			SetChartResolution(Vector2 e_vResolution){m_vChartResolution = e_vResolution;}
	virtual float	GetCurrentTimePercentage(){ return 0.f; }
	virtual float	GetTimeLength(){ return -1.f; }
	virtual void	Pause(bool e_bPause){ m_bPause = e_bPause; }
	virtual void	GoToTime(float e_fElpaseTime) = 0;
};

//http://stackoverflow.com/questions/14536950/applying-kiss-fft-on-audio-samples-and-getting-nan-output
class cKissFFTConvert:public cKissFFTConvertBase
{
	cOpanalWAV*							m_pTestSound;
	std::vector<std::vector<int>* >		m_FFTDataVectorChannelVector;
	int									m_iCurrentFFTDataLineCount;
	std::vector<std::vector<Vector2>*>	m_FFTDataLinePointVectorVector;
	//
	FATMING_CORE::cSoundFile*m_pSoundFile;
	bool	FetchSoundData(int e_iStartDataIndex,int e_iCount);
	bool	FetchSoundDataByTimeRange(float e_fStartTime,float e_fDuring = 0.16f);
	//time for estimate PreProcessedAllData
	UT::sTimeAndFPS	m_Timer;
	void	PreProcessedAllData();
	//
	virtual void	Destroy();
public:
	cKissFFTConvert();
	~cKissFFTConvert();
	//now only support wav
	virtual bool	FetchSoundDataStart(const char*e_strFileName);
	//need another thread to do this?
	virtual void	Update(float e_fElpaseTime);
	virtual void	Render();
	//
	FATMING_CORE::cSoundFile*GetSoundFile(){return m_pSoundFile;}
	virtual float	GetCurrentTimePercentage();
	virtual float	GetTimeLength();
	virtual void	GoToTime(float e_fElpaseTime);
};
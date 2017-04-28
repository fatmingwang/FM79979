#pragma once
//default we expect 60fps
#define	ONE_FRAME_NEED_NUM_FFT_DATA_COUNT	6
#define	WINDOWN_FUNCTION_FRUSTRUM	2

int DoFilter(float e_fFilterEndScaleValue,int e_iTransformLength,int e_iStartArrayIndex,int*e_pFFTDataSrc,kiss_fft_cpx*e_pKiss_FFT_Out,int e_iFilterStrengthValue);

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
	//
	GET_SET_DEC(int,m_iMaxAmplitudeFrequence,GetMaxAmplitudeFrequence,SetMaxAmplitudeFrequence);
	//
	GET_SET_DEC(bool,m_bFilter,IsFilter,SetFilter);
	//https://www.toptal.com/algorithms/shazam-it-music-processing-fingerprinting-and-recognition
	//However, in one song the range of strong frequencies might vary between low C - C1 (32.70 Hz) and high C - C8 (4,186.01 Hz
	GET_SET_DEC(float,m_fFrenquenceFilterEndScaleValue,GetFrenquenceFilterEndScaleValue,SetiFrenquenceFilterEndScaleValue);
	GET_SET_DEC(int,m_iFilterStrengthValue,GetFilterStrengthValue,SetFilterStrengthValue);
	GET_SET_DEC(float,m_fChartScale,GetChartScale,SetChartScale);
public:
	DEFINE_TYPE_INFO();
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
	virtual void	SetFFTSampleScale(float e_fScale,bool e_bForceSet = false){}
	//http://stackoverflow.com/questions/7674877/how-to-get-frequency-from-fft-result
	int				GetCurrentMaxFrequence(int e_iIndexOfFFTData,int e_iFrequence,int e_iCount);//freq = max_index * Fs(1 second how many sample) / N(fft sample count)
	static float	GetFrequencyGapByFPS(int e_iFrequency,int e_iFPS);
	TiXmlElement*	ToTiXmlElement();
	void			SetDataFromTiXmlElement(TiXmlElement*e_pTiXmlElement);
};

//http://stackoverflow.com/questions/14536950/applying-kiss-fft-on-audio-samples-and-getting-nan-output
class cKissFFTConvert:public cKissFFTConvertBase
{
	cOpanalWAV*							m_pTestSound;
	std::vector<std::vector<int>* >		m_FFTDataVectorChannelVector;
		//it won't bigger than this...I guess
	std::vector<std::vector<float>* >	m_FFTResultPhaseVector;
	int									m_iCurrentFFTDataLineCount;
	std::vector<std::vector<Vector2>*>	m_FFTDataLinePointVectorVector;
	//
	FATMING_CORE::cSoundFile*m_pSoundFile;
	bool	FetchSoundData(int e_iStartDataIndex,int e_iCount);
	bool	FetchSoundDataByTimeRange(float e_fStartTime,float e_fDuring = 0.16f);
	//time for estimate PreProcessedAllData
	UT::sTimeAndFPS	m_Timer;
	void	PreProcessedAllData(bool e_bFilter);
	//
	virtual void	Destroy();
public:
	cKissFFTConvert();
	~cKissFFTConvert();
	//now only support wav
	virtual bool	FetchSoundDataStart(const char*e_strFileName,bool e_bPlaySound = true);
	//need another thread to do this?
	virtual void	Update(float e_fElpaseTime);
	virtual void	Render();
	//
	FATMING_CORE::cSoundFile*GetSoundFile(){return m_pSoundFile;}
	virtual float	GetCurrentTimePercentage();
	virtual float	GetTimeLength();
	virtual void	GoToTime(float e_fElpaseTime);

	std::vector<std::vector<int>* >*GetFFTDataVectorChannelVector(){return &m_FFTDataVectorChannelVector;}
};
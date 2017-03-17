#pragma once

class cKissFFTConvertBase:public NamedTypedObject
{
	virtual void	Destroy() = 0;
protected:
	float	m_fCurrentTime;
	int		m_iWaveUpdateIndex;
	int		m_iTargetFPS;
public:
	cKissFFTConvertBase();
	~cKissFFTConvertBase();
	//now only support wav
	virtual bool	FetchSoundDataStart(const char*e_strFileName) = 0;
	//need another thread to do this?
	virtual void	Update(float e_fElpaseTime) = 0;
	virtual void	Render() = 0;
	void			SetWaveUpdateIndex(int e_iWaveUpdateIndex){ m_iWaveUpdateIndex = e_iWaveUpdateIndex; }
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
	//ex:frequence is 44100 and fps is 60,count is 44100/60 = 735.
	int		m_iFPSDataCount;
	//for chart
	float	m_fScale;
	float	m_fXGap;
	float	m_fYGap;
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
	float	GetCurrentTimePercentage();
};
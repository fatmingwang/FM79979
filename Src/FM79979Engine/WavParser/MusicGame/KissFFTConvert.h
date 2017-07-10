#pragma once

#include "../kiss_fft130/kiss_fft.h"
//#include "SwapBuffer.h"


class cKissFFTConvertBase:public NamedTypedObject
{
	virtual void	Destroy() = 0;
protected:
	int					m_iFreq;
	std::string			m_strSourceFileName;
	//
	UT::sTimeCounter	m_TimeToUpdateFFTData;
	//
	float				m_fCurrentTime;
	//this value will dive data into parts,if value is frequence/m_iPerFrameFFTDataCount,value gigher performance higer but low accurance
	GET_SET_DEC(int,m_iDivideFFTDataToNFrame,GetDivideFFTDataToNFrame,SetDivideFFTDataToNFrame);
	//ex:frequence is 44100 and fps is 60,count is 44100/60 = 735.
	int					m_iOneFrameFFTDataCount;
	//for chart
	Vector2				m_vChartResolution;
	Vector2				m_vChartShowPos;
	float				m_fChartAmplitudeScale;
	float				m_fNextChannelYGap;
	bool				m_bPause;
	//
	GET_SET_DEC(int,m_iMaxAmplitudeFrequence,GetMaxAmplitudeFrequence,SetMaxAmplitudeFrequence);
	GET_SET_DEC(int,m_iMaxAmplitude,GetMaxAmplitude,SetMaxAmplitude);
	//
	GET_SET_DEC(bool,m_bFilter,IsFilter,SetFilter);
	//https://www.toptal.com/algorithms/shazam-it-music-processing-fingerprinting-and-recognition
	//However, in one song the range of strong frequencies might vary between low C - C1 (32.70 Hz) and high C - C8 (4,186.01 Hz
	GET_SET_DEC(float,m_fFrenquenceFilterEndScaleValue,GetFrenquenceFilterEndScaleValue,SetiFrenquenceFilterEndScaleValue);
	GET_SET_DEC(int,m_iFilterStrengthValue,GetFilterStrengthValue,SetFilterStrengthValue);
	GET_SET_DEC(float,m_fChartScale,GetChartScale,SetChartScale);
	//
	void			SetOneFrameFFTDataCount(int e_iFrequency);
	void			RenderDebugAmplitudeLine(float e_fAmplitude);
	cFFTDataStore	m_FFTDataStore;
public:
	DEFINE_TYPE_INFO();
	cKissFFTConvertBase();
	virtual ~cKissFFTConvertBase();
	//
	const int		GetOneFrameFFTDataCount(){return m_iOneFrameFFTDataCount;}
	//now only support wav
	virtual bool	FetchSoundDataStart(const char*e_strFileName,bool e_bPlaySound = true,bool e_bDoFFTDataStore = false){return false;}
	//need another thread to do this?
	virtual void	Update(float e_fElpaseTime) = 0;
	virtual void	Render() = 0;
	void			SetFFTDataUpdateTime(float e_fTime);
	void			SetChartResolution(Vector2 e_vResolution){m_vChartResolution = e_vResolution;}
	void			SetChartShowPos(Vector2 e_vChartShowPos){m_vChartShowPos = e_vChartShowPos;}
	virtual float	GetCurrentTimePercentage(){ return 0.f; }
	virtual float	GetTimeLength(){ return -1.f; }
	virtual void	Pause(bool e_bPause){ m_bPause = e_bPause; }
	virtual void	GoToTime(float e_fElpaseTime) = 0;
	virtual void	SetFFTSampleScale(float e_fScale,bool e_bForceSet = false){}
	//http://stackoverflow.com/questions/7674877/how-to-get-frequency-from-fft-result
	int				GetCurrentMaxFrequence(int e_iIndexOfFFTData,int e_iFrequence,int e_iCount,int e_iMaxAmplitude);//freq = max_index * Fs(1 second how many sample) / N(fft sample count)
	static float	GetFrequencyGapByFPS(int e_iFrequency,int e_iFPS);
	TiXmlElement*	ToTiXmlElement();
	void			SetDataFromTiXmlElement(TiXmlElement*e_pTiXmlElement);
	void			RenderMaxAmplitudeAndFrequencyInfo(int e_iPosX,int e_iPosY);
	cFFTDataStore*	GetFFTDataStore(){return &m_FFTDataStore;}

	std::string		GetSourceFileName(){return m_strSourceFileName;}
	void			SetCurrentTime(float e_fTime){ m_fCurrentTime = e_fTime; }
	virtual void	DumpDebugInfo(int e_iDeciblesThreshold,const char*e_strFileName);
};

//http://stackoverflow.com/questions/14536950/applying-kiss-fft-on-audio-samples-and-getting-nan-output
class cKissFFTConvert:public cKissFFTConvertBase
{
	std::vector<float>					m_StartCaptureTimeVector;
	void								RenderDecibels(int e_iNumSampleCount,float*e_pfDeciblesData,Vector2 e_vShowPos,Vector2 e_vResolution);
	int									m_iNumFFTGraph;
	float*								m_pfDecibles;
	float								m_fMaxDecible;
	cOpanalWAV*							m_pTestSound;
	std::vector<int>					m_FFTDataVector;
	//it won't bigger than this...I guess
	//std::vector<std::vector<float>* >	m_FFTResultPhaseVector;
	int									m_iCurrentFFTDataLineCount;
	std::vector<Vector2>				m_FFTDataLinePointVector;
	//
	FATMING_CORE::cSoundFile*m_pSoundFile;
	bool	FetchSoundData(int e_iStartDataIndex,int e_iCount);
	bool	FetchSoundDataByTimeRange(float e_fStartTime,float e_fDuring = 0.16f);
	int		GetStartFFTIndexByTime(float e_fTime);
	//time for estimate PreProcessedAllData
	UT::sTimeAndFPS	m_Timer;
	void	PreProcessedAllData(bool e_bFilter,cFFTDataStore*e_pFFTDataStore = nullptr);
	//void	PreProcessedDoubleAllData(bool e_bFilter,cFFTDataStore*e_pFFTDataStore);
	//
	virtual void	Destroy();
public:
	cKissFFTConvert();
	virtual ~cKissFFTConvert();
	//now only support wav
	virtual bool	FetchSoundDataStart(const char*e_strFileName,bool e_bPlaySound = true,bool e_bDoFFTDataStore = false);
	//need another thread to do this?
	virtual void	Update(float e_fElpaseTime);
	virtual void	Render();
	//
	FATMING_CORE::cSoundFile*GetSoundFile(){return m_pSoundFile;}
	virtual float	GetCurrentTimePercentage();
	virtual float	GetTimeLength();
	virtual void	GoToTime(float e_fElpaseTime);
	void			Play();
	int				GetNumFFTGraphByDuration(float e_fDuration);
	//bool			ExportFFTDataStore(const char*e_strFileName,const char*e_strExportFileName);
	std::vector<int>*GetFFTDataVector(){return &m_FFTDataVector;}
};
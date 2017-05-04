#pragma once


#define	TOLERATE_TIME	0.016f
#define	MIN_LEEP_TIME	0.016f
//the filter min value is 3
#define	MIN_AMPLITUDE	20

class cKissFFTConvert;
//for spectrum to get higest value and get its neighbor then find average.
struct sFrequencyAndAmplitude
{
	float	fFrequency;
	int		iAmplitude;
};

//if frequency amplitude is bigger than X,store it and keep record while the value is smaller than something
struct sFrequenceAndAmplitudeAndTime:public sFrequencyAndAmplitude
{
	sFrequenceAndAmplitudeAndTime(TiXmlElement*e_pElement);
	sFrequenceAndAmplitudeAndTime(){}
	~sFrequenceAndAmplitudeAndTime(){}
	float	fStartTime;
	float	fKeepTime;
	//the last time frequency and amplitude is matched
	float	fLastMatchTime;
	TiXmlElement*ToElement();
};

//struct sFrequenceAndTimeCompare:public sFrequenceAndTime
//{
//	//how many much
//	int		iNumMatch;
//};

class cTimeFrequencyAmplitudeValueCapture
{
private:
	//the data is during the working range
	std::vector<sFrequenceAndAmplitudeAndTime>	m_CurrentWorkingFrequenceAndTimeVector;
	//final write to file,this should sort by time.
	std::vector<sFrequenceAndAmplitudeAndTime>	m_AllData;
	//
	cKissFFTConvert*							m_pKissFFTConvert;
	int*										m_piSoundDataForParse;
	//
	bool										AnalyizeStart(const char*e_strFileName,int e_iFilterStregth,float e_fFilterEndFrequencyValue);
	void										FrameByFrameAnaylize(float e_fFreqDistance,const int*e_pData,int e_iDataLength,float e_fElpaseTime);
	void										FromCurrentWorkingToAllData();
	std::vector<sFrequencyAndAmplitude>			GetSatisfiedFrequencyAndAmplitudeVector(float e_fFreqDistance,const int*e_pData,int e_iDataLength,int e_AmplitudeCondition);
	//
	TiXmlElement*	SaveConditionToTiXmlElement();
	//
public:
	int		m_iParseFPS;//default is 30
	float	m_fCurrentTime;
	//frequencty change could have a range offset,assume default is CD quality
	float	m_fFrequencyOffsetRange;
	//some times sample rate is possible not enough or missing some frequency
	float	m_fTolerateTime;
	//
	float	m_fMinKeepTime;
	//
	int		m_iMinAmplitude;
	//
public:
	cTimeFrequencyAmplitudeValueCapture();
	~cTimeFrequencyAmplitudeValueCapture();
	//
	bool	ParseAndSaveFileName(const char*e_strParseFileName,int e_iFilterStregth,float e_fFilterEndFrequencyValue,const char*e_strOutputFileName);
};
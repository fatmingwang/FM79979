#pragma once


#define	TOLERATE_TIME	0.016f
#define	MIN_LEEP_TIME	0.016f
#define	MIN_AMPLITUDE	20

class cKissFFTConvert;
//for spectrum to get higest value and get its neighbor then find average.


//if frequency amplitude is bigger than X,store it and keep record while the value is smaller than something
struct sFrequenceAndTime
{
	float	fFrequency;
	int		iAmplitude;
	float	fKeepTime;
};

struct sFrequencyAndAmplitude
{
	float	fFrequency;
	int		iAmplitude;
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
	std::vector<sFrequenceAndTime>	m_CurrentWorkingFrequenceAndTimeVector;
	//final write to file
	std::vector<sFrequenceAndTime>	m_AllData;
	cKissFFTConvert*	m_pKissFFTConvert;
	int*				m_piSoundDataForParse;
	//
	bool	AnalyizeStart(const char*e_strFileName);
	void	FrameByFrameAnaylize(float e_fFreqDistance,const int*e_pData,int e_iDataLength,float e_fElpaseTime);
	std::vector<sFrequencyAndAmplitude>	GetSatisfiedFrequencyAndAmplitudeVector(float e_fFreqDistance,const int*e_pData,int e_iDataLength,int e_AmplitudeCondition);
	//
	//
	int		m_iParseFPS;//default is 30
	float	m_fCurrentTime;
	//frequencty change could have a range offset,assume default is CD quality
	float	m_fOffsetRange;
	//some times sample rate is possible not enough or missing some frequency
	float	m_fTolerateTime;
	//
	float	m_fMinKeepTime;
	//
	int		m_iMinAmplitude;
	//
	bool	SaveFile(const char*e_strFileName);
	//
public:
	cTimeFrequencyAmplitudeValueCapture();
	~cTimeFrequencyAmplitudeValueCapture();
	//
	bool	ParseAndSaveFileName(const char*e_strParseFileName,const char*e_strOutputFileName);
};
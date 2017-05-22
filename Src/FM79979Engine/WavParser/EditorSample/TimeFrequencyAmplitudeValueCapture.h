#pragma once


//#define	TOLERATE_TIME	0.016f

#define	SOUD_SOURCE_FILE_NAME "ParseFileName"


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
	sFrequenceAndAmplitudeAndTime(sFrequenceAndAmplitudeAndTime*e_pFrequenceAndAmplitudeAndTime);
	sFrequenceAndAmplitudeAndTime(){bMatched=false;}
	~sFrequenceAndAmplitudeAndTime(){}
	float	fStartTime;
	float	fKeepTime;
	//the last time frequency and amplitude is matched
	float	fLastMatchTime;
	bool	bMatched;
	TiXmlElement*ToElement();
};

//struct sFrequenceAndTimeCompare:public sFrequenceAndTime
//{
//	//how many much
//	int		iNumMatch;
//};

//export soundFFT data
//format like below
//<Root ParseFileName="Sound\21.wav">
//    <TimeFrequencyAmplitudeValueCapture ParseFPS="25" FrequencyOffsetRange="12.50000" TolerateTime="0.03200" MinKeepTime="0.70000" MinAmplitude="30" />
//    <FrequenceAndAmplitudeAndTime StartTime="0.00000" KeepTime="0.00000" LastMatchTime="0.00000" Frequency="322.99805" Amplitude="122" />
//    <FrequenceAndAmplitudeAndTime StartTime="0.00000" KeepTime="0.00000" LastMatchTime="0.00000" Frequency="344.53125" Amplitude="126" />
//</Root>
//
//

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
	void										FrameByFrameAnaylize(const int*e_pData,int e_iDataLength,float e_fElpaseTime);
	void										FromCurrentWorkingToAllData();
	std::vector<sFrequencyAndAmplitude>			GetSatisfiedFrequencyAndAmplitudeVector(const int*e_pData,int e_iDataLength,int e_AmplitudeCondition);
	//
	TiXmlElement*	SaveConditionToTiXmlElement();
	//
public:
	int		m_iParseFPS;//default is 30
	float	m_fCurrentTime;
	//frequencty change could have a range offset,assume default is CD quality
	//float	m_fFrequencyOffsetRange;
	//some times sample rate is possible not enough or missing some frequency
	//float	m_fTolerateTime;
	//
	float	m_fCaptureSoundRequireMinTime;
	//
	int		m_iMinAmplitude;
	//to strip some frequency is not need
	int		m_iMinAllowFrequency;
	int		m_iMaxAllowFrequency;
	//
public:
	cTimeFrequencyAmplitudeValueCapture();
	~cTimeFrequencyAmplitudeValueCapture();
	//the output extension file name should be soundFFT
	bool	ParseAndSaveFileName(const char*e_strParseFileName,int e_iFilterStregth,float e_fFilterEndFrequencyValue,const char*e_strOutputFileName);
};
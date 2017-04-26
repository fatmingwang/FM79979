#pragma once

//for spectrum to get higest value and get its neighbor then find average.


//if frequency amplitude is bigger than X,store it and keep record while the value is smaller than something
struct sFrequenceAndTime
{
	int		iFrequency;
	int		iOffsetRange;
	float	fKeepTime;
};

class cFrequenceAndTimeAnalyizer
{
	float	m_fMinKeepTime;
	int		m_iMinAmplitude;
	void	Analyize(const short*e_pData,int e_iDataLength,float e_fTime);
public:
	cFrequenceAndTimeAnalyizer();
	~cFrequenceAndTimeAnalyizer();
	//
	void	FrameByFrameAnaylize(float e_fFreqDistance,const short*e_pData,int e_iDataLength,float e_fElpaseTime);
};

class cFrequenceAndTimeCapture
{
	bool	SaveFile(const char*e_strFileName);
public:
	void	ParseAndSaveFileName();
};
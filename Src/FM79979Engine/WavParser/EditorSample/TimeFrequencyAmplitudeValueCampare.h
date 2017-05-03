#pragma once

#include "TimeFrequencyAmplitudeValueCapture.h"
#include "SoundFFTCapture.h"

//template<class T> struct sCountAndData
//{
//	int	iNum;
//	T*	pData;
//	T*operator [](int e_iIndex)
//	{
//		if( iNum > e_iIndex )
//		{
//			return &pData[e_iIndex];
//		}
//		return nullptr;
//	}
//	sCountAndData(){iNum = 0; pData = nullptr;}
//	~sCountAndData(){ SAFE_DELETE(pData); }
//};

//typedef sCountAndData<sCountAndData<sFrequenceAndAmplitudeAndTime> >  sFrequenceAndAmplitudeAndTimeData;
//typedef sCountAndData<std::vector<sFrequenceAndAmplitudeAndTime> >  sFrequenceAndAmplitudeAndTimeData;
typedef std::vector<std::vector<sFrequenceAndAmplitudeAndTime*>* >  sFrequenceAndAmplitudeAndTimeData;


struct sFrequenceAndAmplitudeAndTimeFinder
{
	sFrequenceAndAmplitudeAndTimeFinder(TiXmlElement*e_pTiXmlElement);
	sFrequenceAndAmplitudeAndTimeFinder();
	~sFrequenceAndAmplitudeAndTimeFinder();
	//
	sFrequenceAndAmplitudeAndTimeData	OneScondFrequenceAndAmplitudeAndTimeData;
	bool	GetDataByTime(float e_fTime,float e_fTolerateTime,std::vector<sFrequenceAndAmplitudeAndTime*>*e_pOutVector,bool e_bIgnodeSameObject);
};

struct sFrequencyAndAmplitudeWithFFTBinIndex:public sFrequenceAndAmplitudeAndTime
{
	//a quick index to find out amplitude value,because frequency could be different so make a captable offset
	int	iFFTBinIndex[2];
};

class cTimeFrequencyAmplitudeValueCompare:public cNodeISAX
{
	cSoundFFTCapture*					m_pSoundFFTCapture;
	//
	sFrequenceAndAmplitudeAndTimeFinder*m_pFrequenceAndAmplitudeAndTimeFinder;
	//
	float m_fCurrentTime;
	//
	virtual	bool	MyParse(TiXmlElement*e_pRoot);
	//once time is over due erase data.
	std::vector<sFrequencyAndAmplitudeWithFFTBinIndex>	WaitForCompareData;
	//

	void	SetupInputDataFFT(int e_iNumBins);
public:
	cTimeFrequencyAmplitudeValueCompare();
	~cTimeFrequencyAmplitudeValueCompare();

	void	Update(float e_fElpaseTime);
	void	AddNewData(sFrequencyAndAmplitude e_FrequencyAndAmplitude);
};
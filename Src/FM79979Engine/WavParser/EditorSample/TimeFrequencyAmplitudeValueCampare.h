#pragma once

#include "TimeFrequencyAmplitudeValueCapture.h"
#include "SoundFFTCapture.h"
#include "SingleSoundCompare.h"

//2way to compare sound
//1:parse a sound file capture all sound data to a list
//2:make instrument each sound as a file,record all the sounds and make a database

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


//this one is 1.
//I prefer method 2.
//
class cTimeFrequencyAmplitudeValueCompare:public cNodeISAX
{
	cSoundFFTCapture*									m_pSoundFFTCapture;
	cSoundCapture*										m_pSoundCapture;
	//
	sFrequenceAndAmplitudeAndTimeFinder*				m_pFrequenceAndAmplitudeAndTimeFinder;
	//
	float												m_fCurrentTime;
	//
	virtual	bool										MyParse(TiXmlElement*e_pRoot);
	void												SetInternalData(TiXmlElement*e_pRoot);
	//player always have a delay or play before tune comes.
	//so we have to get a range data to make sure play right.
	float												m_fPreLoadTimeOffset;
	bool												m_bFirstTimePreLoad;
	//once time is over due erase data.
	//std::vector<sFrequencyAndAmplitudeWithFFTBinIndex>	WaitForCompareData;
	void												FetchPreBuildData(float e_fCurrentTime);
	//time warping...
	void												SetCurrentTime(float e_fTargetTime);
	//
	void												SetupInputDataFFT(int e_iNumBins);
	//
public:
	cTimeFrequencyAmplitudeValueCompare(float e_fPreLoadTimeOffset,const char*e_strFileName);
	virtual ~cTimeFrequencyAmplitudeValueCompare();

	void	StartCompare();
	void	Update(float e_fElpaseTime);
	void	AddNewData(sFrequencyAndAmplitude e_FrequencyAndAmplitude);
};
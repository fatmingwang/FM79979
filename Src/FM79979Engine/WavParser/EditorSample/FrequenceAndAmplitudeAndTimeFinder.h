#pragma once

#include "TimeFrequencyAmplitudeValueCapture.h"
#include "QuickFFTDataFrequencyFinder.h"

//typedef sCountAndData<sCountAndData<sFrequenceAndAmplitudeAndTime> >  sFrequenceAndAmplitudeAndTimeData;
//typedef sCountAndData<std::vector<sFrequenceAndAmplitudeAndTime> >  sFrequenceAndAmplitudeAndTimeData;
typedef std::vector<std::vector<sFrequenceAndAmplitudeAndTime*>* >  sFrequenceAndAmplitudeAndTimeData;

//data from soundFFT
struct sFrequenceAndAmplitudeAndTimeFinder
{
	sFrequenceAndAmplitudeAndTimeFinder(TiXmlElement*e_pTiXmlElement);
	sFrequenceAndAmplitudeAndTimeFinder();
	~sFrequenceAndAmplitudeAndTimeFinder();
	////why need this one? how about just a reference?
	//sFrequenceAndAmplitudeAndTimeFinder(sFrequenceAndAmplitudeAndTimeFinder*e_pFrequenceAndAmplitudeAndTimeFinder);
	//sFrequenceAndAmplitudeAndTimeFinder*Clone();
	//
	sFrequenceAndAmplitudeAndTimeData	OneScondFrequenceAndAmplitudeAndTimeData;
	//
	std::vector<int>					TimeGapVector;
	//
	bool	GetDataByTime(float e_fTime,float e_fTolerateTime,std::vector<sFrequenceAndAmplitudeAndTime*>*e_pOutVector,bool e_bIgnodeSameObject);
};
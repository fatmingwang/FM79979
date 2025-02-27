#pragma once

#include "TimeFrequencyAmplitudeValueCapture.h"
#include "FFTFrequency.h"

//typedef sCountAndData<sCountAndData<sFrequenceAndAmplitudeAndTime> >  sFrequenceAndAmplitudeAndTimeData;
//typedef sCountAndData<std::vector<sFrequenceAndAmplitudeAndTime> >  sFrequenceAndAmplitudeAndTimeData;
typedef std::vector<std::vector<sFrequenceAndAmplitudeAndTime*>* >  sFrequenceAndAmplitudeAndTimeData;

//data from soundFFT
struct sFindTimeDomainFrequenceAndAmplitude
{
	//return total seconds
	int		SetupTotalSecond(TiXmlElement*e_pTiXmlElement);
	bool	GenerateFrequenceAndAmplitudeAndTime(TiXmlElement*e_pTiXmlElement,int e_iCurrentTimeInSecond);
	//
	sFindTimeDomainFrequenceAndAmplitude(const char*e_strFileName);
	//sFindTimeDomainFrequenceAndAmplitude(TiXmlElement*e_pTiXmlElement);
	sFindTimeDomainFrequenceAndAmplitude();
	sFindTimeDomainFrequenceAndAmplitude(const sFindTimeDomainFrequenceAndAmplitude*e_pFindTimeDomainFrequenceAndAmplitude);
	~sFindTimeDomainFrequenceAndAmplitude();
	////why need this one? how about just a reference?
	//sFindTimeDomainFrequenceAndAmplitude(sFindTimeDomainFrequenceAndAmplitude*e_pFrequenceAndAmplitudeAndTimeFinder);
	//sFindTimeDomainFrequenceAndAmplitude*Clone();
	//
	sFrequenceAndAmplitudeAndTimeData	OneScondFrequenceAndAmplitudeAndTimeData;
	//
	bool	GetDataByTime(float e_fTime,float e_fTolerateTime,std::vector<sFrequenceAndAmplitudeAndTime*>*e_pOutVector,bool e_bIgnodeSameObject);
};


class cNoteFrequencyAndDecibles:public NamedTypedObject
{
	bool ProcessData(TiXmlElement*e_pFFTHitCountAndTimeElement);
public:
	int								m_iFFTcount;
	int								m_iFrequency;
	std::vector<int>				FrequencyHittedCountVector;
	std::vector<int>				FrequencyVector;
	std::vector<int>				FrequencyBinIndexVector;
	std::vector<int>				FrequencyHittedValueVector;
	cNoteFrequencyAndDecibles(const char*e_strFileName);
	~cNoteFrequencyAndDecibles();
};
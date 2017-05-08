#pragma once

#include "TimeFrequencyAmplitudeValueCapture.h"
#include "QuickFFTDataFrequencyFinder.h"

class cSoundCompareParameter
{
public:
	static float	m_sfTolerateTime;
	//if the frequency amplitude close enought
	static int		m_siAmplitudeOffset;
};

//typedef sCountAndData<sCountAndData<sFrequenceAndAmplitudeAndTime> >  sFrequenceAndAmplitudeAndTimeData;
//typedef sCountAndData<std::vector<sFrequenceAndAmplitudeAndTime> >  sFrequenceAndAmplitudeAndTimeData;
typedef std::vector<std::vector<sFrequenceAndAmplitudeAndTime*>* >  sFrequenceAndAmplitudeAndTimeData;


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


//idea is one compare object has a reference sound data sFrequenceAndAmplitudeAndTimeFinder,
//we have current matched index,and go through all steps(m_pFrequenceAndAmplitudeAndTimeFinder->OneScondFrequenceAndAmplitudeAndTimeData),
//but the problem is what if its a sequence sound and the ime offset is small this will has some problem
//so ensure the sequency sound has a flag must match after proior
//
//here dont care about time only care about the frequency is matched by order
class cSindSoundCompare
{
	//this just a reference.
	sFrequenceAndAmplitudeAndTimeFinder*m_pFrequenceAndAmplitudeAndTimeFinder;
	std::vector<int>					m_MatchIndexOrderVector;
	//
	int									m_iCurrentMatchedIndex;
	int									m_iNumMatched;
	float								GetSimilarityResultScore();
	//float								m_fResultScore;
	//for the sound happen in the rythem time line.
	float								m_fCompareTime;
	//avoid same sound is sequence.
	GET_SET_DEC(bool,m_bMustMatchAfterProior,IsMustMatchAfterProior,SetMustMatchAfterProior);
	//time is over?
	bool								IsFinish(float e_fCurrentTime);
public:
	cSindSoundCompare(sFrequenceAndAmplitudeAndTimeFinder*e_pData,float e_fCompareTime);
	~cSindSoundCompare();
	//if finish return true
	bool		Compare(float e_fCurrentTime,cQuickFFTDataFrequencyFinder*e_pQuickFFTDataFrequencyFinder);
};

struct sFrequencyAndAmplitudeWithFFTBinIndex:public sFrequenceAndAmplitudeAndTime
{
	//a quick index to find out amplitude value,because frequency could be different so make a captable offset
	int	iFFTBinIndex[2];
};
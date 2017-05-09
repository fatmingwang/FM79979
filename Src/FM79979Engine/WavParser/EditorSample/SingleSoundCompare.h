#pragma once

#include "QuickFFTDataFrequencyFinder.h"
//#include "FrequenceAndAmplitudeAndTimeFinder.h"
//#include "ToneData.h"

//class cToneData;
struct sFrequenceAndAmplitudeAndTimeFinder;

class cSoundCompareParameter
{
public:
	static float	m_sfTolerateTime;
	//if the frequency amplitude close enought
	static int		m_siAmplitudeOffset;
};

//idea is one compare object has a reference sound data sFrequenceAndAmplitudeAndTimeFinder,
//we have current matched index,and go through all steps(m_pFrequenceAndAmplitudeAndTimeFinder->OneScondFrequenceAndAmplitudeAndTimeData),
//but the problem is what if its a sequence sound and the ime offset is small this will has some problem
//so ensure the sequency sound has a flag must match after proior
//
//here dont care about time only care about the frequency is matched by order
class cSindSoundCompare:public NamedTypedObject
{
	//this just a reference.
	sFrequenceAndAmplitudeAndTimeFinder*m_pFrequenceAndAmplitudeAndTimeFinder;
	std::vector<int>					m_MatchIndexOrderVector;
	//
	int									m_iCurrentMatchedIndex;
	int									m_iNumMatched;
	float								GetSimilarityResultScore();
	//get how many percent matched
	float								m_fResultScore;
	//the frequency is quite close but not same...so hard to do this.
	//float								m_fErrorScore;
	//for the sound happen in the rythem time line.
	float								m_fCompareTime;
	//avoid same sound is sequence.
	GET_SET_DEC(bool,m_bMustMatchAfterProior,IsMustMatchAfterProior,SetMustMatchAfterProior);
	//time is over?
	bool								IsFinish(float e_fCurrentTime);
public:
	DEFINE_TYPE_INFO();
	cSindSoundCompare(sFrequenceAndAmplitudeAndTimeFinder*e_pData,float e_fCompareTime);
	virtual ~cSindSoundCompare();
	//if finish return true
	bool		Compare(float e_fCurrentTime,cQuickFFTDataFrequencyFinder*e_pQuickFFTDataFrequencyFinder);
};
//<cSoundTimeLineCollection MusicInstrumentName="Quitar">
//		<cSindSoundCompare ID="" SoundFilePath="" FFTSequenceFileName="" />
//		...
//		...
//		...
//		<cSindSoundCompare ID="" SoundFilePath="" />
//</cSoundTimeLineCollection>
//basicly should have a list of sound file and correspond ID for timeline sound data
//this is the resources provide to access cSindSoundCompare.
class cSoundTimeLineCollection:public cNamedTypedObjectVector<cSindSoundCompare>,public cNodeISAX
{
	virtual	bool	MyParse(TiXmlElement*e_pRoot);
public:
	cSoundTimeLineCollection();
	virtual ~cSoundTimeLineCollection();
};

//struct sFrequencyAndAmplitudeWithFFTBinIndex:public sFrequenceAndAmplitudeAndTime
//{
//	//a quick index to find out amplitude value,because frequency could be different so make a captable offset
//	int	iFFTBinIndex[2];
//};
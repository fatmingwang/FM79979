#pragma once

#include "QuickFFTDataFrequencyFinder.h"
//#include "FindTimeDomainFrequenceAndAmplitude.h"

//class cToneData;
struct sFindTimeDomainFrequenceAndAmplitude;
class cToneDataVector;

class cSoundCompareParameter
{
public:
	static float	m_sfTolerateTime;
	//if the frequency amplitude close enought
	static int		m_siAmplitudeOffset;
};

//idea is one compare object has a reference sound data sFindTimeDomainFrequenceAndAmplitude,
//we have current matched index,and go through all steps(m_pFrequenceAndAmplitudeAndTimeFinder->OneScondFrequenceAndAmplitudeAndTimeData),
//but the problem is what if its a sequence sound and the ime offset is small this will has some problem
//so ensure the sequency sound has a flag must match after proior
//
//here dont care about time only care about the frequency is matched by order
class cSoundTimeLineData:public NamedTypedObject
{
	//this just a reference.
	const sFindTimeDomainFrequenceAndAmplitude*m_pFrequenceAndAmplitudeAndTimeFinder;
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
	cSoundTimeLineData(const sFindTimeDomainFrequenceAndAmplitude*e_pData,float e_fCompareTime);
	virtual ~cSoundTimeLineData();
	//if finish return true
	bool		Compare(float e_fCurrentTime,cQuickFFTDataFrequencyFinder*e_pQuickFFTDataFrequencyFinder);
};

//<cSoundTimeLineDataCollection ToneDataFileName="Quitar.xml">
//		<cSoundTimeLineData ID="" Time="0.5" />
//		...
//		...
//		...
//		<cSoundTimeLineData ID="" SoundFilePath="" />
//</cSoundTimeLineDataCollection>
//basicly should have a list of sound file and correspond ID for timeline sound data
//this is the resources provide to access cSoundTimeLineData.
class cSoundTimeLineDataCollection:public cNamedTypedObjectVector<cSoundTimeLineData>,public cNodeISAX
{
	virtual	bool	MyParse(TiXmlElement*e_pRoot);
	cToneDataVector*m_pToneDataVector;
public:
	cSoundTimeLineDataCollection();
	virtual ~cSoundTimeLineDataCollection();
};

//struct sFrequencyAndAmplitudeWithFFTBinIndex:public sFrequenceAndAmplitudeAndTime
//{
//	//a quick index to find out amplitude value,because frequency could be different so make a captable offset
//	int	iFFTBinIndex[2];
//};
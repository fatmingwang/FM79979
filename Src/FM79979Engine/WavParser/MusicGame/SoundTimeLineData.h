#pragma once

#include "FFTFrequency.h"

#define	CORRECT_TUNE_COLOR	Vector4::Green
#define	WAIT_TIME_TUNE		Vector4::One
#define	TUNE_TIME_PAST		Vector4::Red

#define	SOUND_TIME_LINE_DATA_TIME			"Time"
#define	SOUND_TIME_LINE_DATA_TUNE_KEEP_TIME	"TuneKeepTime"
//#include "FindTimeDomainFrequenceAndAmplitude.h"

//class cToneData;
struct sFindTimeDomainFrequenceAndAmplitude;
class cNoteFrequencyAndDecibles;
class cToneData;
class cToneDataVector;

//idea is one compare object has a reference sound data sFindTimeDomainFrequenceAndAmplitude,
//we have current matched index,and go through all steps(m_pFrequenceAndAmplitudeAndTimeFinder->OneScondFrequenceAndAmplitudeAndTimeData),
//but the problem is what if its a sequence sound and the ime offset is small this will has some problem
//so ensure the sequency sound has a flag must match after proior
//
//here dont care about time only care about the frequency is matched by order
class cSoundTimeLineData:public NamedTypedObject
{
	bool								m_bActivedToCompare;
	float								m_fActivedElpaseTime;
	//for auto play test
	bool								m_bAlreadyPlayTestFlag;
	//
	bool								m_bTuneMatched;
	bool								m_bTimeOver;//current time is big than its compare time
	//this just a reference.
	cToneData*							m_pToneData;
	sFindTimeDomainFrequenceAndAmplitude*m_pFrequenceAndAmplitudeAndTimeFinder;
	cNoteFrequencyAndDecibles*			m_pNoteFrequencyAndDecibles;
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
	float								m_fTuneKeepTime;//some instrument can long press(piano)
	//avoid same sound is sequence.
	GET_SET_DEC(bool,m_bMustMatchAfterProior,IsMustMatchAfterProior,SetMustMatchAfterProior);
	//time is over?
	bool								IsFinish(float e_fCurrentTime);
	bool								IsStillInCompareTime(float e_fTargetTime);
	//for fire event
	Vector3								m_vPos;
public:
	DEFINE_TYPE_INFO();
	cSoundTimeLineData(const sFindTimeDomainFrequenceAndAmplitude*e_pData,float e_fCompareTime,float e_fTuneKeepTime,cToneData*e_pToneData);
	cSoundTimeLineData(const cNoteFrequencyAndDecibles*e_pData,float e_fCompareTime,float e_fTuneKeepTime,cToneData*e_pToneData);
	virtual ~cSoundTimeLineData();

	void				Init();
	void				Update(float e_fCurrentTime);

	bool				IsTuneMatched(){return m_bTuneMatched;}
	void				SetTuneMatched(bool e_bTuneMatched){m_bTuneMatched = e_bTuneMatched;}
	//if finish return true
	bool				Compare(float e_fElpaseTime,float e_fCurrentTime,cQuickFFTDataFrequencyFinder*e_pQuickFFTDataFrequencyFinder);
	bool				CompareWithNoteFrequencyAndDecibles(float e_fElpaseTime,float e_fCurrentTime,cQuickFFTDataFrequencyFinder*e_pQuickFFTDataFrequencyFinder);
	bool				CompareWithFrequenceAndAmplitudeAndTimeFinder(float e_fElpaseTime,float e_fCurrentTime,cQuickFFTDataFrequencyFinder*e_pQuickFFTDataFrequencyFinder);
	float				GetCompareTime(){return m_fCompareTime;}
	cToneData*			GetToneData(){return m_pToneData;}
	bool				IsTimeOver(){ return m_bTimeOver; }
	void				SetTimeOver(bool e_bTimeOver){m_bTimeOver = e_bTimeOver;}
	float				GetTuneKeepTime(){ return m_fTuneKeepTime; }
	Vector3				GetPos(){return m_vPos;}
	void				SetPos(Vector3 e_vPos){m_vPos = e_vPos;}
};

//<cSoundTimeLineDataCollection ToneDataFileName="Quitar.xml">
//		<cSoundTimeLineData ID="" Time="0.5" />
//		...
//		...
//		...
//		<cSoundTimeLineData ID="" Time="5" />
//</cSoundTimeLineDataCollection>
//basicly should have a list of sound file and correspond ID for timeline sound data
//this is the resources provide to access cSoundTimeLineData.
class cSoundTimeLineDataCollection:public cNamedTypedObjectVector<cSoundTimeLineData>,public cNodeISAX
{
	bool				ParseMusicFile(TiXmlElement*e_pTiXmlElement);
	virtual	bool		MyParseWithFindTimeDomainFrequenceAndAmplitude(TiXmlElement*e_pRoot);
	virtual	bool		MyParseWithNoteFrequencyAndDecibles(TiXmlElement*e_pRoot);
protected:
	virtual	bool		MyParse(TiXmlElement*e_pRoot);
	cToneDataVector*	m_pToneDataVector;
	//for quick access to do tsomething
	float				m_fLastToneDataCompareTime;
public:
	cSoundTimeLineDataCollection();
	virtual ~cSoundTimeLineDataCollection();
	void			Init();
	float			GetLastObjectCompareEndTime();
};

//struct sFrequencyAndAmplitudeWithFFTBinIndex:public sFrequenceAndAmplitudeAndTime
//{
//	//a quick index to find out amplitude value,because frequency could be different so make a captable offset
//	int	iFFTBinIndex[2];
//};
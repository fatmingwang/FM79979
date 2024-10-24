#include "stdafx.h"
#include "ToneData.h"
#include "SoundTimeLineData.h"
#include "FindTimeDomainFrequenceAndAmplitude.h"
#include "Parameters.h"
#include "AllPhaseName.h"

TYPDE_DEFINE_MARCO(cSoundTimeLineData);

cSoundTimeLineData::cSoundTimeLineData(const sFindTimeDomainFrequenceAndAmplitude*e_pData,float e_fCompareTime,float e_fTuneKeepTime,cToneData*e_pToneData)
{
	m_bAllowToCompare = false;
	m_pNoteFrequencyAndDecibles = nullptr;
	m_bTimeOver = false;
	m_pToneData = e_pToneData;
	//m_fErrorScore = 0.f;
	m_fCompareTime = e_fCompareTime;
	m_pFrequenceAndAmplitudeAndTimeFinder = new sFindTimeDomainFrequenceAndAmplitude(e_pData);
	m_iCurrentMatchedIndex = 0;
	m_iNumMatched = 0;
	m_bMustMatchAfterProior = false;
	m_bAlreadyPlayTestFlag = false;
	m_bTuneMatched = false;
	m_bActivedToCompare = false;
	m_fActivedElpaseTime = 0.f;
	m_fTuneKeepTime = e_fTuneKeepTime;

	m_iMatchTime = 0;
	//fuck...PC and IOS 5 is okay but android not work...why have no idea... 
#ifdef ANDROID
	m_iMatchTimeCondition = 3;
#else
	m_iMatchTimeCondition = 5;
#endif
	m_bStartHittedCount = false;
}

cSoundTimeLineData::cSoundTimeLineData(cNoteFrequencyAndDecibles*e_pData,float e_fCompareTime,float e_fTuneKeepTime,cToneData*e_pToneData)
{
	m_bAllowToCompare = false;
	m_pNoteFrequencyAndDecibles = e_pData;
	m_bTimeOver = false;
	m_pToneData = e_pToneData;
	//m_fErrorScore = 0.f;
	m_fCompareTime = e_fCompareTime;
	m_pFrequenceAndAmplitudeAndTimeFinder = nullptr;
	m_iCurrentMatchedIndex = 0;
	m_iNumMatched = 0;
	m_bMustMatchAfterProior = false;
	m_bAlreadyPlayTestFlag = false;
	m_bTuneMatched = false;
	m_bActivedToCompare = false;
	m_fActivedElpaseTime = 0.f;
	m_fTuneKeepTime = e_fTuneKeepTime;

	m_iMatchTime = 0;
	//fuck...PC and IOS 5 is okay but android not work...why have no idea... 
#ifdef ANDROID
	m_iMatchTimeCondition = 3;
#else
	m_iMatchTimeCondition = 5;
#endif
	m_bStartHittedCount = false;
}

cSoundTimeLineData::~cSoundTimeLineData()
{
	m_pNoteFrequencyAndDecibles = nullptr;
	SAFE_DELETE(m_pFrequenceAndAmplitudeAndTimeFinder);
}

void		cSoundTimeLineData::Init()
{
	m_bActivedToCompare = false;
	m_fActivedElpaseTime = 0.f;
	m_bTuneMatched = false;
	m_bTimeOver = false;
	m_bAlreadyPlayTestFlag = false;
	m_bAllowToCompare = false;
}

void		cSoundTimeLineData::Update(float e_fCurrentTime)
{
	if( !m_bAlreadyPlayTestFlag )
	{
		//if(this->m_fCompareTime+cSoundCompareParameter::m_sfCompareTuneTolerateTime-e_fCurrentTime<=cSoundCompareParameter::m_sfCompareTuneTolerateTime)
		if(IsStillInCompareTime(e_fCurrentTime))
		{
			m_bAlreadyPlayTestFlag = true;
			if(cGameApp::m_sbDebugFunctionWorking)
				cGameApp::SoundPlay(this->GetName(),true);
		}
	}
}

bool		cSoundTimeLineData::Compare(float e_fElpaseTime,float e_fCurrentTime,cQuickFFTDataFrequencyFinder*e_pQuickFFTDataFrequencyFinder)
{
	if( m_pFrequenceAndAmplitudeAndTimeFinder )
		return CompareWithFrequenceAndAmplitudeAndTimeFinder(e_fElpaseTime,e_fCurrentTime,e_pQuickFFTDataFrequencyFinder);
	return CompareWithNoteFrequencyAndDecibles(e_fElpaseTime,e_fCurrentTime,e_pQuickFFTDataFrequencyFinder);
}

bool		cSoundTimeLineData::CompareWithFrequenceAndAmplitudeAndTimeFinder(float e_fElpaseTime,float e_fCurrentTime,cQuickFFTDataFrequencyFinder*e_pQuickFFTDataFrequencyFinder)
{
	//already matched.
	if( m_bTuneMatched )
		return true;
	if(!IsStillInCompareTime(e_fCurrentTime))
	{
		return false;
	}
	float l_fLocalTime = e_fCurrentTime-this->m_fCompareTime;
	int	l_iAllMatched = 0;
	auto l_pDataVector = this->m_pFrequenceAndAmplitudeAndTimeFinder->OneScondFrequenceAndAmplitudeAndTimeData[this->m_iCurrentMatchedIndex];
	if( !l_pDataVector )
	{//fuck
		return false;
	}
	//if( l_fLocalTime < 0.f || l_fLocalTime >= l_pDataVector->size() )
	//{
	//	//not ready to match or time over.
	//	return;
	//}
	std::vector<int> l_MatchedVector;
	for(auto l_pInnerData :	*l_pDataVector)
	{
		//std::vector<int>l_iAmplitudeVector = e_pQuickFFTDataFrequencyFinder->GetDecibelsByFrequency((int)l_pInnerData->fFrequency);
		std::vector<int>l_iAmplitudeVector = e_pQuickFFTDataFrequencyFinder->GetDecibelsByFFTBinIndex((int)l_pInnerData->fFrequency);
		int l_iNumHitted = 0;
		for(int l_iAmplitude :l_iAmplitudeVector)
		{
			if( l_iAmplitude <= 0 )
			{
				int a=0;
			}
			if( l_pInnerData->bMatched )
			{
				++l_iAllMatched;
				l_iNumHitted++;
				break;
			}
			else
			if( l_iAmplitude >= cSoundCompareParameter::m_siDebugAmplitudeValue )
			//if(cSoundCompareParameter::m_siAmplitudeOffset >= abs(l_iAmplitude-l_pInnerData->iAmplitude))
			{//matched
				l_pInnerData->fCompareKeepTime += e_fElpaseTime;
				l_iNumHitted++;
				//if( l_pInnerData->fCompareKeepTime >= l_pInnerData->fKeepTime)
				//if( l_pInnerData->fCompareKeepTime >= 1/30.f)
				{
					l_MatchedVector.push_back((int)l_pInnerData->fFrequency);
					++l_iAllMatched;
					l_pInnerData->bMatched = true;
				}
				break;
			}
		}
		if( l_iNumHitted == 0 )
			l_pInnerData->fCompareKeepTime = 0.f;
	}  
	//because some frequency just not we want but I have no idea how to filter this so...
	float l_fPercent = (float)l_iAllMatched/l_pDataVector->size();
	//return l_fPercent;
	//float l_fToomanySampleSoGiveALittleReduce = l_uiSize/800.f;
	//return l_fPercent+l_fToomanySampleSoGiveALittleReduce;
	if(l_fPercent >= 0.7)
	{
		m_bStartHittedCount = true;
	}
	if( m_bStartHittedCount )
	{
		if(l_fPercent >= 0.7)
		{
			++m_iMatchTime;
			if( m_iMatchTime < m_iMatchTimeCondition )
			{
				l_fPercent = 0.f;
			}
		}
		else
		{
			m_bStartHittedCount = false;
		}
	}
	else
	{
		m_iMatchTime = 0;
		l_fPercent = 0.f;
	}
	if( l_fPercent >= 0.95f )
	{
		m_bTuneMatched = true;
		cGameApp::EventMessageShot(TUNE_MATCH_EVENT_ID,this);
		
#ifdef DEBUG
		cGameApp::OutputDebugInfoString(ValueToStringW(l_MatchedVector));
#endif
		return true;
	}
	return this->IsFinish(e_fCurrentTime);
}


bool		cSoundTimeLineData::CompareWithNoteFrequencyAndDecibles(float e_fElpaseTime,float e_fCurrentTime,cQuickFFTDataFrequencyFinder*e_pQuickFFTDataFrequencyFinder)
{
	if(!this->m_pNoteFrequencyAndDecibles)
		return false;
	//already matched.
	if( m_bTuneMatched )
		return true;
	if(!IsStillInCompareTime(e_fCurrentTime))
	{
		return false;
	}
	int l_iNumMatched = 0;
	size_t l_uiSize = m_pNoteFrequencyAndDecibles->FrequencyVector.size();
	if( l_uiSize == 0 )
		return false;
	for(size_t i=0;i<l_uiSize;++i  )
	{
		//l_fCurrentProgress += l_fOneStep;
		//int l_iFrequency = m_pNoteFrequencyAndDecibles->FrequencyVector[i];
		int l_iFFTBinIndex = m_pNoteFrequencyAndDecibles->FrequencyBinIndexVector[i];
		//int l_iTargetDecibels = m_pNoteFrequencyAndDecibles->FrequencyHittedValueVector[i]-13;
		float l_fTargetDecibels = m_pNoteFrequencyAndDecibles->FrequencyHittedValueVector[i]/cSoundCompareParameter::m_sfDecibelsMatchDivideValue;
		//int l_fTargetDecibels = 8;
		std::vector<int>	l_DeciblesVector = e_pQuickFFTDataFrequencyFinder->GetDecibelsByFFTBinIndex(l_iFFTBinIndex);
		//l_fTargetDecibels -= (1-l_fCurrentProgress)*13.f;
		//l_fTargetDecibels -= 13;
		for( int l_iDecible : l_DeciblesVector )
		{
			if( (float)l_iDecible > l_fTargetDecibels)
			{
				++l_iNumMatched;
				break;
			}
		}
	}	
	//because some frequency just not we want but I have no idea how to filter this so...
	float l_fPercent = (float)l_iNumMatched/l_uiSize;
	//if(l_fPercent >= 0.7)
	//{
	//	++m_iMatchTime;
	//	if( m_iMatchTime < m_iMatchTimeCondition )
	//	{
	//		l_fPercent = 0.f;
	//	}
	//}
	//else
	//	m_bStartHittedCount = false;

	if( l_fPercent >= cSoundCompareParameter::m_sfAcceptableNoteMatchValue  )
	{
		m_bTuneMatched = true;
		cGameApp::EventMessageShot(TUNE_MATCH_EVENT_ID,this);
	}
	return this->IsFinish(e_fCurrentTime);
}


bool		cSoundTimeLineData::IsStillInCompareTime(float e_fTargetTime)
{
	if( this->IsTimeOver() )
		return false;
	float l_fTimeDifference = abs( e_fTargetTime - this->m_fCompareTime );
	if( l_fTimeDifference > cSoundCompareParameter::m_sfCompareTuneTolerateTime )
	{
		if(IsFinish(e_fTargetTime))
		{
			if( !this->IsTimeOver() )
			{
				cGameApp::EventMessageShot(TUNE_TIME_OVER_EVENTID,this);
				this->SetTimeOver(true);
			}
		}
		//cGameApp::OutputDebugInfoString(L"Not compare!!");
		m_bAllowToCompare = false;
		return false;
	}
	//cGameApp::OutputDebugInfoString(L"compare!!");
	m_bAllowToCompare = true;
	return true;
}

bool		cSoundTimeLineData::IsFinish(float e_fCurrentTime)
{
	float l_iEndSecond = cSoundCompareParameter::m_sfCompareTuneTolerateTime+m_fCompareTime;
	if( e_fCurrentTime-l_iEndSecond >= 0.f)
	{
		return true;
	}
	return false;
}


//<cSoundTimeLineDataCollection MusicInstrumentName="Quitar">
//		<cSoundTimeLineData ID="" SoundFilePath="" FFTSequenceFileName="" />
//		...
//		...
//		...
//		<cSoundTimeLineData ID="" SoundFilePath="" />
//</cSoundTimeLineDataCollection>

cSoundTimeLineDataCollection::cSoundTimeLineDataCollection()
{
	m_pToneDataVector = nullptr;
	m_fLastToneDataCompareTime = -1.f;
}

cSoundTimeLineDataCollection::~cSoundTimeLineDataCollection()
{
	SAFE_DELETE(m_pToneDataVector);
}

void	cSoundTimeLineDataCollection::Init()
{
	for(auto l_pData :this->m_ObjectList)
	{
		l_pData->Init();
	}
}
//http://www.sengpielaudio.com/calculator-notenames.htm

//here is wrong because start should be a0,but I just lazy to fix this and change wav resources name start from a1
const char*g_strKeyNymberToPianoString[88] = {
"a0" ,"a0m","b0" ,"c1" ,"c1m","d1" ,"d1m","e1" ,"f1" ,"f1m","g1" ,"g1m",//12
"a1" ,"a1m","b1" ,"c2" ,"c2m","d2" ,"d2m","e2" ,"f2" ,"f2m","g2" ,"g2m",//13-24
"a2" ,"a2m","b2" ,"c3" ,"c3m","d3" ,"d3m","e3" ,"f3" ,"f3m","g3" ,"g3m",//25-36
"a3" ,"a3m","b3" ,"c4" ,"c4m","d4" ,"d4m","e4" ,"f4" ,"f4m","g4" ,"g4m",//37-48
"a4" ,"a4m","b4" ,"c5" ,"c5m","d5" ,"d5m","e5" ,"f5" ,"f5m","g5" ,"g5m",//49-60
"a5" ,"a5m","b5" ,"c6" ,"c6m","d6" ,"d6m","e6" ,"f6" ,"f6m","g6" ,"g6m",//61-72
"a6" ,"a6m","b6" ,"c7" ,"c7m","d7" ,"d7m","e7" ,"f7" ,"f7m","g7" ,"g7m",//73-84
"a7" ,"a7m","b7" ,"c8"};

bool	cSoundTimeLineDataCollection::ParseMusicFile(TiXmlElement*e_pTiXmlElement)
{
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pTiXmlElement)
		COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pTiXmlElement,cSoundTimeLineData::TypeID)
		{
			const WCHAR*l_strSoundID = e_pTiXmlElement->Attribute(CHAR_TO_WCHAR_DEFINE(TONE_DATA_ID));
			const WCHAR*l_strTime = e_pTiXmlElement->Attribute(CHAR_TO_WCHAR_DEFINE(SOUND_TIME_LINE_DATA_TIME));
			const WCHAR*l_strTuneKeepTime = e_pTiXmlElement->Attribute(CHAR_TO_WCHAR_DEFINE(SOUND_TIME_LINE_DATA_TUNE_KEEP_TIME));
			float l_fTuneKeepTime = 0.1f;
			if( l_strTuneKeepTime )
				l_fTuneKeepTime = GetFloat(l_strTuneKeepTime);
			if( l_strSoundID && l_strTime )
			{
				//int		l_iSoundID	= GetInt(l_strSoundID);
				float l_fTime = GetFloat(l_strTime);
				//const sFindTimeDomainFrequenceAndAmplitude*l_pFrequenceAndAmplitudeAndTimeFinder = m_pToneDataVector->GetFrequenceAndAmplitudeAndTimeFinderBySoundID(l_iSoundID);
				int l_iKeyNumber = GetInt(l_strSoundID);
				//here is 88 key piano...out of range ignore
				int l_iIndex = l_iKeyNumber-21;
				if( l_iIndex < 0 || l_iIndex >= 88 )
				{
					continue;
				}
				auto l_pToneData = m_pToneDataVector->GetObject(g_strKeyNymberToPianoString[l_iIndex]);
				if( !l_pToneData )
				{
					UT::ErrorMsg(g_strKeyNymberToPianoString[l_iIndex],"tone data not exists");
					continue;
				}
				cSoundTimeLineData*l_pSingleSoundCompare = nullptr;
				if( l_pToneData->GetFrequenceAndAmplitudeAndTimeFinder() )
					l_pSingleSoundCompare = new cSoundTimeLineData(l_pToneData->GetFrequenceAndAmplitudeAndTimeFinder(),l_fTime,l_fTuneKeepTime,l_pToneData);
				else
					l_pSingleSoundCompare = new cSoundTimeLineData(l_pToneData->GetNoteFrequencyAndDecibles(),l_fTime,l_fTuneKeepTime,l_pToneData);
				l_pSingleSoundCompare->SetName(g_strKeyNymberToPianoString[l_iIndex]);
				this->AddObjectNeglectExist(l_pSingleSoundCompare);
			}
			else
			{
				UT::ErrorMsg(cSoundTimeLineData::TypeID,L"data error no time or no Sound ID");
			}
		}
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pTiXmlElement)
	auto l_pLastObject = this->GetLastObject();
	if( l_pLastObject )
	{
		m_fLastToneDataCompareTime = l_pLastObject->GetCompareTime()+cSoundCompareParameter::m_sfCompareTuneTolerateTime;
	}
	return true;
}

bool		cSoundTimeLineDataCollection::MyParseWithNoteFrequencyAndDecibles(TiXmlElement*e_pRoot)
{
	return false;
}

bool		cSoundTimeLineDataCollection::MyParseWithFindTimeDomainFrequenceAndAmplitude(TiXmlElement*e_pRoot)
{
	const WCHAR*l_strToneDataFileName = e_pRoot->Attribute(L"ToneDataFileName");
	if( !l_strToneDataFileName )
	{
		UT::ErrorMsg("ToneDataFileName not valid",this->m_strFileName);
		return false;
	}
	if(m_pToneDataVector)
	{
		//same music instrument
		if(m_pToneDataVector->IsSameName(l_strToneDataFileName))
		{
			return ParseMusicFile(e_pRoot);
		}
		SAFE_DELETE(m_pToneDataVector);
	}
	m_pToneDataVector = new cToneDataVector();
	if(m_pToneDataVector->ParseWithMyParse(UT::WcharToChar(l_strToneDataFileName).c_str()))
	{
		m_pToneDataVector->SetName(l_strToneDataFileName);
		return ParseMusicFile(e_pRoot);
	}
	return false;
}

bool	cSoundTimeLineDataCollection::MyParse(TiXmlElement*e_pRoot)
{
	return MyParseWithFindTimeDomainFrequenceAndAmplitude(e_pRoot);
}

float	cSoundTimeLineDataCollection::GetLastObjectCompareEndTime()
{
	auto l_pLastObject = this->GetLastObject();
	if(l_pLastObject)
		return l_pLastObject->GetCompareTime();
	return -1.f;
}
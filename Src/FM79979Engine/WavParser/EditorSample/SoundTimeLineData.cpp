#include "stdafx.h"
#include "ToneData.h"
#include "SoundTimeLineData.h"
#include "FindTimeDomainFrequenceAndAmplitude.h"
#include "Parameters.h"

TYPDE_DEFINE_MARCO(cSoundTimeLineData);

cSoundTimeLineData::cSoundTimeLineData(const sFindTimeDomainFrequenceAndAmplitude*e_pData,float e_fCompareTime,cToneData*e_pToneData)
{
	m_pToneData = e_pToneData;
	//m_fErrorScore = 0.f;
	m_fResultScore = 0.f;
	m_fCompareTime = e_fCompareTime;
	m_pFrequenceAndAmplitudeAndTimeFinder = new sFindTimeDomainFrequenceAndAmplitude(e_pData);
	m_iCurrentMatchedIndex = 0;
	m_iNumMatched = 0;
	m_bMustMatchAfterProior = false;
	m_bAlreadyPlayTestFlag = false;
	m_bMatched = false;
}

cSoundTimeLineData::~cSoundTimeLineData()
{
	SAFE_DELETE(m_pFrequenceAndAmplitudeAndTimeFinder);
}

void		cSoundTimeLineData::Init()
{
	this->m_fResultScore = 0.f;
	m_bMatched = false;
	m_bAlreadyPlayTestFlag = false;
}

void		cSoundTimeLineData::Update(float e_fCurrentTime)
{
#ifdef PARSE_TEST_SOUND
	if( !m_bAlreadyPlayTestFlag )
	{
		//if(this->m_fCompareTime+cSoundCompareParameter::m_sfTolerateTime-e_fCurrentTime<=cSoundCompareParameter::m_sfTolerateTime)
		if(IsStillInCompareTime(e_fCurrentTime))
		{
			m_bAlreadyPlayTestFlag = true;
			cGameApp::SoundPlay(this->GetName(),true);
		}
	}
#endif
}

bool		cSoundTimeLineData::Compare(float e_fElpaseTime,float e_fCurrentTime,cQuickFFTDataFrequencyFinder*e_pQuickFFTDataFrequencyFinder)
{
	//already matched.
	if( m_bMatched )
		return true;
	if(!IsStillInCompareTime(e_fCurrentTime))
		return false;
	float l_fLocalTime = e_fCurrentTime-this->m_fCompareTime;
	int	l_iAllMatched = 0;
	auto l_pDataVector = this->m_pFrequenceAndAmplitudeAndTimeFinder->OneScondFrequenceAndAmplitudeAndTimeData[this->m_iCurrentMatchedIndex];
	//if( l_fLocalTime < 0.f || l_fLocalTime >= l_pDataVector->size() )
	//{
	//	//not ready to match or time over.
	//	return;
	//}
	for(auto l_pInnerData :	*l_pDataVector)
	{
		std::vector<int>l_iAmplitudeVector = e_pQuickFFTDataFrequencyFinder->GetAmplitude((int)l_pInnerData->fFrequency);
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
				if( l_pInnerData->fCompareKeepTime >= l_pInnerData->fKeepTime/2.0f)
				//if( l_pInnerData->fCompareKeepTime >= e_fElpaseTime*2)
				{
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
	if(m_fResultScore < l_fPercent )
	{
		m_fResultScore = l_fPercent;
		std::wstring l_str = this->GetName();
		l_str += L",Percent:";
		l_str += ValueToStringW((int)(l_fPercent*100));
		cGameApp::OutputDebugInfoString(l_str);
	}

	if( l_fPercent >= 0.75f )
	//if( l_iAllMatched >= l_pDataVector->size()/l_iLazyDivide )
	{
//		m_fResultScore = (float)m_iCurrentMatchedIndex/l_pDataVector->size();
		++m_iCurrentMatchedIndex;
	}
	if(this->m_iCurrentMatchedIndex >= (int)this->m_pFrequenceAndAmplitudeAndTimeFinder->OneScondFrequenceAndAmplitudeAndTimeData.size())
	{
		m_bMatched = true;
		//all matched
		return true;
	}
	return this->IsFinish(e_fCurrentTime);
}

bool		cSoundTimeLineData::IsStillInCompareTime(float e_fTargetTime)
{
	float l_fTimeDifference = abs( e_fTargetTime - this->m_fCompareTime );
	if( l_fTimeDifference > cSoundCompareParameter::m_sfTolerateTime )
	{
		return false;
	}
	return true;
}

bool		cSoundTimeLineData::IsFinish(float e_fCurrentTime)
{
	float l_iEndSecond = this->m_pFrequenceAndAmplitudeAndTimeFinder->OneScondFrequenceAndAmplitudeAndTimeData.size()+this->m_fCompareTime;
	if( e_fCurrentTime-l_iEndSecond >= cSoundCompareParameter::m_sfTolerateTime )
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

bool	cSoundTimeLineDataCollection::ParseMusicFile(TiXmlElement*e_pTiXmlElement)
{
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pTiXmlElement)
		COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pTiXmlElement,cSoundTimeLineData::TypeID)
		{
			const WCHAR*l_strSoundID = e_pTiXmlElement->Attribute(CHAR_TO_WCHAR_DEFINE(TONE_DATA_ID));
			const WCHAR*l_strTime = e_pTiXmlElement->Attribute(CHAR_TO_WCHAR_DEFINE(SOUND_TIME_LINE_DATA_TIME));
			if( l_strSoundID && l_strTime )
			{
				//int		l_iSoundID	= GetInt(l_strSoundID);
				float l_fTime = GetFloat(l_strTime);
				//const sFindTimeDomainFrequenceAndAmplitude*l_pFrequenceAndAmplitudeAndTimeFinder = m_pToneDataVector->GetFrequenceAndAmplitudeAndTimeFinderBySoundID(l_iSoundID);
				auto l_pToneData = m_pToneDataVector->GetObject(l_strSoundID);
				if( !l_pToneData )
				{
					UT::ErrorMsg(l_strSoundID,L"tone data not exists");
					continue;
				}
				cSoundTimeLineData*l_pSingleSoundCompare = new cSoundTimeLineData(l_pToneData->GetFrequenceAndAmplitudeAndTimeFinder(),l_fTime,l_pToneData);
				l_pSingleSoundCompare->SetName(l_strSoundID);
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
		m_fLastToneDataCompareTime = l_pLastObject->GetCompareTime()+cSoundCompareParameter::m_sfTolerateTime;
	}
	return true;
}

bool	cSoundTimeLineDataCollection::MyParse(TiXmlElement*e_pRoot)
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

float	cSoundTimeLineDataCollection::GetLastObjectCompareEndTime()
{
	auto l_pLastObject = this->GetLastObject();
	if(l_pLastObject)
		return l_pLastObject->GetCompareTime();
	return -1.f;
}
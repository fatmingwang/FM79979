#include "stdafx.h"
#include "ToneData.h"
#include "SoundTimeLineData.h"
#include "FindTimeDomainFrequenceAndAmplitude.h"

TYPDE_DEFINE_MARCO(cSoundTimeLineData);

float	cSoundCompareParameter::m_sfTolerateTime = 0.32f;
int		cSoundCompareParameter::m_siAmplitudeOffset = 30;
float	cSoundCompareParameter::m_sfBeforeCurrentTimeViewRange = 2.f;
float	cSoundCompareParameter::m_sfAfterCurrentTimeViewRange = 10.f;

cSoundTimeLineData::cSoundTimeLineData(const sFindTimeDomainFrequenceAndAmplitude*e_pData,float e_fCompareTime)
{
	//m_fErrorScore = 0.f;
	m_fResultScore = 0.f;
	m_fCompareTime = e_fCompareTime;
	m_pFrequenceAndAmplitudeAndTimeFinder = e_pData;
	m_iCurrentMatchedIndex = 0;
	m_iNumMatched = 0;
	m_bMustMatchAfterProior = false;
}

cSoundTimeLineData::~cSoundTimeLineData()
{

}

bool		cSoundTimeLineData::Compare(float e_fCurrentTime,cQuickFFTDataFrequencyFinder*e_pQuickFFTDataFrequencyFinder)
{
	float l_fLocalTime = e_fCurrentTime-this->m_fCompareTime;
	int	l_iAllMatched = 0;
	auto l_pDataVector = this->m_pFrequenceAndAmplitudeAndTimeFinder->OneScondFrequenceAndAmplitudeAndTimeData[this->m_iCurrentMatchedIndex];
	for(auto l_pInnerData :	*l_pDataVector)
	{
		std::vector<int>l_iAmplitudeVector = e_pQuickFFTDataFrequencyFinder->GetAmplitude((int)l_pInnerData->fFrequency);
		for(int l_iAmplitude :l_iAmplitudeVector)
		{
			if(cSoundCompareParameter::m_siAmplitudeOffset <= abs(l_iAmplitude-l_pInnerData->iAmplitude))
			{//matched
				++l_iAllMatched;
				break;
			}
			else
			{//detect error part?
			
			}
		}
	}

	if( l_iAllMatched == l_pDataVector->size() )
	{
		m_fResultScore = (float)m_iCurrentMatchedIndex/l_pDataVector->size();
		++m_iCurrentMatchedIndex;
	}
	if(this->m_iCurrentMatchedIndex >= (int)this->m_pFrequenceAndAmplitudeAndTimeFinder->OneScondFrequenceAndAmplitudeAndTimeData.size())
	{
		//all matched
		return true;
	}
	return this->IsFinish(e_fCurrentTime);
}

bool		cSoundTimeLineData::IsStillInCompareTime(float e_fTargetTime)
{
	float l_fTimeDifference = abs( e_fTargetTime - this->m_fCompareTime );
	if( l_fTimeDifference > this->m_fCompareTime+cSoundCompareParameter::m_sfTolerateTime )
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
	
}

bool	cSoundTimeLineDataCollection::MyParse(TiXmlElement*e_pRoot)
{
	const WCHAR*l_strToneDataFileName = e_pRoot->Attribute(L"ToneDataFileName");
	if(m_pToneDataVector)
	{
		//same music instrument
		if(m_pToneDataVector->IsSameName(l_strToneDataFileName))
			return true;
		SAFE_DELETE(m_pToneDataVector);
	}
	m_pToneDataVector = new cToneDataVector();
	if(m_pToneDataVector->ParseTextWithMyParse(UT::WcharToChar(l_strToneDataFileName).c_str()))
	{
		PARSE_ELEMENT_START(e_pRoot)
			COMPARE_VALUE_WITH_DEFINE(cSoundTimeLineData::TypeID)
			{
				const WCHAR*l_strSoundID = e_pRoot->Attribute(L"SoundID");
				const WCHAR*l_strTime = e_pRoot->Attribute(L"Time");
				if( l_strSoundID && l_strTime )
				{
					int		l_iSoundID	= GetInt(l_strSoundID);
					float l_fTime = GetFloat(l_strTime);
					const sFindTimeDomainFrequenceAndAmplitude*l_pFrequenceAndAmplitudeAndTimeFinder = m_pToneDataVector->GetFrequenceAndAmplitudeAndTimeFinderBySoundID(l_iSoundID);
					cSoundTimeLineData*l_pSingleSoundCompare = new cSoundTimeLineData(l_pFrequenceAndAmplitudeAndTimeFinder,l_fTime);
					this->AddObjectNeglectExist(l_pSingleSoundCompare);
				}
				else
				{
					UT::ErrorMsg(cSoundTimeLineData::TypeID,L"data error no time or no Sound ID");
				}
			}
		PARSE_NAME_VALUE_END
		auto l_pLastObject = this->GetLastObject();
		if( l_pLastObject )
		{
			m_fLastToneDataCompareTime = l_pLastObject->GetCompareTime()+cSoundCompareParameter::m_sfTolerateTime;
		}
		return true;
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
#include "stdafx.h"
#include "KissFFTConvert.h"
#include "TimeFrequencyAmplitudeValueCapture.h"


sFrequenceAndAmplitudeAndTime::sFrequenceAndAmplitudeAndTime(TiXmlElement*e_pElement)
{
	PARSE_ELEMENT_START(e_pElement)
		COMPARE_NAME("Frequency")
		{
			fFrequency = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("Amplitude")
		{
			iAmplitude = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("StartTime")
		{
			fStartTime = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("KeepTime")
		{
			fKeepTime = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("LastMuchTime")
		{
			fLastMuchTime = VALUE_TO_FLOAT;
		}
	PARSE_NAME_VALUE_END
}

TiXmlElement*sFrequenceAndAmplitudeAndTime::ToElement()
{
	TiXmlElement*l_pTiXmlElement = new TiXmlElement(L"FrequenceAndAmplitudeAndTime");
	l_pTiXmlElement->SetAttribute(L"Frequency",fFrequency);
	l_pTiXmlElement->SetAttribute(L"Amplitude",iAmplitude);
	l_pTiXmlElement->SetAttribute(L"StartTime",fStartTime);
	l_pTiXmlElement->SetAttribute(L"KeepTime",fKeepTime);
	l_pTiXmlElement->SetAttribute(L"LastMuchTime",fLastMuchTime);
	return l_pTiXmlElement;
}

cTimeFrequencyAmplitudeValueCapture::cTimeFrequencyAmplitudeValueCapture()
{
	m_pKissFFTConvert = nullptr;
	m_piSoundDataForParse = nullptr;
	//
	m_iParseFPS = ONE_FRAME_NEED_NUM_FFT_DATA_COUNT;
	m_fCurrentTime = 0.f;
	//frequencty change could have a range offset
	m_fFrequencyOffsetRange = cKissFFTConvertBase::GetFrequencyGapByFPS(44100,m_iParseFPS);
	//some times sample rate is possible not enough or missing some frequency
	m_fTolerateTime = TOLERATE_TIME;
	m_fMinKeepTime = MIN_LEEP_TIME;
	m_iMinAmplitude = MIN_AMPLITUDE;
}

cTimeFrequencyAmplitudeValueCapture::~cTimeFrequencyAmplitudeValueCapture()
{
	SAFE_DELETE(m_piSoundDataForParse);
	SAFE_DELETE(m_pKissFFTConvert);
}

bool	cTimeFrequencyAmplitudeValueCapture::AnalyizeStart(const char*e_strFileName)
{
	SAFE_DELETE(m_pKissFFTConvert);
	SAFE_DELETE(m_piSoundDataForParse);
	m_AllData.clear();
	m_CurrentWorkingFrequenceAndTimeVector.clear();
	m_pKissFFTConvert = new cKissFFTConvert();
	if(m_pKissFFTConvert->FetchSoundDataStart(e_strFileName,false))
	{
		FATMING_CORE::cSoundFile*l_pSoundFile = m_pKissFFTConvert->GetSoundFile();
		//if channel is bigger than 1 add N channel and get average.
		int l_iNumChannel = l_pSoundFile->m_iChannel;
		std::vector<std::vector<int>* >*l_pFFTDataVectorChannelVector = m_pKissFFTConvert->GetFFTDataVectorChannelVector();
		size_t l_uiFFTDAtaCount = (*l_pFFTDataVectorChannelVector)[0]->size();
		m_piSoundDataForParse = new int[l_uiFFTDAtaCount];
		if( l_iNumChannel > 1 )
		{
			for(size_t j = 0;j<l_uiFFTDAtaCount;++j)
			{
				int l_iValue = 0;
				for(int i = 0;i<l_iNumChannel;++i)
				{
					int l_iFFTData = (*(*l_pFFTDataVectorChannelVector)[i])[j];
					l_iValue += l_iFFTData;
				}
				l_iValue /= l_iNumChannel;
				m_piSoundDataForParse[j] = l_iValue;
			}
		}
		else
		{
			int*l_picSoundData = (int*)(*l_pFFTDataVectorChannelVector)[0];
			memcpy(m_piSoundDataForParse,l_picSoundData,sizeof(int)*l_uiFFTDAtaCount);
		}
		float l_fTimeLength = l_pSoundFile->m_fTime;
		float l_fFrameTimeElpase = 1.f/m_iParseFPS;
		int l_iNumCount = (int)(l_fTimeLength/l_fFrameTimeElpase);
		int l_iOneFrameDataCount = l_pSoundFile->m_iSampleCount/l_iNumCount;
		m_fFrequencyOffsetRange = cKissFFTConvertBase::GetFrequencyGapByFPS(l_pSoundFile->m_iFreq,m_iParseFPS);
		for( int i=0;i<l_iNumCount;++i )
		{
			this->m_fCurrentTime = l_fFrameTimeElpase*i;
			int*l_piStartData = &m_piSoundDataForParse[i*l_iOneFrameDataCount];
			FrameByFrameAnaylize(m_fFrequencyOffsetRange,l_piStartData,l_iOneFrameDataCount,l_fFrameTimeElpase);

		}
		return true;
	}
	return false;
}


std::vector<sFrequencyAndAmplitude>	cTimeFrequencyAmplitudeValueCapture::GetSatisfiedFrequencyAndAmplitudeVector(float e_fFreqDistance,const int*e_pData,int e_iDataLength,int e_AmplitudeCondition)
{
	std::vector<sFrequencyAndAmplitude>	l_ResultVector;
	int		l_iSourceFreq = this->m_pKissFFTConvert->GetSoundFile()->m_iFreq;
	float	l_fFreqGap = (float)l_iSourceFreq/e_iDataLength;
	for( int i=0;i<e_iDataLength;++i )
	{
		if(e_pData[i]>=e_AmplitudeCondition)
		{
			float l_fFreq = (float)l_iSourceFreq;
			sFrequencyAndAmplitude l_FrequencyAndAmplitude;
			l_FrequencyAndAmplitude.fFrequency = l_fFreqGap*i;
			l_FrequencyAndAmplitude.iAmplitude = e_pData[i];
			l_ResultVector.push_back(l_FrequencyAndAmplitude);
		}
	}
	return l_ResultVector;
}


sFrequenceAndAmplitudeAndTime FrequencyAndAmplitudeToFrequenceAndAmplitudeAndTime(sFrequencyAndAmplitude*e_pFrequencyAndAmplitude,float e_fCurrentTime)
{
	sFrequenceAndAmplitudeAndTime l_sFrequenceAndAmplitudeAndTime;
	l_sFrequenceAndAmplitudeAndTime.fFrequency = e_pFrequencyAndAmplitude->fFrequency;
	l_sFrequenceAndAmplitudeAndTime.iAmplitude = e_pFrequencyAndAmplitude->iAmplitude;
	l_sFrequenceAndAmplitudeAndTime.fKeepTime = 0.f;
	l_sFrequenceAndAmplitudeAndTime.fLastMuchTime = l_sFrequenceAndAmplitudeAndTime.fStartTime = e_fCurrentTime;
	return l_sFrequenceAndAmplitudeAndTime;
}

void	cTimeFrequencyAmplitudeValueCapture::FrameByFrameAnaylize(float e_fFreqDistance,const int*e_pData,int e_iDataLength,float e_fElpaseTime)
{
	std::vector<sFrequencyAndAmplitude>l_CurrentFFTFrequencyAndAmplitudeVector =	GetSatisfiedFrequencyAndAmplitudeVector(e_fFreqDistance,e_pData,e_iDataLength,m_iMinAmplitude);
	size_t l_uiSize = l_CurrentFFTFrequencyAndAmplitudeVector.size();
	if(l_uiSize == 0 )
		return;
	size_t l_uiWorkingSize = m_CurrentWorkingFrequenceAndTimeVector.size();
	if( l_uiWorkingSize == 0 )
	{
		for( size_t i=0;i<l_uiSize;++i )
		{
			sFrequenceAndAmplitudeAndTime l_FrequenceAndTime = FrequencyAndAmplitudeToFrequenceAndAmplitudeAndTime(&l_CurrentFFTFrequencyAndAmplitudeVector[i],this->m_fCurrentTime);
			m_CurrentWorkingFrequenceAndTimeVector.push_back(l_FrequenceAndTime);
		}
		return;
	}
	else
	{
		for(size_t j=0;j<l_CurrentFFTFrequencyAndAmplitudeVector.size();++j )
		{
			bool	l_bMatched = false;
			sFrequencyAndAmplitude*l_pFrequencyAndAmplitude = &l_CurrentFFTFrequencyAndAmplitudeVector[j];
			for(size_t i=0;i<m_CurrentWorkingFrequenceAndTimeVector.size();++i )
			{
				sFrequenceAndAmplitudeAndTime*l_pFrequenceAndAmplitudeAndTime = &m_CurrentWorkingFrequenceAndTimeVector[i];
				assert(l_bMatched == false &&"m_fFrequencyOffsetRange too big? how come!");
				if( l_bMatched )
					break;
				if( abs(l_pFrequenceAndAmplitudeAndTime->fFrequency-l_pFrequencyAndAmplitude->fFrequency) <= this->m_fFrequencyOffsetRange)
				{
					l_pFrequenceAndAmplitudeAndTime->fKeepTime += e_fElpaseTime;
					l_pFrequenceAndAmplitudeAndTime->fLastMuchTime = this->m_fCurrentTime;
					l_bMatched = true;
					//I am afraid something wrong so now just make break as comment
					//break;
				}
			}
			//new data!
			if( !l_bMatched )
			{
				sFrequenceAndAmplitudeAndTime l_FrequenceAndTime = FrequencyAndAmplitudeToFrequenceAndAmplitudeAndTime(&l_CurrentFFTFrequencyAndAmplitudeVector[j],this->m_fCurrentTime);
				m_CurrentWorkingFrequenceAndTimeVector.push_back(l_FrequenceAndTime);
			}
		}
	}
	//move overdue data
	FromCurrentWorkingToAllData();
}

void	cTimeFrequencyAmplitudeValueCapture::FromCurrentWorkingToAllData()
{
	for(size_t i=0;i<m_CurrentWorkingFrequenceAndTimeVector.size();++i )
	{
		sFrequenceAndAmplitudeAndTime*l_pFrequenceAndAmplitudeAndTime = &m_CurrentWorkingFrequenceAndTimeVector[i];
		//new data
		if( l_pFrequenceAndAmplitudeAndTime->fStartTime == l_pFrequenceAndAmplitudeAndTime->fLastMuchTime )
			continue;
		assert(this->m_fCurrentTime>=l_pFrequenceAndAmplitudeAndTime->fLastMuchTime&&"what happen!?FromCurrentWorkingToAllData");
		float l_fElpaseTime = this->m_fCurrentTime-l_pFrequenceAndAmplitudeAndTime->fLastMuchTime;
		if( l_fElpaseTime >= this->m_fTolerateTime )
		{//move to all data
			m_CurrentWorkingFrequenceAndTimeVector.erase(m_CurrentWorkingFrequenceAndTimeVector.begin()+i);
			this->m_AllData.push_back(*l_pFrequenceAndAmplitudeAndTime);
			--i;
		}
	}
}

bool	cTimeFrequencyAmplitudeValueCapture::ParseAndSaveFileName(const char*e_strParseFileName,const char*e_strOutputFileName)
{
	if(AnalyizeStart(e_strParseFileName))
	{
		if( this->m_AllData.size() )
		{
			ISAXCallback l_ISAXCallback;
			TiXmlDocument*l_pTiXmlDocument = new TiXmlDocument();
			TiXmlElement*l_pRootTiXmlElement = new TiXmlElement(L"Root");
			TiXmlElement*l_pConditionTiXmlElement = SaveConditionToTiXmlElement();
			l_pRootTiXmlElement->SetAttribute(L"ParseFileName",ValueToStringW(e_strParseFileName).c_str());
			l_pTiXmlDocument->LinkEndChild(l_pRootTiXmlElement);
			l_pRootTiXmlElement->LinkEndChild(l_pConditionTiXmlElement);
			for(auto l_FrequenceAndAmplitudeAndTime:m_AllData)
			{
				TiXmlElement*l_pTiXmlElement = l_FrequenceAndAmplitudeAndTime.ToElement();
				l_pRootTiXmlElement->LinkEndChild(l_pTiXmlElement);
			}
			l_ISAXCallback.SetDoc(l_pTiXmlDocument);
			l_ISAXCallback.Export(e_strOutputFileName,false);
			return true;
		}
	}
	return false;
}

TiXmlElement*	cTimeFrequencyAmplitudeValueCapture::SaveConditionToTiXmlElement()
{
	TiXmlElement*l_pTiXmlElement = new TiXmlElement();
	l_pTiXmlElement->SetAttribute(L"ParseFPS",m_iParseFPS);
	l_pTiXmlElement->SetAttribute(L"FrequencyOffsetRange",m_fFrequencyOffsetRange);
	l_pTiXmlElement->SetAttribute(L"TolerateTime",m_fTolerateTime);
	l_pTiXmlElement->SetAttribute(L"MinKeepTime",m_fMinKeepTime);
	l_pTiXmlElement->SetAttribute(L"MinAmplitude",m_iMinAmplitude);
	return l_pTiXmlElement;
}
#include "stdafx.h"
#include "KissFFTConvert.h"
#include "TimeFrequencyAmplitudeValueCapture.h"


sFrequenceAndAmplitudeAndTime::sFrequenceAndAmplitudeAndTime(TiXmlElement*e_pElement)
{
	fCompareKeepTime = 0.f;
	bMatched = false;
	PARSE_ELEMENT_START(e_pElement)
		COMPARE_ASSIGN_FLOAT("Frequency",fFrequency)
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
		COMPARE_NAME("LastMatchTime")
		{
			fLastMatchTime = VALUE_TO_FLOAT;
		}
	PARSE_NAME_VALUE_END
}

sFrequenceAndAmplitudeAndTime::sFrequenceAndAmplitudeAndTime(sFrequenceAndAmplitudeAndTime*e_pFrequenceAndAmplitudeAndTime)
{
	fFrequency = e_pFrequenceAndAmplitudeAndTime->fFrequency;
	iAmplitude = e_pFrequenceAndAmplitudeAndTime->iAmplitude;
	fStartTime = e_pFrequenceAndAmplitudeAndTime->fStartTime;
	fKeepTime = e_pFrequenceAndAmplitudeAndTime->fKeepTime;
	fCompareKeepTime = 0.f;
	fLastMatchTime = e_pFrequenceAndAmplitudeAndTime->fLastMatchTime;
	bMatched = false;
}

TiXmlElement*sFrequenceAndAmplitudeAndTime::ToElement()
{
	TiXmlElement*l_pTiXmlElement = new TiXmlElement(L"FrequenceAndAmplitudeAndTime");
	l_pTiXmlElement->SetAttribute(L"StartTime",fStartTime);
	l_pTiXmlElement->SetAttribute(L"KeepTime",fKeepTime);
	l_pTiXmlElement->SetAttribute(L"LastMatchTime",fLastMatchTime);
	l_pTiXmlElement->SetAttribute(L"Frequency",fFrequency);
	l_pTiXmlElement->SetAttribute(L"Amplitude",iAmplitude);
	return l_pTiXmlElement;
}

cTimeFrequencyAmplitudeValueCapture::cTimeFrequencyAmplitudeValueCapture()
{
	m_iMinAllowFrequency = 0;
	m_iMaxAllowFrequency = 44100;
	m_pKissFFTConvert = nullptr;
	m_piSoundDataForParse = nullptr;
	//
	m_iParseFPS = ONE_FRAME_NEED_NUM_FFT_DATA_COUNT;
	m_fCurrentTime = 0.f;
	//frequencty change could have a range offset
	//m_fFrequencyOffsetRange = cKissFFTConvertBase::GetFrequencyGapByFPS(44100,m_iParseFPS);
	//some times sample rate is possible not enough or missing some frequency
	//m_fTolerateTime = TOLERATE_TIME;
	m_fCaptureSoundRequireMinTime = 0.1f;
	m_iMinAmplitude = 80;
}

cTimeFrequencyAmplitudeValueCapture::~cTimeFrequencyAmplitudeValueCapture()
{
	SAFE_DELETE(m_piSoundDataForParse);
	SAFE_DELETE(m_pKissFFTConvert);
}

bool	cTimeFrequencyAmplitudeValueCapture::AnalyizeStart(const char*e_strFileName,int e_iFilterStregth,float e_fFilterEndFrequencyValue)
{
	SAFE_DELETE(m_pKissFFTConvert);
	SAFE_DELETE(m_piSoundDataForParse);
	m_AllData.clear();
	m_CurrentWorkingFrequenceAndTimeVector.clear();
	m_pKissFFTConvert = new cKissFFTConvert();
	m_pKissFFTConvert->SetFilter(true);
	m_pKissFFTConvert->SetiFrenquenceFilterEndScaleValue(e_fFilterEndFrequencyValue);
	m_pKissFFTConvert->SetFilterStrengthValue(e_iFilterStregth);
	m_pKissFFTConvert->SetDivideFFTDataToNFrame(m_iParseFPS);

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
		//
		int l_iOneFrameDataCount = m_pKissFFTConvert->GetOneFrameFFTDataCount();
		int l_iNumCount = (int)(l_uiFFTDAtaCount/l_iOneFrameDataCount);
		//m_fFrequencyOffsetRange = cKissFFTConvertBase::GetFrequencyGapByFPS(l_pSoundFile->m_iFreq,m_iParseFPS);
		for( int i=0;i<l_iNumCount;++i )
		{
			this->m_fCurrentTime = l_fFrameTimeElpase*i;
			int l_iIndex = i*l_iOneFrameDataCount;
			if( l_iIndex >= (int)l_uiFFTDAtaCount )
			{
				assert(0&&"fuck how come I a fucking angry...");
				continue;
			}
			int*l_piStartData = &m_piSoundDataForParse[i*l_iOneFrameDataCount];
			//FrameByFrameAnaylize(m_fFrequencyOffsetRange,l_piStartData,l_iOneFrameDataCount,l_fFrameTimeElpase);
			FrameByFrameAnaylize(l_piStartData,l_iOneFrameDataCount,l_fFrameTimeElpase);

		}
		return true;
	}
	return false;
}


std::vector<sFrequencyAndAmplitude>	cTimeFrequencyAmplitudeValueCapture::GetSatisfiedFrequencyAndAmplitudeVector(const int*e_pData,int e_iDataLength,int e_AmplitudeCondition)
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
	l_sFrequenceAndAmplitudeAndTime.fLastMatchTime = l_sFrequenceAndAmplitudeAndTime.fStartTime = e_fCurrentTime;
	return l_sFrequenceAndAmplitudeAndTime;
}

void	cTimeFrequencyAmplitudeValueCapture::FrameByFrameAnaylize(const int*e_pData,int e_iDataLength,float e_fElpaseTime)
{
	std::vector<sFrequencyAndAmplitude>l_CurrentFFTFrequencyAndAmplitudeVector =	GetSatisfiedFrequencyAndAmplitudeVector(e_pData,e_iDataLength,m_iMinAmplitude);
	size_t l_uiSize = l_CurrentFFTFrequencyAndAmplitudeVector.size();
	if(l_uiSize == 0 )
		return;
	size_t l_uiWorkingSize = m_CurrentWorkingFrequenceAndTimeVector.size();
	if( l_uiWorkingSize == 0 )
	{
		for( size_t i=0;i<l_uiSize;++i )
		{
			//assert(0&&"fuck");
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
			size_t l_uiCurrentWorkingFrequenceAndTimeVectorSize = m_CurrentWorkingFrequenceAndTimeVector.size();
			for(size_t i=0;i<l_uiCurrentWorkingFrequenceAndTimeVectorSize;++i )
			{
				sFrequenceAndAmplitudeAndTime*l_pFrequenceAndAmplitudeAndTime = &m_CurrentWorkingFrequenceAndTimeVector[i];
				//if( l_bMatched )
					//break;
				if( abs(l_pFrequenceAndAmplitudeAndTime->fFrequency-l_pFrequencyAndAmplitude->fFrequency) <= 1)
				{
					assert(l_bMatched == false &&"m_fFrequencyOffsetRange too big? how come!");
					l_pFrequenceAndAmplitudeAndTime->fKeepTime += e_fElpaseTime;
					l_pFrequenceAndAmplitudeAndTime->fLastMatchTime = this->m_fCurrentTime;
					l_bMatched = true;
					//I am afraid something wrong so now just make break as comment
					break;
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
		assert(this->m_fCurrentTime>=l_pFrequenceAndAmplitudeAndTime->fLastMatchTime&&"what happen!?FromCurrentWorkingToAllData");
		float l_fElpaseTime = this->m_fCurrentTime-l_pFrequenceAndAmplitudeAndTime->fLastMatchTime;
		if( l_fElpaseTime > 1.f/m_iParseFPS )
		{//move to all data

			m_CurrentWorkingFrequenceAndTimeVector.erase(m_CurrentWorkingFrequenceAndTimeVector.begin()+i);
			//here should do sort.
			//very stupid way to do this,but I dont care because I am lazy...
			size_t l_uiAllDataSize = this->m_AllData.size();
			size_t l_uiHittedIndex = l_uiAllDataSize-1;
			if( l_uiAllDataSize == 0 )
				l_uiHittedIndex = 0;
			for(size_t j=0;j<l_uiAllDataSize;++j)
			{
				if(l_pFrequenceAndAmplitudeAndTime->fStartTime < m_AllData[j].fStartTime)
				{
					l_uiHittedIndex = j;
					break;
				}
			}
			m_AllData.insert(m_AllData.begin()+l_uiHittedIndex,*l_pFrequenceAndAmplitudeAndTime);
			--i;
		}
	}
}

bool	cTimeFrequencyAmplitudeValueCapture::ParseAndSaveFileName(const char*e_strParseFileName,int e_iFilterStregth,float e_fFilterEndFrequencyValue,const char*e_strOutputFileName)
{
	if(AnalyizeStart(e_strParseFileName,e_iFilterStregth,e_fFilterEndFrequencyValue))
	{
		//dump all current working object ao all data
		size_t l_uiSize = this->m_CurrentWorkingFrequenceAndTimeVector.size();
		for(size_t i=0;i<l_uiSize;++i)
		{
			m_AllData.push_back(m_CurrentWorkingFrequenceAndTimeVector[i]);
		}
		m_CurrentWorkingFrequenceAndTimeVector.clear();
		if( this->m_AllData.size() )
		{
			ISAXCallback l_ISAXCallback;
			TiXmlDocument*l_pTiXmlDocument = new TiXmlDocument();
			TiXmlElement*l_pRootTiXmlElement = new TiXmlElement(L"Root");
			TiXmlElement*l_pConditionTiXmlElement = SaveConditionToTiXmlElement();
			l_pRootTiXmlElement->SetAttribute(CHAR_TO_WCHAR_DEFINE(SOUD_SOURCE_FILE_NAME),ValueToStringW(e_strParseFileName).c_str());
			l_pTiXmlDocument->LinkEndChild(l_pRootTiXmlElement);
			l_pRootTiXmlElement->LinkEndChild(l_pConditionTiXmlElement);
			//lazy find the amplitude value is bigger than average
			//float	l_fTotalValue = 0.f;
			//int		l_iAverageValue = 0;
			std::vector<sFrequenceAndAmplitudeAndTime> l_MatchedDataVector;
			//strip close frequency
			FATMING_CORE::cSoundFile*l_pSoundFile = m_pKissFFTConvert->GetSoundFile();
			int l_fFrequencyOffsetRange = (int)cKissFFTConvertBase::GetFrequencyGapByFPS(l_pSoundFile->m_iFreq,m_iParseFPS)*2;
			int l_iLastCloseFrequency = -79979;
			int   l_iLastBigAmplitude = 0;
			if( 1 )
			{//strip close frequency
				for(auto l_FrequenceAndAmplitudeAndTime:m_AllData)
				{
					if(l_FrequenceAndAmplitudeAndTime.fKeepTime >= m_fCaptureSoundRequireMinTime &&
						l_FrequenceAndAmplitudeAndTime.fFrequency >= this->m_iMinAllowFrequency &&
						l_FrequenceAndAmplitudeAndTime.fFrequency <= this->m_iMaxAllowFrequency)
					{
						if( l_iLastBigAmplitude <= l_FrequenceAndAmplitudeAndTime.iAmplitude || abs(l_iLastCloseFrequency - l_FrequenceAndAmplitudeAndTime.fFrequency) > l_fFrequencyOffsetRange )
						{
							l_iLastBigAmplitude = l_FrequenceAndAmplitudeAndTime.iAmplitude;
							l_MatchedDataVector.push_back(l_FrequenceAndAmplitudeAndTime);
							l_iLastCloseFrequency = (int)l_FrequenceAndAmplitudeAndTime.fFrequency;
							int l_iLastFrequencyForBack = l_iLastCloseFrequency;
							for( int i = l_MatchedDataVector.size()-2 ; i > 0; --i )
							{
								if(abs(l_iLastFrequencyForBack-l_MatchedDataVector[i].fFrequency)<=l_fFrequencyOffsetRange)
								{
									l_MatchedDataVector.erase(l_MatchedDataVector.begin()+i);
									break;
								}
								else
								{
									break;
								}
							}
						}
						else
						{
							l_iLastBigAmplitude = 0;
							l_iLastCloseFrequency = (int)l_FrequenceAndAmplitudeAndTime.fFrequency;					
						}
					}
				}
				for(auto l_FrequenceAndAmplitudeAndTime:l_MatchedDataVector)
				{
					TiXmlElement*l_pTiXmlElement = l_FrequenceAndAmplitudeAndTime.ToElement();
					l_pRootTiXmlElement->LinkEndChild(l_pTiXmlElement);
				}
			}
			else
			if( 0 )
			{//find average
				float l_fTotalValue = 0.f;
				int l_iAverageValue = 0;
				for(auto l_FrequenceAndAmplitudeAndTime:m_AllData)
				{
					if(l_FrequenceAndAmplitudeAndTime.fKeepTime >= m_fCaptureSoundRequireMinTime &&
						l_FrequenceAndAmplitudeAndTime.fFrequency >= this->m_iMinAllowFrequency &&
						l_FrequenceAndAmplitudeAndTime.fFrequency <= this->m_iMaxAllowFrequency)
					{
						l_fTotalValue += l_FrequenceAndAmplitudeAndTime.iAmplitude;
						l_MatchedDataVector.push_back(l_FrequenceAndAmplitudeAndTime);
						//TiXmlElement*l_pTiXmlElement = l_FrequenceAndAmplitudeAndTime.ToElement();
						//l_pRootTiXmlElement->LinkEndChild(l_pTiXmlElement);
					}
				}
				l_iAverageValue = (int)l_fTotalValue/l_MatchedDataVector.size();
				for(auto l_FrequenceAndAmplitudeAndTime:l_MatchedDataVector)
				{
					if(l_FrequenceAndAmplitudeAndTime.iAmplitude >= l_iAverageValue)
					{
						TiXmlElement*l_pTiXmlElement = l_FrequenceAndAmplitudeAndTime.ToElement();
						l_pRootTiXmlElement->LinkEndChild(l_pTiXmlElement);
					}
				}
			}
			else
			if( 0 )
			{//
				for(auto l_FrequenceAndAmplitudeAndTime:m_AllData)
				{
					if(l_FrequenceAndAmplitudeAndTime.fKeepTime >= m_fCaptureSoundRequireMinTime &&
						l_FrequenceAndAmplitudeAndTime.fFrequency >= this->m_iMinAllowFrequency &&
						l_FrequenceAndAmplitudeAndTime.fFrequency <= this->m_iMaxAllowFrequency)
					{
						TiXmlElement*l_pTiXmlElement = l_FrequenceAndAmplitudeAndTime.ToElement();
						l_pRootTiXmlElement->LinkEndChild(l_pTiXmlElement);
					}
				}		
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
	TiXmlElement*l_pTiXmlElement = new TiXmlElement(L"TimeFrequencyAmplitudeValueCapture");
	l_pTiXmlElement->SetAttribute(L"ParseFPS",m_iParseFPS);
	//l_pTiXmlElement->SetAttribute(L"FrequencyOffsetRange",m_fFrequencyOffsetRange);
	//l_pTiXmlElement->SetAttribute(L"TolerateTime",m_fTolerateTime);
	l_pTiXmlElement->SetAttribute(L"CaptureSoundRequireMinTime",m_fCaptureSoundRequireMinTime);
	l_pTiXmlElement->SetAttribute(L"MinAmplitude",m_iMinAmplitude);
	l_pTiXmlElement->SetAttribute(L"SourceFrequency",this->m_pKissFFTConvert->GetSoundFile()->m_iFreq);
	return l_pTiXmlElement;
}
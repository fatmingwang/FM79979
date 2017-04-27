#include "stdafx.h"
#include "KissFFTConvert.h"
#include "TimeFrequencyAmplitudeValueCapture.h"

cTimeFrequencyAmplitudeValueCapture::cTimeFrequencyAmplitudeValueCapture()
{
	m_pKissFFTConvert = nullptr;
	m_piSoundDataForParse = nullptr;
	//
	m_iParseFPS = ONE_FRAME_NEED_NUM_FFT_DATA_COUNT;
	m_fCurrentTime = 0.f;
	//frequencty change could have a range offset
	m_fOffsetRange = cKissFFTConvertBase::GetFrequencyGapByFPS(44100,m_iParseFPS);
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
		m_fOffsetRange = cKissFFTConvertBase::GetFrequencyGapByFPS(l_pSoundFile->m_iFreq,m_iParseFPS);
		for( int i=0;i<l_iNumCount;++i )
		{
			this->m_fCurrentTime = l_fFrameTimeElpase*i;
			int*l_piStartData = &m_piSoundDataForParse[i*l_iOneFrameDataCount];
			FrameByFrameAnaylize(m_fOffsetRange,l_piStartData,l_iOneFrameDataCount,l_fFrameTimeElpase);

		}
		return true;
	}
	return false;
}


std::vector<sFrequencyAndAmplitude>	GetSatisfiedFrequencyAndAmplitudeVector(float e_fFreqDistance,const int*e_pData,int e_iDataLength,int e_AmplitudeCondition)
{
	std::vector<sFrequencyAndAmplitude>	l_Result;
	return l_Result;
}

void	cTimeFrequencyAmplitudeValueCapture::FrameByFrameAnaylize(float e_fFreqDistance,const int*e_pData,int e_iDataLength,float e_fElpaseTime)
{
	std::vector<sFrequencyAndAmplitude>l_FrequencyAndAmplitudeVector =	GetSatisfiedFrequencyAndAmplitudeVector(e_fFreqDistance,e_pData,e_iDataLength,m_iMinAmplitude);
	size_t l_uiSize = l_FrequencyAndAmplitudeVector.size();
	if(l_uiSize == 0 )
		return;
	size_t l_uiWorkingSize = m_CurrentWorkingFrequenceAndTimeVector.size();
	if( l_uiWorkingSize == 0 )
	{
		for( size_t i=0;i<l_uiSize;++i )
		{
			sFrequenceAndTime l_FrequenceAndTime;
			l_FrequenceAndTime.fFrequency = l_FrequencyAndAmplitudeVector[i].fFrequency;
			l_FrequenceAndTime.iAmplitude = l_FrequencyAndAmplitudeVector[i].iAmplitude;
			l_FrequenceAndTime.fKeepTime = 0.f;
			m_AllData.push_back(l_FrequenceAndTime);
		}
		return;
	}
	l_FrequencyAndAmplitudeVector;
	//find matched data
	m_CurrentWorkingFrequenceAndTimeVector;
	//if all not match,and time range is out of range add to data vector
	//m_AllData.push_back();
}

bool	cTimeFrequencyAmplitudeValueCapture::ParseAndSaveFileName(const char*e_strParseFileName,const char*e_strOutputFileName)
{
	return false;
}

bool	cTimeFrequencyAmplitudeValueCapture::SaveFile(const char*e_strFileName)
{
	return false;
}
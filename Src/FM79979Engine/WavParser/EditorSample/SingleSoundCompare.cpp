#include "stdafx.h"
#include "SingleSoundCompare.h"
#include "FrequenceAndAmplitudeAndTimeFinder.h"

TYPDE_DEFINE_MARCO(cSindSoundCompare);

float	cSoundCompareParameter::m_sfTolerateTime = 0.32f;
int		cSoundCompareParameter::m_siAmplitudeOffset = 30;

cSindSoundCompare::cSindSoundCompare(sFrequenceAndAmplitudeAndTimeFinder*e_pData,float e_fCompareTime)
{
	//m_fErrorScore = 0.f;
	m_fResultScore = 0.f;
	m_fCompareTime = e_fCompareTime;
	m_pFrequenceAndAmplitudeAndTimeFinder = e_pData;
	m_iCurrentMatchedIndex = 0;
	m_iNumMatched = 0;
	m_bMustMatchAfterProior = false;
}

cSindSoundCompare::~cSindSoundCompare()
{

}

bool		cSindSoundCompare::Compare(float e_fCurrentTime,cQuickFFTDataFrequencyFinder*e_pQuickFFTDataFrequencyFinder)
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

bool		cSindSoundCompare::IsFinish(float e_fCurrentTime)
{
	float l_iEndSecond = this->m_pFrequenceAndAmplitudeAndTimeFinder->OneScondFrequenceAndAmplitudeAndTimeData.size()+this->m_fCompareTime;
	if( e_fCurrentTime-l_iEndSecond >= cSoundCompareParameter::m_sfTolerateTime )
	{
		return true;	
	}
	return false;
}
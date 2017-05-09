#include "stdafx.h"
#include "QuickFFTDataFrequencyFinder.h"


cQuickFFTDataFrequencyFinder::cQuickFFTDataFrequencyFinder(int e_iFFTBins,int e_iMaxFrequency)
{
	m_iFFTBinCount = e_iFFTBins;
	m_fFrequencyGap = e_iMaxFrequency/2.f/m_iFFTBinCount;
	m_piFFTData = nullptr;
}

cQuickFFTDataFrequencyFinder::~cQuickFFTDataFrequencyFinder()
{

}

std::vector<int>		cQuickFFTDataFrequencyFinder::GetAmplitude(int e_iFrequency)
{
	std::vector<int> l_Result;
	int l_iStep = (int)(e_iFrequency/m_fFrequencyGap);
	if( l_iStep < this->m_iFFTBinCount )
	{
		if( l_iStep == 0 )
			l_iStep = 1;
		if( l_iStep >= m_iFFTBinCount-1)
			l_iStep = m_iFFTBinCount-2;
		int l_iAmplitudeLeft = this->m_piFFTData[l_iStep-1];
		int l_iAmplitudeCenter = this->m_piFFTData[l_iStep];
		int l_iAmplitudeRight = this->m_piFFTData[l_iStep+1];
		l_Result.push_back(l_iAmplitudeLeft);
		l_Result.push_back(l_iAmplitudeCenter);
		l_Result.push_back(l_iAmplitudeRight);
	}
	return l_Result;
}
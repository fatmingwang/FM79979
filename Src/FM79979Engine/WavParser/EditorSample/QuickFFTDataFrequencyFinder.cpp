#include "stdafx.h"
#include "QuickFFTDataFrequencyFinder.h"


cQuickFFTDataFrequencyFinder::cQuickFFTDataFrequencyFinder(int e_iFFTBins,int e_iMaxFrequency)
{
	m_iFrequency = e_iMaxFrequency;
	m_iFFTBinCount = e_iFFTBins;
	m_fFrequencyGap = (float)e_iMaxFrequency/WINDOWN_FUNCTION_FRUSTRUM/m_iFFTBinCount;
	m_piFFTData = nullptr;
}

cQuickFFTDataFrequencyFinder::~cQuickFFTDataFrequencyFinder()
{

}

std::vector<int>		cQuickFFTDataFrequencyFinder::GetAmplitude(int e_iFrequency)
{
	std::vector<int> l_Result;
	//int l_iStep = (int)(e_iFrequency/WINDOWN_FUNCTION_FRUSTRUM/m_fFrequencyGap);
	int l_iStep = (int)(((float)e_iFrequency/m_iFrequency)*m_iFFTBinCount);
	if( l_iStep < this->m_iFFTBinCount )
	{
		int l_iNeighborsData = 2;
		if( l_iStep < l_iNeighborsData )
			l_iStep = l_iNeighborsData;
		if( l_iStep >= m_iFFTBinCount-l_iNeighborsData)
			l_iStep = m_iFFTBinCount-l_iNeighborsData;
		int l_iAmplitudeCenter = this->m_piFFTData[l_iStep];

		if( 1 )
		{
			l_Result.push_back(l_iAmplitudeCenter);
			for( int i=1;i<l_iNeighborsData;++i )
			{
				int l_iAmplitudeLeftSide = this->m_piFFTData[l_iStep-i];
				int l_iAmplitudeRightSide = this->m_piFFTData[l_iStep+i];
				l_Result.push_back(l_iAmplitudeLeftSide);
				l_Result.push_back(l_iAmplitudeRightSide);
			}
		}
		else
		{
			//int l_iAmplidute[3] = {l_iAmplitudeLeft,l_iAmplitudeCenter,l_iAmplitudeRight};
			//int l_iFinalResult = 0;
			//for( int i=0;i<3;++i)
			//{
			//	l_iFinalResult += l_iAmplidute[i];
			//	//l_iAmplidute[i]-(m_fFrequencyGap*l_iStep)
			//}
			//l_iFinalResult /= 3;
			//l_Result.push_back(l_iFinalResult);
		}
	}
	else
	{
		//how come!?
		int a = 0;
	}
	return l_Result;
}
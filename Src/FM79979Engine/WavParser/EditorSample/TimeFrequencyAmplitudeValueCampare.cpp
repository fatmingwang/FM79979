#include "stdafx.h"
#include "TimeFrequencyAmplitudeValueCampare.h"

//<FrequenceAndAmplitudeAndTime>
	//<///wewewewewe>
//</FrequenceAndAmplitudeAndTime>

cTimeFrequencyAmplitudeValueCompare::cTimeFrequencyAmplitudeValueCompare(float	e_fPreLoadTimeOffset,const char*e_strFileName)
{
	m_pSoundFFTCapture = nullptr;
	m_pSoundCapture = nullptr;
	m_pSoundFFTCapture = nullptr;
	m_fCurrentTime = 0.f;
	m_fPreLoadTimeOffset = e_fPreLoadTimeOffset;
	m_bFirstTimePreLoad = false;
	if( !this->ParseWithMyParse(e_strFileName) )
	{
		UT::ErrorMsg("file parse failed!",e_strFileName);
	}
}

cTimeFrequencyAmplitudeValueCompare::~cTimeFrequencyAmplitudeValueCompare()
{

}

bool	cTimeFrequencyAmplitudeValueCompare::MyParse(TiXmlElement*e_pRoot)
{
	e_pRoot;
	return true;
}

void	cTimeFrequencyAmplitudeValueCompare::SetInternalData(TiXmlElement*e_pRoot)
{

}

void	cTimeFrequencyAmplitudeValueCompare::StartCompare()
{
	this->m_fCurrentTime = 0.f;
}

void	cTimeFrequencyAmplitudeValueCompare::FetchPreBuildData(float e_fCurrentTime)
{

}

void	cTimeFrequencyAmplitudeValueCompare::SetCurrentTime(float e_fTargetTime)
{
	m_fCurrentTime = e_fTargetTime;
	//WaitForCompareData.clear();
	FetchPreBuildData(m_fCurrentTime);
}

void	cTimeFrequencyAmplitudeValueCompare::Update(float e_fElpaseTime)
{
	if( m_pSoundFFTCapture )
	{
		m_pSoundFFTCapture->Update(e_fElpaseTime);
	}
}

void	cTimeFrequencyAmplitudeValueCompare::AddNewData(sFrequencyAndAmplitude e_FrequencyAndAmplitude)
{

}

void	cTimeFrequencyAmplitudeValueCompare::SetupInputDataFFT(int e_iNumBins)
{

}
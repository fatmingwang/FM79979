#include "stdafx.h"
#include "SoundBufferSwaper.h"

cSoundFFTDataCalculator::cSoundFFTDataCalculator()
{
	m_iFPS = 0;
	m_iNewDataCount = 0;
	m_iOldDataCount = 0;
	m_iFrequency = 0;
	m_iNewSoundDataSize = 0;
	m_iOldSoundDataCountSize = 0;
	m_iOneUpdateNeedDataSize = 0;
}
cSoundFFTDataCalculator::~cSoundFFTDataCalculator()
{
	
}

#define DEFAULT_FFT_FPS		30
#define MIN_DO_FFT_FPS		10

void	cSoundFFTDataCalculator::SetupParameter(int e_iDoFFTFPS,int e_iFrequency,int e_iOldFFTCount)
{
	m_iFPS = e_iDoFFTFPS;
	if( m_iFPS > DEFAULT_FFT_FPS )
		m_iFPS = DEFAULT_FFT_FPS;
	if( m_iFPS < MIN_DO_FFT_FPS)
		m_iFPS = MIN_DO_FFT_FPS;
	m_iNewDataCount = DEFAULT_FFT_FPS/m_iFPS;
	m_iOldDataCount = e_iOldFFTCount;
	m_iFrequency = e_iFrequency;
	m_iOneUpdateNeedDataSize = m_iFrequency/m_iFPS*sizeof(short);
	//divide 
	int l_iNumSoundSampleInOneFrame = m_iNewDataCount+m_iOldDataCount;
	m_iNewSoundDataSize = m_iOneUpdateNeedDataSize/l_iNumSoundSampleInOneFrame*m_iNewDataCount;
	m_iOldSoundDataCountSize = m_iOneUpdateNeedDataSize-m_iNewSoundDataSize;
}

//OldDataCapcility higher get more accurate
//void	SetupParameter(int e_iTargetFPS,int e_iOldDataCapcility);
int		cSoundFFTDataCalculator::GetOneUpdateNeededFFTDataSize()
{
	return m_iOneUpdateNeedDataSize;
}

int		cSoundFFTDataCalculator::GetFFTBinCount()
{
	int l_iOneFPSFFTCOunt = m_iFrequency/m_iFPS;
	return l_iOneFPSFFTCOunt;
}

int     cSoundFFTDataCalculator::GetNewSoundDataSize()
{
	return m_iNewSoundDataSize;
}

int     cSoundFFTDataCalculator::GetOldSoundDataSize()
{
	return m_iOldSoundDataCountSize;
}


cSoundBufferSwaper::cSoundBufferSwaper()
{
	m_iTargetDataSize = 0;
	m_iRestDataSize = 0;
	m_pPreviousBuffer = nullptr;
	//SetupBuffer(e_iBufferCount,e_iBudderSize);
}

cSoundBufferSwaper::~cSoundBufferSwaper()
{
	SAFE_DELETE(m_pPreviousBuffer);
}

void	cSoundBufferSwaper::SetupBufferData(int e_iBufferCount,int e_iDoFFTFPS,int e_iFrequency,int e_iOldFFTCount)
{
	m_iTargetDataSize = 0;
	m_iRestDataSize = 0;
	m_SoundFFTDataCalculator.SetupParameter(e_iDoFFTFPS,e_iFrequency,e_iOldFFTCount);
	m_SoundFFTDataCalculator.GetFFTBinCount();
	m_SoundFFTDataCalculator.GetNewSoundDataSize();
	
	int l_iOneUpdateNeededFFTDataSize = m_SoundFFTDataCalculator.GetOneUpdateNeededFFTDataSize();
	m_SoundData.SetupBufferData(e_iBufferCount,l_iOneUpdateNeededFFTDataSize);
	m_WaitFetchData.SetupBufferData(e_iBufferCount,l_iOneUpdateNeededFFTDataSize);
	SAFE_DELETE(m_pPreviousBuffer);
	m_pPreviousBuffer = new char[m_SoundFFTDataCalculator.GetOldSoundDataSize()];
}

//0 still need data,1 data is filled,2 data is too many,need more buffer
int cSoundBufferSwaper::PushData(int e_iTargetDataSize,int e_iCurrentDataSize,char*e_pData,float e_fCurrentTime)
{
	assert(m_iTargetDataSize == e_iTargetDataSize && "it should not happen");
	//what!? the data is too big
	if( e_iTargetDataSize > m_iRestDataSize )
	{
	
	}
	//buffer filled
	if( m_iRestDataSize <= e_iCurrentDataSize )
	{
		m_SoundData;
	}
	else//need more buffer
	{
		
	}
	return true;
}
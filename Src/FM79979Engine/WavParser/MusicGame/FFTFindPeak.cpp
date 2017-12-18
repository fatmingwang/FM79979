#include "stdafx.h"
#include "FFTFindPeak.h"


	//struct sFFTBinData
	//{
	//	int				iPriorAmplitude;
	//	int				iCurrentAmplitude;
	//	int				iMaxAmplitude;
	//	float			fCurrentDirectionKeepTime;
	//	eMoveDirection	MoveDirection;
	//	bool			bFrequencyStrong;
	//};

cFFTBinStatus::sFFTBinData::sFFTBinData()
{
	memset(this,0,sizeof(cFFTBinStatus));
	eCurrentMoveDirection = eMD_MAX;
}


eMoveDirection	cFFTBinStatus::sFFTBinData::GetMoveDirectionByMoveDirectionHistory(int e_iMoveDirectionHistoryIndex)
{
	int l_iMaxAmplitude = 0;
	int l_iNumMatch = 0;
	for( int i=0;i<MOVE_DIRECTION_HISTORY_COUNT;++i )
	{
		int l_iIndex = UT::GetLoopIndex(e_iMoveDirectionHistoryIndex,MOVE_DIRECTION_HISTORY_COUNT);
		//if(l_eMoveDirection == eMoveDirectionHistory[l_iIndex])
		{
			++l_iNumMatch;
		}
		//else
		{

		}
	}
	return eMD_MAX;
}

cFFTBinStatus::cFFTBinStatus(int e_iNumFFTBins,int*e_pFFTSource)
{
	m_iNumFrame = 0;
	m_iNumFFTBin = e_iNumFFTBins;
	m_pFFTBinData = new sFFTBinData[e_iNumFFTBins];
	m_pFFTSource = e_pFFTSource;
}

cFFTBinStatus::~cFFTBinStatus()
{
	SAFE_DELETE(m_pFFTBinData);
}

void	cFFTBinStatus::Update(float e_fTime)
{
	int l_iMoveDirectionHistoryIndex = m_iNumFrame%MOVE_DIRECTION_HISTORY_COUNT;
	for( int i=0;i<m_iNumFFTBin;++i )
	{
		int l_iCurrentAmplitude = m_pFFTSource[i];
		int l_iPrioriAmplitude = m_pFFTBinData[i].iCurrentAmplitude;
		int l_iDiff = l_iCurrentAmplitude-l_iPrioriAmplitude;
		m_pFFTBinData[i].iTempAmplitudeDiffForMoveDirection += l_iDiff;
		m_pFFTBinData[i].iAmplitudeHistory[l_iMoveDirectionHistoryIndex] = l_iCurrentAmplitude;

	}
	++m_iNumFrame;
}
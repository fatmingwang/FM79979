#include "stdafx.h"
#include "WavInfo.h"
#include "WavWaves.h"


cWavWaves::cWavWaves()
{
	m_pData = nullptr;;
	m_iLength = 0;
	m_iDataBlock = 0;
	m_LinesPoint.reserve(10000);
}

cWavWaves::~cWavWaves()
{
}

void	cWavWaves::SetData(cWaveInfo*e_pWaveInfo)
{
	e_pWaveInfo->m_WAVChunkHdr_Struct;
	m_LinesPoint.clear();
	//m_pData = e_pData;
	//m_iLength = e_iLength;
	//m_iDataBlock = e_iDataBlock;
	//unsigned char*l_pData = e_pData;

	//
	for(int i=0;i<m_iLength;++i)
	{
		//m_pData;
		//l_pData += m_iDataBlock;
	}
}

void	cWavWaves::Render()
{
//void	RenderLine(float*e_pPosition,int e_iNumPoints,Vector4 e_vColor,int e_iStride,float*e_pfMatrix,float e_fLineSize)
}
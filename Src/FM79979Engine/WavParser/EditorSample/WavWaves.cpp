#include "stdafx.h"
#include "WavInfo.h"
#include "WavWaves.h"


void	cWavWaves::sChannelCurve::AssignData(unsigned char*e_pData,int e_iCount,int e_iDataStep)
{
	assert(e_iDataStep == 4&&"sorry only support 4 byte sound data");
	iCount = e_iCount;
	const int l_iGapX = 10;
	LinesPoint.resize(e_iCount);
	unsigned char*l_pData = e_pData;
	this->iMax = 0;
	//for(int i=0;i<150;++i)
	for(int i=0;i<e_iCount/1000;++i)
	{
		int *l_piData = (int*)l_pData;
		int l_iInt = *l_piData;
		//l_iInt = sqrt(abs(l_iInt));
		//l_iInt = sqrt(l_iInt);
		//l_iInt /= 65535;
		Vector3 l_vPos((float)l_iGapX*i,(float)*l_piData,0);
		//OutputDebugStringW(ValueToStringW(l_iInt).c_str());
		//OutputDebugStringW(L"\n");
		LinesPoint[i] = l_vPos;
		l_pData += e_iDataStep/4;
		if((int)*l_piData > this->iMax )
			iMax = (int)*l_piData;
	}
}

void	cWavWaves::sChannelCurve::SetAverageAndMax(int e_iMax,int e_iAverage)
{
	this->iAverage = e_iAverage;
	this->iMax = e_iMax;
}

void	cWavWaves::sChannelCurve::Render(Vector2 e_vPos)
{
	auto l_Mat = cMatrix44::TranslationMatrix(e_vPos.x,e_vPos.y,0);
	std::vector<Vector2> l_Pos;
	Vector4 l_vColor;
	//for X
	l_Pos.push_back(Vector2(0,0));
	l_Pos.push_back(Vector2(iCount,0));
	//for Y
	l_Pos.push_back(Vector2(0,0));
	l_Pos.push_back(Vector2(0,iMax));
	GLRender::RenderLine(&l_Pos,l_vColor,l_Mat);

	GLRender::RenderLine(&LinesPoint,l_vColor,l_Mat);
}

cWavWaves::cWavWaves()
{

}

cWavWaves::~cWavWaves()
{
}

void	cWavWaves::SetData(cWaveInfo*e_pWaveInfo)
{
	this->m_pWaveInfo = e_pWaveInfo;
	int	l_iMax = 0;
	for(int i=0;i<this->m_pWaveInfo->m_WAVFmtHdr_Struct.Channels;++i)
	{
		sChannelCurve*l_pData = new sChannelCurve();
		l_pData->AssignData(e_pWaveInfo->GetChannelData(i),e_pWaveInfo->m_iSampleCount,4);
		if( l_iMax <= l_pData->iMax )
			l_iMax = l_pData->iMax;
		m_ChannelCurveVector.push_back(l_pData);
	}
	for(int i=0;i<this->m_pWaveInfo->m_WAVFmtHdr_Struct.Channels;++i)
	{
		sChannelCurve*l_pData = m_ChannelCurveVector[i];
		l_pData->SetAverageAndMax(l_iMax,l_iMax/2);
	}
	
}

void	cWavWaves::Render()
{
	for(size_t i=0;i<m_ChannelCurveVector.size();++i)
	{
		if( i==0 )
			m_ChannelCurveVector[i]->Render(Vector2(0,0));
	}

}
#include "stdafx.h"
#include "FFTFrequency.h"
#include "Parameters.h"

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



cFFTDataStore::cFFTHitCountAndTime::cFFTHitCountAndTime(float e_fStartTime,int e_iFFTBinCount)
{
	m_iFFTBinCount = e_iFFTBinCount;
	m_fStartTime = e_fStartTime;
	m_pHittedCountArray = new int[e_iFFTBinCount];
	memset(m_pHittedCountArray,0,sizeof(int)*e_iFFTBinCount);
	m_pHittedValueArray = new int[e_iFFTBinCount];
	memset(m_pHittedValueArray,0,sizeof(int)*e_iFFTBinCount);
}

cFFTDataStore::cFFTHitCountAndTime::~cFFTHitCountAndTime()
{
	SAFE_DELETE(m_pHittedValueArray);
	SAFE_DELETE(m_pHittedCountArray);
}


cFFTDataStore::cFFTDataStore()
{
	//because I am lazy lah
	m_fNextDataTimeGap = 999.f;
	m_fCurrentTime = 0.f;
	m_pCurrentFFTHitCountAndTime = nullptr;
	this->m_vShowPos = Vector2(100,1200);
	this->m_vResolution.x = 1800;
	this->m_vResolution.y = 600;
	m_iHittedCountScaleForVisual = 3;
	m_iMaxValue = 0;
	m_iExportThresholdValue = cSoundCompareParameter::m_siFFTStoreThresholeValue;
	//m_iThreusholdAmplitude = cSoundCompareParameter::m_siDebugAmplitudeValue;
}

cFFTDataStore::~cFFTDataStore()
{
	Destroy();
}


void	cFFTDataStore::Destroy()
{
	//SAFE_DELETE(m_pCurrentFFTHitCountAndTime);
	DELETE_VECTOR(m_FFTHitCountAndTimeVector,cFFTHitCountAndTime*);
}

void	cFFTDataStore::Start()
{
	Destroy();
	m_fRestNextDataTimeGap = 0.f;
}

void	cFFTDataStore::UpdateFFTData(float e_fElpaseTime,int*e_piFFTData,int e_iCount)
{
	m_fCurrentTime += e_fElpaseTime;
	m_fRestNextDataTimeGap -= e_fElpaseTime;
	if( m_fRestNextDataTimeGap <= 0.f )
	{
		m_fRestNextDataTimeGap = m_fNextDataTimeGap+m_fRestNextDataTimeGap;
		//SAFE_DELETE(m_pCurrentFFTHitCountAndTime);
		cFFTHitCountAndTime*l_pNewFFTHitCountAndTime = new cFFTHitCountAndTime(m_fCurrentTime,e_iCount);
		m_pCurrentFFTHitCountAndTime = l_pNewFFTHitCountAndTime;
		this->m_FFTHitCountAndTimeVector.push_back(l_pNewFFTHitCountAndTime);
		m_iMaxValue = 0;
	}
	for( int i=0;i<e_iCount;++i )
	{
		if( e_piFFTData[i] >= cSoundCompareParameter::m_siDebugAmplitudeValue )
		{
			m_pCurrentFFTHitCountAndTime->m_pHittedCountArray[i] += 1;
			m_pCurrentFFTHitCountAndTime->m_pHittedValueArray[i] += e_piFFTData[i];
			if(m_iMaxValue <m_pCurrentFFTHitCountAndTime->m_pHittedCountArray[i] )
				m_iMaxValue = m_pCurrentFFTHitCountAndTime->m_pHittedCountArray[i];
		}
	}
}

void	cFFTDataStore::RenderFFTHitCountAndTime(cFFTHitCountAndTime*e_pFFTHitCountAndTime)
{
	if( e_pFFTHitCountAndTime )
	{
		int l_iFFTBinCount = e_pFFTHitCountAndTime->m_iFFTBinCount;
		float l_fGap = this->GetWidthGapByPoints(e_pFFTHitCountAndTime->m_iFFTBinCount);
		Vector2 l_vShowPos = this->m_vShowPos;
		for( int i=0;i<l_iFFTBinCount;++i )
		{
			m_vLineTempPos[i*2] = l_vShowPos;
			m_vLineTempPos[i*2+1] = l_vShowPos;
			m_vLineTempPos[i*2+1].y -= e_pFFTHitCountAndTime->m_pHittedCountArray[i]*m_iHittedCountScaleForVisual;
			l_vShowPos.x += l_fGap;
		}
		GLRender::RenderLine((float*)&m_vLineTempPos[0],l_iFFTBinCount*2,Vector4::One,2);
		std::wstring l_strInfo = L"RestTime:";
		l_strInfo += ValueToStringW(m_fRestNextDataTimeGap);
		l_strInfo += L"\nMaxValue:";
		l_strInfo += ValueToStringW(m_iMaxValue);
		cGameApp::RenderFont(this->m_vShowPos.x,this->m_vShowPos.y+200,l_strInfo);

		if(cGameApp::m_sbDebugFunctionWorking)
		{
			Vector2	l_vChartResolution = this->m_vResolution;
			Vector2 l_vLinePos[2];
			l_vLinePos[0] = this->m_vShowPos;
			l_vLinePos[0].y -= m_iExportThresholdValue*m_iHittedCountScaleForVisual;

			l_vLinePos[1] = l_vLinePos[0];
			l_vLinePos[1].x += l_vChartResolution.x;
			RenderLine((float*)&l_vLinePos,2,Vector4::Green,2);
		}
	}
}

void	cFFTDataStore::RenderCurrentData()
{
	RenderFFTHitCountAndTime(this->m_pCurrentFFTHitCountAndTime);
}

int	cFFTDataStore::GetExportThresholdValue()
{
	return m_iExportThresholdValue;
}

void	cFFTDataStore::SetExportThresholdValue(int e_iExportThresholdValue)
{
	m_iExportThresholdValue = e_iExportThresholdValue;
}

void	cFFTDataStore::RenderByTime(float e_fTargetTime)
{
	cFFTHitCountAndTime*l_pTargetcFFTHitCountAndTime = nullptr;
	size_t l_uiSize = m_FFTHitCountAndTimeVector.size();
	for( size_t i=0;i<l_uiSize;++i )
	{
		if(e_fTargetTime <= m_FFTHitCountAndTimeVector[i]->m_fStartTime )
		{
			l_pTargetcFFTHitCountAndTime = m_FFTHitCountAndTimeVector[i];
			break;
		}
	}
	if( l_pTargetcFFTHitCountAndTime )
	{
		l_pTargetcFFTHitCountAndTime;
	}
}

bool	cFFTDataStore::Export(const char*e_strFileName,const char*e_strOriginalSourceFileName,int e_iFrequency,int e_iDecibleThreshold)
{
	size_t l_uiSize = m_FFTHitCountAndTimeVector.size();
	if( l_uiSize == 0 )
		return false;
	int l_iFFTCount = m_FFTHitCountAndTimeVector[0]->m_iFFTBinCount;
	ISAXCallback l_ISAXCallback;
	TiXmlDocument*l_pTiXmlDocument = new TiXmlDocument();
	TiXmlElement*l_pRootTiXmlElement = new TiXmlElement(L"Root");
	l_pTiXmlDocument->LinkEndChild(l_pRootTiXmlElement);
	l_ISAXCallback.SetDoc(l_pTiXmlDocument);
	l_pRootTiXmlElement->SetAttribute(L"Frequency",e_iFrequency);
	l_pRootTiXmlElement->SetAttribute(L"SoundFile",ValueToStringW(e_strOriginalSourceFileName).c_str());
	l_pRootTiXmlElement->SetAttribute(L"FFTCount",l_iFFTCount);
	l_pRootTiXmlElement->SetAttribute(L"DecibleThreshole",e_iDecibleThreshold);
	float l_fFrequenctForOneFFT = (float)e_iFrequency/l_iFFTCount;
	for(size_t i=0;i<l_uiSize;++i)
	{
		cFFTHitCountAndTime*l_pFFTHitCountAndTime = m_FFTHitCountAndTimeVector[i];
		std::vector<int>	l_FrequencyVector;
		std::vector<int>	l_FrequencyHittedCountVector;
		std::vector<int>	l_FrequencyHittedValueVector;
		for( int l_iCurrentFFTIndex = 0;l_iCurrentFFTIndex<l_pFFTHitCountAndTime->m_iFFTBinCount;++l_iCurrentFFTIndex )
		{
			int l_iCount = l_pFFTHitCountAndTime->m_pHittedCountArray[l_iCurrentFFTIndex];
			if(l_iCount >=this->m_iExportThresholdValue)
			{
				int l_iFrequency = (int)(l_fFrequenctForOneFFT*l_iCurrentFFTIndex);
				l_FrequencyVector.push_back(l_iFrequency);
				l_FrequencyHittedCountVector.push_back(l_iCount);
				int l_iAeverageValue = l_pFFTHitCountAndTime->m_pHittedValueArray[l_iCurrentFFTIndex]/l_iCount;
				l_FrequencyHittedValueVector.push_back(l_iAeverageValue);
			}
		}
		TiXmlElement*l_pFFTHitCountAndTimeElement = new TiXmlElement(L"FFTHitCountAndTime");
		l_pFFTHitCountAndTimeElement->SetAttribute(L"StartTime",l_pFFTHitCountAndTime->m_fStartTime);
		l_pFFTHitCountAndTimeElement->SetAttribute(L"FrequencyVector",ValueToStringW(l_FrequencyVector).c_str());
		l_pFFTHitCountAndTimeElement->SetAttribute(L"FrequencyHittedCount",ValueToStringW(l_FrequencyHittedCountVector).c_str());
		l_pFFTHitCountAndTimeElement->SetAttribute(L"FrequencyHittedValue",ValueToStringW(l_FrequencyHittedValueVector).c_str());
		l_pRootTiXmlElement->LinkEndChild(l_pFFTHitCountAndTimeElement);
	}
	l_ISAXCallback.Export(e_strFileName,false);
	return false;
}
//<Root ParseFileName="C:\Users\leeyo\Desktop\FM79979\Media\MusicGame\Piano\a2m.wav">
//    <TimeFrequencyAmplitudeValueCapture ParseFPS="60" CaptureSoundRequireMinTime="0.10000" MinAmplitude="80" SourceFrequency="11025" />
//</Root>
bool	cFFTDataStore::MyParse(TiXmlElement*e_pRoot)
{
	return false;
}
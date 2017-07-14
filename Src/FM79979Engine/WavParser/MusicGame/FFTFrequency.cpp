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

std::vector<int>		cQuickFFTDataFrequencyFinder::GetDecibelsByFrequency(int e_iFrequency)
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



cFFTDecibelsAnalyzer::cFFTHitCountAndTime::cFFTHitCountAndTime(float e_fStartTime,int e_iFFTBinCount)
{
	m_iFFTBinCount = e_iFFTBinCount;
	m_fStartTime = e_fStartTime;
	m_pHittedCountArray = new int[e_iFFTBinCount];
	m_pHittedValueArray = new int[e_iFFTBinCount];
	memset(m_pHittedCountArray,0,sizeof(int)*e_iFFTBinCount);
	memset(m_pHittedValueArray,0,sizeof(int)*e_iFFTBinCount);
	m_pFFTFrequencyDecibels = new cFFTFrequencyDecibels(m_iFFTBinCount);
}

cFFTDecibelsAnalyzer::cFFTHitCountAndTime::~cFFTHitCountAndTime()
{
	SAFE_DELETE(m_pFFTFrequencyDecibels);
	SAFE_DELETE(m_pHittedValueArray);
	SAFE_DELETE(m_pHittedCountArray);
}


cFFTDecibelsAnalyzer::cFFTFrequencyDecibels::cFFTFrequencyDecibels(int e_iFFTCount)
{
	e_iFFTCount /= 2;
	m_piTotalDecibleValue = new int[e_iFFTCount]; 
	m_piAverageDecibleValue = new int[e_iFFTCount];
	m_piDecibleBiggetThanZeroCount =  new int[e_iFFTCount];
	memset(m_piTotalDecibleValue,0,sizeof(int)*e_iFFTCount);
	memset(m_piAverageDecibleValue,0,sizeof(int)*e_iFFTCount);
	memset(m_piDecibleBiggetThanZeroCount,0,sizeof(int)*e_iFFTCount);
	m_iFFTCount = e_iFFTCount;
	m_iUpdateCount = 0;
	this->m_vShowPos.x = 100;
	this->m_vShowPos.y = 700;
	this->m_vResolution.x = 3600;
	this->m_vResolution.y = 300;
}

cFFTDecibelsAnalyzer::cFFTFrequencyDecibels::~cFFTFrequencyDecibels()
{
	SAFE_DELETE(m_piTotalDecibleValue);
	SAFE_DELETE(m_piAverageDecibleValue);
	SAFE_DELETE(m_piDecibleBiggetThanZeroCount);
}

void	cFFTDecibelsAnalyzer::cFFTFrequencyDecibels::Update(int e_iCount,int*e_piFFTCount)
{
	e_iCount /= 2;
	assert(e_iCount == this->m_iFFTCount);
	++m_iUpdateCount;
	for( int i=0;i<this->m_iFFTCount;++i )
	{
		int l_iValue = e_piFFTCount[i];
		if( l_iValue > 0 )
		{
			++m_piDecibleBiggetThanZeroCount[i];
			this->m_piTotalDecibleValue[i] += l_iValue;
		}
		if( 1 )
		{
			this->m_piAverageDecibleValue[i] = this->m_piTotalDecibleValue[i]/m_iUpdateCount;
			//if( this->m_piAverageDecibleValue[i] == 0 )
				//this->m_piAverageDecibleValue[i] = -5;
			if(this->m_piAverageDecibleValue[i] == 0)
				this->m_piAverageDecibleValue[i] = -5;
			if( this->m_piAverageDecibleValue[i] >= cSoundCompareParameter::m_siDebugAmplitudeValue )
			{
				int a=0;
			}
		}
		else
		{
			if( m_piDecibleBiggetThanZeroCount[i] > 0 )
				this->m_piAverageDecibleValue[i] = this->m_piTotalDecibleValue[i]/m_piDecibleBiggetThanZeroCount[i];
			else
				this->m_piAverageDecibleValue[i] = 0;
		}
	}
}
void	cFFTDecibelsAnalyzer::cFFTFrequencyDecibels::Render(int e_iThresholdValue)
{
	Vector4 l_vColor = Vector4(1,1,0,0.3f);
	int l_iScaleValue = 10;
	Vector2 l_LazyPoints[44100/6*2];
	RenderWithData(m_vShowPos,m_vResolution,m_piAverageDecibleValue,m_iFFTCount,l_LazyPoints,l_vColor,l_iScaleValue*-1.f);

	Vector2	l_vChartResolution = this->m_vResolution;
	Vector2 l_vLinePos[2];
	l_vLinePos[0] = this->m_vShowPos;
	l_vLinePos[0].y -= e_iThresholdValue*l_iScaleValue;

	l_vLinePos[1] = l_vLinePos[0];
	l_vLinePos[1].x += l_vChartResolution.x;
	RenderLine((float*)&l_vLinePos,2,l_vColor,2);
	//RenderWithData(m_vShowPos,m_vResolution,m_piAverageDecibleValue,m_iFFTCount,l_LazyPoints);
	//RenderWithData(m_vShowPos,m_vResolution,m_piAverageDecibleValue,m_iFFTCount,l_LazyPoints);
}

cFFTDecibelsAnalyzer::cFFTDecibelsAnalyzer()
{
	sFilterData l_sFilterData = {768,8};
	m_FilterDataVector.push_back(l_sFilterData);
	m_iMouseMoveFreq = -1;
	m_iMouseMoveFreqFFTBinIndex = -1;
	m_iMouseMoveFreqDecibelAverage = -1;
	m_iFrequency = 44100;
	//why 999?because I am lazy lah
	m_fNextDataTimeGap = 999.f;
	m_fCurrentTime = 0.f;
	m_pCurrentFFTHitCountAndTime = nullptr;
	this->m_vShowPos = Vector2(100,1200);
	this->m_vResolution.x = 3600;
	this->m_vResolution.y = 600;
	m_iHittedCountScaleForVisual = 1;
	m_iMaxValue = 0;
	m_iExportThresholdValue = cSoundCompareParameter::m_siFFTStoreThresholeValue;
	//m_iThreusholdAmplitude = cSoundCompareParameter::m_siDebugAmplitudeValue;
}

cFFTDecibelsAnalyzer::~cFFTDecibelsAnalyzer()
{
	Destroy();
}



void	cFFTDecibelsAnalyzer::Destroy()
{
	//SAFE_DELETE(m_pCurrentFFTHitCountAndTime);
	DELETE_VECTOR(m_FFTHitCountAndTimeVector,cFFTHitCountAndTime*);
}

void	cFFTDecibelsAnalyzer::Start()
{
	Destroy();
	m_fRestNextDataTimeGap = 0.f;
}

void	cFFTDecibelsAnalyzer::UpdateFFTData(float e_fElpaseTime,int*e_piFFTData,int e_iCount)
{
	m_fCurrentTime += e_fElpaseTime;
	m_fRestNextDataTimeGap -= e_fElpaseTime;
	if( m_fRestNextDataTimeGap <= 0.f )
	{
		m_fRestNextDataTimeGap = m_fNextDataTimeGap+m_fRestNextDataTimeGap;
		//SAFE_DELETE(m_pCurrentFFTHitCountAndTime);
		cFFTHitCountAndTime*l_pNewFFTHitCountAndTime = new cFFTHitCountAndTime(m_fCurrentTime,e_iCount);
		l_pNewFFTHitCountAndTime->m_pFFTFrequencyDecibels->SetShowPos(this->m_vShowPos);
		l_pNewFFTHitCountAndTime->m_pFFTFrequencyDecibels->SetResolution(this->m_vResolution);
		m_pCurrentFFTHitCountAndTime = l_pNewFFTHitCountAndTime;
		this->m_FFTHitCountAndTimeVector.push_back(l_pNewFFTHitCountAndTime);
		m_iMaxValue = 0;
	}
	if( m_pCurrentFFTHitCountAndTime )
	{
		float l_fFrequenctForOneFFT = (float)this->m_iFrequency/e_iCount;

		size_t l_uiSize = m_FilterDataVector.size();
		int l_iCurrentIndex = 0;
		sFilterData*l_pFilterData = nullptr;
		if( l_uiSize > 0 )
			l_pFilterData = &m_FilterDataVector[l_iCurrentIndex];

		for( int i=0;i<e_iCount;++i )
		{
			int l_iCurrentFrequency = (int)(l_fFrequenctForOneFFT*i);
			int l_iDecibelsThresholdValue = cSoundCompareParameter::m_siDebugAmplitudeValue;

			int l_iNexdIndex = l_iCurrentIndex+1;
			if( (int)l_uiSize > l_iNexdIndex )
			{
				sFilterData*l_pNewFilterData = &m_FilterDataVector[l_iNexdIndex];
				if(l_pNewFilterData->iFrequencyThresholdStart >= l_iCurrentFrequency)
				{
					l_pFilterData = &m_FilterDataVector[l_iNexdIndex];
				}
			}
			if( l_pFilterData && l_iCurrentFrequency >= l_pFilterData->iFrequencyThresholdStart )
			{
				l_iDecibelsThresholdValue = l_pFilterData->iFrequencyDecibelsThreshold;
			}
			if( e_piFFTData[i] >= l_iDecibelsThresholdValue )
			{
				m_pCurrentFFTHitCountAndTime->m_pHittedCountArray[i] += 1;
				m_pCurrentFFTHitCountAndTime->m_pHittedValueArray[i] += e_piFFTData[i];
				if(m_iMaxValue <m_pCurrentFFTHitCountAndTime->m_pHittedCountArray[i] )
					m_iMaxValue = m_pCurrentFFTHitCountAndTime->m_pHittedCountArray[i];
			}
		}
		if( m_pCurrentFFTHitCountAndTime->m_pFFTFrequencyDecibels )
			m_pCurrentFFTHitCountAndTime->m_pFFTFrequencyDecibels->Update(e_iCount,e_piFFTData);
	}
	if( cGameApp::m_sucKeyData['Z'] )
	{
		this->m_vResolution.x *= 2;
	}

	if( cGameApp::m_sucKeyData['X'] )
	{
		this->m_vResolution.x /= 2;
	}
	if( cGameApp::m_sucKeyData['C'] )
	{
		this->m_vShowPos.x += this->m_vResolution.x/30;
	}
	if( cGameApp::m_sucKeyData['V'] )
	{
		this->m_vShowPos.x -= this->m_vResolution.x/30;
	}
	if( m_pCurrentFFTHitCountAndTime->m_pFFTFrequencyDecibels )
	{
		m_pCurrentFFTHitCountAndTime->m_pFFTFrequencyDecibels->SetResolution(this->m_vResolution);
		m_pCurrentFFTHitCountAndTime->m_pFFTFrequencyDecibels->SetShowPos(m_vShowPos);
	}
}

void	cFFTDecibelsAnalyzer::RenderFFTHitCountAndTime(cFFTHitCountAndTime*e_pFFTHitCountAndTime)
{
	if( e_pFFTHitCountAndTime )
	{
		int l_iFFTBinCount = e_pFFTHitCountAndTime->m_iFFTBinCount;
		//have no idea...while I jump time
		if( l_iFFTBinCount < 0 || l_iFFTBinCount > 100000 )
			return;
		float l_fGap = this->GetWidthGapByPoints(e_pFFTHitCountAndTime->m_iFFTBinCount);
		Vector2 l_vShowPos = this->m_vShowPos;
		if( 0 )
		{
			for( int i=0;i<l_iFFTBinCount;++i )
			{
				m_vLineTempPos[i*2] = l_vShowPos;
				m_vLineTempPos[i*2+1] = l_vShowPos;
				m_vLineTempPos[i*2+1].y -= e_pFFTHitCountAndTime->m_pHittedCountArray[i]*m_iHittedCountScaleForVisual;
				l_vShowPos.x += l_fGap;
			}
			GLRender::RenderLine((float*)&m_vLineTempPos[0],l_iFFTBinCount*2,Vector4::One,2);

			if(cGameApp::m_sbDebugFunctionWorking)
			{
				Vector2	l_vChartResolution = this->m_vResolution;
				Vector2 l_vLinePos[2];
				l_vLinePos[0] = this->m_vShowPos;
				l_vLinePos[0].y -= m_iExportThresholdValue*m_iHittedCountScaleForVisual;

				l_vLinePos[1] = l_vLinePos[0];
				l_vLinePos[1].x += l_vChartResolution.x;
				RenderLine((float*)&l_vLinePos,2,Vector4::Green,2);

				size_t l_uiSize = m_FilterDataVector.size();
				for( size_t i=0;i<l_uiSize;++i )
				{
					auto l_pFilterDataVector = &m_FilterDataVector[i];
					Vector2	l_vChartResolution = this->m_vResolution;
					Vector2 l_vLinePos[2];
					l_vLinePos[0] = this->m_vShowPos;
					l_vLinePos[0].x += (float)l_pFilterDataVector->iFrequencyThresholdStart;
					l_vLinePos[0].y -= l_pFilterDataVector->iFrequencyDecibelsThreshold *m_iHittedCountScaleForVisual;

					l_vLinePos[1] = l_vLinePos[0];
					l_vLinePos[1].x += l_vChartResolution.x-l_pFilterDataVector->iFrequencyThresholdStart;
					RenderLine((float*)&l_vLinePos,2,Vector4::Red,2);			
				}
			}
		}

		std::wstring l_strInfo = L"RestTime:";
		l_strInfo += ValueToStringW(m_fRestNextDataTimeGap);
		l_strInfo += L"\nMaxValue:";
		l_strInfo += ValueToStringW(m_iMaxValue);
		cGameApp::RenderFont(this->m_vShowPos.x,this->m_vShowPos.y+200,l_strInfo);

		if( m_pCurrentFFTHitCountAndTime->m_pFFTFrequencyDecibels )
			this->m_pCurrentFFTHitCountAndTime->m_pFFTFrequencyDecibels->Render(this->m_iExportThresholdValue);
	}
}

void	cFFTDecibelsAnalyzer::RenderMouseMoveInfo()
{
	if(this->m_iMouseMoveFreq != -1)
	{
		std::wstring l_str = L"Freq:";
		l_str += ValueToStringW(m_iMouseMoveFreq);
		l_str += L"\nDecibelsAverage:";
		l_str += ValueToStringW(this->m_iMouseMoveFreqDecibelAverage);
		l_str += L"\nIndex:";
		l_str += ValueToStringW(this->m_iMouseMoveFreqFFTBinIndex);
		Vector2 l_vPos = this->m_vShowPos;
		l_vPos.y += 100;
		l_vPos.x = 100;
		cGameApp::RenderFont(l_vPos,l_str.c_str());
	}
}

void	cFFTDecibelsAnalyzer::RenderCurrentData()
{
	RenderFFTHitCountAndTime(this->m_pCurrentFFTHitCountAndTime);
	RenderMouseMoveInfo();
}

int	cFFTDecibelsAnalyzer::GetExportThresholdValue()
{
	return m_iExportThresholdValue;
}

void	cFFTDecibelsAnalyzer::MouseMove(int e_iMousePosX,int e_iMousePosY)
{
	float l_fEndPosX = m_vShowPos.x+this->m_vResolution.x;
	m_iMouseMoveFreq = -1;
	m_iMouseMoveFreqDecibelAverage = -1;
	m_iMouseMoveFreqFFTBinIndex = -1;
	//if( e_iMousePosX <= l_fEndPosX && e_iMousePosX >= m_vShowPos.x)
	{
		float l_fDis = e_iMousePosX-m_vShowPos.x;
		float l_fLERP = l_fDis/this->m_vResolution.x;

		int l_iCurrentMouseFreq = (int)(this->m_iFrequency*l_fLERP);
		m_iMouseMoveFreq = l_iCurrentMouseFreq;
		if( m_pCurrentFFTHitCountAndTime )
		{
			int l_iIndex = (int)(m_pCurrentFFTHitCountAndTime->m_iFFTBinCount/WINDOWN_FUNCTION_FRUSTRUM*l_fLERP);
			m_iMouseMoveFreqDecibelAverage = this->m_pCurrentFFTHitCountAndTime->m_pFFTFrequencyDecibels->m_piAverageDecibleValue[l_iIndex];
			m_iMouseMoveFreqFFTBinIndex = l_iIndex;
			m_iMouseMoveFreq = (int)((float)this->m_iFrequency/m_pCurrentFFTHitCountAndTime->m_iFFTBinCount*l_iIndex);
		}
	}
}

void	cFFTDecibelsAnalyzer::SetExportThresholdValue(int e_iExportThresholdValue)
{
	m_iExportThresholdValue = e_iExportThresholdValue;
}

void	cFFTDecibelsAnalyzer::RenderByTime(float e_fTargetTime)
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


int		FintSmallestValueInVector(std::vector<int> e_Vector)
{
	int l_iIndex = -1;
	int l_iBiggestValue = INT_MAX;
	size_t l_uiSize = e_Vector.size();
	for(size_t i=0;i<l_uiSize;++i)
	{
		if( l_iBiggestValue >= e_Vector[i] )
		{
			l_iBiggestValue = e_Vector[i];
			l_iIndex = i;
		}
	}
	return l_iIndex;
}

void	cFFTDecibelsAnalyzer::SortMostHittedFequency(std::vector<int>&	e_FrequencyVector,std::vector<int>&e_FrequencyHittedCountVector,std::vector<int>&e_FrequencyHittedValueVector)
{
	std::vector<int> l_OriginaleFrequencyVector = e_FrequencyVector;
	std::vector<int> l_OriginalFrequencyHittedCountVector = e_FrequencyHittedCountVector;
	std::vector<int> l_OriginalFrequencyHittedValueVector = e_FrequencyHittedValueVector;
	e_FrequencyVector.clear();
	e_FrequencyHittedCountVector.clear();
	e_FrequencyHittedValueVector.clear();
	while(l_OriginalFrequencyHittedCountVector.size())
	{
		int l_iIndex = FintSmallestValueInVector(l_OriginalFrequencyHittedCountVector);
		e_FrequencyVector.push_back(l_OriginaleFrequencyVector[l_iIndex]);
		e_FrequencyHittedCountVector.push_back(l_OriginalFrequencyHittedCountVector[l_iIndex]);
		e_FrequencyHittedValueVector.push_back(l_OriginalFrequencyHittedValueVector[l_iIndex]);
		l_OriginaleFrequencyVector.erase(l_OriginaleFrequencyVector.begin()+l_iIndex);
		l_OriginalFrequencyHittedCountVector.erase(l_OriginalFrequencyHittedCountVector.begin()+l_iIndex);
		l_OriginalFrequencyHittedValueVector.erase(l_OriginalFrequencyHittedValueVector.begin()+l_iIndex);
	}
}

bool	cFFTDecibelsAnalyzer::ExportWithAverageDecibels(const char*e_strFileName,const char*e_strOriginalSourceFileName,int e_iDecibleThreshold,int e_iFrequencyThreshold)
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
	l_pRootTiXmlElement->SetAttribute(L"Frequency",m_iFrequency);
	l_pRootTiXmlElement->SetAttribute(L"SoundFile",ValueToStringW(e_strOriginalSourceFileName).c_str());
	l_pRootTiXmlElement->SetAttribute(L"FFTCount",l_iFFTCount);
	l_pRootTiXmlElement->SetAttribute(L"DecibleThreshole",m_iExportThresholdValue);
	l_pRootTiXmlElement->SetAttribute(L"FrequencyThreshold",e_iFrequencyThreshold);
	float l_fFrequenctForOneFFT = (float)this->m_iFrequency/l_iFFTCount;
	for(size_t i=0;i<l_uiSize;++i)
	{
		cFFTHitCountAndTime*l_pFFTHitCountAndTime = m_FFTHitCountAndTimeVector[i];
		std::vector<int>	l_FrequencyVector;
		std::vector<int>	l_FrequencyHittedCountVector;
		std::vector<int>	l_FrequencyHittedValueVector;
		for( int l_iCurrentFFTIndex = 0;l_iCurrentFFTIndex<l_pFFTHitCountAndTime->m_iFFTBinCount/WINDOWN_FUNCTION_FRUSTRUM;++l_iCurrentFFTIndex )
		{
			int l_iDecibels = l_pFFTHitCountAndTime->m_pFFTFrequencyDecibels->m_piAverageDecibleValue[l_iCurrentFFTIndex];
			//l_pFFTHitCountAndTime->m_pFFTFrequencyDecibels->
			//check threshold value
			if(l_iDecibels >=this->m_iExportThresholdValue)
			{
				int l_iFrequency = (int)(l_fFrequenctForOneFFT*l_iCurrentFFTIndex);
				if( e_iFrequencyThreshold >= l_iFrequency || l_iFrequency >= 38000 )//dont ask me why 38000...I am lazy
					continue;
				l_FrequencyVector.push_back(l_iFrequency);
				int l_iCount = l_pFFTHitCountAndTime->m_pFFTFrequencyDecibels->m_piDecibleBiggetThanZeroCount[l_iCurrentFFTIndex];
				int l_iAeverageValue = l_pFFTHitCountAndTime->m_pFFTFrequencyDecibels->m_piAverageDecibleValue[l_iCurrentFFTIndex];
				l_FrequencyHittedCountVector.push_back(l_iCount);
				l_FrequencyHittedValueVector.push_back(l_iAeverageValue);
			}
		}
		TiXmlElement*l_pFFTHitCountAndTimeElement = new TiXmlElement(L"FFTHitCountAndTime");
		l_pFFTHitCountAndTimeElement->SetAttribute(L"StartTime",l_pFFTHitCountAndTime->m_fStartTime);
		l_pFFTHitCountAndTimeElement->SetAttribute(L"FrequencyVector",ValueToStringW(l_FrequencyVector).c_str());
		l_pFFTHitCountAndTimeElement->SetAttribute(L"FrequencyHittedCount",ValueToStringW(l_FrequencyHittedCountVector).c_str());
		l_pFFTHitCountAndTimeElement->SetAttribute(L"FrequencyHittedValue",ValueToStringW(l_FrequencyHittedValueVector).c_str());
		l_pFFTHitCountAndTimeElement->SetAttribute(L"Count",ValueToStringW(l_FrequencyHittedValueVector.size()).c_str());
		l_pRootTiXmlElement->LinkEndChild(l_pFFTHitCountAndTimeElement);

		TiXmlElement*l_pSortFFTHitCountAndTimeElement = new TiXmlElement(L"SortFFTHitCountAndTime");
		SortMostHittedFequency(l_FrequencyVector,l_FrequencyHittedCountVector,l_FrequencyHittedValueVector);
		l_pSortFFTHitCountAndTimeElement->SetAttribute(L"StartTime",l_pFFTHitCountAndTime->m_fStartTime);
		l_pSortFFTHitCountAndTimeElement->SetAttribute(L"FrequencyVector",ValueToStringW(l_FrequencyVector).c_str());
		l_pSortFFTHitCountAndTimeElement->SetAttribute(L"FrequencyHittedCount",ValueToStringW(l_FrequencyHittedCountVector).c_str());
		l_pSortFFTHitCountAndTimeElement->SetAttribute(L"FrequencyHittedValue",ValueToStringW(l_FrequencyHittedValueVector).c_str());
		l_pSortFFTHitCountAndTimeElement->SetAttribute(L"Count",ValueToStringW(l_FrequencyHittedValueVector.size()).c_str());
		l_pRootTiXmlElement->LinkEndChild(l_pSortFFTHitCountAndTimeElement);
	}
	std::string l_strDirectory = "MusicGame/Piano/";
	l_strDirectory += e_strFileName;
	l_ISAXCallback.Export(l_strDirectory.c_str(),false);
	return false;
}

bool	cFFTDecibelsAnalyzer::Export(const char*e_strFileName,const char*e_strOriginalSourceFileName,int e_iDecibleThreshold,int e_iFrequencyThreshold)
{
	return ExportWithAverageDecibels(e_strFileName,e_strOriginalSourceFileName,e_iDecibleThreshold,e_iFrequencyThreshold);
	size_t l_uiSize = m_FFTHitCountAndTimeVector.size();
	if( l_uiSize == 0 )
		return false;
	int l_iFFTCount = m_FFTHitCountAndTimeVector[0]->m_iFFTBinCount;
	ISAXCallback l_ISAXCallback;
	TiXmlDocument*l_pTiXmlDocument = new TiXmlDocument();
	TiXmlElement*l_pRootTiXmlElement = new TiXmlElement(L"Root");
	l_pTiXmlDocument->LinkEndChild(l_pRootTiXmlElement);
	l_ISAXCallback.SetDoc(l_pTiXmlDocument);
	l_pRootTiXmlElement->SetAttribute(L"Frequency",m_iFrequency);
	l_pRootTiXmlElement->SetAttribute(L"SoundFile",ValueToStringW(e_strOriginalSourceFileName).c_str());
	l_pRootTiXmlElement->SetAttribute(L"FFTCount",l_iFFTCount);
	l_pRootTiXmlElement->SetAttribute(L"DecibleThreshole",e_iDecibleThreshold);
	l_pRootTiXmlElement->SetAttribute(L"FrequencyThreshold",e_iFrequencyThreshold);
	float l_fFrequenctForOneFFT = (float)this->m_iFrequency/l_iFFTCount;
	//to avoid noise...
	int l_iPreviousFrequencyHittedCount = -1;
	for(size_t i=0;i<l_uiSize;++i)
	{
		cFFTHitCountAndTime*l_pFFTHitCountAndTime = m_FFTHitCountAndTimeVector[i];
		std::vector<int>	l_FrequencyVector;
		std::vector<int>	l_FrequencyHittedCountVector;
		std::vector<int>	l_FrequencyHittedValueVector;
		for( int l_iCurrentFFTIndex = 0;l_iCurrentFFTIndex<l_pFFTHitCountAndTime->m_iFFTBinCount/WINDOWN_FUNCTION_FRUSTRUM;++l_iCurrentFFTIndex )
		{
			int l_iCount = l_pFFTHitCountAndTime->m_pHittedCountArray[l_iCurrentFFTIndex];
			//check threshold value
			if(l_iCount >=this->m_iExportThresholdValue)
			{
				int l_iFrequency = (int)(l_fFrequenctForOneFFT*l_iCurrentFFTIndex);
				if( e_iFrequencyThreshold >= l_iFrequency || l_iFrequency >= 38000 )//dont ask me why 38000...I am lazy
					continue;
				if( l_iPreviousFrequencyHittedCount == l_iCount)
					continue;
				l_iPreviousFrequencyHittedCount = l_iCount;
				l_FrequencyVector.push_back(l_iFrequency);
				l_FrequencyHittedCountVector.push_back(l_iCount);
				int l_iAeverageValue = l_pFFTHitCountAndTime->m_pHittedValueArray[l_iCurrentFFTIndex]/l_iCount;
				l_FrequencyHittedValueVector.push_back(l_iAeverageValue);
			}
			else
			{
				l_iPreviousFrequencyHittedCount = 0;
			}
		}
		TiXmlElement*l_pFFTHitCountAndTimeElement = new TiXmlElement(L"FFTHitCountAndTime");
		l_pFFTHitCountAndTimeElement->SetAttribute(L"StartTime",l_pFFTHitCountAndTime->m_fStartTime);
		l_pFFTHitCountAndTimeElement->SetAttribute(L"FrequencyVector",ValueToStringW(l_FrequencyVector).c_str());
		l_pFFTHitCountAndTimeElement->SetAttribute(L"FrequencyHittedCount",ValueToStringW(l_FrequencyHittedCountVector).c_str());
		l_pFFTHitCountAndTimeElement->SetAttribute(L"FrequencyHittedValue",ValueToStringW(l_FrequencyHittedValueVector).c_str());
		l_pFFTHitCountAndTimeElement->SetAttribute(L"Count",ValueToStringW(l_FrequencyHittedValueVector.size()).c_str());
		l_pRootTiXmlElement->LinkEndChild(l_pFFTHitCountAndTimeElement);

		TiXmlElement*l_pSortFFTHitCountAndTimeElement = new TiXmlElement(L"SortFFTHitCountAndTime");
		SortMostHittedFequency(l_FrequencyVector,l_FrequencyHittedCountVector,l_FrequencyHittedValueVector);
		l_pSortFFTHitCountAndTimeElement->SetAttribute(L"StartTime",l_pFFTHitCountAndTime->m_fStartTime);
		l_pSortFFTHitCountAndTimeElement->SetAttribute(L"FrequencyVector",ValueToStringW(l_FrequencyVector).c_str());
		l_pSortFFTHitCountAndTimeElement->SetAttribute(L"FrequencyHittedCount",ValueToStringW(l_FrequencyHittedCountVector).c_str());
		l_pSortFFTHitCountAndTimeElement->SetAttribute(L"FrequencyHittedValue",ValueToStringW(l_FrequencyHittedValueVector).c_str());
		l_pSortFFTHitCountAndTimeElement->SetAttribute(L"Count",ValueToStringW(l_FrequencyHittedValueVector.size()).c_str());
		l_pRootTiXmlElement->LinkEndChild(l_pSortFFTHitCountAndTimeElement);
	}
	std::string l_strDirectory = "MusicGame/Piano/";
	l_strDirectory += e_strFileName;
	l_ISAXCallback.Export(l_strDirectory.c_str(),false);
	return false;
}
//<Root ParseFileName="C:\Users\leeyo\Desktop\FM79979\Media\MusicGame\Piano\a2m.wav">
//    <TimeFrequencyAmplitudeValueCapture ParseFPS="60" CaptureSoundRequireMinTime="0.10000" MinAmplitude="80" SourceFrequency="11025" />
//</Root>
bool	cFFTDecibelsAnalyzer::MyParse(TiXmlElement*e_pRoot)
{
	return false;
}

void	cFFTDecibelsAnalyzer::AddFFTUpdateFilterData(int e_iStartFrequenct,int e_iDecibelsThresholdValue)
{
	sFilterData l_sFilterData = {e_iStartFrequenct,e_iDecibelsThresholdValue};
	this->m_FilterDataVector.push_back(l_sFilterData);
}
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

std::vector<int>		cQuickFFTDataFrequencyFinder::GetDecibelsByFFTBinIndex(int e_iIndex)
{
	std::vector<int> l_Result;
	if( m_iFFTBinCount <= e_iIndex)
		return l_Result;
	int l_iStep = e_iIndex;
	if( l_iStep < this->m_iFFTBinCount )
	{
		int l_iNeighborsData = 2;
		if( l_iStep < l_iNeighborsData )
			l_iStep = l_iNeighborsData;
		if( l_iStep >= m_iFFTBinCount-l_iNeighborsData)
			l_iStep = m_iFFTBinCount-l_iNeighborsData;
		int l_iAmplitudeCenter = this->m_piFFTData[l_iStep];
		l_Result.push_back(l_iAmplitudeCenter);
		for( int i=1;i<l_iNeighborsData;++i )
		{
			int l_iAmplitudeLeftSide = this->m_piFFTData[l_iStep-i];
			int l_iAmplitudeRightSide = this->m_piFFTData[l_iStep+i];
			l_Result.push_back(l_iAmplitudeLeftSide);
			l_Result.push_back(l_iAmplitudeRightSide);
		}
	}
	return l_Result;
}

std::vector<int>		cQuickFFTDataFrequencyFinder::GetDecibelsByFrequency(int e_iFrequency)
{
	std::vector<int> l_Result;
	//int l_iStep = (int)(e_iFrequency/WINDOWN_FUNCTION_FRUSTRUM/m_fFrequencyGap);
	int l_iStep = (int)(((float)e_iFrequency/m_iFrequency)*m_iFFTBinCount);
	if( l_iStep < this->m_iFFTBinCount )
	{
		int l_iNeighborsData = 1;
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



cFFTDecibelsAnalyzer::cFFTFrequencyDecibels::cFFTFrequencyDecibels(int e_iFFTCount)
{
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

bool	cFFTDecibelsAnalyzer::cFFTFrequencyDecibels::CopyAverageValue(cFFTFrequencyDecibels*e_pFFTFrequencyDecibels,int*e_pNewFFTData,int e_iFFTCount)
{
	if( e_iFFTCount != m_iFFTCount )
		return false;
	if( m_piAverageDecibleValue && this->m_iFFTCount )
	{
		memcpy(e_pFFTFrequencyDecibels->m_piTotalDecibleValue,m_piAverageDecibleValue,sizeof(int)*m_iFFTCount);
		//why?
		e_pFFTFrequencyDecibels->m_iUpdateCount = 5;
		for( int i=0;i<e_iFFTCount;++i )
		{
			e_pFFTFrequencyDecibels->m_piTotalDecibleValue[i] += e_pNewFFTData[i];
		}
		return true;
	}
	return false;
}

void	cFFTDecibelsAnalyzer::cFFTFrequencyDecibels::Update(int e_iCount,int*e_piFFTCount)
{
	assert(e_iCount == this->m_iFFTCount);
	++m_iUpdateCount;
	for( int i=0;i<this->m_iFFTCount;++i )
	{
		int l_iValue = e_piFFTCount[i];
		if( l_iValue > 0 )
		{
			++m_piDecibleBiggetThanZeroCount[i];
			int l_iOriginalValue = this->m_piTotalDecibleValue[i];
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
void	cFFTDecibelsAnalyzer::cFFTFrequencyDecibels::Render(int e_iThresholdValue,int e_iScale)
{
	Vector4 l_vColor = Vector4(1,1,0,0.3f);
	int l_iScaleValue = e_iScale;
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
	m_piCurrentFFTDataReferencePointer = nullptr;
	m_iMouseMoveFreq = -1;
	m_iMouseMoveFreqFFTBinIndex = -1;
	m_iMouseMoveFreqDecibelAverage = -1;
	m_iFrequency = 44100;
	//why 999?because I am lazy lah
	m_fNextDataTimeGap = 1;
	m_fCurrentTime = 0.f;
	m_pCurrentFFTFrequencyDecibels = nullptr;
	this->m_vShowPos = Vector2(100,1200);
	this->m_vResolution.x = 3600;
	this->m_vResolution.y = 600;
	m_iHittedCountScaleForVisual = 1;
	m_iMaxValue = 0;
	m_iExportThresholdValue = cSoundCompareParameter::m_siFFTStoreThresholeValue;
	m_bMouseDown = false;
	m_iDecibelsRenderScale = 10;
	m_eCurrentMarkFrequencyMode = eCurrentMarkFrequencyMode::eCMFM_NONE;
	//m_iThreusholdAmplitude = cSoundCompareParameter::m_siDebugAmplitudeValue;
}

cFFTDecibelsAnalyzer::~cFFTDecibelsAnalyzer()
{
	Destroy();
}



void	cFFTDecibelsAnalyzer::Destroy()
{
	//SAFE_DELETE(m_pCurrentFFTFrequencyDecibels);
	DELETE_VECTOR(m_FFTFrequencyDecibelsVector);
	m_pCurrentFFTFrequencyDecibels = nullptr;
}

void	cFFTDecibelsAnalyzer::Start(int e_iFrequency)
{
	Destroy();
	this->m_iFrequency = e_iFrequency;
	m_fRestNextDataTimeGap = 0.f;
	m_PickupIndexVector.clear();
}

void	cFFTDecibelsAnalyzer::UpdateFFTData(float e_fElpaseTime,int*e_piFFTData,int e_iCount)
{
	//ctrl
	if( cGameApp::m_sucKeyData[17] )	
		m_eCurrentMarkFrequencyMode = eCMFM_DELETE;
	else//alter
	if( cGameApp::m_sucKeyData[18] )	
		m_eCurrentMarkFrequencyMode = eCMFM_ADD;
	else//'C'
	if( cGameApp::m_sucKeyData[37] )
	{
		Start(m_iFrequency);
	}
	else
		m_eCurrentMarkFrequencyMode = eCMFM_NONE;//eCMFM_ADD
	m_piCurrentFFTDataReferencePointer = e_piFFTData;
	m_fCurrentTime += e_fElpaseTime;
	m_fRestNextDataTimeGap -= e_fElpaseTime;
	m_iMaxValue = 0;
	if( m_fRestNextDataTimeGap <= 0.f )
	{
		m_fRestNextDataTimeGap = m_fNextDataTimeGap+m_fRestNextDataTimeGap;
		if( m_FFTFrequencyDecibelsVector.size() == 0 )
		{
			//SAFE_DELETE(m_pCurrentFFTFrequencyDecibels);
			cFFTFrequencyDecibels*l_pFFTFrequencyDecibels = new cFFTFrequencyDecibels(e_iCount);
			l_pFFTFrequencyDecibels->SetShowPos(this->m_vShowPos);
			l_pFFTFrequencyDecibels->SetResolution(this->m_vResolution);
			if( m_pCurrentFFTFrequencyDecibels )
				m_pCurrentFFTFrequencyDecibels->CopyAverageValue(l_pFFTFrequencyDecibels,e_piFFTData,e_iCount);
			m_pCurrentFFTFrequencyDecibels = l_pFFTFrequencyDecibels;
			this->m_FFTFrequencyDecibelsVector.push_back(l_pFFTFrequencyDecibels);
			m_iMaxValue = 0;
		}
		else
		{
			const int l_iSmmothDivide = (int)(m_fNextDataTimeGap*60)/15;
			for(int i=0;i<m_pCurrentFFTFrequencyDecibels->m_iFFTCount;++i)
				m_pCurrentFFTFrequencyDecibels->m_piTotalDecibleValue[i] /= l_iSmmothDivide;
			m_pCurrentFFTFrequencyDecibels->m_iUpdateCount /= l_iSmmothDivide;
		}
	}

	for( int i=0;i<e_iCount;++i )
	{
		int l_iDecibelsThresholdValue = cSoundCompareParameter::m_siDebugAmplitudeValue;
		if(m_iMaxValue < e_piFFTData[i] )
			m_iMaxValue = e_piFFTData[i];
	}
	if( m_pCurrentFFTFrequencyDecibels )
	{
		float l_fFrequenctForOneFFT = (float)this->m_iFrequency/e_iCount;
		if( m_pCurrentFFTFrequencyDecibels)
			m_pCurrentFFTFrequencyDecibels->Update(e_iCount,e_piFFTData);
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
	if( m_pCurrentFFTFrequencyDecibels )
	{
		m_pCurrentFFTFrequencyDecibels->SetResolution(this->m_vResolution);
		m_pCurrentFFTFrequencyDecibels->SetShowPos(m_vShowPos);
	}
}

void	cFFTDecibelsAnalyzer::RenderFFTHitCountAndTime(cFFTFrequencyDecibels*e_pFFTFrequencyDecibels)
{
	if( e_pFFTFrequencyDecibels )
	{
		int l_iFFTBinCount = e_pFFTFrequencyDecibels->m_iFFTCount;
		//have no idea...while I jump time
		if( l_iFFTBinCount < 0 || l_iFFTBinCount > 100000 )
			return;
		float l_fGap = this->GetWidthGapByPoints(e_pFFTFrequencyDecibels->m_iFFTCount);
		Vector2 l_vShowPos = this->m_vShowPos;

		std::wstring l_strInfo = L"RestTime:";
		l_strInfo += ValueToStringW(m_fRestNextDataTimeGap);
		l_strInfo += L"\nMaxValue:";
		l_strInfo += ValueToStringW(m_iMaxValue);
		cGameApp::RenderFont(this->m_vShowPos.x,this->m_vShowPos.y+200,l_strInfo);

		if( m_pCurrentFFTFrequencyDecibels )
			m_pCurrentFFTFrequencyDecibels->Render(this->m_iExportThresholdValue,m_iDecibelsRenderScale);

		if(m_iMouseMoveFreqFFTBinIndex != -1)
		{
			Vector2 l_vLinePos[2];
			l_vLinePos[1].x = l_vLinePos[0].x = this->m_vShowPos.x+(l_fGap*m_iMouseMoveFreqFFTBinIndex);
			l_vLinePos[0].y = this->m_vShowPos.y;
			l_vLinePos[1].y = l_vLinePos[0].y-30;
			RenderLine((float*)&l_vLinePos,2,Vector4(1,1,1,1),2);
		}

		if(e_pFFTFrequencyDecibels->m_iFFTCount >= 4096)
			return;
		size_t l_uiPickupIndexVectorSize = m_PickupIndexVector.size();
		if( l_uiPickupIndexVectorSize > 0 )
		{
			Vector2 l_LazyPoints[4096*2];
			float l_fGap = this->GetWidthGapByPoints(e_pFFTFrequencyDecibels->m_iFFTCount);
			for( size_t i=0;i<l_uiPickupIndexVectorSize;++i )
			{
				int l_iIndex = m_PickupIndexVector[i];
				l_LazyPoints[i*2+1].x = l_LazyPoints[i*2].x = this->m_vShowPos.x+l_iIndex*l_fGap;
				l_LazyPoints[i*2].y = this->m_vShowPos.y;
				l_LazyPoints[i*2+1].y = l_LazyPoints[i*2].y+30;
			}
			RenderLine((float*)&l_LazyPoints[0],(int)l_uiPickupIndexVectorSize*2,Vector4::Red,2);
		}
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
	RenderFFTHitCountAndTime(this->m_pCurrentFFTFrequencyDecibels);
	RenderMouseMoveInfo();
}

int	cFFTDecibelsAnalyzer::GetExportThresholdValue()
{
	return m_iExportThresholdValue;
}

void	cFFTDecibelsAnalyzer::MouseDown(int e_iMousePosX,int e_iMousePosY)
{
	m_bMouseDown = true;
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
		if( m_pCurrentFFTFrequencyDecibels )
		{
			int l_iIndex = (int)(m_pCurrentFFTFrequencyDecibels->m_iFFTCount*l_fLERP);
			if( l_iIndex >= m_pCurrentFFTFrequencyDecibels->m_iFFTCount )
			{
				m_iMouseMoveFreq = m_pCurrentFFTFrequencyDecibels->m_iFFTCount-1;
			}
			else
			if( l_iIndex < 0 )
			{
				m_iMouseMoveFreq = 0;
			}
			m_iMouseMoveFreqDecibelAverage = this->m_pCurrentFFTFrequencyDecibels->m_piAverageDecibleValue[l_iIndex];
			m_iMouseMoveFreqFFTBinIndex = l_iIndex;
			m_iMouseMoveFreq = (int)((float)this->m_iFrequency/m_pCurrentFFTFrequencyDecibels->m_iFFTCount*l_iIndex);
			if( m_bMouseDown && m_piCurrentFFTDataReferencePointer )
			{
				float l_fDecibels = m_piCurrentFFTDataReferencePointer[l_iIndex]*m_iDecibelsRenderScale+this->m_vShowPos.y;
				//if( e_iMousePosY <= l_fDecibels )
				{
					int l_iIndexOfVector = UT::IsVectorContainValue<int>(m_PickupIndexVector,l_iIndex);
					if( l_iIndexOfVector == -1 && m_eCurrentMarkFrequencyMode == eCMFM_ADD )
						m_PickupIndexVector.push_back(l_iIndex);
					else
					if( l_iIndexOfVector != -1 && m_eCurrentMarkFrequencyMode == eCMFM_DELETE )
					{
						m_PickupIndexVector.erase(m_PickupIndexVector.begin()+l_iIndexOfVector);
					}
				}
			}
		}
	}
}



void	cFFTDecibelsAnalyzer::MouseUp(int e_iMousePosX,int e_iMousePosY)
{
	m_bMouseDown = false;
}


void	cFFTDecibelsAnalyzer::SetExportThresholdValue(int e_iExportThresholdValue)
{
	m_iExportThresholdValue = e_iExportThresholdValue;
}

void	cFFTDecibelsAnalyzer::RenderByTime(float e_fTargetTime)
{
	//cFFTFrequencyDecibels*l_pTargetFFTFrequencyDecibels = nullptr;
	//size_t l_uiSize = m_FFTFrequencyDecibelsVector.size();
	//for( size_t i=0;i<l_uiSize;++i )
	//{
	//	if(e_fTargetTime <= m_FFTFrequencyDecibelsVector[i]->m_fStartTime )
	//	{
	//		l_pTargetFFTFrequencyDecibels = m_FFTFrequencyDecibelsVector[i];
	//		break;
	//	}
	//}
	//if( l_pTargetFFTFrequencyDecibels )
	//{
	//	l_pTargetFFTFrequencyDecibels;
	//}
}

int		FintBiggestValueInVector(std::vector<int>*e_pVector,std::vector<int>*e_pVectorIndex)
{
	int l_iIndex = -1;
	int l_iBiggestValue = INT_MIN;
	size_t l_uiSize = e_pVectorIndex->size();
	for(size_t i=0;i<l_uiSize;++i)
	{
		int l_iTargetIndex = (*e_pVectorIndex)[i];
		if( l_iBiggestValue <= (*e_pVector)[l_iTargetIndex] )
		{
			l_iBiggestValue = (*e_pVector)[l_iTargetIndex];
			l_iIndex = (int)i;
		}
	}
	return l_iIndex;
}

std::vector<int> SortVectorGetOrder(std::vector<int>*e_pCompareData)
{
	std::vector<int> l_ResultDataToCompareVector;
	std::vector<int> l_Result;
	size_t l_uiSize = e_pCompareData->size();
	for(size_t i=0;i<l_uiSize;++i)
		l_ResultDataToCompareVector.push_back((int)i);
	while( l_ResultDataToCompareVector.size() )
	{
		int l_iIndex = FintBiggestValueInVector(e_pCompareData,&l_ResultDataToCompareVector);
		l_ResultDataToCompareVector.erase(l_ResultDataToCompareVector.begin()+l_iIndex);
		l_Result.push_back(l_iIndex);
	}
	return l_Result;
}


void	cFFTDecibelsAnalyzer::SortMostHittedFequency(std::vector<int>&e_Vector1,std::vector<int>&e_Vector2,std::vector<int>&e_Vector3,std::vector<int>&e_Vector4)
{
	std::vector<std::vector<int> > l_AllVector;
	l_AllVector.push_back(e_Vector1);
	l_AllVector.push_back(e_Vector2);
	l_AllVector.push_back(e_Vector3);
	l_AllVector.push_back(e_Vector4);
	e_Vector1.clear();
	e_Vector2.clear();
	e_Vector3.clear();
	e_Vector4.clear();
	std::vector<int> l_VectorSortedIndex = SortVectorGetOrder(&l_AllVector[0]);
	size_t l_uiSize = l_VectorSortedIndex.size();
	for( size_t i=0;i<l_uiSize;++i )
	{
		int l_iIndex = l_VectorSortedIndex[i];
		e_Vector1.push_back(l_AllVector[0][l_iIndex]);
		e_Vector2.push_back(l_AllVector[1][l_iIndex]);
		e_Vector3.push_back(l_AllVector[2][l_iIndex]);
		e_Vector4.push_back(l_AllVector[3][l_iIndex]);

	}
}

bool	cFFTDecibelsAnalyzer::ExportBySelectedDecibels(const char*e_strFileName,const char*e_strOriginalSourceFileName)
{
	size_t l_uiSize = m_FFTFrequencyDecibelsVector.size();
	if( l_uiSize == 0 )
		return false;
	int l_iFFTCount = m_FFTFrequencyDecibelsVector[0]->m_iFFTCount;
	ISAXCallback l_ISAXCallback;
	TiXmlDocument*l_pTiXmlDocument = new TiXmlDocument();
	TiXmlElement*l_pRootTiXmlElement = new TiXmlElement(L"Root");
	l_pTiXmlDocument->LinkEndChild(l_pRootTiXmlElement);
	l_ISAXCallback.SetDoc(l_pTiXmlDocument);
	l_pRootTiXmlElement->SetAttribute(L"Frequency",m_iFrequency);
	l_pRootTiXmlElement->SetAttribute(L"SoundFile",ValueToStringW(e_strOriginalSourceFileName).c_str());
	l_pRootTiXmlElement->SetAttribute(L"FFTCount",l_iFFTCount);
	l_pRootTiXmlElement->SetAttribute(L"DecibleThreshole",m_iExportThresholdValue);
	float l_fFrequenctForOneFFT = (float)this->m_iFrequency/l_iFFTCount;
	for(size_t i=0;i<l_uiSize;++i)
	{
		cFFTFrequencyDecibels*l_pFFTFrequencyDecibels = m_FFTFrequencyDecibelsVector[i];
		std::vector<int>	l_FrequencyVector;
		std::vector<int>	l_FrequencyBinIndexVector;
		std::vector<int>	l_FrequencyHittedCountVector;
		std::vector<int>	l_FrequencyHittedValueVector;
		int l_iPickupIndexSize = (int)this->m_PickupIndexVector.size();
		for( int l_iCurrentFFTIndex = 0;l_iCurrentFFTIndex<l_iPickupIndexSize;++l_iCurrentFFTIndex )
		{
			int l_iIndex = m_PickupIndexVector[l_iCurrentFFTIndex];
			int l_iDecibels = l_pFFTFrequencyDecibels->m_piAverageDecibleValue[l_iIndex];
			int l_iFrequency = (int)(l_fFrequenctForOneFFT*l_iIndex);
			l_FrequencyBinIndexVector.push_back(l_iIndex);
			l_FrequencyVector.push_back(l_iFrequency);
			int l_iCount = l_pFFTFrequencyDecibels->m_piDecibleBiggetThanZeroCount[l_iIndex];
			int l_iAeverageValue = l_pFFTFrequencyDecibels->m_piAverageDecibleValue[l_iIndex];
			l_FrequencyHittedCountVector.push_back(l_iCount);
			l_FrequencyHittedValueVector.push_back(l_iAeverageValue);
		}
		TiXmlElement*l_pFFTFrequencyDecibelsElement = new TiXmlElement(L"FFTFrequencyDecibels");
		l_pFFTFrequencyDecibelsElement->SetAttribute(L"FrequencyVector",ValueToStringW(l_FrequencyVector).c_str());
		l_pFFTFrequencyDecibelsElement->SetAttribute(L"FFTBinIndex",ValueToStringW(l_FrequencyBinIndexVector).c_str());
		l_pFFTFrequencyDecibelsElement->SetAttribute(L"FrequencyHittedCount",ValueToStringW(l_FrequencyHittedCountVector).c_str());
		l_pFFTFrequencyDecibelsElement->SetAttribute(L"FrequencyHittedValue",ValueToStringW(l_FrequencyHittedValueVector).c_str());
		l_pFFTFrequencyDecibelsElement->SetAttribute(L"Frequency",ValueToStringW(this->m_iFrequency));
		l_pFFTFrequencyDecibelsElement->SetAttribute(L"FFTCount",ValueToStringW(l_pFFTFrequencyDecibels->m_iFFTCount));
		l_pFFTFrequencyDecibelsElement->SetAttribute(L"Count",ValueToStringW(l_FrequencyHittedValueVector.size()).c_str());
		l_pRootTiXmlElement->LinkEndChild(l_pFFTFrequencyDecibelsElement);

		TiXmlElement*l_pSortFFTFrequencyDecibelsElement = new TiXmlElement(L"SortFFTFrequencyDecibels");
		SortMostHittedFequency(l_FrequencyVector,l_FrequencyBinIndexVector,l_FrequencyHittedCountVector,l_FrequencyHittedValueVector);

		l_pSortFFTFrequencyDecibelsElement->SetAttribute(L"FrequencyVector",ValueToStringW(l_FrequencyVector).c_str());
		l_pSortFFTFrequencyDecibelsElement->SetAttribute(L"FFTBinIndex",ValueToStringW(l_FrequencyBinIndexVector).c_str());
		l_pSortFFTFrequencyDecibelsElement->SetAttribute(L"FrequencyHittedCount",ValueToStringW(l_FrequencyHittedCountVector).c_str());
		l_pSortFFTFrequencyDecibelsElement->SetAttribute(L"FrequencyHittedValue",ValueToStringW(l_FrequencyHittedValueVector).c_str());

		l_pSortFFTFrequencyDecibelsElement->SetAttribute(L"Frequency",ValueToStringW(this->m_iFrequency));
		l_pSortFFTFrequencyDecibelsElement->SetAttribute(L"FFTCount",ValueToStringW(l_pFFTFrequencyDecibels->m_iFFTCount));
		l_pSortFFTFrequencyDecibelsElement->SetAttribute(L"Count",ValueToStringW(l_FrequencyHittedValueVector.size()).c_str());
		l_pRootTiXmlElement->LinkEndChild(l_pSortFFTFrequencyDecibelsElement);
	}
	std::string l_strDirectory = "MusicGame/Piano/";
	l_strDirectory += e_strFileName;
	l_ISAXCallback.Export(l_strDirectory.c_str(),false);
	return false;
}

bool	cFFTDecibelsAnalyzer::ExportWithAverageDecibels(const char*e_strFileName,const char*e_strOriginalSourceFileName,int e_iDecibleThreshold,int e_iFrequencyThreshold)
{
	//size_t l_uiSize = m_FFTFrequencyDecibelsVector.size();
	//if( l_uiSize == 0 )
	//	return false;
	//int l_iFFTCount = m_FFTFrequencyDecibelsVector[0]->m_iFFTCount;
	//ISAXCallback l_ISAXCallback;
	//TiXmlDocument*l_pTiXmlDocument = new TiXmlDocument();
	//TiXmlElement*l_pRootTiXmlElement = new TiXmlElement(L"Root");
	//l_pTiXmlDocument->LinkEndChild(l_pRootTiXmlElement);
	//l_ISAXCallback.SetDoc(l_pTiXmlDocument);
	//l_pRootTiXmlElement->SetAttribute(L"Frequency",m_iFrequency);
	//l_pRootTiXmlElement->SetAttribute(L"SoundFile",ValueToStringW(e_strOriginalSourceFileName).c_str());
	//l_pRootTiXmlElement->SetAttribute(L"FFTCount",l_iFFTCount);
	//l_pRootTiXmlElement->SetAttribute(L"DecibleThreshole",m_iExportThresholdValue);
	//l_pRootTiXmlElement->SetAttribute(L"FrequencyThreshold",e_iFrequencyThreshold);
	//float l_fFrequenctForOneFFT = (float)this->m_iFrequency/l_iFFTCount;
	//for(size_t i=0;i<l_uiSize;++i)
	//{
	//	cFFTHitCountAndTime*l_pFFTHitCountAndTime = m_FFTFrequencyDecibelsVector[i];
	//	std::vector<int>	l_FrequencyVector;
	//	std::vector<int>	l_FrequencyBinIndexVector;
	//	std::vector<int>	l_FrequencyHittedCountVector;
	//	std::vector<int>	l_FrequencyHittedValueVector;
	//	for( int l_iCurrentFFTIndex = 0;l_iCurrentFFTIndex<l_pFFTHitCountAndTime->m_iFFTCount;++l_iCurrentFFTIndex )
	//	{
	//		int l_iDecibels = l_pFFTHitCountAndTime->m_pFFTFrequencyDecibels->m_piAverageDecibleValue[l_iCurrentFFTIndex];
	//		//l_pFFTHitCountAndTime->m_pFFTFrequencyDecibels->
	//		//check threshold value
	//		if(l_iDecibels >=this->m_iExportThresholdValue)
	//		{
	//			int l_iFrequency = (int)(l_fFrequenctForOneFFT*l_iCurrentFFTIndex);
	//			if( e_iFrequencyThreshold >= l_iFrequency || l_iFrequency >= 38000 )//dont ask me why 38000...I am lazy
	//				continue;
	//			l_FrequencyBinIndexVector.push_back(l_iCurrentFFTIndex);
	//			l_FrequencyVector.push_back(l_iFrequency);
	//			int l_iCount = l_pFFTHitCountAndTime->m_pFFTFrequencyDecibels->m_piDecibleBiggetThanZeroCount[l_iCurrentFFTIndex];
	//			int l_iAeverageValue = l_pFFTHitCountAndTime->m_pFFTFrequencyDecibels->m_piAverageDecibleValue[l_iCurrentFFTIndex];
	//			l_FrequencyHittedCountVector.push_back(l_iCount);
	//			l_FrequencyHittedValueVector.push_back(l_iAeverageValue);
	//		}
	//	}
	//	TiXmlElement*l_pFFTHitCountAndTimeElement = new TiXmlElement(L"FFTHitCountAndTime");
	//	l_pFFTHitCountAndTimeElement->SetAttribute(L"StartTime",l_pFFTHitCountAndTime->m_fStartTime);
	//	l_pFFTHitCountAndTimeElement->SetAttribute(L"FrequencyVector",ValueToStringW(l_FrequencyVector).c_str());
	//	l_pFFTHitCountAndTimeElement->SetAttribute(L"FFTBinIndex",ValueToStringW(l_FrequencyBinIndexVector).c_str());
	//	l_pFFTHitCountAndTimeElement->SetAttribute(L"FrequencyHittedCount",ValueToStringW(l_FrequencyHittedCountVector).c_str());
	//	l_pFFTHitCountAndTimeElement->SetAttribute(L"FrequencyHittedValue",ValueToStringW(l_FrequencyHittedValueVector).c_str());
	//	l_pFFTHitCountAndTimeElement->SetAttribute(L"Frequency",ValueToStringW(this->m_iFrequency));
	//	l_pFFTHitCountAndTimeElement->SetAttribute(L"FFTCount",ValueToStringW(l_pFFTHitCountAndTime->m_iFFTCount));
	//	l_pFFTHitCountAndTimeElement->SetAttribute(L"Count",ValueToStringW(l_FrequencyHittedValueVector.size()).c_str());
	//	l_pRootTiXmlElement->LinkEndChild(l_pFFTHitCountAndTimeElement);

	//	TiXmlElement*l_pSortFFTHitCountAndTimeElement = new TiXmlElement(L"SortFFTHitCountAndTime");
	//	SortMostHittedFequency(l_FrequencyVector,l_FrequencyBinIndexVector,l_FrequencyHittedCountVector,l_FrequencyHittedValueVector);
	//	l_pSortFFTHitCountAndTimeElement->SetAttribute(L"StartTime",l_pFFTHitCountAndTime->m_fStartTime);

	//	l_pSortFFTHitCountAndTimeElement->SetAttribute(L"FrequencyVector",ValueToStringW(l_FrequencyVector).c_str());
	//	l_pSortFFTHitCountAndTimeElement->SetAttribute(L"FFTBinIndex",ValueToStringW(l_FrequencyBinIndexVector).c_str());
	//	l_pSortFFTHitCountAndTimeElement->SetAttribute(L"FrequencyHittedCount",ValueToStringW(l_FrequencyHittedCountVector).c_str());
	//	l_pSortFFTHitCountAndTimeElement->SetAttribute(L"FrequencyHittedValue",ValueToStringW(l_FrequencyHittedValueVector).c_str());

	//	l_pSortFFTHitCountAndTimeElement->SetAttribute(L"Frequency",ValueToStringW(this->m_iFrequency));
	//	l_pSortFFTHitCountAndTimeElement->SetAttribute(L"FFTCount",ValueToStringW(l_pFFTHitCountAndTime->m_iFFTCount));
	//	l_pSortFFTHitCountAndTimeElement->SetAttribute(L"Count",ValueToStringW(l_FrequencyHittedValueVector.size()).c_str());
	//	l_pRootTiXmlElement->LinkEndChild(l_pSortFFTHitCountAndTimeElement);
	//}
	//std::string l_strDirectory = "MusicGame/Piano/";
	//l_strDirectory += e_strFileName;
	//l_ISAXCallback.Export(l_strDirectory.c_str(),false);
	return false;
}

bool	cFFTDecibelsAnalyzer::Export(const char*e_strFileName,const char*e_strOriginalSourceFileName)
{
	return ExportBySelectedDecibels(e_strFileName,e_strOriginalSourceFileName);
}
//<Root ParseFileName="C:\Users\leeyo\Desktop\FM79979\Media\MusicGame\Piano\a2m.wav">
//    <TimeFrequencyAmplitudeValueCapture ParseFPS="60" CaptureSoundRequireMinTime="0.10000" MinAmplitude="80" SourceFrequency="11025" />
//</Root>
bool	cFFTDecibelsAnalyzer::MyParse(TiXmlElement*e_pRoot)
{
	return false;
}

//void	cFFTDecibelsAnalyzer::AddFFTUpdateFilterData(int e_iStartFrequenct,int e_iDecibelsThresholdValue)
//{
//	sFilterData l_sFilterData = {e_iStartFrequenct,e_iDecibelsThresholdValue};
//	this->m_FilterDataVector.push_back(l_sFilterData);
//}
#pragma once

#include "TimelineBasicInfo.h"
//for quick find the ampliture in frequency

class cQuickFFTDataFrequencyFinder
{
	//its a reference.
	GET_SET_DEC(int*,m_piFFTData,GetFFTData,SetFFTData);
	float					m_fFrequencyGap;
	int						m_iFFTBinCount;
	int						m_iFrequency;
public:
	cQuickFFTDataFrequencyFinder(int e_iFFTBins,int e_iMaxFrequency);
	~cQuickFFTDataFrequencyFinder();
	//get left center and right data
	//here is possible have some precious problem,so I give close result
	std::vector<int>						GetAmplitude(int e_iFrequency);
};




class cFFTDataStore:public cNodeISAX,public cChartBasicInfo
{
	void	Destroy();
	virtual	bool						MyParse(TiXmlElement*e_pRoot);
	//I am lazy...
	Vector2								m_vLineTempPos[44100*2];
	int									m_iAnplitudeScale;
	int									m_iMaxValue;
public:
	cFFTDataStore();
	~cFFTDataStore();
	class cFFTHitCountAndTime
	{
	public:
		cFFTHitCountAndTime(float e_fStartTime,int e_iFFTBinCount);
		~cFFTHitCountAndTime();
		float   m_fStartTime;
		int*	m_pHittedCountArray;
		int		m_iFFTBinCount;
	};
	void								UpdateFFTData(float e_fElpaseTime,int*e_piFFTData,int e_iCount);
	void								Start();
	void								Export(char*e_strFileName);
	void								RenderByTime(float e_fTargetTime);
	void								RenderCurrentData();
	float								GetCurrentTime(){ return m_fCurrentTime; }
private:
	//int									m_iThreusholdAmplitude;
	std::vector<cFFTHitCountAndTime*>	m_FFTHitCountAndTimeVector;
	float								m_fNextDataTimeGap;
	float								m_fCurrentTime;
	float								m_fRestNextDataTimeGap;
private:
	cFFTHitCountAndTime*				m_pCurrentFFTHitCountAndTime;
	void								RenderFFTHitCountAndTime(cFFTHitCountAndTime*e_pFFTHitCountAndTime);
};



	//if( 0 )
	//{
	//	//bool
	//	static std::vector<std::vector<int> >	l_iDebugInfoVectorVector;
	//	//for debug
	//	static bool l_bQKeyPress = false;
	//	if( cGameApp::m_sucKeyData['Q'] )
	//	{
	//		l_bQKeyPress = true;
	//	}
	//	//if( l_bQKeyPress && !cGameApp::m_sucKeyData['Q'] && m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size() )
	//	if( cGameApp::m_sucKeyData['Q'] && m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size() )
	//	{
	//		l_bQKeyPress = false;
	//		auto*l_pTimeAndFFTData = m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0];
	//		int l_iFFTDataLength = l_pTimeAndFFTData->iFFTDataOneSample/WINDOWN_FUNCTION_FRUSTRUM;
	//		int l_iFrequency = this->m_pSoundCapture->GetFrequency();
	//		int l_iSampleGap = l_iFrequency/l_iFFTDataLength;
	//		std::vector<int> l_Result;
	//		std::wstring l_strDebugInfo;
	//		for( int i=0;i<l_iFFTDataLength;++i )
	//		{
	//			if( this->m_piFFTData[i] >= cSoundCompareParameter::m_siDebugAmplitudeValue )
	//			{
	//				l_strDebugInfo += ValueToStringW(l_iSampleGap*i);
	//				l_strDebugInfo += L",";
	//				l_Result.push_back(l_iSampleGap*i);
	//			}
	//		}
	//		cGameApp::OutputDebugInfoString(l_strDebugInfo);
	//		l_iDebugInfoVectorVector.push_back(l_Result);
	//	}
	//	static bool l_bWKeyPress = false;
	//	if( cGameApp::m_sucKeyData['W'] )
	//	{
	//		l_bWKeyPress = true;
	//	}
	//	if( l_bWKeyPress && !cGameApp::m_sucKeyData['W'])
	//	{
	//		l_bWKeyPress = false;
	//		std::wstring l_strDebugInfo;
	//		std::map<int,int> l_FrequencyAndCount;
	//		for(auto l_ResultVector: l_iDebugInfoVectorVector)
	//		{
	//			for(auto l_Frequency: l_ResultVector)
	//			{
	//				if(l_FrequencyAndCount.find(l_Frequency) == l_FrequencyAndCount.end() )
	//				{
	//					l_FrequencyAndCount[l_Frequency] = 1;
	//				}
	//				else
	//				{
	//					l_FrequencyAndCount[l_Frequency] = l_FrequencyAndCount[l_Frequency]+1;
	//				}
	//			}
	//		}
	//		int l_iTotalCount = 0;
	//		int l_iFrequencyCount = 0;
	//		int l_iAverage = 0;
	//		for(std::map<int,int>::iterator l_Iterator = l_FrequencyAndCount.begin();l_Iterator!=l_FrequencyAndCount.end();++l_Iterator)
	//		{
	//			l_iTotalCount += l_Iterator->second;
	//			++l_iFrequencyCount;
	//		}
	//		l_iAverage = l_iTotalCount/l_iFrequencyCount;
	//		std::vector<int> l_FinalResultFrequencyVector;
	//		for(std::map<int,int>::iterator l_Iterator = l_FrequencyAndCount.begin();l_Iterator!=l_FrequencyAndCount.end();++l_Iterator)
	//		{
	//			if( l_Iterator->second >= l_iAverage )
	//			{
	//				l_FinalResultFrequencyVector.push_back(l_Iterator->first);

	//				std::wstring l_strLazyOutput = L"<FrequenceAndAmplitudeAndTime StartTime=\"0.01667\" KeepTime=\"0.11667\" LastMatchTime=\"0.13333\" Frequency=\"";
	//				l_strLazyOutput += ValueToStringW(l_Iterator->first);
	//				l_strLazyOutput += L"\" Amplitude=\"118\" />";
	//				cGameApp::OutputDebugInfoString(l_strLazyOutput);
	//			}
	//		}
	//		//cGameApp::OutputDebugInfoString(ValueToStringW(l_FinalResultFrequencyVector));
	//	}
	//}
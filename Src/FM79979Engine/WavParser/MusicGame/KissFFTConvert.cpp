#include "stdafx.h"
#include "KissFFTConvert.h"
#include "Parameters.h"
#include "KissFFTFunction.h"
TYPDE_DEFINE_MARCO(cKissFFTConvertBase);


cKissFFTConvertBase::cKissFFTConvertBase()
{
	m_iFreq = 0;
	m_iMaxAmplitude = 0;
	m_iMaxAmplitudeFrequence = 0;
	m_iMaxAmplitude = 0;
	m_iDivideFFTDataToNFrame = ONE_FRAME_NEED_NUM_FFT_DATA_COUNT;
	m_fCurrentTime = 0.f;
	m_iOneFrameFFTDataCount = 0;
	m_vResolution = Vector2(3600,1000.f);
	m_fChartAmplitudeScale = 10.f;
	m_fNextChannelYGap = 700.f;
	m_vShowPos = Vector2(100,700);
	m_bPause = false;
	m_fChartScale = 1.f;
	m_pFFTDataStore = nullptr;
	m_fNextFFTUpdateElpaseTime = 1.f/ONE_FRAME_NEED_NUM_FFT_DATA_COUNT;
//	m_fFrenquenceFilterEndScaleValue = 1.f;
//	m_iFilterStrengthValue = 6;
}

cKissFFTConvertBase::~cKissFFTConvertBase()
{
	SAFE_DELETE(m_pFFTDataStore);
}

void	cKissFFTConvertBase::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_pFFTDataStore )
		this->m_pFFTDataStore->MouseDown(e_iPosX,e_iPosY);
}

void	cKissFFTConvertBase::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pFFTDataStore )
		this->m_pFFTDataStore->MouseUp(e_iPosX,e_iPosY);
}

void	cKissFFTConvertBase::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pFFTDataStore )
		this->m_pFFTDataStore->MouseMove(e_iPosX,e_iPosY);
}


void	cKissFFTConvertBase::DumpDebugInfo(int e_iDeciblesThreshold,const char*e_strFileName)
{
	std::string l_strOutputFileName = UT::ChangeFileExtensionName(e_strFileName,FREQUENCY_AND_DEIBELS_EXTENSION_FILE_NAME);
	if( m_pFFTDataStore )
		this->m_pFFTDataStore->Export(l_strOutputFileName.c_str(),e_strFileName);//,e_iDeciblesThreshold);
}

int	cKissFFTConvertBase::GetCurrentMaxFrequence(int e_iIndexOfFFTData,int e_iFrequence,int e_iCount,int e_iMaxAmplitude)
{
	int l_iFrequence = e_iIndexOfFFTData*e_iFrequence/e_iCount;
	this->m_iMaxAmplitude = e_iMaxAmplitude;
	//if( m_iMaxAmplitude <= 0 )
	//{
	//	int a=0;
	//}
	this->m_iMaxAmplitudeFrequence = l_iFrequence;
	return l_iFrequence;
}

float	cKissFFTConvertBase::GetFrequencyGapByFPS(int e_iFrequency,int e_iFPS)
{
	float l_fOffsetRange = (float)e_iFrequency/2.f/((float)e_iFrequency/e_iFPS);
	return l_fOffsetRange;
}

TiXmlElement*	cKissFFTConvertBase::ToTiXmlElement()
{
	TiXmlElement*l_pTiXmlElement = new TiXmlElement(cKissFFTConvertBase::TypeID);
	l_pTiXmlElement->SetAttribute(L"DivideFFTDataToNFrame",m_iDivideFFTDataToNFrame);
	l_pTiXmlElement->SetAttribute(L"NFrameFFTDataCount",m_iOneFrameFFTDataCount);
//	l_pTiXmlElement->SetAttribute(L"FrenquenceFilterEndScaleValue",m_fFrenquenceFilterEndScaleValue);
//	l_pTiXmlElement->SetAttribute(L"FilterStrengthValue",m_iFilterStrengthValue);
	//l_pTiXmlElement->SetAttribute(L"MaxAmplitudeFrequence",m_iMaxAmplitudeFrequence);
	//l_pTiXmlElement->SetAttribute(L"ChartScale",m_fChartScale);
	return l_pTiXmlElement;
}

void	cKissFFTConvertBase::SetDataFromTiXmlElement(TiXmlElement*e_pTiXmlElement)
{
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("DivideFFTDataToNFrame")
		{
			m_iDivideFFTDataToNFrame = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("OneFrameFFTDataCount")
		{
			m_iOneFrameFFTDataCount = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("FrenquenceFilterEndScaleValue")
		{
//			m_fFrenquenceFilterEndScaleValue = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("FilterStrengthValue")
		{
			//m_iFilterStrengthValue = VALUE_TO_INT;
		}
	PARSE_NAME_VALUE_END
}

void	cKissFFTConvertBase::RenderMaxAmplitudeAndFrequencyInfo(int e_iPosX,int e_iPosY)
{
	if( cGameApp::m_sbDebugFunctionWorking )
	{
		std::wstring l_strInfo = L"Frequence:";
		l_strInfo += ValueToStringW(this->m_iMaxAmplitudeFrequence);
		l_strInfo += L"\n";
		l_strInfo += L"MaxAmplitude";
		l_strInfo += ValueToStringW(this->m_iMaxAmplitude);
		cGameApp::m_spGlyphFontRender->SetScale(2.f);
		cGameApp::RenderFont(e_iPosX,e_iPosY,l_strInfo);
		cGameApp::m_spGlyphFontRender->SetScale(1.f);
	}
}


void	cKissFFTConvertBase::SetOneFrameFFTDataCount(int e_iFrequency)
{
	m_iFreq = e_iFrequency;
	int l_iOneFrameNumFFTData = e_iFrequency/m_iDivideFFTDataToNFrame;
	m_iOneFrameFFTDataCount = power_of_two(l_iOneFrameNumFFTData);
}

void	cKissFFTConvertBase::RenderDebugAmplitudeLine(float e_fAmplitude)
{
	if(cGameApp::m_sbDebugFunctionWorking)
	{
		Vector2	l_vChartResolution = this->m_vResolution;
		Vector2 l_vLinePos[2];
		l_vLinePos[0] = m_vShowPos;
		l_vLinePos[0].y -= e_fAmplitude*m_fChartAmplitudeScale;

		l_vLinePos[1] = l_vLinePos[0];
		l_vLinePos[1].x += l_vChartResolution.x;
		RenderLine((float*)&l_vLinePos,2,Vector4::Green,2);
	}
}


cKissFFTConvert::cKissFFTConvert()
{
	m_pfEachFFTDataDecibles = nullptr;
	m_pTestSound = nullptr;
	m_pSoundFile = nullptr;
	m_iCurrentFFTDataLineCount = 0;
	m_iNumFFTGraph = 0;
}
cKissFFTConvert::~cKissFFTConvert()
{
	Destroy();
}

int	cKissFFTConvert::GetNumFFTGraphByDuration(float e_fDuration)
{
	float l_fOneFFTDuration = this->m_pSoundFile->m_fTime/this->m_iNumFFTGraph;
	int l_iCount = (int)(e_fDuration/l_fOneFFTDuration);
	return l_iCount;
}

void	cKissFFTConvert::RenderDecibels(int e_iNumSampleCount,float*e_pfDeciblesData,Vector2 e_vShowPos,Vector2 e_vResolution)
{
	if(cGameApp::m_sbDebugFunctionWorking)
	{
		//0.1 second one decibles * 44100 sample rate * 60 seconds
		const int l_iCount = (int)(44100/256*60*2);
		float l_fXGap = e_vResolution.x/e_iNumSampleCount;
		if( e_iNumSampleCount >= l_iCount )
			return;
		Vector2 l_vShowPos = e_vShowPos;
		Vector2 l_vIAMLazyVector[l_iCount];
		for( int i=0;i<e_iNumSampleCount;++i )
		{
			l_vIAMLazyVector[i*2].x = e_vShowPos.x;
			l_vIAMLazyVector[i*2+1].x = e_vShowPos.x;
			l_vIAMLazyVector[i*2].y = e_vShowPos.y;
			l_vIAMLazyVector[i*2+1].y = (e_pfDeciblesData[i]*200)+e_vShowPos.y;
			e_vShowPos.x += l_fXGap;
		}
		RenderLine((float*)&l_vIAMLazyVector,e_iNumSampleCount*2,Vector4::Green,2);
		e_vShowPos = l_vShowPos;
		e_vShowPos.x -= l_fXGap;
		int l_iNumAccept = 0;
		int l_bHittedHigest = false;
		int l_iRestCount = cKissFFTConvert::GetNumFFTGraphByDuration(0.3f);
		int l_iRestCount2 = cKissFFTConvert::GetNumFFTGraphByDuration(0.5);
		for( int i=0;i<e_iNumSampleCount;++i )
		{
			e_vShowPos.x += l_fXGap;
			if( !l_bHittedHigest )
			{
				if( e_pfDeciblesData[i]+0.1f >= 1.f  )
					l_bHittedHigest = true;
				else
					continue;
			}
			if( e_pfDeciblesData[i] >= 0.1f  )
			{
				++l_iNumAccept;
			}
			else
				continue;
			int l_iIndex = l_iNumAccept-1;
			l_vIAMLazyVector[l_iIndex*2].x = e_vShowPos.x;
			l_vIAMLazyVector[l_iIndex*2+1].x = e_vShowPos.x;
			l_vIAMLazyVector[l_iIndex*2].y = e_vShowPos.y;
			l_vIAMLazyVector[l_iIndex*2+1].y = (e_pfDeciblesData[i]*200)+e_vShowPos.y;
		}
		RenderLine((float*)&l_vIAMLazyVector,l_iNumAccept*2,Vector4::Blue,2);

		Vector2 l_vCurrentTimeIndicator[2];
		float l_fCurrentPos = this->m_fCurrentTime/this->m_pSoundFile->m_fTime*e_vResolution.x+l_vShowPos.x;
		l_vCurrentTimeIndicator[0].x = l_vCurrentTimeIndicator[1].x = l_fCurrentPos;
		l_vCurrentTimeIndicator[0].y = l_vShowPos.y;
		l_vCurrentTimeIndicator[1].y = l_vCurrentTimeIndicator[0].y+50;
		RenderLine((float*)&l_vCurrentTimeIndicator,2,Vector4::Red,2);
	}
}

//void	cKissFFTConvert::PreProcessedAllData()
//{
//	if( !m_pSoundFile )
//		return;
//	int  channels = m_pSoundFile->m_iChannel;
//
//	m_FFTDataVectorChannelVector.reserve(m_pSoundFile->m_iSampleCount);
//
//	m_Timer.Update();
//	const int l_iFetchDataCount = m_iOneFrameFFTDataCount*this->m_pSoundFile->m_iChannel;
//	assert(l_iFetchDataCount>=this->m_iOneFrameFFTDataCount&&"frenquence is too high,is this okay?");
//	kiss_fft_cpx* l_pKiss_FFT_In = new kiss_fft_cpx[l_iFetchDataCount];
//	kiss_fft_cpx* l_pKiss_FFT_Out = new kiss_fft_cpx[l_iFetchDataCount];
//
//	size_t roof = m_pSoundFile->m_iSampleCount;
//	size_t framePointer = 0;
//
//	unsigned char*l_pucSoundData = m_pSoundFile->GetChannelData(0);;
//	int l_iNumWindow = 0;
//	while(framePointer < roof  )
//	{
//		size_t l_uiCount = 0;
//		size_t j = 0;
//		for( l_uiCount = framePointer; l_uiCount < (framePointer + l_iFetchDataCount)
//							&& framePointer < roof - l_iFetchDataCount ; l_uiCount++,j++  )
//		{
//			//https://en.wikipedia.org/wiki/Window_function#Hann_window
//			//Apply window function on the sample,Hann window
//			//double multiplier = 0.5 * (1 - cos(2*D3DX_PI*j/(l_iFetchDataCount-1)));
//			double multiplier = 1.f;
//			l_pKiss_FFT_In[j].r = (float)(multiplier * l_pucSoundData[l_uiCount]);
//			l_pKiss_FFT_In[j].i = 0;  //stores N samples 
//		}
//		++l_iNumWindow;
//		if(framePointer < roof-l_iFetchDataCount )
//		{
//			framePointer = l_uiCount;
//		}
//		else
//		{    
//			m_Timer.Update();
//			double secs = m_Timer.fElpaseTime;
//			// print_vec(array);
//			//std::cout<<"Total exec time: "<<secs<<std::endl;
//			break;
//		}
//		//std::cout<<"Framepointer = "<<framePointer<<std::endl;
//		GetFft(l_iFetchDataCount,l_pKiss_FFT_In,l_pKiss_FFT_Out);
//		// calculate magnitude of first n/2 FFT
//		for(int i = 0; i < l_iFetchDataCount/2; i++ )
//		{
//			int val;
//			float l_Msg = sqrt((l_pKiss_FFT_Out[i].r * l_pKiss_FFT_Out[i].r) + (l_pKiss_FFT_Out[i].i * l_pKiss_FFT_Out[i].i));
//			// N/2 Log magnitude values.
//			//for (i = 0; i < N/2 ; ++i){
//			//  x =   10 * log10(mag[i]) ;
//			//  printf("  log x= %g ", log(x));
//			//val = l_Msg;
//			val = (int)(log(l_Msg) *10); 
//			//m_FFTDataVectorChannelVector[i] = val;
//			m_FFTDataVectorChannelVector.push_back(val);
//		}
//	}
//	size_t l_uiSize = m_FFTDataVectorChannelVector.size();
//	SAFE_DELETE(l_pKiss_FFT_In);
//	SAFE_DELETE(l_pKiss_FFT_Out);
//}
//http://stackoverflow.com/questions/4675457/how-to-generate-the-audio-spectrum-using-fft-in-c
//1.convert audio data to format required by FFT (e.g. int -> float, separate L/R channels)
//2.apply suitable window function (e.g. Hann aka Hanning window)
//3.apply FFT (NB: if using typical complex-to-complex FFT then set imaginary parts of input array to zero)
//4.calculate magnitude of first N/2 FFT output bins (sqrt(re*re + im*im))
//5.optionally convert magnitude to dB (log) scale (20 * log10(magnitude))
//6.plot N/2 (log) magnitude values
void	cKissFFTConvert::PreProcessedAllData(cFFTDecibelsAnalyzer*e_pFFTDataStore)
{
	if( !m_pSoundFile )
		return;
	if( e_pFFTDataStore )
		e_pFFTDataStore->Start(m_pSoundFile->m_iFreq);
	assert(m_iOneFrameFFTDataCount>=this->m_iOneFrameFFTDataCount&&"frenquence is too high,is this okay?");
	assert(this->m_pSoundFile->m_iBitPerSample/8 == sizeof(short)&&"now only support one channel for 8 byte");

	m_Timer.Update();
	int					l_iChannels = m_pSoundFile->m_iChannel;
	int					l_iSameCount = m_pSoundFile->m_iSampleCount;
	kiss_fft_cpx*		l_pKiss_FFT_In = new kiss_fft_cpx[m_iOneFrameFFTDataCount];
	kiss_fft_cpx*		l_pKiss_FFT_Out = new kiss_fft_cpx[m_iOneFrameFFTDataCount];
	kiss_fft_state*		l_pkiss_fft_state = kiss_fft_alloc(m_iOneFrameFFTDataCount, 0/*is_inverse_fft*/, NULL, NULL);
	float*				l_pfWindowFunctionConstantValue = GenerateWindowsFunctionValue(m_iOneFrameFFTDataCount);		
	int					l_iOneStepSoundCount = m_iOneFrameFFTDataCount*l_iChannels*sizeof(short);
	float				l_fOneFrameDuration = (float)m_iOneFrameFFTDataCount/(float)m_pSoundFile->m_iSampleCount*m_pSoundFile->m_fTime;
	int					l_iNumFFT = l_iSameCount/m_iOneFrameFFTDataCount;
	int					l_iHalfFFTCount = m_iOneFrameFFTDataCount/WINDOWN_FUNCTION_FRUSTRUM;
	m_iNumFFTGraph = l_iNumFFT;
	m_pfEachFFTDataDecibles = new float[l_iNumFFT];
	m_FFTDataVector.resize(l_iNumFFT*l_iHalfFFTCount);
	m_fMaxDecible = -77979.f;
	for( int i=0;i<l_iNumFFT;++i )
	{
		int l_iSoundIndex = i*l_iOneStepSoundCount;
		char*l_pTargetData = (char*)m_pSoundFile->m_pSoundData+l_iSoundIndex;
		int*l_pOutFFTData = &(m_FFTDataVector)[i*l_iHalfFFTCount];
		sTimeAndPCMData l_sTimeAndPCMData(0,0,l_iChannels,l_pTargetData,m_iOneFrameFFTDataCount,eDataType::eDT_SHORT);
		ProcessFFT(&l_sTimeAndPCMData,l_pkiss_fft_state,l_pKiss_FFT_In,l_pKiss_FFT_Out,l_pfWindowFunctionConstantValue,l_pOutFFTData);
		float l_fDecibles = 0.f;
		for( int j=0;j<l_iHalfFFTCount;++j )
		{
			l_fDecibles += l_pOutFFTData[j];
		}
		m_pfEachFFTDataDecibles[i] = l_fDecibles;
		if( m_fMaxDecible <= l_fDecibles)
			m_fMaxDecible = l_fDecibles;
		if( e_pFFTDataStore )
			e_pFFTDataStore->UpdateFFTData(l_fOneFrameDuration,l_pOutFFTData,l_iHalfFFTCount);
	}
	for( int i=0;i<l_iNumFFT;++i )
	{
		m_pfEachFFTDataDecibles[i] /= m_fMaxDecible;
		if( m_pfEachFFTDataDecibles[i] +0.1f >= 1.f )
		{
			m_StartCaptureTimeVector.push_back(i*l_fOneFrameDuration);
		}
	}

	
	

	SAFE_DELETE(l_pfWindowFunctionConstantValue);
	free(l_pkiss_fft_state);
	delete[] l_pKiss_FFT_In;
	delete[] l_pKiss_FFT_Out;
	m_Timer.Update();
	double l_dbElpaseTime = m_Timer.fElpaseTime;
	std::wstring l_strDebugInfo = L"PreProcessedAllData spent:";
	l_strDebugInfo += ValueToStringW(l_dbElpaseTime);
	cGameApp::OutputDebugInfoString(l_strDebugInfo);
}

const int g_iFFTCountQuanty = 4;

//void	cKissFFTConvert::PreProcessedDoubleAllData(bool e_bFilter,cFFTDecibelsAnalyzer*e_pFFTDataStore)
//{
//	m_FFTDataStore.Start();
//	if( !m_pSoundFile )
//		return;
//	m_iOneFrameFFTDataCount *= g_iFFTCountQuanty;
//	assert(m_iOneFrameFFTDataCount>=this->m_iOneFrameFFTDataCount&&"frenquence is too high,is this okay?");
//	assert(this->m_pSoundFile->m_iBitPerSample/8 == sizeof(short)&&"now only support one channel for 8 byte");
//	m_Timer.Update();
//	int					l_iChannels = m_pSoundFile->m_iChannel;
//	size_t				l_uiSameCount = m_pSoundFile->m_iSampleCount;
//	kiss_fft_cpx*		l_pKiss_FFT_In = new kiss_fft_cpx[m_iOneFrameFFTDataCount];
//	kiss_fft_cpx*		l_pKiss_FFT_Out = new kiss_fft_cpx[m_iOneFrameFFTDataCount];
//	kiss_fft_state*		l_pkiss_fft_state = kiss_fft_alloc(m_iOneFrameFFTDataCount, 0/*is_inverse_fft*/, NULL, NULL);
//	float*				l_pfWindowFunctionConstantValue = GenerateWindowsFunctionValue(m_iOneFrameFFTDataCount);
//	float				l_fOneFrameDuration = (float)m_iOneFrameFFTDataCount/(float)m_pSoundFile->m_iSampleCount*m_pSoundFile->m_fTime;
//	int					l_iHalfFFTCount = m_iOneFrameFFTDataCount/WINDOWN_FUNCTION_FRUSTRUM;
//
//	int					l_iNumFFT = l_uiSameCount/m_iOneFrameFFTDataCount*g_iFFTCountQuanty;
//	int					l_iOneStepSoundCount = m_iOneFrameFFTDataCount/g_iFFTCountQuanty*l_iChannels*sizeof(short);
//
//	m_FFTDataVector.resize(l_iNumFFT*l_iHalfFFTCount);
//	for( int i=0;i<l_iNumFFT;++i )
//	{
//		int l_iSoundIndex = i*l_iOneStepSoundCount;
//		char*l_pTargetData = (char*)m_pSoundFile->m_pSoundData+l_iSoundIndex;
//		int l_EndIndex = m_iOneFrameFFTDataCount/g_iFFTCountQuanty+m_iOneFrameFFTDataCount;
//		if( l_EndIndex >= (int)l_uiSameCount)
//		{
//			break;
//		}
//		int*l_pOutFFTData = &(m_FFTDataVector)[i*l_iHalfFFTCount];
//		sTimeAndPCMData l_sTimeAndPCMData(0,0,l_iChannels,l_pTargetData,m_iOneFrameFFTDataCount,eDataType::eDT_SHORT);
//		ProcessFFT(&l_sTimeAndPCMData,l_pkiss_fft_state,l_pKiss_FFT_In,l_pKiss_FFT_Out,
//						   l_pfWindowFunctionConstantValue,l_pOutFFTData,
//						   e_bFilter,m_iFilterStrengthValue,m_fFrenquenceFilterEndScaleValue);
//		if( e_pFFTDataStore )
//			e_pFFTDataStore->UpdateFFTData(l_fOneFrameDuration,l_pOutFFTData,l_iHalfFFTCount);
//	}
//	SAFE_DELETE(l_pfWindowFunctionConstantValue);
//	free(l_pkiss_fft_state);
//	SAFE_DELETE(l_pKiss_FFT_In);
//	SAFE_DELETE(l_pKiss_FFT_Out);
//	m_Timer.Update();
//	double l_dbElpaseTime = m_Timer.fElpaseTime;
//	std::wstring l_strDebugInfo = L"PreProcessedDoubleAllData spent:";
//	l_strDebugInfo += ValueToStringW(l_dbElpaseTime);
//	cGameApp::OutputDebugInfoString(l_strDebugInfo);
//}

void	cKissFFTConvert::Destroy()
{
	m_StartCaptureTimeVector.clear();
	SAFE_DELETE(m_pfEachFFTDataDecibles);
	m_FFTDataVector.clear();
	m_FFTDataLinePointVector.clear();

	//DELETE_VECTOR(m_FFTDataLinePointVectorVector,std::vector<Vector2>*);
	//DELETE_VECTOR(m_FFTDataVectorChannelVector,std::vector<int>*);
	//DELETE_VECTOR(m_FFTResultPhaseVector,std::vector<float>*);
	SAFE_RELEASE(m_pTestSound,this);
	SAFE_DELETE(m_pSoundFile);
	SAFE_DELETE(m_pFFTDataStore);
}

void	cKissFFTConvert::Pause(bool e_bPause)
{
	this->m_bPause = e_bPause;
	if(this->m_pTestSound)
		m_pTestSound->Pause(e_bPause);
}

//
bool	cKissFFTConvert::FetchSoundDataStart(const char*e_strFileName,bool e_bPlaySound,bool e_bDoFFTDataStore)
{
	m_strSourceFileName = e_strFileName;
	Destroy();
	m_pSoundFile = new FATMING_CORE::cSoundFile();
	if(!m_pSoundFile->OpenFile(e_strFileName))
	{
		SAFE_DELETE(m_pSoundFile);
		return false;
	}
	cKissFFTConvertBase::SetOneFrameFFTDataCount(m_pSoundFile->m_iFreq);

	if( e_bDoFFTDataStore && !m_pFFTDataStore )
	{
		this->m_pFFTDataStore = new cFFTDecibelsAnalyzer();
		this->m_pFFTDataStore->Start(m_pSoundFile->m_iFreq);
	}

	PreProcessedAllData(e_bDoFFTDataStore?this->m_pFFTDataStore:nullptr);
	//PreProcessedDoubleAllData(this->m_bFilter,e_bDoFFTDataStore?&this->m_FFTDataStore:nullptr);
	if( e_bPlaySound )
	{
		//m_pTestSound = new cOpanalWAV(this,e_strFileName,false);
		m_pTestSound = new cOpanalWAV(this,nullptr,false);
		m_pTestSound->OpenFile(e_strFileName,true);
		m_pTestSound->Play(true);
	}
	m_fCurrentTime = 0.f;
	cGameApp::m_sTimeAndFPS.Update();
	return true;	
}

bool	cKissFFTConvert::FetchSoundData(int e_iStartDataIndex,int e_iCount)
{
	m_FFTDataLinePointVector.clear();
	int l_iTotalFFTData = (int)m_FFTDataVector.size();
	if( e_iStartDataIndex+e_iCount >= l_iTotalFFTData )
		return false;
	m_iCurrentFFTDataLineCount = 0;
	int l_iEndIndex = (int)e_iStartDataIndex+e_iCount;
	float l_fXGap = this->m_vResolution.x/(l_iEndIndex-e_iStartDataIndex)*m_fChartScale;	
	//float l_fXGap = this->m_pSoundFile->m_iFreq/this->m_vChartResolution.x*m_fChartScale;
	float l_fYGap = 0;
	float l_fStartXPos = m_vShowPos.x;
	float l_fYStartPos = m_vShowPos.y;
	//assert(m_FFTDataLinePointVectorVector.size() == m_FFTDataVectorChannelVector.size());

	int l_iMaxAmplitude = 0;
	int l_iMaxAmplitudeFrequency = 0;


	auto l_pFFTPointsData = &m_FFTDataLinePointVector;
	auto l_pFFTDataVector = &m_FFTDataVector;

	//auto l_pFFTPhaseDataVector = this->m_FFTResultPhaseVector[l_iChannelIndex];

	float l_fYPos = l_fYGap;
	int l_iIndex = 0;
	for(int i=e_iStartDataIndex;i<l_iEndIndex;++i)
	{
		int l_iAmplidude = (*l_pFFTDataVector)[i];
		Vector2 l_vPos,l_vPos1;
		//this->m_fFFTResultPhase[l_iChannelIndex][]
		//l_vPos1.x = l_vPos.x = (l_fXGap*(*l_pFFTPhaseData)[l_iIndex])+l_fStartXPos;
		l_vPos1.x = l_vPos.x = (l_fXGap*l_iIndex)+l_fStartXPos;
		l_vPos1.y = l_fYStartPos+l_fYPos;
		l_vPos.y = l_fYStartPos-(l_iAmplidude*m_fChartAmplitudeScale)+l_fYPos;
		//l_vPos.y = l_fYStartPos-((*l_pFFTDataVector)[i]*m_fChartAmplitudeScale*(*l_pFFTPhaseDataVector)[i]/4)+l_fYPos;
		l_pFFTPointsData->push_back(l_vPos);
		l_pFFTPointsData->push_back(l_vPos1);
		++l_iIndex;
		if( l_iMaxAmplitude < l_iAmplidude )
		{
			l_iMaxAmplitude = l_iAmplidude;
			this->GetCurrentMaxFrequence(i-e_iStartDataIndex,m_pSoundFile->m_iFreq,e_iCount,l_iMaxAmplitude);
		}
	}
	m_iCurrentFFTDataLineCount = l_iIndex;
	return true;
}

int		cKissFFTConvert::GetStartFFTIndexByTime(float e_fTime)
{
	float l_fTotalTime = m_pSoundFile->m_fTime;
	float l_fPercent = e_fTime/l_fTotalTime;
	if( l_fPercent >= 1.f )
		return -1;
	size_t l_uiFFTCount = this->m_FFTDataVector.size();
	int l_iIndexOfFFTData = (int)l_uiFFTCount/(this->m_iOneFrameFFTDataCount/WINDOWN_FUNCTION_FRUSTRUM);
	int l_iStartIndex = (int)((int)(l_iIndexOfFFTData*l_fPercent)*this->m_iOneFrameFFTDataCount/WINDOWN_FUNCTION_FRUSTRUM);
	return l_iStartIndex;
}

bool	cKissFFTConvert::FetchSoundDataByTimeRange(float e_fStartTime,float e_fDuring)
{
	int l_iStartIndex = GetStartFFTIndexByTime(this->m_fCurrentTime);
	if( l_iStartIndex == -1 )
		return false;
	int l_iDuringRangeCount = this->m_iOneFrameFFTDataCount/WINDOWN_FUNCTION_FRUSTRUM;
	return FetchSoundData(l_iStartIndex,l_iDuringRangeCount);
}

void	cKissFFTConvert::Update(float e_fElpaseTime)
{
	if( m_pSoundFile && !m_bPause )
	{
		float l_fElpaseTime = e_fElpaseTime;//0.016f;//e_fElpaseTime
		m_fCurrentTime += e_fElpaseTime;
		if( m_fCurrentTime >= this->m_pSoundFile->m_fTime )
		{//its end.
			return;
		}
		FetchSoundDataByTimeRange(m_fCurrentTime,l_fElpaseTime);



		//if(m_StartCaptureTimeVector.size())
		{
			//float l_fStartCaptureTime = m_StartCaptureTimeVector[0];
			//if( this->m_fCurrentTime >= l_fStartCaptureTime && l_fStartCaptureTime + 0.3f >= this->m_fCurrentTime )
			{
				int l_iStartIndex = GetStartFFTIndexByTime(this->m_fCurrentTime);
				if( l_iStartIndex != -1 )
				{
					auto l_pFFTDataVector = &m_FFTDataVector;
					int* l_piFFTData = &(*l_pFFTDataVector)[l_iStartIndex];
					if( m_pFFTDataStore )
						this->m_pFFTDataStore->UpdateFFTData(e_fElpaseTime,l_piFFTData,m_iOneFrameFFTDataCount/WINDOWN_FUNCTION_FRUSTRUM);
				}
			}
		}
	}
}

void	cKissFFTConvert::Render()
{
	if( m_iCurrentFFTDataLineCount > 0 )
	{
		cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::Red);
		cGameApp::m_spGlyphFontRender->SetScale(5.f);
		bool l_OnlyPoint = true;
		auto l_pDataVector = &m_FFTDataLinePointVector;
		if( !l_OnlyPoint )
		{
			for(int i=0;i<m_iCurrentFFTDataLineCount;++i)
			{
				float*l_pData = (float*)&(*l_pDataVector)[i*2];
				//if( i%m_iOneFrameFFTDataCount == 0 )
					//RenderLine(l_pData,2,Vector4::Red,2,cMatrix44::Identity,2);
				//else
					RenderLine(l_pData,2,Vector4::One,2,cMatrix44::Identity,2);
			}
		}
		else
		{
			RenderLine(l_pDataVector,Vector4::One);
		}
		cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
		cGameApp::m_spGlyphFontRender->SetScale(1.f);
		RenderMaxAmplitudeAndFrequencyInfo(1000,200);
		RenderDebugAmplitudeLine((float)cSoundCompareParameter::m_siDebugAmplitudeValue);
		Vector2 l_vPos = m_vShowPos;
		l_vPos.y += 100;
		RenderDecibels(m_iNumFFTGraph,this->m_pfEachFFTDataDecibles,l_vPos,this->m_vResolution);

		if( m_pFFTDataStore )
			this->m_pFFTDataStore->RenderCurrentData();
	}
	//std::vector<Vector2> l_Test;
	//l_Test.push_back(Vector2(100,500));
	//l_Test.push_back(Vector2(100,100));
	//l_Test.push_back(Vector2(200,500));
	//l_Test.push_back(Vector2(200,200));
	//RenderLine((float*)&l_Test[0],2,Vector4::Red,2,cMatrix44::Identity,2);
	//RenderLine((float*)&l_Test[2],2,Vector4::Red,2,cMatrix44::Identity,2);
}

float	cKissFFTConvert::GetCurrentTimePercentage()
{
	if(!this->m_pSoundFile)
		return 0.f;
	float l_fPercentage = this->m_fCurrentTime/this->m_pSoundFile->m_fTime;
	if( l_fPercentage >= 1.f )
		l_fPercentage = 1.f;
	return l_fPercentage;
	
}

float	cKissFFTConvert::GetTimeLength()
{
	if(this->m_pSoundFile)
	{
		return this->m_pSoundFile->m_fTime;
	}
	return 0.f;
}

void	cKissFFTConvert::Play()
{
	SAFE_RELEASE(m_pTestSound,this);
	m_pTestSound = new cOpanalWAV(this,nullptr,false);
	m_pTestSound->OpenFile(this->m_strSourceFileName.c_str(),true);
	m_pTestSound->Play(true);
	if( m_pFFTDataStore )
		this->m_pFFTDataStore->Start(m_pTestSound->GetFreq());
}

void	cKissFFTConvert::GoToTime(float e_fElpaseTime)
{
	if( m_fCurrentTime == e_fElpaseTime )
		return;
	m_fCurrentTime = e_fElpaseTime;
	if( m_pSoundFile && m_pFFTDataStore )
		this->m_pFFTDataStore->Start(this->m_pSoundFile->m_iFreq);
	return;
	if(this->m_pTestSound)
	{//fuck I have no idea...why crash...
		m_pTestSound->GoTo(e_fElpaseTime);
	}
}


//http://stackoverflow.com/questions/7674877/how-to-get-frequency-from-fft-result
////N = 1024          // size of FFT and sample window
//Fs = 44100        // sample rate = 44.1 kHz
//data[N]           // input PCM data buffer
//fft[N * 2]        // FFT complex buffer (interleaved real/imag)
//magnitude[N / 2]  // power spectrum
//
//capture audio in data[] buffer
//apply window function to data[]
//
//// copy real input data to complex FFT buffer
//for i = 0 to N - 1
//  fft[2*i] = data[i]
//  fft[2*i+1] = 0
//
//perform in-place complex-to-complex FFT on fft[] buffer
//
//// calculate power spectrum (magnitude) values from fft[]
//for i = 0 to N / 2 - 1
//  re = fft[2*i]
//  im = fft[2*i+1]
//  magnitude[i] = sqrt(re*re+im*im)
//
//// find largest peak in power spectrum
//max_magnitude = -INF
//max_index = -1
//for i = 0 to N / 2 - 1
//  if magnitude[i] > max_magnitude
//    max_magnitude = magnitude[i]
//    max_index = i
//
//// convert index of largest peak to frequency
//freq = max_index * Fs / N

//bool	cKissFFTConvert::ExportFFTDataStore(const char*e_strFileName,const char*e_strExportFileName)
//{
//	if(FetchSoundDataStart(e_strFileName,false,true))
//	{
//		return this->m_FFTDataStore.Export(e_strExportFileName);
//	}
//	return false;
//}

//void	cKissFFTConvert::Update(float e_fElpaseTime)
//{
//	
//}


//http://stackoverflow.com/questions/35484919/how-to-perform-fft-on-wav-file-data
//void calculateFrequencies(fftw_complex *data, size_t len, int Fs) {
//    for (int i = 0; i < len; i++) {
//        int re, im;
//        float freq, magnitude;
//        int index;
//
//        re = data[i][0];
//        im = data[i][1];
//
//        magnitude = sqrt(re * re + im * im);
//        freq = i * Fs / len;
//
//        index = freq / 1000;//round(freq);
//        if (index <= MAX_FREQ) {
//            freqMagnitude[index] += magnitude;
//        }
//    }
//}
//https://rosettacode.org/wiki/Fast_Fourier_transform
//
//#include <complex>
//#include <iostream>
//#include <valarray>
// 
//const double PI = 3.141592653589793238460;
// 
//typedef std::complex<double> Complex;
//typedef std::valarray<Complex> CArray;
// 
//// Cooley¡VTukey FFT (in-place, divide-and-conquer)
//// Higher memory requirements and redundancy although more intuitive
//void fft(CArray& x)
//{
//    const size_t N = x.size();
//    if (N <= 1) return;
// 
//    // divide
//    CArray even = x[std::slice(0, N/2, 2)];
//    CArray  odd = x[std::slice(1, N/2, 2)];
// 
//    // conquer
//    fft(even);
//    fft(odd);
// 
//    // combine
//    for (size_t k = 0; k < N/2; ++k)
//    {
//        Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k];
//        x[k    ] = even[k] + t;
//        x[k+N/2] = even[k] - t;
//    }
//}
// 
//// Cooley-Tukey FFT (in-place, breadth-first, decimation-in-frequency)
//// Better optimized but less intuitive
//// !!! Warning : in some cases this code make result different from not optimased version above (need to fix bug)
//void fft(CArray &x)
//{
//	// DFT
//	unsigned int N = x.size(), k = N, n;
//	double thetaT = 3.14159265358979323846264338328L / N;
//	Complex phiT = Complex(cos(thetaT), sin(thetaT)), T;
//	while (k > 1)
//	{
//		n = k;
//		k >>= 1;
//		phiT = phiT * phiT;
//		T = 1.0L;
//		for (unsigned int l = 0; l < k; l++)
//		{
//			for (unsigned int a = l; a < N; a += n)
//			{
//				unsigned int b = a + k;
//				Complex t = x[a] - x[b];
//				x[a] += x[b];
//				x[b] = t * T;
//			}
//			T *= phiT;
//		}
//	}
//	// Decimate
//	unsigned int m = (unsigned int)log2(N);
//	for (unsigned int a = 0; a < N; a++)
//	{
//		unsigned int b = a;
//		// Reverse bits
//		b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
//		b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
//		b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
//		b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
//		b = ((b >> 16) | (b << 16)) >> (32 - m);
//		if (b > a)
//		{
//			Complex t = x[a];
//			x[a] = x[b];
//			x[b] = t;
//		}
//	}
//	//// Normalize (This section make it not working correctly)
//	//Complex f = 1.0 / sqrt(N);
//	//for (unsigned int i = 0; i < N; i++)
//	//	x[i] *= f;
//}
// 
//// inverse fft (in-place)
//void ifft(CArray& x)
//{
//    // conjugate the complex numbers
//    x = x.apply(std::conj);
// 
//    // forward fft
//    fft( x );
// 
//    // conjugate the complex numbers again
//    x = x.apply(std::conj);
// 
//    // scale the numbers
//    x /= x.size();
//}
// 
//int main()
//{
//    const Complex test[] = { 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0 };
//    CArray data(test, 8);
// 
//    // forward fft
//    fft(data);
// 
//    std::cout << "fft" << std::endl;
//    for (int i = 0; i < 8; ++i)
//    {
//        std::cout << data[i] << std::endl;
//    }
// 
//    // inverse fft
//    ifft(data);
// 
//    std::cout << std::endl << "ifft" << std::endl;
//    for (int i = 0; i < 8; ++i)
//    {
//        std::cout << data[i] << std::endl;
//    }
//    return 0;
//}
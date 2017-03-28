#include "stdafx.h"
#include "KissFFTConvert.h"

cKissFFTConvertBase::cKissFFTConvertBase()
{
	m_TimeToUpdateFFTData.SetTargetTime(1.f/6.f);
	m_TimeToUpdateFFTData.SetLoop(true);
	m_iDivideFFTDataToNFrame = 60;
	m_fCurrentTime = 0.f;
	m_iNFrameFFTDataCount = 0;
	m_vChartResolution = Vector2(1280.f,600.f);
	m_fScale = 3.f;
	m_fNextChannelYGap = 800.f;
	m_vChartShowPos = Vector2(200,cGameApp::m_svGameResolution.y-200);
	m_bPause = false;
}

cKissFFTConvertBase::~cKissFFTConvertBase()
{

}

void	cKissFFTConvertBase::SetFFTDataUpdateTime(float e_fTime)
{
	m_TimeToUpdateFFTData.SetTargetTime(e_fTime);
}

//bool GetFft(const int e_iCount,const kiss_fft_cpx*e_pIn,kiss_fft_cpx*e_pOut)
//{
//	kiss_fft_cfg cfg;
//	if ((cfg = kiss_fft_alloc(e_iCount, 0/*is_inverse_fft*/, NULL, NULL)) != NULL)
//	{
//		kiss_fft(cfg, e_pIn, e_pOut);
//		free(cfg);
//	}
//	else
//	{
//		printf("not enough memory?\n");
//		return false;
//	}
//	return true;
//}

cKissFFTConvert::cKissFFTConvert()
{
	m_pTestSound = nullptr;
	m_pSoundFile = nullptr;
	m_iCurrentFFTDataLineCount = 0;
}
cKissFFTConvert::~cKissFFTConvert()
{
	Destroy();
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
//	const int l_iFetchDataCount = m_iNFrameFFTDataCount*this->m_pSoundFile->m_iChannel;
//	assert(l_iFetchDataCount>=this->m_iNFrameFFTDataCount&&"frenquence is too high,is this okay?");
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
void	cKissFFTConvert::PreProcessedAllData()
{
	if( !m_pSoundFile )
		return;
	int  l_iChannels = m_pSoundFile->m_iChannel;
	m_Timer.Update();
	const int l_iFetchDataCount = m_iNFrameFFTDataCount;//*this->m_pSoundFile->m_iChannel;
	assert(l_iFetchDataCount>=this->m_iNFrameFFTDataCount&&"frenquence is too high,is this okay?");
	assert(this->m_pSoundFile->m_iBitPerSample/8 == sizeof(short)&&"now only support one channel for 8 byte");

	size_t l_uiSameCount = m_pSoundFile->m_iSampleCount;
	kiss_fft_cpx* l_pKiss_FFT_In = new kiss_fft_cpx[l_iFetchDataCount];
	kiss_fft_cpx* l_pKiss_FFT_Out = new kiss_fft_cpx[l_iFetchDataCount];
	kiss_fft_state* l_pkiss_fft_state = kiss_fft_alloc(l_iFetchDataCount, 0/*is_inverse_fft*/, NULL, NULL);
	bool l_bDoFFT = true;
	for(int l_iCurrentChannelIndex = 0;l_iCurrentChannelIndex<l_iChannels;++l_iCurrentChannelIndex)
	{
		std::vector<int>*l_pFFTDataVector = new std::vector<int>;
		l_pFFTDataVector->reserve(m_pSoundFile->m_iSampleCount);

		size_t l_uiCurrentSoundDataIndex = 0;

		short*l_pucSoundData = (short*)m_pSoundFile->GetChannelData(l_iCurrentChannelIndex);
		int l_iNumWindow = 0;
		while(l_uiCurrentSoundDataIndex < l_uiSameCount  )
		{
			if( l_bDoFFT )
			{
				size_t l_uiCount = 0;
				size_t j = 0;
				for( l_uiCount = l_uiCurrentSoundDataIndex;
					 l_uiCount < (l_uiCurrentSoundDataIndex + l_iFetchDataCount) && l_uiCurrentSoundDataIndex < l_uiSameCount - l_iFetchDataCount ;
					 l_uiCount++,j++  )
				{
					//https://en.wikipedia.org/wiki/Window_function#Hann_window
					//Apply window function on the sample,Hann window
					double multiplier = 0.5 * (1 - cos(2*D3DX_PI*j/(l_iFetchDataCount-1)));
					l_pKiss_FFT_In[j].r = (float)(multiplier * l_pucSoundData[l_uiCount]);
					l_pKiss_FFT_In[j].i = 0;  //stores N samples 
				}
				++l_iNumWindow;
				if(l_uiCurrentSoundDataIndex < l_uiSameCount-l_iFetchDataCount )
				{
					l_uiCurrentSoundDataIndex = l_uiCount;
				}
				else
				{    
					size_t l_uFFTDataVectoriSize = l_pFFTDataVector->size();
					m_FFTDataVectorChannelVector.push_back(l_pFFTDataVector);
					break;
				}
				//GetFft(l_iFetchDataCount,l_pKiss_FFT_In,l_pKiss_FFT_Out);
				kiss_fft(l_pkiss_fft_state, l_pKiss_FFT_In, l_pKiss_FFT_Out);
				// calculate magnitude of first n/2 FFT
				for(int i = 0; i < l_iFetchDataCount/2; i++ )
				{
					int val;
					float l_Msg = sqrt((l_pKiss_FFT_Out[i].r * l_pKiss_FFT_Out[i].r) + (l_pKiss_FFT_Out[i].i * l_pKiss_FFT_Out[i].i));
					// N/2 Log magnitude values.
					//for (i = 0; i < N/2 ; ++i){
					//  x =   10 * log10(mag[i]) ;
					//  printf("  log x= %g ", log(x));
					//val = l_Msg;
					val = (int)(log(l_Msg) *10); 
					//l_FFTDataVector[i] = val;
					l_pFFTDataVector->push_back(val);
				}
			}
			else
			{
				m_fScale = 0.1f;
				size_t l_uiCount = 0;
				for( l_uiCount = l_uiCurrentSoundDataIndex;l_uiCount < l_uiSameCount;l_uiCount++)
				{
					l_pFFTDataVector->push_back(l_pucSoundData[l_uiCount]);
				}
				size_t l_uFFTDataVectoriSize = l_pFFTDataVector->size();
				m_FFTDataVectorChannelVector.push_back(l_pFFTDataVector);
				break;	
			}
		}
		size_t l_uiSize = l_pFFTDataVector->size();
	}
	free(l_pkiss_fft_state);
	SAFE_DELETE(l_pKiss_FFT_In);
	SAFE_DELETE(l_pKiss_FFT_Out);
	m_Timer.Update();
	double secs = m_Timer.fElpaseTime;
	cGameApp::OutputDebugInfoString(ValueToString(secs));
}
void	cKissFFTConvert::Destroy()
{
	DELETE_VECTOR(m_FFTDataLinePointVectorVector,std::vector<Vector2>*);
	DELETE_VECTOR(m_FFTDataVectorChannelVector,std::vector<int>*);
	SAFE_RELEASE(m_pTestSound,this);
	//m_pTestSound->Release();
	SAFE_DELETE(m_pSoundFile);
}
//
bool	cKissFFTConvert::FetchSoundDataStart(const char*e_strFileName)
{
	m_fCurrentTime = 0.f;
	Destroy();
	m_pSoundFile = new FATMING_CORE::cSoundFile();
	if(!m_pSoundFile->OpenFile(e_strFileName))
	{
		SAFE_DELETE(m_pSoundFile);
		return false;
	}
	//for 60 fps
	m_iNFrameFFTDataCount = m_pSoundFile->m_iFreq/m_iDivideFFTDataToNFrame;
	PreProcessedAllData();
	m_pTestSound = new cOpanalWAV(this,e_strFileName,false);
	m_pTestSound->Play(true);
	return true;	
}

bool	cKissFFTConvert::FetchSoundData(int e_iStartDataIndex,int e_iCount)
{
	if(m_FFTDataVectorChannelVector.size() < 1 || e_iCount == 0 )
		return false;
	for(size_t i=0;i<m_FFTDataLinePointVectorVector.size();++i)
	{
		m_FFTDataLinePointVectorVector[i]->clear();
	}
	m_iCurrentFFTDataLineCount = 0;
	int l_m_FFTDataVectorChannelVectorSize = (int)m_FFTDataVectorChannelVector[0]->size();
	if( e_iStartDataIndex >= l_m_FFTDataVectorChannelVectorSize )
		return false;
	int l_iEndIndex = (int)e_iStartDataIndex+e_iCount;
	if( l_iEndIndex >= (int)m_FFTDataVectorChannelVector[0]->size())
	{
		l_iEndIndex = (int)m_FFTDataVectorChannelVector[0]->size()-1;
	}
	float l_fXGap = this->m_vChartResolution.x/(l_iEndIndex-e_iStartDataIndex);
	float l_fYGap = m_fNextChannelYGap;
	float l_fStartXPos = m_vChartShowPos.x;
	float l_fYStartPos = m_vChartShowPos.y;
	int l_iIndex = 0;
	assert(m_FFTDataLinePointVectorVector.size() == m_FFTDataVectorChannelVector.size());
	for( size_t l_iChannelIndex = 0;l_iChannelIndex<m_FFTDataLinePointVectorVector.size();++l_iChannelIndex )
	{
		auto l_pData = m_FFTDataLinePointVectorVector[l_iChannelIndex];
		auto l_pFFTData = this->m_FFTDataVectorChannelVector[l_iChannelIndex];
		float l_fYPos = l_fYGap*l_iChannelIndex;
		int l_iIndex = 0;
		for(int i=e_iStartDataIndex;i<l_iEndIndex;++i)
		{
			Vector2 l_vPos,l_vPos1;
			l_vPos1.x = l_vPos.x = (l_fXGap*l_iIndex)+l_fStartXPos;
			l_vPos1.y = l_fYStartPos+l_fYPos;
			l_vPos.y = l_fYStartPos-((*l_pFFTData)[i]*m_fScale)+l_fYPos;
			l_pData->push_back(l_vPos);
			l_pData->push_back(l_vPos1);
			++l_iIndex;
		}
		m_iCurrentFFTDataLineCount = l_iIndex;
	}
	return true;
}

bool	cKissFFTConvert::FetchSoundDataByTimeRange(float e_fStartTime,float e_fDuring)
{
	if( m_FFTDataVectorChannelVector.size() < 1 )
		return false;
	float l_fTotalTime = m_pSoundFile->m_fTime;
	float l_fPercent = m_fCurrentTime/l_fTotalTime;
	if( l_fPercent >= 1.f )
		return false;
	int	l_iTargetIndex = (int)(m_fCurrentTime/(1.f/this->m_iDivideFFTDataToNFrame));
	//because char to short? or
	//because 6.plot N/2 (log) magnitude values,so divide 2
	int l_iStartIndex = l_iTargetIndex*(m_iNFrameFFTDataCount/WINDOWN_FUNCTION_FRUSTRUM);
	int l_iDuringRangeCount = this->m_iNFrameFFTDataCount/WINDOWN_FUNCTION_FRUSTRUM;
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
		if( m_FFTDataLinePointVectorVector.size() == 0 )
		{
			for(int i=0;i<m_pSoundFile->m_iChannel;++i)
			{
				std::vector<Vector2>*l_pData = new std::vector<Vector2>();
				l_pData->reserve(m_iNFrameFFTDataCount);
				m_FFTDataLinePointVectorVector.push_back(l_pData);
			}
		}
		m_TimeToUpdateFFTData.Update(e_fElpaseTime);
		if( m_TimeToUpdateFFTData.bTragetTimrReached )
			FetchSoundDataByTimeRange(m_fCurrentTime,l_fElpaseTime);
	}
}

void	cKissFFTConvert::Render()
{
	if( m_iCurrentFFTDataLineCount > 0 )
	{
		size_t l_uiSize = m_FFTDataLinePointVectorVector.size();
		cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::Red);
		cGameApp::m_spGlyphFontRender->SetScale(5.f);
		bool l_OnlyPoint = true;
		for(size_t l_iCurrentIndex=0;l_iCurrentIndex<l_uiSize;++l_iCurrentIndex)
		{
			auto l_pDataVector = m_FFTDataLinePointVectorVector[l_iCurrentIndex];
			if( !l_OnlyPoint )
			{
				for(int i=0;i<m_iCurrentFFTDataLineCount;++i)
				{
					float*l_pData = (float*)&(*l_pDataVector)[i*2];
					//if( i%m_iNFrameFFTDataCount == 0 )
						//RenderLine(l_pData,2,Vector4::Red,2,cMatrix44::Identity,2);
					//else
						RenderLine(l_pData,2,Vector4::One,2,cMatrix44::Identity,2);
					if( i == 0 )
					{
						if( l_iCurrentIndex == 0 )
							cGameApp::RenderFont(l_pData[0]-200,1000.f,L"LeftChannel");
						else
							cGameApp::RenderFont(l_pData[0]-200,1800.f,L"RightChannel");
					}
					//if( i % 30 == 0 )
					//{
						//cGameApp::RenderFont((int)l_pData[0],100,ValueToStringW(i));
						//l_pData[1] = -100;
						//RenderLine(l_pData,2,Vector4::Red,2,cMatrix44::Identity,2);
					//}
				}
			}
			else
			{
				RenderLine(l_pDataVector,Vector4::One);
			}
		}
		cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
		cGameApp::m_spGlyphFontRender->SetScale(1.f);
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

void	cKissFFTConvert::GoToTime(float e_fElpaseTime)
{
	if(this->m_pSoundFile)
	{
		this->GoToTime(e_fElpaseTime);
	}
}

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
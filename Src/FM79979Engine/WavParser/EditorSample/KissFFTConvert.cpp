#include "stdafx.h"
#include "KissFFTConvert.h"


float Scale(kiss_fft_scalar val)
{
    int g = 0;
    return val < 0 ? val*(1/32768.0f ) : val*(1/32767.0f);
}

bool GetFft(const int e_iCount,const kiss_fft_cpx*e_pIn,kiss_fft_cpx*e_pOut)
{
	kiss_fft_cfg cfg;
	if ((cfg = kiss_fft_alloc(e_iCount, 0/*is_inverse_fft*/, NULL, NULL)) != NULL)
	{
		kiss_fft(cfg, e_pIn, e_pOut);
		free(cfg);
	}
	else
	{
		printf("not enough memory?\n");
		return false;
	}
	return true;
}

cKissFFTConvert::cKissFFTConvert()
{
	m_pTestSound = nullptr;
	m_pSoundFile = nullptr;
	m_pSampleDataArray = nullptr;
	m_fCurrentTime = 0.f;
	m_iCurrentFFTDataLineCount = 0;
	m_iFPSDataCount = 0;
	m_iTargetFPS = 60;
	m_fScale = 1.5f;
	m_fXGap = 5.f;
}
cKissFFTConvert::~cKissFFTConvert()
{
	SAFE_DELETE(m_pTestSound);
	SAFE_DELETE(m_pSoundFile);
	SAFE_DELETE(m_pSampleDataArray);
}

//void	cKissFFTConvert::PreProcessedAllData()
//{
//	if( !m_pSoundFile )
//		return;
//	int  channels = m_pSoundFile->m_iChannel;
//
//	m_FFTDataArray.reserve(m_pSoundFile->m_iSampleCount);
//
//	m_Timer.Update();
//	const int l_iFetchDataCount = this->m_pSoundFile->m_iFreq/m_iTargetFPS*this->m_pSoundFile->m_iChannel;
//	assert(l_iFetchDataCount>=this->m_iFPSDataCount&&"frenquence is too high,is this okay?");
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
//			//m_FFTDataArray[i] = val;
//			m_FFTDataArray.push_back(val);
//		}
//	}
//	size_t l_uiSize = m_FFTDataArray.size();
//	SAFE_DELETE(l_pKiss_FFT_In);
//	SAFE_DELETE(l_pKiss_FFT_Out);
//}
void	cKissFFTConvert::PreProcessedAllData()
{
	if( !m_pSoundFile )
		return;
	int  channels = m_pSoundFile->m_iChannel;

	m_FFTDataArray.reserve(m_pSoundFile->m_iSampleCount);

	m_Timer.Update();
	const int l_iFetchDataCount = this->m_pSoundFile->m_iFreq/m_iTargetFPS*this->m_pSoundFile->m_iChannel;
	assert(l_iFetchDataCount>=this->m_iFPSDataCount&&"frenquence is too high,is this okay?");
	kiss_fft_cpx* l_pKiss_FFT_In = new kiss_fft_cpx[l_iFetchDataCount];
	kiss_fft_cpx* l_pKiss_FFT_Out = new kiss_fft_cpx[l_iFetchDataCount];

	size_t roof = m_pSoundFile->m_iSampleCount;
	size_t framePointer = 0;

	unsigned char*l_pucSoundData = m_pSoundFile->GetChannelData(0);
	int l_iNumWindow = 0;
	while(framePointer < roof  )
	{
		size_t l_uiCount = 0;
		size_t j = 0;
		for( l_uiCount = framePointer; l_uiCount < (framePointer + l_iFetchDataCount)
							&& framePointer < roof - l_iFetchDataCount ; l_uiCount++,j++  )
		{
			//https://en.wikipedia.org/wiki/Window_function#Hann_window
			//Apply window function on the sample,Hann window
			//double multiplier = 0.5 * (1 - cos(2*D3DX_PI*j/(l_iFetchDataCount-1)));
			double multiplier = 1.f;
			l_pKiss_FFT_In[j].r = (float)(multiplier * l_pucSoundData[l_uiCount]);
			l_pKiss_FFT_In[j].i = 0;  //stores N samples 
		}
		++l_iNumWindow;
		if(framePointer < roof-l_iFetchDataCount )
		{
			framePointer = l_uiCount;
		}
		else
		{    
			m_Timer.Update();
			double secs = m_Timer.fElpaseTime;
			// print_vec(array);
			//std::cout<<"Total exec time: "<<secs<<std::endl;
			break;
		}
		//std::cout<<"Framepointer = "<<framePointer<<std::endl;
		GetFft(l_iFetchDataCount,l_pKiss_FFT_In,l_pKiss_FFT_Out);
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
			//m_FFTDataArray[i] = val;
			m_FFTDataArray.push_back(val);
		}
	}
	size_t l_uiSize = m_FFTDataArray.size();
	SAFE_DELETE(l_pKiss_FFT_In);
	SAFE_DELETE(l_pKiss_FFT_Out);
}
//http://stackoverflow.com/questions/4675457/how-to-generate-the-audio-spectrum-using-fft-in-c
//1.convert audio data to format required by FFT (e.g. int -> float, separate L/R channels)
//2.apply suitable window function (e.g. Hann aka Hanning window)
//3.apply FFT (NB: if using typical complex-to-complex FFT then set imaginary parts of input array to zero)
//4.calculate magnitude of first N/2 FFT output bins (sqrt(re*re + im*im))
//5.optionally convert magnitude to dB (log) scale (20 * log10(magnitude))
//6.plot N/2 (log) magnitude values
bool	cKissFFTConvert::FetchSoundDataStart(const char*e_strFileName)
{
	SAFE_DELETE(m_pSoundFile);
	SAFE_DELETE(m_pSampleDataArray);
	m_pSoundFile = new FATMING_CORE::cSoundFile();
	if(!m_pSoundFile->OpenFile(e_strFileName))
	{
		SAFE_DELETE(m_pSoundFile);
		return false;
	}
	m_fCurrentTime = 0.f;
	//for 60 fps
	m_iFPSDataCount = m_pSoundFile->m_iFreq/m_iTargetFPS;
	m_FFTDataLinePointVector.reserve(m_iFPSDataCount);
	m_pSampleDataArray = new char[m_iFPSDataCount];
	PreProcessedAllData();
	SAFE_RELEASE(m_pTestSound,this);
	m_pTestSound = new cOpanalWAV(this,e_strFileName,false);
	m_pTestSound->Play(true);
	m_iTest = 0;
	return true;	
}

bool	cKissFFTConvert::FetchSoundData(int e_iStartDataIndex,int e_iCount)
{
	m_FFTDataLinePointVector.clear();
	m_iCurrentFFTDataLineCount = 0;
	if( e_iStartDataIndex >= (int)m_FFTDataArray.size() )
		return false;
	int l_iEndIndex = (int)e_iStartDataIndex+e_iCount;
	if( l_iEndIndex >= (int)m_FFTDataArray.size())
	{
		l_iEndIndex = (int)m_FFTDataArray.size()-1;
	}
	float l_fXGap = m_fXGap;
	float l_fStartXPos = 200.f;
	float l_fYStartPos = cGameApp::m_svGameResolution.y-200;
	int l_iIndex = 0;
	for(int i=e_iStartDataIndex;i<l_iEndIndex;++i)
	{
		Vector2 l_vPos,l_vPos1;
		l_vPos1.x = l_vPos.x = (l_fXGap*m_iCurrentFFTDataLineCount)+l_fStartXPos;
		l_vPos1.y = l_fYStartPos;
		l_vPos.y = l_fYStartPos-(m_FFTDataArray[i]*m_fScale);
		m_FFTDataLinePointVector.push_back(l_vPos);
		m_FFTDataLinePointVector.push_back(l_vPos1);
		++m_iCurrentFFTDataLineCount;
	}
	//one line 2 points
	//m_iCurrentFFTDataLineCount *= 2;
	return true;
}

bool	cKissFFTConvert::FetchSoundDataByTimeRange(float e_fStartTime,float e_fDuring)
{
	float l_fTotalTime = m_pSoundFile->m_fTime;
	float l_fPercent = m_fCurrentTime/l_fTotalTime;
	if( l_fPercent >= 1.f )
		return false;
	int l_iStartIndex = (int)(l_fPercent*m_FFTDataArray.size());
	//int l_iDuringRangeCount = 100;//(int)(m_FFTDataArray.size()*(e_fDuring/l_fTotalTime));
	int l_iDuringRangeCount = (int)(m_FFTDataArray.size()*(e_fDuring/l_fTotalTime));
	//int l_iIndex = m_iTest;
	//++m_iTest;
	//int l_iStartIndex = l_iIndex*m_iFPSDataCount;
	//int l_iDuringRangeCount = m_iFPSDataCount;
	return FetchSoundData(l_iStartIndex,l_iDuringRangeCount);
}

void	cKissFFTConvert::Update(float e_fElpaseTime)
{
	if( m_pSoundFile )
	{
		float l_fElpaseTime = 0.016f;//e_fElpaseTime
		m_fCurrentTime += e_fElpaseTime;
		if( m_fCurrentTime >= this->m_pSoundFile->m_fTime )
		{//its end.
			return;
		}
		FetchSoundDataByTimeRange(m_fCurrentTime,l_fElpaseTime);
	}
}

void	cKissFFTConvert::Render()
{
	if( m_iCurrentFFTDataLineCount > 0 )
	{
		size_t l_uiSize = m_FFTDataLinePointVector.size();
		for(int i=0;i<m_iCurrentFFTDataLineCount;++i)
		{
			float*l_pData = (float*)&m_FFTDataLinePointVector[i*2];
			if( i%m_iFPSDataCount == 0 )
				RenderLine(l_pData,2,Vector4::Red,2,cMatrix44::Identity,2);
			else
				RenderLine(l_pData,2,Vector4::One,2,cMatrix44::Identity,2);
			if( i % 30 == 0 )
			{
				//cGameApp::RenderFont((int)l_pData[0],100,ValueToStringW(i));
				l_pData[1] = -100;
				RenderLine(l_pData,2,Vector4::Red,2,cMatrix44::Identity,2);
			}
		}
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

//void	cKissFFTConvert::Update(float e_fElpaseTime)
//{
//	
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
#pragma once
#include "KissFFTStreaming.h"

void	SoundFFTCaptureKissFFTStreamingConvertThread(size_t _workParameter, size_t _pUri);
void	SoundFFTCaptureKissFFTStreamingConvertThreadDone(size_t _workParameter, size_t _pUri);

class cSoundFFTCapture:public FATMING_CORE::cSounRecordCallBackObject,public cKissFFTConvertBase
{
	friend void	SoundFFTCaptureKissFFTStreamingConvertThread(size_t _workParameter, size_t _pUri);
	friend void	SoundFFTCaptureKissFFTStreamingConvertThreadDone(size_t _workParameter, size_t _pUri);
	//int								m_iFFTSampleForOneFrame;
	cFUSynchronized					m_FUSynchronizedForTimeAndPCMDataVector;
	bool							m_bThreadInPause;
	bool							m_bThreadAlreadyStop;
	bool							m_bThreadStop;
	bool							m_bThreadPause;
	float							m_fNextSampleTime;
	cPCMToFFTDataConvertr			m_PCMToFFTDataConvertr;
	cFUThreadPool*					m_pFUThreadPool;
	//
	int								m_iCurrentStreamingBufferDataIndex;
	char							m_StreamingBufferData[PCM_SWAP_BUFFER_COUNT][OGG_STREAMING_SOUND_BUFFER_SIZE];
	std::vector<sTimeAndPCMData*>	m_TimeAndPCMDataVector;
	Vector2							m_vFFTDataToPoints[FFT_DATA_LINE_POINTS_COUNT];
public:
	cSoundFFTCapture();
	~cSoundFFTCapture();
	void			Destroy();
	virtual void	CaptureSoundStartCallBack();
	virtual void	CaptureSoundNewDataCallBack(ALCint e_iSamplesIn,char*e_pData);
	virtual void	CaptureSoundEndCallBack();
	void			Update(float e_fElpaseTime);
	void			Render();
	virtual void	GoToTime(float e_fElpaseTime){}
	void			SetFFTSampleScale(float e_fScale);
	//use this one for cSoundCapture(buffersize)
	static int		GetOpanalCaptureBufferSize(int e_iFPS,int e_iFrequence);
};


//double GoertzelFilter(float* samples, double freq, int start, int end,int SIGNAL_SAMPLE_RATE)
//{
//	double sPrev = 0.0;
//	double sPrev2 = 0.0;
//	int i;
//	double normalizedfreq = freq / SIGNAL_SAMPLE_RATE;
//	double coeff = 2 * cos(2 * D3DX_PI * normalizedfreq);
//	for (i = start; i < end; i++)
//	{
//		double s = samples[i] + coeff * sPrev - sPrev2;
//		sPrev2 = sPrev;
//		sPrev = s;
//	}
//	double power = sPrev2 * sPrev2 + sPrev * sPrev - coeff * sPrev * sPrev2;
//	return power;
//}
//
//
//float goertzel_mag(int numSamples,int TARGET_FREQUENCY,int SAMPLING_RATE, float* data)
//{
//	int     k,i;
//	float   floatnumSamples;
//	float   omega,sine,cosine,coeff,q0,q1,q2,magnitude,real,imag;
//
//	float   scalingFactor = numSamples / 2.0;
//
//	floatnumSamples = (float) numSamples;
//	k = (int) (0.5 + ((floatnumSamples * TARGET_FREQUENCY) / SAMPLING_RATE));
//	omega = (2.0 * D3DX_PI * k) / floatnumSamples;
//	sine = sin(omega);
//	cosine = cos(omega);
//	coeff = 2.0 * cosine;
//	q0=0;
//	q1=0;
//	q2=0;
//
//	for(i=0; i<numSamples; i++)
//	{
//		q0 = coeff * q1 - q2 + data[i];
//		q2 = q1;
//		q1 = q0;
//	}
//
//	// calculate the real and imaginary results
//	// scaling appropriately
//	real = (q1 - q2 * cosine) / scalingFactor;
//	imag = (q2 * sine) / scalingFactor;
//
//	magnitude = sqrtf(real*real + imag*imag);
//	return magnitude;
//}
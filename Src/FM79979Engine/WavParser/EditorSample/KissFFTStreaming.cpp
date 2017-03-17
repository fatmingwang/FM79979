#include "stdafx.h"
#include "KissFFTStreaming.h"


void	KissFFTStreamingConvertThread(size_t _workParameter, size_t _pUri)
{
	cKissFFTStreamingConvert*l_pKissFFTStreamingConvert = (cKissFFTStreamingConvert*)_workParameter;
	l_pKissFFTStreamingConvert->m_bThreadAlreadyStop = false;
	while( !l_pKissFFTStreamingConvert->m_bThreadStop && l_pKissFFTStreamingConvert->m_pOpanalOgg )
	{
		//do fft
		if( l_pKissFFTStreamingConvert->m_SoundThreadTimeAndPCMDataVector.size() )
		{
		
		}
		l_pKissFFTStreamingConvert->m_pOpanalOgg->Update(cGameApp::m_sTimeAndFPS.fElpaseTime);
		if( l_pKissFFTStreamingConvert->m_bNewData )
		{
			//do FFT convert
			//
			l_pKissFFTStreamingConvert->ProcessFFTData();
			l_pKissFFTStreamingConvert->m_bNewData = false;
		}
	}
}

void	KissFFTStreamingConvertThreadDone(size_t _workParameter, size_t _pUri)
{
	cKissFFTStreamingConvert*l_pKissFFTStreamingConvert = (cKissFFTStreamingConvert*)_workParameter;
	l_pKissFFTStreamingConvert->m_bThreadAlreadyStop = true;
}

cKissFFTStreamingConvert::cKissFFTStreamingConvert()
{
	m_pFUThreadPool = nullptr;
	m_pOpanalOgg = nullptr;
	m_bThreadStop = false;
	m_bThreadAlreadyStop = true;
	m_bNewData = false;
}

cKissFFTStreamingConvert::~cKissFFTStreamingConvert()
{
	Destroy();
}

void	cKissFFTStreamingConvert::Destroy()
{
	SAFE_DELETE(m_pFUThreadPool);
	SAFE_RELEASE(m_pOpanalOgg,this);
	DELETE_VECTOR(m_TimeAndPCMDataVector,sTimeAndPCMData*);
	DELETE_VECTOR(m_SoundThreadTimeAndPCMDataVector,sTimeAndPCMData*);
}

void	cKissFFTStreamingConvert::ProcessFFTData()
{

}
//1.find proper data by time
//2.compare time and check performance
//3.check how many fft data need to do
//4.push data into m_SoundThreadTimeAndPCMDataVector
//5.calculate fft data in sound thread.
void	cKissFFTStreamingConvert::StreamingBuffer(int e_iCount,char*e_pData,size_t e_iCurrentPCMDataPosIndex)
{
	if(m_pOpanalOgg)
	{
		m_TimeAndPCMDataVector.clear();
		float	l_fTimeLength = m_pOpanalOgg->GetTimeLength();
		ALsizei	l_iPCMDataLength = m_pOpanalOgg->GetPCMDataSize();
		float	l_fCurrentTime = ((float)e_iCurrentPCMDataPosIndex/l_iPCMDataLength)*l_fTimeLength;
		float	l_fDuring = (float)e_iCount/l_iPCMDataLength*l_fTimeLength;
		if(this->m_fCurrentTime >= l_fCurrentTime+l_fDuring )
		{
			//machine is too slow...
			cGameApp::OutputDebugInfoString(L"this machine just too slow...");
			return;
		}
		memcpy(m_StreamingBufferData[0],e_pData,e_iCount);
		//m_StreamingBufferector.push_back(l_pData);
	}
	m_bNewData = true;
}


bool	cKissFFTStreamingConvert::FetchSoundDataStart(const char*e_strFileName)
{
	m_bThreadStop = true;
	while( !m_bThreadAlreadyStop ){
		//wait for thread end...
	}
	Destroy();
	m_pOpanalOgg = new cOpanalOgg(this,e_strFileName,true);
	m_pOpanalOgg->SetUpdateNewBufferCallbackFunction(std::bind(&cKissFFTStreamingConvert::StreamingBuffer,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
	this->m_pFUThreadPool = new cFUThreadPool();
	this->m_pFUThreadPool->Spawn(1);
	FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&KissFFTStreamingConvertThread);
	FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&KissFFTStreamingConvertThreadDone);
	this->m_pFUThreadPool->ExecuteWork(workFunctor,doneFunctor,(size_t)this,0);
	return true;
}

void	cKissFFTStreamingConvert::Update(float e_fElpaseTime)
{
	if( this->m_pOpanalOgg )
	{
		this->m_fCurrentTime += e_fElpaseTime;
		if( m_TimeAndPCMDataVector.size() )
		{//fo fft to line.
		
		}
	}
}

void	cKissFFTStreamingConvert::Render()
{

}
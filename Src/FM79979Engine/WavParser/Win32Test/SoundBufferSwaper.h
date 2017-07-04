#pragma once

template <class T>class sDataWithLimitCount:public NamedTypedObject
{
	T**			m_ppData;
	T*			m_pCurrentData;
	int			m_iEachBufferSize;
	int			m_iBufferDataCount;
	int			m_iCurrentDataIndex;
	//
	void		Destroy()
	{
		SAFE_DELETE_ARRAY(m_ppData);
		m_pCurrentData = nullptr;
		m_iBufferDataCount = 0;
		m_iCurrentDataIndex = 0;
	}
public:
	sDataWithLimitCount()
	{
		m_ppData = nullptr;
		m_pCurrentData = nullptr;
		m_iBufferDataCount = 0;
		m_iCurrentDataIndex = 0;
	}
	~sDataWithLimitCount()
	{
		SAFE_DELETE_ARRAY(m_ppData);
		m_pCurrentData = nullptr;
	}
	void	SetupBufferData(int e_iBufferDataSize,int e_iEachBufferSize)
	{
		Destroy();
		m_iEachBufferSize = e_iEachBufferSize;
		m_ppData = new T*[e_iBufferDataSize];
		for( int i=0;i<e_iBufferDataSize;++i )
		{
			m_ppData = new T*[e_iEachBufferSize];
		}
		m_pCurrentData = m_ppData[0];
	}
	char*	GoOneStep()
	{
		++m_iCurrentDataIndex;
		if( m_iCurrentDataIndex >= m_iBufferDataCount )
			m_iCurrentDataIndex = 0;
		m_pCurrentData = m_ppData[m_iCurrentDataIndex];
		return m_pCurrentData;
	}
	T*	GetCurrentData(){ return m_pCurrentData; }

	int			GetEachBufferSize(){return m_iEachBufferSize;}
	int			GetBufferDataCount(){return m_iBufferDataCount;}
	int			GetCurrentDataIndex(){return m_iCurrentDataIndex;}
};

class cSoundBufferData:public sDataWithLimitCount<char>
{
public:
	float m_fStartTime;
	float m_fEndTime;
public:
	cSoundBufferData()
	{
		m_fStartTime = 0.f;
		m_fEndTime = 0.f;
	}
	~cSoundBufferData(){}
};

//the sould data need to be overlap,so we can get more accurate FFT data
//
//now I am planning 30 fps for FFT data but merge to 10 FPS,this mean 
//so the parameter is one step forward 3 but with old data 2,depend on performance
//
//one second 30 fps retargeting to 10 fps.
//step = 30/10 = 3;
//old data capacity 2,
//so 1 data has 5 sound data,2 old 3 new.
//see below
//1  2  3  4  5,  4  5  6  7  8, 7  8  9 10 11,
//11 12 13 14 15,14 15 16 17 18,17 18 19 20 21,
//20 21 22 23 24,23 24 25 26 27,26 27 28 29 30
//if the peformance is not good enough,reduce old data count.

//fps 30
//44100/2/30 = 735
//44100/735 = 60
//each fft bin is 60 hz

//now fps as same as 30 but with old data
//parameter 3 new old 2 ,
//FFTcount count is=> 44100/2/30*(50/30)=1225,
//44100/1225 = 36
//each fft bin is 36 hz


class cSoundFFTDataCalculator
{
	int m_iNewSoundDataSize;
	int m_iOldSoundDataCountSize;
	int m_iNewDataCount;
	int m_iOldDataCount;
	int m_iFrequency;
	int m_iFPS;
	int m_iOneUpdateNeedDataSize;
public:
	cSoundFFTDataCalculator();
	~cSoundFFTDataCalculator();
	//OldDataCapcility higher get more accurate,e_iDoFFTFPS suggest 10
	void	SetupParameter(int e_iDoFFTFPS,int e_iFrequency,int e_iOldFFTCount);
	//with short return. 
	int		GetOneUpdateNeededFFTDataSize();
	int		GetFFTBinCount();
	//int     GetNewDataCount(){ return m_iNewDataCount; }
	//int     GetOldDataCount(){ return m_iOldDataCount; }

	int     GetNewSoundDataSize();
	int     GetOldSoundDataSize();
};

class cSoundBufferSwaper
{
	cSoundFFTDataCalculator					m_SoundFFTDataCalculator;
	//sDataWithLimitCount<sDataAndValueKey>	m_WaitFetchData;
	cSoundBufferData						m_SoundBufferData;
	//
	int										m_iCurrentDataPos;
	int										m_iRestDataSize;
	int										m_iTargetDataSize;
	//
	void									StartNewData();
	void									BufferCopy(char*e_pData,int e_iCount);
public:
	cSoundBufferSwaper();
	~cSoundBufferSwaper();
	void			SetupBufferData(int e_iBufferCount,int e_iDoFFTFPS,int e_iFrequency,int e_iOldFFTCount);
	//0 still need data,1 data is filled,2 data is too many,need more buffer
	int				PushData(int e_iTargetDataSize,int e_iCurrentDataSize,char*e_pData,float e_fCurrentTime,float e_fEndTime);
	char*			GetSoundBufferByTime(float e_fTime);
};
#pragma once



class cStoreNoisePhase:public cSimplePhase
{
	uint64*				m_pi64FFTDataFroNoise;
	std::wstring		m_strMessage;
	struct sFFTBinData
	{
		int		iNumBin;
		int64*	pi64AccumulateFFTAmplitude;
		int		iAccmulationCount;
	};
public:
	cStoreNoisePhase();
	virtual ~cStoreNoisePhase();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	virtual	void	Destroy();
	virtual	void	DebugRender();
	virtual	void*	GetData();
	virtual	void	KeyUp(char e_cKey);
};
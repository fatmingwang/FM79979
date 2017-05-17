#pragma once

class cTimeLineRangeChart;
class cQuickFFTDataFrequencyFinder;
class cPerformMusicPhase:public cSimplePhase
{
	cTimeLineRangeChart*			m_pTimeLineRangeChart;
	void							FetchData(const wchar_t*e_strPhaseName,void*e_pData);
	std::string						m_strMusicFileName;
public:
	cPerformMusicPhase();
	virtual ~cPerformMusicPhase();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	virtual	void	Destroy();
	virtual	void	DebugRender();
	virtual	void*	GetData();
};
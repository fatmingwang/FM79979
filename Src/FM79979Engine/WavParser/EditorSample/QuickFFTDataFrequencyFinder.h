#pragma once

//for quick find the ampliture in frequency

class cQuickFFTDataFrequencyFinder
{
public:
	cQuickFFTDataFrequencyFinder(int e_iFFTBins,int e_iMaxFrequency);
	~cQuickFFTDataFrequencyFinder();
	int						GetAmplitude(int e_iFrequency);
};
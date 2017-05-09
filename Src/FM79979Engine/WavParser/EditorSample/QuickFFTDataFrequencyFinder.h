#pragma once

//for quick find the ampliture in frequency

class cQuickFFTDataFrequencyFinder
{
	//its a reference.
	GET_SET_DEC(int*,m_piFFTData,GetFFTData,SetFFTData);
	float					m_fFrequencyGap;
	int						m_iFFTBinCount;
public:
	cQuickFFTDataFrequencyFinder(int e_iFFTBins,int e_iMaxFrequency);
	~cQuickFFTDataFrequencyFinder();
	//get left center and right data
	//here is possible have some precious problem,so I give close result
	std::vector<int>						GetAmplitude(int e_iFrequency);
};
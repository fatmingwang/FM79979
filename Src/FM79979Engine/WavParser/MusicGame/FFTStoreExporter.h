#pragma once

#include "KissFFTConvert.h"

class cFFTStoreExporter:public cNamedTypedObjectVector<cKissFFTConvert>
{
	int		m_iSelectedIndex;
public:
	cFFTStoreExporter();
	~cFFTStoreExporter();
	//
	void	Update(float e_fElpaseTime);
	void	Render();
	//
	bool	AddFile(const char*e_strFileName);
	int		Export(const char*e_strDirectory);

	void	SetSelectedIndex(int e_iIndex);
	void	SetHittedCountThresholdValue(int e_iValue);
};
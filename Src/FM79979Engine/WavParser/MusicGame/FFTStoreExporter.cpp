#include "stdafx.h"
#include "FFTStoreExporter.h"

cFFTStoreExporter::cFFTStoreExporter()
{
	m_iSelectedIndex = -1;	
}

cFFTStoreExporter::~cFFTStoreExporter()
{
	
}

//
void	cFFTStoreExporter::Update(float e_fElpaseTime)
{
	if( m_iSelectedIndex == -1 )
		return;
	cKissFFTConvert*l_pKissFFTConvert = this->GetObject(m_iSelectedIndex);
	if( l_pKissFFTConvert )
		l_pKissFFTConvert->Update(e_fElpaseTime);
}

void	cFFTStoreExporter::Render()
{
	if( m_iSelectedIndex == -1 )
		return;
	cKissFFTConvert*l_pKissFFTConvert = this->GetObject(m_iSelectedIndex);
	if( l_pKissFFTConvert )
		l_pKissFFTConvert->Render();
}
//
bool	cFFTStoreExporter::AddFile(const char*e_strFileName)
{
	if(this->GetObject(e_strFileName))
		return false;
	cKissFFTConvert*l_pKissFFTConvert = new cKissFFTConvert();
	l_pKissFFTConvert->FetchSoundDataStart(e_strFileName,false,true);
	std::string l_strFileName = ::GetFileNameWithoutFullPath(e_strFileName);
	l_pKissFFTConvert->SetName(l_strFileName.c_str());
	this->AddObject(l_pKissFFTConvert);
	return true;
}

int	cFFTStoreExporter::Export(const char*e_strDirectory,int e_iThresholdFrequency)
{
	int l_iNumFailed = 0;
	int l_icount = this->Count();
	for( int i=0;i<l_icount;++i )
	{
		cKissFFTConvert*l_pKissFFTConvert = this->GetObject(i);
		std::string l_strDirectory = e_strDirectory;
		l_strDirectory += "/";
		l_strDirectory += ValueToString(l_pKissFFTConvert->GetName());
		l_strDirectory += ".fftstore";
		if(!l_pKissFFTConvert->GetFFTDataStore()->Export(l_strDirectory.c_str(),l_pKissFFTConvert->GetSourceFileName().c_str(),l_pKissFFTConvert->GetFFTDataStore()->GetExportThresholdValue(),e_iThresholdFrequency))
			++l_iNumFailed;
	}
	return l_iNumFailed;
}

void	cFFTStoreExporter::SetSelectedIndex(int e_iIndex)
{
	this->m_iSelectedIndex = e_iIndex;
	if( m_iSelectedIndex == -1 )
		return;
	cKissFFTConvert*l_pKissFFTConvert = this->GetObject(m_iSelectedIndex);
	if( l_pKissFFTConvert )
	{
		l_pKissFFTConvert->SetCurrentTime(0.f);
		l_pKissFFTConvert->Play();
	}
}

void	cFFTStoreExporter::SetHittedCountThresholdValue(int e_iValue)
{
	cKissFFTConvert*l_pKissFFTConvert = this->GetObject(m_iSelectedIndex);
	if( l_pKissFFTConvert )
	{
		l_pKissFFTConvert->GetFFTDataStore()->SetExportThresholdValue(e_iValue);
	}
}
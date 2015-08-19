#include "StdAfx.h"
#include "FatmingUtilities.h"

namespace FMBookEditor
{
	void	cFatmingUtilitiesData::Refresh()
	{
		m_pMPDI->RefreshMPDIList();;
		m_pParticle->RefreshList();;
		m_pPI->RefreshPIList();;
		m_pSound->ReflashSoundList();;
		m_pXMLText->Refresh();
		m_pImage->RefreshImageList();
	}
}

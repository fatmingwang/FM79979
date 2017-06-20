#include "stdafx.h"
#include "PerformScoreCalculate.h"	
#include "SoundTimeLineData.h"
cPerformScoreCalculate::cPerformScoreCalculate(cSoundTimeLineDataCollection*e_pSoundTimeLineDataCollection)
{
	m_iCorrect = 0;
	m_iWrong = 0;
	m_iScore = 0;
	m_pSoundTimeLineDataCollection = e_pSoundTimeLineDataCollection;
}

cPerformScoreCalculate::~cPerformScoreCalculate()
{
	
}

bool		cPerformScoreCalculate::MyParse(TiXmlElement*e_pRoot)
{
	return false;
}

//void		cPerformScoreCalculate::SaveFile(const char*e_strFileName)
//{
//
//}


void	cPerformScoreCalculate::CalculateScore()
{
	m_TunesResultVector.clear();
	m_iCorrect = 0;
	m_iWrong = 0;
	m_iScore = 0;
	if( m_pSoundTimeLineDataCollection  )
	{
		int l_iCount = m_pSoundTimeLineDataCollection->Count();
		for( int i=0;i<l_iCount;++i )
		{
			auto l_pData = m_pSoundTimeLineDataCollection->GetObject(i);
			if(l_pData->IsTuneMatched())
			{
				m_TunesResultVector.push_back(true);
				++m_iCorrect;
			}
			else
			{
				m_TunesResultVector.push_back(false);
				++m_iWrong;
			}
		}
		if( l_iCount && m_iCorrect )
			m_iScore = (int)((float)m_iCorrect/l_iCount*100);
	}
}
#include "stdafx.h"
#include "PerformScoreCalculate.h"	

cPerformScoreCalculate::cPerformScoreCalculate(cSoundTimeLineDataCollection*e_pSoundTimeLineDataCollection)
{
	m_iCorrect = 0;
	m_iWrong = 0;
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
}
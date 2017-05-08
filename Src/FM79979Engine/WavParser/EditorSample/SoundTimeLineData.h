#pragma once

#include "TimeFrequencyAmplitudeValueCapture.h"


class cSoundTimeLineDataVisualizar
{

};

class cScoreCalculator
{
	int							m_iTotalScore;
	int							m_iCurrentScore;
	struct sTimeAndScore
	{
		float	fTime;
		int		iScore;
	};
	std::vector<sTimeAndScore>	m_PerformResult;
	//from file.
	//std::vector<sTimeAndScore>	m_PreParseResult;
public:
	cScoreCalculator(std::vector<sFrequenceAndAmplitudeAndTime>*e_pData);
	~cScoreCalculator();
};
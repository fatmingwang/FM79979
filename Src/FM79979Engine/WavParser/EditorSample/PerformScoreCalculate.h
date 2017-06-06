#pragma once


class cPerformScoreCalculate
{
	void		CalculateScore();
public:
	cPerformScoreCalculate(cSoundTimeLineDataCollection*e_pSoundTimeLineDataCollection);
	~cPerformScoreCalculate();
	void		SaveFile(const char*e_strFileName);
};
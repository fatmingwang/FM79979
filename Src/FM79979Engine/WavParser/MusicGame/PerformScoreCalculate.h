#pragma once

//<PerformScoreCalculate SongName="Test">
//	<PerformResult Result"0,1,0,1,0,1,0.....">
//</PerformScoreCalculate>
class cSoundTimeLineDataCollection;

class cPerformScoreCalculate:public cNodeISAX
{
	std::vector<bool>				m_TunesResultVector;
	cSoundTimeLineDataCollection*	m_pSoundTimeLineDataCollection;
	virtual	bool					MyParse(TiXmlElement*e_pRoot);
public:
	cPerformScoreCalculate(cSoundTimeLineDataCollection*e_pSoundTimeLineDataCollection);
	~cPerformScoreCalculate();
	void		ParseFile(const char*e_strFileName);
	void							CalculateScore();
	int								m_iCorrect;
	int								m_iWrong;
	int								m_iScore;
};
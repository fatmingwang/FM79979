#ifndef _FISH_SHOW_PROBABILITY_H_
#define _FISH_SHOW_PROBABILITY_H_

class	cFishBehaviorManager;
class	cFishShowProbability
{
	struct	sAreaData
	{
		Vector2		vFishAppearDistance;
		WCHAR		ShowAreaID;
		Vector2		Range;
		float		fTotalProbability;//for the fishes's show total probability in this area
	};
	struct	sFishAndProbability
	{
		std::wstring	strFileName;
		float			fProbability;
		WCHAR			ShowAreaID;
		sFishAndProbability(std::wstring e_strFileName,float e_fProbability,WCHAR e_ShowAreaID){ strFileName = e_strFileName; fProbability = e_fProbability; ShowAreaID = e_ShowAreaID; }
		~sFishAndProbability(){}
	};
	struct	sFishesShowProbability
	{
		Vector2								vFishShowRange;
		int									iAppearFishesCount;
		std::vector<sFishAndProbability>	FishAndProbabilityVector;
	};
	sFishesShowProbability	m_FishesShowProbability;
	cFishBehaviorManager*	m_pFishBehaviorManager;
	std::vector<sAreaData>	m_AreaDataVector;
public:
	cFishShowProbability(cFishBehaviorManager*e_pFishBehaviorManager);
	~cFishShowProbability();
	void	AssignFishShowData(Vector2 e_vSize);
	void	ProcessFishesShowProbabilityData(TiXmlElement*e_pTiXmlElement);
	void	ProcessAreaData(TiXmlElement*e_pTiXmlElement);
};

#endif
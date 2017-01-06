#pragma once
#include <string>
#include <map>
//explain
//there are 3 object and each object cost as below,
//gems 30,90,3					,if add one more 30
//total gems
//	30+90+3 = 123				,153
//1st get scroe
//	target 3
//		123/3=41 score				    153/3 = 51
//	target 30
//		123/30 = 5 score				153/30 = 6
//	target 90
//		123/90 = 2 score				153/90 = 2
//	total score
//		41+5+2 = 48						51+6+2+6 = 65//one more 30
//2nd calculate probability
//	target 3 score is 41
//		41/48 = 0.854					51/65 = 0.784
//	target 30 score is 5
//		5/48 = 0.104   					6/65 = 0.092,0.092
//	target 90 score is 2
//		2/48 = 0.041					2/65 = 0.030
//	total probability = 0.999			0.998



class cGemPriceToProbability
{
public:
	int			m_iTotalGems;
	int			m_iTotalScore;
	float		m_fTotalProbability;
	//
	//int	m_iCoinToGemsConvertValue;//some object maybe is not the gems,so we need to convert it to gems
	//int m_iConvertedGems;
	//
	//std::map<std::wstring,int>	m_NameAndCoinsValue;
	std::map<std::wstring,int>		m_NameAndGemsValue;
	//
	std::map<std::wstring,int>		m_NameAndScore;
	std::map<std::wstring,float>	m_NameAndProbability;
public:
	cGemPriceToProbability();
	~cGemPriceToProbability();
private:
	std::vector<std::wstring>	m_GemsDataNameVector;
	void						CalculateScore();//step2
	void						CalculateGems();//step3
public:
	bool	AddGemsData(std::wstring e_strName,int e_iValue);
	int		GetScoreByAddGemsDataOrder(int e_iIndex);
	bool	SetScoreByAddGemsDataOrder(int e_iIndex,int e_iScore);
	std::wstring	GetScoreInfoByAddGemsDataOrder(int e_iIndex);
	//bool	AddCoinsData(std::wstring e_strName,int e_iValue);
	//just add object and score
	//bool	AddScoreData(std::wstring e_strName,int e_iValue);
	void						CalculateTotalScore();
	void						ClearData();
	void						CalculateProbability();
	std::vector<std::wstring>	DumpInfo(bool e_bDumpExtraInfo = false);
	std::vector<std::wstring> DumpProbabilityResult(bool e_bDumpExtraInfo = false);
};
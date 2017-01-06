#include "stdafx.h"
#include "GemPriceToProbability.h"

cGemPriceToProbability::cGemPriceToProbability()
{
	m_iTotalGems = 0;
	m_iTotalScore = 0;
	m_fTotalProbability = 0.f;
	//m_iCoinToGemsConvertValue = 0;
	//m_iConvertedGems = 0;
}

cGemPriceToProbability::~cGemPriceToProbability()
{
	
}

bool	cGemPriceToProbability::AddGemsData(std::wstring e_strName,int e_iValue)
{
	if(this->m_NameAndGemsValue.find(e_strName) == this->m_NameAndGemsValue.end())
	{
		m_NameAndGemsValue.insert(std::make_pair(e_strName,e_iValue));
		return true;
	}
	return false;
}

int		cGemPriceToProbability::GetScoreByAddGemsDataOrder(int e_iIndex)
{
	if( (int)m_GemsDataNameVector.size() > e_iIndex )
	{
		auto l_Iterator = m_NameAndScore.find(m_GemsDataNameVector[e_iIndex]);
		if( l_Iterator != m_NameAndScore.end() )
		{
			std::wstring l_strName = l_Iterator->first;
			return l_Iterator->second;
		}
	}
	return 0;
}
bool	cGemPriceToProbability::SetScoreByAddGemsDataOrder(int e_iIndex,int e_iScore)
{
	if( (int)m_GemsDataNameVector.size() > e_iIndex )
	{
		auto l_Iterator = m_NameAndScore.find(m_GemsDataNameVector[e_iIndex]);
		if( l_Iterator != m_NameAndScore.end() )
		{
			l_Iterator->second = e_iScore;
			return true;
		}
	}
	return false;
}

std::wstring	cGemPriceToProbability::GetScoreInfoByAddGemsDataOrder(int e_iIndex)
{
	if( (int)m_GemsDataNameVector.size() > e_iIndex )
	{
		auto l_Iterator = m_NameAndScore.find(m_GemsDataNameVector[e_iIndex]);
		if( l_Iterator != m_NameAndScore.end() )
		{
			std::wstring l_strInfo = l_Iterator->first;
			l_strInfo += L"\tScore:";
			l_strInfo += ValueToStringW(l_Iterator->second);
			return l_strInfo;
		}
	}
	return L"error";
}


//bool	cGemPriceToProbability::AddCoinsData(std::wstring e_strName,int e_iValue)
//{
//	if(this->m_NameAndCoinsValue.find(e_strName) == this->m_NameAndCoinsValue.end())
//	{
//		m_NameAndCoinsValue.insert(std::make_pair(e_strName,e_iValue));
//		return true;
//	}
//	return false;
//}

//bool	cGemPriceToProbability::AddScoreData(std::wstring e_strName,int e_iValue)
//{
//	if(this->m_NameAndScore.find(e_strName) == this->m_NameAndScore.end())
//	{
//		m_NameAndScore.insert(std::make_pair(e_strName,e_iValue));
//		return true;
//	}
//	return false;
//}

void	cGemPriceToProbability::ClearData()
{
	//m_NameAndCoinsValue.clear();
	m_NameAndGemsValue.clear();
	m_NameAndScore.clear();
	m_NameAndProbability.clear();
	m_GemsDataNameVector.clear();
	m_iTotalGems = 0;
	m_iTotalScore = 0;
	//m_iCoinToGemsConvertValue = 0;
	//m_iConvertedGems = 0;
}

void	cGemPriceToProbability::CalculateGems()
{
	m_iTotalGems = 0;
	for(auto l_Iterator : m_NameAndGemsValue)
	{
		m_iTotalGems += l_Iterator.second;
	}
}

void	cGemPriceToProbability::CalculateTotalScore()
{
	m_iTotalScore = 0;
	for(auto l_Iterator : m_NameAndScore)
	{
		m_iTotalScore += l_Iterator.second;
	}
}

void	cGemPriceToProbability::CalculateScore()
{
	m_iTotalScore = 0;
	m_NameAndScore.clear();
	for(auto l_Iterator : m_NameAndGemsValue)
	{
		int l_iScore = ValueDivideAddOneIfModulusIsNotZero(m_iTotalGems,l_Iterator.second);
		m_iTotalScore += l_iScore;
		m_NameAndScore.insert(std::make_pair(l_Iterator.first,l_iScore));
	}
}

void	cGemPriceToProbability::CalculateProbability()
{
	m_fTotalProbability = 0.f;
	m_NameAndProbability.clear();
	for(auto l_Iterator : m_NameAndScore)
	{
		float l_fProbability = (float)l_Iterator.second/(float)m_iTotalScore;
		m_fTotalProbability += l_fProbability;
		m_NameAndProbability.insert(std::make_pair(l_Iterator.first,l_fProbability));
	}
}

std::vector<std::wstring>	cGemPriceToProbability::DumpInfo(bool e_bDumpExtraInfo)
{
	m_GemsDataNameVector.clear();
	std::vector<std::wstring>	l_strOutputStringVector;
	if( e_bDumpExtraInfo )
	{
		std::wstring	l_strOutputString = L"TotalGems:";
		l_strOutputString += ValueToStringW(m_iTotalGems);
		l_strOutputString += L"\tTotalScore:";
		l_strOutputString += ValueToStringW(m_iTotalScore);
		l_strOutputString += L"\tProbability:";
		l_strOutputString += ValueToStringW(m_fTotalProbability);
		l_strOutputString += L"\tItemCount";
		l_strOutputString += ValueToStringW(m_NameAndScore.size());
		l_strOutputString += L"\n";
		l_strOutputStringVector.push_back(l_strOutputString);
	}
	for(auto l_Iterator = m_NameAndScore.begin();
		l_Iterator != m_NameAndScore.end();
		++l_Iterator)
	{
		std::wstring	l_strOutputString;					
		l_strOutputString += l_Iterator->first;
		l_strOutputString += L"\tScore:";
		l_strOutputString += ValueToStringW(l_Iterator->second);
		m_GemsDataNameVector.push_back(l_Iterator->first);
		if( e_bDumpExtraInfo )
		{
			auto l_GemsIterator = m_NameAndGemsValue.find(l_Iterator->first);
			if( l_GemsIterator != m_NameAndGemsValue.end() )
			{
				l_strOutputString += L"\tConvertedGems:";
				l_strOutputString += ValueToStringW(l_GemsIterator->second);
			}
			auto l_ProbabilityIterator = m_NameAndProbability.find(l_Iterator->first);
			if( l_ProbabilityIterator != m_NameAndProbability.end() )
			{
				l_strOutputString += L"\tProbability:";
				l_strOutputString += ValueToStringW(l_ProbabilityIterator->second);
			}
		}
		l_strOutputString += L"\n";
		l_strOutputStringVector.push_back(l_strOutputString);
	}
	return l_strOutputStringVector;
}

std::vector<std::wstring> cGemPriceToProbability::DumpProbabilityResult(bool e_bDumpExtraInfo)
{
	CalculateGems();
	CalculateScore();
	CalculateProbability();
	return DumpInfo(e_bDumpExtraInfo);
}
#include "stdafx.h"
#include "BluffingGameProbability.h"

extern int	g_iSpecialDiceRulePoint;

std::vector<int>	GenerateDiceTable(int e_iNumDice,int e_iMaximumNumber,float e_fPercentForDice1)
{
	std::vector<int>l_Redsult = UT::GenerateOverlapableRandomTable(e_iNumDice,e_iMaximumNumber);
	if( e_fPercentForDice1 >0.f )
	{
		for( size_t i=0;i<l_Redsult.size();++i )
		{
			if( l_Redsult[i] != g_iSpecialDiceRulePoint )//check dice 1.
			{
				float	l_fValue = frand(0,1);
				if( l_fValue <= e_fPercentForDice1 )
				{
					l_Redsult[i] = g_iSpecialDiceRulePoint;
				}
			}
		}
	}
	return l_Redsult;
}

std::vector<int>	GenerateDiceTableAvoidNumber(int e_iNumDice,int e_iMaximumNumber,bool e_bAvoidSpecialPoint,int e_iAvoidPoint)
{
	std::vector<int>l_Redsult;
	while( l_Redsult.size() != e_iNumDice )
	{
		int	l_iValue = rand(0,e_iNumDice);
		if( (e_bAvoidSpecialPoint && l_iValue == g_iSpecialDiceRulePoint) || e_iAvoidPoint == l_iValue )
			continue;
		l_Redsult.push_back(l_iValue);
	}
	return l_Redsult;
}
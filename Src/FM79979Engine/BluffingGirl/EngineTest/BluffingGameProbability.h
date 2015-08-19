#ifndef _BLUFFING_DICE_PROBABILITY_H_
#define _BLUFFING_DICE_PROBABILITY_H_
//e_fPercentForDice for game diffcult.
std::vector<int>	GenerateDiceTable(int e_iNumDice,int e_iMaximumNumber,float e_fPercentForDice1);

std::vector<int>	GenerateDiceTableAvoidNumber(int e_iNumDice,int e_iMaximumNumber,bool e_bAvoidSpecialPoint,int e_iAvoidPoint);
//
//
//
//
#endif
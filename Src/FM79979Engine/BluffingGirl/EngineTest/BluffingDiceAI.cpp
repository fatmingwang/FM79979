#include "stdafx.h"
#include "BluffingDiceAI.h"
#include "DiceTable.h"
#include "BluffingGameProbability.h"
extern int	g_iSpecialDiceRulePoint;

cBluffingDiceAI::cBluffingDiceAI(TiXmlElement*e_pElement,sDiceRule*e_pDiceRule)
{
	m_iDiceCountCallResult = -1;
	m_iDicePointCallResult = -1;
	m_pPI = 0;
	m_SkipStep = Vector2(1,e_pDiceRule->iNumDice/2);
	m_ExtraVisibleDice = Vector2(1,e_pDiceRule->iNumDice);
	m_SafeToCall = Vector2(0.7f,1.f);
	m_CallByAmount = Vector2(0.5,0.7);
	m_CheatToWin.Min = 0;
	m_CheatToWin.Max = 0;
	m_fWinCallPercentage = 0.5f;
	m_pDiceRule = e_pDiceRule;
	m_bCallOrCatch = true;
	m_bEnableToCheat = true;
	m_pUseClairvoyantMPDI = 0;
	ProcessAIData(e_pElement);
}



cBluffingDiceAI::~cBluffingDiceAI()
{

}

void	cBluffingDiceAI::AssignTableData(std::vector<cDiceTable*>*e_DiceTableVector)
{
	m_DiceTableVector.clear();
	for( size_t i=0;i<e_DiceTableVector->size();++i )
		m_DiceTableVector.push_back((*e_DiceTableVector)[i]);
}
//<AIData WinCallPercentage="0.6" SkipStep="1,2" VisibleDice="5,6" />
void	cBluffingDiceAI::ProcessAIData(TiXmlElement*e_pElement)
{
	PARSE_ELEMENT_START(e_pElement)
		COMPARE_NAME("WinCallPercentage")
		{
			m_fWinCallPercentage = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("SkipStep")
		{
			m_SkipStep = VALUE_TO_VECTOR2;
		}
		else
		COMPARE_NAME("ExtraVisibleDice")
		{
			m_ExtraVisibleDice = VALUE_TO_VECTOR2;
		}
		else
		COMPARE_NAME("SafeToCall")
		{
			m_SafeToCall = VALUE_TO_VECTOR2;
		}
		else
		COMPARE_NAME("CallByAmount")
		{
			m_CallByAmount = VALUE_TO_VECTOR2;
		}
		else
		COMPARE_NAME("CheatToWin")
		{
			m_CheatToWin = VALUE_TO_VECTOR2;
		}
	PARSE_NAME_VALUE_END
}

int	cBluffingDiceAI::MatchDicePointCount(int e_iPlayerIndex,int e_iCurrentDicePoint,bool e_bSpecialOne,int e_iVisibleCount)
{
	if( e_iVisibleCount <1 )
		return 0;
	int	l_iNumDice = 0;
	if( e_iPlayerIndex != -1 && (int)m_DiceTableVector.size() > e_iPlayerIndex )
	{
		cDiceTable*l_pDiceTable = this->m_DiceTableVector[e_iPlayerIndex];
		for( size_t j=0;j<l_pDiceTable->m_MyDiceTable.size();++j )
		{
			int	l_iDicePoint = l_pDiceTable->m_MyDiceTable[j];
			if( e_iCurrentDicePoint == l_iDicePoint || ( e_bSpecialOne && l_iDicePoint == g_iSpecialDiceRulePoint ) )
			{
				if( e_iVisibleCount > (int)j )
					++l_iNumDice;
			}
		}	
	}
	return l_iNumDice;
}

int		cBluffingDiceAI::GetVisibleMatchCount(int e_iCurrentDicePoint,bool e_bSpecialOne,int e_iDiceAIIndex,int*e_piMyMatchCount,int*e_piVisibleCount)
{
	int	l_iMyMatch = MatchDicePointCount(e_iDiceAIIndex,e_iCurrentDicePoint,e_bSpecialOne,this->m_pDiceRule->iNumDice);
	if( e_piMyMatchCount )
		*e_piMyMatchCount = l_iMyMatch;
	int	l_iNumMatch = l_iMyMatch;
	int	l_iVisibleCount = m_ExtraVisibleDice.Rand();
	if( e_piVisibleCount )
		*e_piVisibleCount = l_iVisibleCount;
	//calculate my dice
	for( size_t i=0;i<m_DiceTableVector.size();++i )
	{
		if( i == e_iDiceAIIndex )
			continue;
		l_iNumMatch += MatchDicePointCount(i,e_iCurrentDicePoint,e_bSpecialOne,l_iVisibleCount);
		l_iVisibleCount -= m_pDiceRule->iNumDice;
	}
	return l_iNumMatch;
}

float	cBluffingDiceAI::CurrentCatchToWinPercentage(int e_iCurrentDiceCount,int e_iCurrentDicePoint,bool e_bSpecialOne,int e_iDiceAIIndex)
{
	int	l_iMyMatch = 0;
	int	l_iVisibleCount = 0;
	int	l_iNumMatch = GetVisibleMatchCount(e_iCurrentDicePoint,e_bSpecialOne,e_iDiceAIIndex,&l_iMyMatch,&l_iVisibleCount);
	if( l_iNumMatch < e_iCurrentDiceCount )
	{
		return 1.f;
	}
	//-1 fot myself
	int		l_iTotalOrtherDiceCount = m_pDiceRule->iNumDice*(m_DiceTableVector.size()-1);
	int		l_iUnknowDiceCount = l_iTotalOrtherDiceCount-l_iVisibleCount;
	//each dice point's probability is 1/6,but special one will minus 1
	int		l_iExpectDicePointCount = (int)(l_iUnknowDiceCount*(1.f/(e_bSpecialOne?m_pDiceRule->iNumDice-1:m_pDiceRule->iNumDice)))+l_iMyMatch;
	if( l_iTotalOrtherDiceCount % m_pDiceRule->iMax > 0 )
		l_iExpectDicePointCount += 1;

	float	l_fWinPercentage = (float)l_iNumMatch/l_iExpectDicePointCount;
	if( l_fWinPercentage >1.f )
		return 0.f;
	return 1-l_fWinPercentage;
}

void	cBluffingDiceAI::NewDiceData(int e_iMoveGap,int e_iCurrentDicePoint,int e_iCurrentDiceCount,int*e_piNewDiceCount,int*e_piNewDicePoint)
{
	if( e_iCurrentDiceCount < 1 )
		e_iCurrentDiceCount = 1;
	int	l_iNewPoint = e_iMoveGap+e_iCurrentDicePoint;
	int	l_iNewCount = l_iNewPoint/this->m_pDiceRule->iMax+e_iCurrentDiceCount;
	if( l_iNewCount != e_iCurrentDiceCount )
		l_iNewPoint %= m_pDiceRule->iMax;
	*e_piNewDiceCount = l_iNewCount;
	*e_piNewDicePoint = l_iNewPoint;
}

bool	cBluffingDiceAI::NewDiceDataByMostSameDices(bool e_bSpecialOneRule,int e_iDiceAIIndex,int e_iCurrentDicePoint,int e_iCurrentDiceCount,int*e_piNewDiceCount,int*e_piNewDicePoint)
{
	std::vector<int>	l_AllDiceCount;
	l_AllDiceCount.resize(this->m_pDiceRule->iMax);
	for( size_t i=0;i<l_AllDiceCount.size();++i )
		l_AllDiceCount[i] = 0;
	std::vector<int>l_MyDiceContent = m_DiceTableVector[e_iDiceAIIndex]->m_MyDiceTable;
	int	l_iMaxSameCountValue = -1;
	for( size_t i=0;i<l_MyDiceContent.size();++i )
	{
		int	l_iDicePoint = l_MyDiceContent[i];
		if( e_bSpecialOneRule && l_iDicePoint == g_iSpecialDiceRulePoint )
		{
			for(int ii=0;ii<this->m_pDiceRule->iMax;++ii)
				l_AllDiceCount[ii] += 1;
		}
		else
			l_AllDiceCount[l_iDicePoint] += 1;
	}
	for(int ii=0;ii<this->m_pDiceRule->iMax;++ii)
	{
		int l_iNumSame = l_AllDiceCount[ii];
		if( l_iNumSame > l_iMaxSameCountValue )
		{
			l_iMaxSameCountValue = l_iNumSame;
		}
	}
	if( l_iMaxSameCountValue < e_iCurrentDiceCount )
	{
		return false;
	}

	std::vector<int>	l_BluffingDiceVector;
	for( size_t i=0;i<l_AllDiceCount.size();++i )
	{
		if( l_AllDiceCount[i] == l_iMaxSameCountValue )
		{
			l_BluffingDiceVector.push_back(i);
		}
	}
	if( l_BluffingDiceVector.size() == 0 )
	{//impossible lah
		return false;
	}
	assert( e_iCurrentDiceCount > 0 );
	*e_piNewDiceCount = e_iCurrentDiceCount+1;
	*e_piNewDicePoint = l_BluffingDiceVector[rand()%l_BluffingDiceVector.size()];
	//now we have out blugging dices data.
	return true;
}

void	cBluffingDiceAI::Update(int e_iCurrentDiceCount,int e_iCurrentDicePoint,bool e_bSpecialOneRule,int e_iDiceAIIndex)
{
	int	l_iCheatcount = 3;
	if( m_DiceTableVector.size() > 2 )
		l_iCheatcount = 2*m_DiceTableVector.size();
	if( e_iCurrentDiceCount >= l_iCheatcount && m_bEnableToCheat )
	{
		int	l_iNumSpecialDice = MatchDicePointCount(e_iDiceAIIndex,g_iSpecialDiceRulePoint,e_bSpecialOneRule,this->m_pDiceRule->iNumDice);
		if( !e_bSpecialOneRule || l_iNumSpecialDice < 3 )
		{
			float	l_fCheatToWinRandValue = frand(0,1);
			float	l_fCheatToWinValue = m_CheatToWin.Rand();
			if( l_fCheatToWinRandValue < l_fCheatToWinValue )
			{//force to catch.
				this->m_bCallOrCatch = false;
				if(IsCatchWin(e_iCurrentDiceCount,e_iCurrentDicePoint,e_bSpecialOneRule))
				{
					return;
				}
				int	l_iNumPlayerHas = MatchDicePointCount(0,e_iCurrentDicePoint,e_bSpecialOneRule,this->m_pDiceRule->iNumDice);
				if( l_iNumPlayerHas < e_iCurrentDiceCount )
				{
					for( size_t i=1;i<this->m_DiceTableVector.size();++i )
					{//all AI reset data
						std::vector<int>l_Result = GenerateDiceTableAvoidNumber(this->m_pDiceRule->iNumDice,m_pDiceRule->iMax,e_bSpecialOneRule,e_iCurrentDicePoint);
						m_DiceTableVector[i]->SetDiceTable(l_Result);

					}
				}
			}
		}
	}
	float	l_fWinPercentage = this->CurrentCatchToWinPercentage(e_iCurrentDiceCount,e_iCurrentDicePoint,e_bSpecialOneRule,e_iDiceAIIndex);
	if( l_fWinPercentage >= this->m_fWinCallPercentage )
	{
		this->m_bCallOrCatch = false;
		return;
	}
	int	l_iMaximumValue = this->m_pDiceRule->iMax*this->m_DiceTableVector.size()*m_pDiceRule->iNumDice;
	//-1 for start is 1 not 0
	int	l_iCurrentLevelValue = (e_iCurrentDiceCount-1)*m_pDiceRule->iMax+e_iCurrentDicePoint;
	int	l_iRestGap = l_iMaximumValue-l_iCurrentLevelValue;
	if( l_iRestGap > 0 )
	{
		int	l_iSkipStep = m_SkipStep.Rand();
		if( l_iSkipStep > l_iRestGap )
			l_iSkipStep = l_iRestGap;
		int	l_iNewPoint = 0;
		int	l_iNewCount = 0;

		float	l_fCallByAmount = m_CallByAmount.Rand();
		float	l_fRandCallByAmountValue = frand(0,1.f);
		if( l_fRandCallByAmountValue <= l_fCallByAmount )
		{
			if(!NewDiceDataByMostSameDices(e_bSpecialOneRule,e_iDiceAIIndex,e_iCurrentDicePoint,e_iCurrentDiceCount,&l_iNewCount,&l_iNewPoint))
			{
				NewDiceData(l_iSkipStep,e_iCurrentDicePoint,e_iCurrentDiceCount,&l_iNewCount,&l_iNewPoint);
			}
		}
		else
		{
			NewDiceData(l_iSkipStep,e_iCurrentDicePoint,e_iCurrentDiceCount,&l_iNewCount,&l_iNewPoint);
		}

		float	l_fSafeToCall = this->m_SafeToCall.Rand();
		float	l_fRandValue = frand(0,1.f);
		if( l_fRandValue <= l_fSafeToCall )
		{
			int	l_iMyMatchCount = 0;
			int	l_iVisibleCount = 0;
			int	l_iCount = GetVisibleMatchCount(l_iNewPoint,e_bSpecialOneRule,e_iDiceAIIndex,&l_iMyMatchCount,&l_iVisibleCount);
			//at least move 1 step
			while( l_iNewCount > l_iCount && l_iSkipStep > 1 )
			{//too big lah go back,
				--l_iSkipStep;
				NewDiceData(l_iSkipStep,e_iCurrentDicePoint,e_iCurrentDiceCount,&l_iNewCount,&l_iNewPoint);
			}
		}
		//final it should't happen but it happen so move 1 step...fuck stupid but works.
		if( (l_iNewCount == e_iCurrentDiceCount && e_iCurrentDicePoint == l_iNewPoint)
			|| l_iNewCount < e_iCurrentDiceCount || ( l_iNewCount == e_iCurrentDiceCount && e_iCurrentDicePoint >= l_iNewPoint ) )
		{
			l_iNewCount += 1;
			if( l_iNewCount >= (int)this->m_DiceTableVector.size()*m_pDiceRule->iNumDice )
			{
				m_bCallOrCatch = false;
				return;
			}
		}
		this->m_iDiceCountCallResult = l_iNewCount;
		this->m_iDicePointCallResult = l_iNewPoint;
		m_bCallOrCatch = true;
	}
	else
	{//reach max so catch it...
		m_bCallOrCatch = false;
	}
}

bool	cBluffingDiceAI::IsCatchWin(int e_iCount,int e_iPoint,bool e_bSpecialDiceRule)
{
	//because start is 2
	e_iCount += 1;
	size_t l_iDiceTableSize = m_DiceTableVector.size();
	int	l_iNumPoint = 0;
	for( size_t i=0;i<l_iDiceTableSize;++i )
	{
		cDiceTable*l_pDiceTable = this->m_DiceTableVector[i];
		for(size_t j=0;j<l_pDiceTable->m_MyDiceTable.size();++j)
		{
			int	l_iPoint = l_pDiceTable->m_MyDiceTable[j];
			if( l_iPoint == g_iSpecialDiceRulePoint && e_bSpecialDiceRule )
			{//special rule
				++l_iNumPoint;
			}
			else
			if( l_pDiceTable->m_MyDiceTable[j] == e_iPoint )
				++l_iNumPoint;
		}
	}
	if( l_iNumPoint < e_iCount )
		return true;
	return false;
}

//<AIDiceTableResult PI="aa.pi" DiceIndexImagePos="0,0,0" DiceCountImagePos="0,0,0" DiceIndexImagePrefixName"" DiceCountPrefixName=""/>
cAIDiceTableCallResult::cAIDiceTableCallResult(cBluffingDiceAI*e_pBluffingDiceAI,TiXmlElement*e_pTiXmlElement,int e_iAIIndex)
{
	m_pWaitingAIRespond = 0;
	m_iAIIndex = e_iAIIndex;
	m_bWaitForAI = false;
	m_pMyDiceTable = 0;
	m_pBluffingDiceAI = e_pBluffingDiceAI;
	m_pPI = 0;
	m_pDicePointImage = 0;
	m_pDiceCountImage = 0;
	m_vDicePointImagePos = Vector3::Zero;
	m_vDiceCountImagePos = Vector3::Zero;
	m_strDicePointImagePrefixName = L"Dice";
	m_strDiceCountPrefixName = L"Num";
	while( e_pTiXmlElement )
	{
		const WCHAR*l_strValue = e_pTiXmlElement->Value();
		if( !wcscmp(l_strValue,L"AIDiceTableResult") )
		{
			PARSE_ELEMENT_START(e_pTiXmlElement)
				COMPARE_NAME("PI")
				{
					m_pPI = cGameApp::GetPuzzleImageByFileName(l_strValue);
				}
				else
				COMPARE_NAME("DiceIndexImagePos")
				{
					m_vDicePointImagePos = VALUE_TO_VECTOR3;
				}
				else
				COMPARE_NAME("DiceCountImagePos")
				{
					m_vDiceCountImagePos = VALUE_TO_VECTOR3;
				}
				else
				COMPARE_NAME("DicePointImagePrefixName")
				{
					m_strDicePointImagePrefixName = l_strValue;
				}
				else
				COMPARE_NAME("DiceCountPrefixName")
				{
					m_strDiceCountPrefixName = l_strValue;
				}
			PARSE_NAME_VALUE_END
		}
		else
		if( !wcscmp(l_strValue,L"DiceTable") )
		{
			m_pMyDiceTable = new cDiceTable(e_pTiXmlElement,e_pBluffingDiceAI->m_pDiceRule,eBT_AI);
		}
		else
		if(!wcscmp(l_strValue,L"AIThinkingMPDI"))
		{
			m_pWaitingAIRespond = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
			if( m_pWaitingAIRespond )
				m_pWaitingAIRespond->Init();
		}
		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	}
}

cAIDiceTableCallResult::~cAIDiceTableCallResult()
{
	SAFE_DELETE(m_pMyDiceTable);
}

void	cAIDiceTableCallResult::AIUpdate(int e_iCurrentDiceCount,int e_iCurrentDicePoint,bool e_bSpecialRule,float e_fElpaseTime)
{
	if( !m_pBluffingDiceAI )
		return;
	if( m_pWaitingAIRespond )
	{
		m_pWaitingAIRespond->Update(e_fElpaseTime);
		if(!m_pWaitingAIRespond->IsAnimationDone())
			return;
	}
	m_pBluffingDiceAI->Update(e_iCurrentDiceCount,e_iCurrentDicePoint,e_bSpecialRule,m_iAIIndex);
	m_pWaitingAIRespond->Init();
	if( m_pBluffingDiceAI->m_iDiceCountCallResult != -1&& m_pBluffingDiceAI->m_iDicePointCallResult !=-1 )
	{//call
		m_bWaitForAI = false;
		if( m_pPI )
		{
			m_iCallDicePointHistory.push_back(m_pBluffingDiceAI->m_iDicePointCallResult);
			m_iLastCallDiceCountHistory.push_back(m_pBluffingDiceAI->m_iDiceCountCallResult);
			std::wstring	l_strDicePointImageName = m_strDicePointImagePrefixName;			l_strDicePointImageName += ValueToStringW(m_pBluffingDiceAI->m_iDicePointCallResult+1);
			std::wstring	l_strDiceCountPrefixName = m_strDiceCountPrefixName;				l_strDiceCountPrefixName += ValueToStringW(m_pBluffingDiceAI->m_iDiceCountCallResult);
			m_pDicePointImage = m_pPI->GetObject(l_strDicePointImageName.c_str());
			m_pDiceCountImage = m_pPI->GetObject(l_strDiceCountPrefixName.c_str());
		}
	}
	else
	{//catch,play sound
		m_bWaitForAI = false;
	}
}

void	cAIDiceTableCallResult::Init()
{
	if( m_pMyDiceTable )
		this->m_pMyDiceTable->Init();
}

void	cAIDiceTableCallResult::Render()
{
	if( m_pDicePointImage )
	{
		Vector3	l_vPos = m_pDicePointImage->GetPos();
		m_pDicePointImage->SetPos(m_vDicePointImagePos);
		m_pDicePointImage->Render();
		m_pDicePointImage->SetPos(l_vPos);
	}

	if(m_pDiceCountImage)
	{
		Vector3	l_vPos = m_pDiceCountImage->GetPos();
		m_pDiceCountImage->SetPos(m_vDiceCountImagePos);
		m_pDiceCountImage->Render();
		m_pDiceCountImage->SetPos(l_vPos);
	}
	if( m_pMyDiceTable )
		this->m_pMyDiceTable->Render();
	if( m_pWaitingAIRespond && !m_pWaitingAIRespond->IsAnimationDone() )
		m_pWaitingAIRespond->Render();
}

bool	cAIDiceTableCallResult::IsCall()
{
	return this->m_pBluffingDiceAI->m_bCallOrCatch;
}

bool	cAIDiceTableCallResult::IsCatch()
{
	return !this->m_pBluffingDiceAI->m_bCallOrCatch;
}
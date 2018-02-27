#include "stdafx.h"
#include "FishShowPaobability.h"
#include "../ControlPanel/ControlSettingParameter.h"
#include "../Fish/FishBase.h"
//<FishBodyTypeAmount PlayerAmount="4,6,8,10" >
	//<Small MaxAmount="45,55,65,65" >
	//	<Fish ID="0"		ShowProbability="37" />
	//</Small>													 
	//<Medium MaxAmount = "6,8,12,14" >							 
	//	<Fish ID="1"		ShowProbability = "3 />
	//</Medium>													 
	//<Big MaxAmount = "2,2,2,2">								 
	//	<Fish ID="2"		ShowProbability = "3 />
	//</Big>
	//<Enormous>
	//	<Fish ID="3"		ShowProbability = "3 />
	//</Enormous>
//</FishBodyTypeAmount>
cFishShowProbability::cFishShowProbability(TiXmlElement*e_pTiXmlElement)
{
	m_GenerateFishTC.SetTargetTime(0.5f);
	m_GenerateFishTC.SetLoop(true);
	std::vector<int>	l_PlayerCountVector;
	COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pTiXmlElement, L"FishBodyTypeAmount")
	{
		FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pTiXmlElement)
			eFishBodyType l_FishBodyType = GetFishBodyType(e_pTiXmlElement->Value());
			assert(l_FishBodyType != eFBT_Total &&"Fish Body type Error!cFishShowProbability::cFishShowProbability");
			auto l_strMaxAmount = e_pTiXmlElement->Attribute(L"MaxAmount");
			if (l_strMaxAmount)
			{
				auto l_IntegerVector = GetIntegerListByCommaDivide(l_strMaxAmount,(int)l_PlayerCountVector.size());
				assert(l_IntegerVector.size() == l_PlayerCountVector.size()&&"player count is not match data count cFishShowProbability::cFishShowProbability");
				for (size_t i = 0; i < l_PlayerCountVector.size(); ++i)
				{
					m_FishBodyTypeAllowToShowCountByPlayerCount[i][l_FishBodyType] = l_IntegerVector[i];
				}
			}
			std::vector<int> l_iShowProbabilityVector;
			std::vector<int> l_iFishIDVector;
			TiXmlElement*l_pFirstChild = e_pTiXmlElement->FirstChildElement();
			FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(l_pFirstChild)
				auto l_strID = l_pFirstChild->Attribute(L"ID");
				auto l_strShowProbability = l_pFirstChild->Attribute(L"ShowProbability");
				l_iShowProbabilityVector.push_back(GetInt(l_strID));
				l_iFishIDVector.push_back(GetInt(l_strID));
			FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(l_pFirstChild)
			m_FishShowProbabilityAndID[l_FishBodyType].SetupData(l_iShowProbabilityVector, l_iFishIDVector);
		FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pTiXmlElement)
	}
}

cFishShowProbability::~cFishShowProbability()
{
}

void cFishShowProbability::Init()
{
	sCurrentFishBodyTypeFishCount	l_CurrentFishBodyTypeFishCount;
	m_CurrentFishBodyTypeFishCount = l_CurrentFishBodyTypeFishCount;
	for (int i = 0; i < eFBT_Total; ++i)
	{
		m_FishBodyTypeAllowToShowCountWithCurrentPlayerCountSetup[i] = m_FishBodyTypeAllowToShowCountByPlayerCount[sControlSettingParameter::m_siPlayerCountByPlayerLayeoutIndex][i];
	}
}

void	cFishShowProbability::Update(float e_fElpaseTime)
{
	m_GenerateFishTC.Update(e_fElpaseTime);
	if (m_GenerateFishTC.bTragetTimrReached)
	{
		m_CurrentFishBodyTypeFishCount.iGenerateFishIDVector.clear();
		for (int i = 0; i < eFBT_Total; ++i)
		{
			int l_iDiff = m_FishBodyTypeAllowToShowCountWithCurrentPlayerCountSetup[i] - m_CurrentFishBodyTypeFishCount.iCurrentFishBodyTypeFishCount[i];
			if(l_iDiff > 0 )
			{
				for (int j = 0; j < l_iDiff; ++j)
				{
					int l_iIndex = m_FishShowProbabilityAndID[i].GetIndexByProbability();
					assert(l_iIndex != -1 && "m_FishShowProbabilityAndID[i].GetIndexByProbability()");
					int l_iFishID = m_FishShowProbabilityAndID[i].ValueVector[l_iIndex];
					m_CurrentFishBodyTypeFishCount.iGenerateFishIDVector.push_back(l_iFishID);
				}
			}
		}
	}
}

void	cFishShowProbability::FishDired(eFishBodyType e_eFishBodyType)
{
	--m_CurrentFishBodyTypeFishCount.iCurrentFishBodyTypeFishCount[e_eFishBodyType];
}

cFishShowProbability::sCurrentFishBodyTypeFishCount*	cFishShowProbability::GetCurrentFishBodyTypeFishCount()
{
	return &m_CurrentFishBodyTypeFishCount;
}

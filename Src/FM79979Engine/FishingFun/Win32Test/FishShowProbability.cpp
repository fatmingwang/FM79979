#include "stdafx.h"
#include "FishShowProbability.h"
#include "FishBehavior.h"
#include "GameApp.h"
cFishShowProbability::cFishShowProbability(cFishBehaviorManager*e_pFishBehaviorManager)
{
	m_pFishBehaviorManager = e_pFishBehaviorManager;
}

cFishShowProbability::~cFishShowProbability()
{

}

void	cFishShowProbability::AssignFishShowData(Vector2 e_vSize)
{
	if( !m_pFishBehaviorManager )
		return;
	//1.find fishes by areaid
	//2.compare distance and area and show fishes by fish probability
	//
	m_pFishBehaviorManager->m_GeneratedFishes.Destroy();
	size_t	l_iSize = m_AreaDataVector.size();
	
	//if( l_iSize > 0 )
	//	assert(m_AreaDataVector[m_AreaDataVector.size()-1].Range.y >= e_vSize.x &&"area depth is ");
	std::map< WCHAR,std::vector<sFishAndProbability> >	l_MonsterShowData;
	for( size_t i=0;i<l_iSize;++i )
	{
		m_AreaDataVector[i].fTotalProbability = 0;
		WCHAR	l_ID = m_AreaDataVector[i].ShowAreaID;
		std::vector<sFishAndProbability>l_FishAndProbabilityVector;
		size_t	l_iFishesShowProbabilityCount = m_FishesShowProbability.FishAndProbabilityVector.size();
		for(size_t j=0;j<l_iFishesShowProbabilityCount;++j  )
		{
			if(m_FishesShowProbability.FishAndProbabilityVector[j].ShowAreaID == l_ID )
			{
				l_FishAndProbabilityVector.push_back(m_FishesShowProbability.FishAndProbabilityVector[j]);
				m_AreaDataVector[i].fTotalProbability += m_FishesShowProbability.FishAndProbabilityVector[j].fProbability;
			}
		}
		l_MonsterShowData[l_ID] = l_FishAndProbabilityVector;
	}
	int	l_iAreaTypeIndex = 0;
	float	l_fTotalDistance = e_vSize.y;
	float	l_fTopToDown = 0.f;
	cFishRodData*l_pFishRodData = cFishingFunApp::m_spFishRodAndFishThreadData->m_pCurrentSelectedFishRodData;
#ifdef DEBUG
	if( l_pFishRodData == 0 )
	{
		l_pFishRodData = cFishingFunApp::m_spFishRodAndFishThreadData->m_FishRodDataVector[0];
	}
#endif
	while( l_fTotalDistance > 0 )
	{
		UT::sMinMaxData<float>	l_Range = m_AreaDataVector[l_iAreaTypeIndex].vFishAppearDistance;
		float	l_fDis = l_Range.Rand();
		l_fTopToDown += l_fDis;
		l_fTotalDistance -= l_fDis;
		while( l_fTopToDown >= m_AreaDataVector[l_iAreaTypeIndex].Range.y )
		{
			++l_iAreaTypeIndex;
			if( l_iAreaTypeIndex >= (int)m_AreaDataVector.size() )
			{
				//UT::ErrorMsg(L"there is no any fish in this area",L"area depth data error!?");
				return;
			}
		}
		if( l_fTotalDistance > 0.f )
		{
			float	l_fValue = frand(0,m_AreaDataVector[l_iAreaTypeIndex].fTotalProbability);
			float	l_fCurrentProbabilityValue = 0.f;
			std::vector<sFishAndProbability> l_ThisAreaFishBehaviorVector = l_MonsterShowData[m_AreaDataVector[l_iAreaTypeIndex].ShowAreaID];
			int	l_iThisAreaFishesSize = (int)l_ThisAreaFishBehaviorVector.size();
			int	l_iToShowFishTypeIndex = -1;
			for( int i=0;i<l_iThisAreaFishesSize;++i )
			{
				l_fCurrentProbabilityValue += l_ThisAreaFishBehaviorVector[i].fProbability;
				if( l_fCurrentProbabilityValue > l_fValue )
				{
					const WCHAR*l_strMonsterName = l_ThisAreaFishBehaviorVector[i].strFileName.c_str();

					cFishBehavior*l_pFishBehaviorTarget = m_pFishBehaviorManager->GetObject(l_strMonsterName);
					cFishBehavior*l_pAddObject = 0;
					if( !l_pFishBehaviorTarget )
					{
						cNamedTypedObjectVector<cColladaParser>*l_pColladaParserVector = (cNamedTypedObjectVector<cColladaParser>*)cFishingFunApp::m_spColladaParserVector;
						l_pFishBehaviorTarget = new cFishBehavior(UT::WcharToChar(l_strMonsterName).c_str(),l_pColladaParserVector,cFishingFunApp::m_sp2DImageCollisionDataVector);
						m_pFishBehaviorManager->AddObjectNeglectExist(l_pFishBehaviorTarget);
						float	l_fLeaveProbability = l_pFishRodData->GetLeaveFishProbability(l_pFishBehaviorTarget->GetName());
						l_pFishBehaviorTarget->SetLeaveProbability(l_fLeaveProbability);
						l_strMonsterName = l_pFishBehaviorTarget->GetName();
					}
					l_pAddObject = dynamic_cast<cFishBehavior*>(l_pFishBehaviorTarget->Clone());
					//l_pAddObject->SetName(ValueToStringW(l_fTopToDown).c_str());
					l_pAddObject->SetName(l_strMonsterName);
					l_pAddObject->Init();
					l_pAddObject->GenerateHorizontalPath(e_vSize.x,l_fTopToDown);
					//cGameApp::OutputDebugInfoString(ValueToStringW(l_fTopToDown));
					//cGameApp::OutputDebugInfoString(L"\n");
					m_pFishBehaviorManager->m_GeneratedFishes.AddObjectNeglectExist(l_pAddObject);
					break;
				}
			}
		}
	}
}

void	cFishShowProbability::ProcessFishesShowProbabilityData(TiXmlElement*e_pTiXmlElement)
{
	m_FishesShowProbability.FishAndProbabilityVector.clear();
	m_FishesShowProbability.iAppearFishesCount = GetInt(e_pTiXmlElement->Attribute(L"AppearFishTypeCount"));
	m_FishesShowProbability.vFishShowRange = GetVector2(e_pTiXmlElement->Attribute(L"FishShowRange"));
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while( e_pTiXmlElement )
	{
		std::wstring	l_strFileName = e_pTiXmlElement->Attribute(L"FileName");
		float	l_fProbability = GetFloat(e_pTiXmlElement->Attribute(L"Probability"));
		WCHAR	l_wcShowAreaID = e_pTiXmlElement->Attribute(L"AreaID")[0];
		sFishAndProbability	l_sFishAndProbability(l_strFileName,l_fProbability,l_wcShowAreaID);
		m_FishesShowProbability.FishAndProbabilityVector.push_back(l_sFishAndProbability);
		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	}
}

void	cFishShowProbability::ProcessAreaData(TiXmlElement*e_pTiXmlElement)
{
	m_AreaDataVector.clear();
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while( e_pTiXmlElement )
	{
		sAreaData	l_sAreaData;
		l_sAreaData.ShowAreaID = e_pTiXmlElement->Attribute(L"AreaID")[0];
		l_sAreaData.Range = GetVector2(e_pTiXmlElement->Attribute(L"Range"));
		l_sAreaData.vFishAppearDistance = GetVector2(e_pTiXmlElement->Attribute(L"FishAppearDistance"));
		if( m_AreaDataVector.size() > 0 )
		{
			if( m_AreaDataVector[m_AreaDataVector.size()-1].Range.y >= l_sAreaData.Range.x )
			{
				UT::ErrorMsg(L"fish show range data is error",L"ProcessAreaData(TiXmlElement*e_pTiXmlElement)");
			}
		}
		m_AreaDataVector.push_back(l_sAreaData);
		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	}
}
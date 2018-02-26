#include "stdafx.h"
#include "FixedPathFish.h"

//TYPDE_DEFINE_MARCO(cFixedPathFish);
//
//cFixedPathFish::cFixedPathFish()
//{
//	m_pCurrentPath = nullptr;
//}
//cFixedPathFish::~cFixedPathFish()
//{
//
//}
//cFixedPathFish::cFixedPathFish(cFixedPathFish*e_pMonster)
//{
//	m_pCurrentPath = nullptr;
//	m_CurveWithTimeVector.CloneFromList(&e_pMonster->m_CurveWithTimeVector);
//}
//
//cFixedPathFish*cFixedPathFish::GetMe(TiXmlElement*e_pElement)
//{
//	FISH_LAZY_GET_ME_START(e_pElement, cFixedPathFish,
//		COMPARE_TARGET_ELEMENT_VALUE(e_pElement, "Path")
//		{
//			l_pObject->ProcessPathFile(e_pElement);
//		}
//	);
//}
//
////<Path MPDIListFileName="" MPDIPathList="Path1,Path2,Path3"/>
//void	cFixedPathFish::ProcessPathFile(TiXmlElement*e_pTiXmlElement)
//{
//	auto l_strMPDIListFileName = e_pTiXmlElement->Attribute(L"MPDIListFileName");
//	auto l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strMPDIListFileName);
//	if (l_pMPDIList)
//	{
//		auto l_strMPDIPathList = e_pTiXmlElement->Attribute(L"MPDIPathList");
//		std::vector<std::wstring>l_strPathVector = GetWStringListByCommaDivide(l_strMPDIPathList);
//		for (auto l_strPathName : l_strPathVector)
//		{
//			auto l_MPDI = l_pMPDIList->GetObject(l_strPathName);
//			if (l_MPDI->Count() >= 1)
//			{
//				cCurveWithTime*l_pCurveWithTime = new cCurveWithTime((*l_MPDI)[0]);
//				l_pCurveWithTime->SetCalAngle(true);
//				m_CurveWithTimeVector.AddObjectNeglectExist(l_pCurveWithTime);
//			}
//			else
//			{
//				UT::ErrorMsg(l_strPathName,L"cFixedPathFish::ProcessPathFile(TiXmlElement*e_pTiXmlElement)");
//			}
//		}
//	}
//}
//
//void	cFixedPathFish::InternalInit()
//{
//	int l_iCount = m_CurveWithTimeVector.Count();
//	int l_iIndex = rand(0, l_iCount)-1;
//	if (l_iIndex == -1)
//		return;
//	m_pCurrentPath = m_CurveWithTimeVector[l_iIndex];
//	InternalUpdate(0.016f);
//}
//
//void	cFixedPathFish::InternalUpdate(float e_fElpaseTime)
//{
//	if (m_pCurrentPath)
//	{
//		m_pCurrentPath->Update(e_fElpaseTime);
//		Vector3 l_vPos = m_pCurrentPath->GetPos();
//		float l_fAngle = m_pCurrentPath->GetAngle();
//		SetTransform(l_vPos, l_fAngle,Vector3(0,0,0));
//	}
//}
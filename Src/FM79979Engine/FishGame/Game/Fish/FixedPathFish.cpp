#include "stdafx.h"
#include "FixedPathFish.h"
TYPDE_DEFINE_MARCO(cFixedFishMonster);
cFixedFishMonster::cFixedFishMonster()
{
	m_pCurrentPath = nullptr;
}
cFixedFishMonster::~cFixedFishMonster()
{

}
cFixedFishMonster::cFixedFishMonster(cFixedFishMonster*e_pMonster)
{
	m_pCurrentPath = nullptr;
	m_CurveWithTimeVector.CloneFromList(&e_pMonster->m_CurveWithTimeVector);
}

cFixedFishMonster*GetMe(TiXmlElement*e_pTiXmlElement)
{
	return nullptr;
}
//<Path MPDIListFileName="" MPDIPathList="Path1,Path2,Path3"/>
void	cFixedFishMonster::ProcessPathFile(TiXmlElement*e_pTiXmlElement)
{
	auto l_strMPDIListFileName = e_pTiXmlElement->Attribute(L"MPDIListFileName");
	auto l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strMPDIListFileName);
	if (l_pMPDIList)
	{
		auto l_strMPDIPathList = e_pTiXmlElement->Attribute(L"MPDIPathList");
		std::vector<std::wstring>l_strPathVector = GetWStringListByCommaDivide(l_strMPDIPathList);
		for (auto l_strPathName : l_strPathVector)
		{
			auto l_MPDI = l_pMPDIList->GetObject(l_strPathName);
			if (l_MPDI->Count() >= 1)
			{
				cCurveWithTime*l_pCurveWithTime = new cCurveWithTime((*l_MPDI)[0]);
				m_CurveWithTimeVector.AddObjectNeglectExist(l_pCurveWithTime);
			}
			else
			{
				UT::ErrorMsg(l_strPathName,L"cFixedFishMonster::ProcessPathFile(TiXmlElement*e_pTiXmlElement)");
			}
		}
	}
}

void	cFixedFishMonster::SetTransform()
{
	SetTransformCollision(this->GetWorldTransform(),-1);
}

void	cFixedFishMonster::InternalInit()
{
	int l_iCount = m_CurveWithTimeVector.Count()-1;
	int l_iIndex = rand(0, l_iCount);
	m_pCurrentPath = m_CurveWithTimeVector[l_iIndex];
	if (m_pCurrentPath)
		m_pCurrentPath->Init();
	SetTransform();
}

void	cFixedFishMonster::InternalUpdate(float e_fElpaseTime)
{
	if (m_pCurrentPath)
		m_pCurrentPath->Update(e_fElpaseTime);
}


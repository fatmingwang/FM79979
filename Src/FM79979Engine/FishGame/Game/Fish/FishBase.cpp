#include "stdafx.h"
#include "FishBase.h"
#include "FishLockPoints.h"
sMinMaxData<float>	cFishBase::m_StruggleTime = Vector2(1,3);


eFishBodyType	GetFishBodyType(const wchar_t *e_str)
{
	if (wcscmp(e_str, L"Small") == 0 || wcscmp(e_str, L"small") == 0)
		return eFBT_Small;
	if (wcscmp(e_str, L"Medium") == 0 || wcscmp(e_str, L"medium") == 0)
		return eFBT_Medium;
	if (wcscmp(e_str, L"Big") == 0 || wcscmp(e_str, L"big") == 0)
		return eFBT_Big;
	if (wcscmp(e_str, L"Enormous") == 0 || wcscmp(e_str, L"enormous") == 0)
		return eFBT_Enormous;
	return eFBT_Total;
}

cFishBase::cFishBase()
{
	m_pCollisionData = nullptr;
	m_eFishBodyType = eFBT_Total;
	for (int i = 0; i < eFS_MAX; ++i)
		m_pMonsterAnimation[i] = nullptr;;
	m_iID = -1;
	m_fScale = 1.f;
	m_fRadius = -1.f;
	m_eFishStatus = eFS_NONE;
}

cFishBase::cFishBase(cFishBase*e_pFishBase)
{
	m_eFishBodyType = eFBT_Total;
	m_pCollisionData = new c2DImageCollisionData(e_pFishBase->m_pCollisionData);
	for (int i = 0; i < eFS_MAX; ++i)
	{
		if(e_pFishBase->m_pMonsterAnimation[i])
			m_pMonsterAnimation[i] = new cMPDI(e_pFishBase->m_pMonsterAnimation[i]);
		else
			m_pMonsterAnimation[i] = nullptr;
	}
	m_iID = e_pFishBase->m_iID;
	m_fScale = e_pFishBase->m_fScale;
	m_fRadius = e_pFishBase->m_fRadius;
	m_eFishStatus = eFS_NONE;
}

cFishBase::~cFishBase()
{
	for (int i = 0; i < eFS_MAX; ++i)
	{
		SAFE_DELETE(m_pMonsterAnimation[i]);
	}
	SAFE_DELETE(m_pCollisionData);
}

void	cFishBase::SetTransformCollision(cMatrix44 e_mat, int e_iPIUnitIndex)
{
	if (m_pCollisionData)
	{
		auto l_pObject = (*m_pCollisionData)[e_iPIUnitIndex];
		l_pObject->SetTransform(e_mat);
	}
}

void	cFishBase::ProcessCollisionlData(TiXmlElement*e_pElement)
{
	PARSE_ELEMENT_START(e_pElement)
		COMPARE_NAME("CollisionFile")
		{
			std::string	l_strFileName = UT::WcharToChar(l_strValue);
			m_pCollisionData = new c2DImageCollisionData();
			if (!m_pCollisionData->Parse(l_strFileName.c_str()))
			{
				UT::ErrorMsg(l_strFileName.c_str(), "parse failed");
			}
		}
	PARSE_NAME_VALUE_END

}

void	cFishBase::ProcessStatusAnimationData(TiXmlElement*e_pTiXmlElement)
{
	cMPDIList*l_pMPDIList = nullptr;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("MPDIList")
		{
			l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strValue);
			if (!l_pMPDIList)
			{
				UT::ErrorMsg(l_strValue, L"MPDIList not found");
				return;
			}
		}
		else
		COMPARE_NAME("Moving")
		{
			cMPDI*l_pMPDI = l_pMPDIList->GetObject(l_strValue);
			if (l_pMPDI)
			{
				m_pMonsterAnimation[eFS_MOVING] = new cMPDI(l_pMPDI);
				m_pMonsterAnimation[eFS_MOVING]->SetDoPositionOffsetToCenter(true);
				Vector2	l_vSize = m_pMonsterAnimation[eFS_MOVING]->GetDrawSize();
				m_fRadius = l_vSize.x>l_vSize.y ? l_vSize.x : l_vSize.y;
				m_fRadius /= 2.f;
			}
			else
			{
				UT::ErrorMsg(l_strValue, L"Moving MPDI not found");
			}
		}
		else
		COMPARE_NAME("DiedShow")
		{
			cMPDI*l_pMPDI = l_pMPDIList->GetObject(l_strValue);;
			if (l_pMPDI)
			{
				m_pMonsterAnimation[eFS_DIED_SHOW] = new cMPDI(l_pMPDI);
				m_pMonsterAnimation[eFS_DIED_SHOW]->SetDoPositionOffsetToCenter(true);
			}
			else
			{
				UT::ErrorMsg(l_strValue, L"DiedShow MPDI not found");
			}
		}
		else
		COMPARE_NAME("Struggle")
		{
			cMPDI*l_pMPDI = l_pMPDIList->GetObject(l_strValue);;
			if (l_pMPDI)
			{
				m_pMonsterAnimation[eFS_STRUGGLE] = new cMPDI(l_pMPDI);
				m_pMonsterAnimation[eFS_STRUGGLE]->SetDoPositionOffsetToCenter(true);
			}
			else
			{
				UT::ErrorMsg(l_strValue, L"Struggle MPDI not found");
			}
		}
		else
		COMPARE_NAME("Scale")
		{
			m_fScale = VALUE_TO_FLOAT;
		}
	PARSE_NAME_VALUE_END
	for (int i = 0; i < eFS_MAX; ++i)
	{
		if (m_pMonsterAnimation[i])
		{
			m_pMonsterAnimation[i]->Init();
		}
	}
}

void	cFishBase::Init()
{
	m_eFishStatus = eFS_NONE;
	InternalInit();
	if (m_pMonsterAnimation[eFS_DIED_SHOW])
	{
		m_pMonsterAnimation[eFS_DIED_SHOW]->Init();
	}
}

void	cFishBase::Update(float e_fElpaseTime)
{
	if (!m_pMonsterAnimation[m_eFishStatus])
	{
		m_eFishStatus = eFS_MOVING;
	}
	if (m_eFishStatus == eFS_STRUGGLE)
	{
		m_StruggleTC.Update(e_fElpaseTime);
		if (m_StruggleTC.bTragetTimrReached)
		{
			m_eFishStatus = eFS_MOVING;
		}
	}
	if(m_pMonsterAnimation[m_eFishStatus])
		m_pMonsterAnimation[m_eFishStatus]->Update(e_fElpaseTime);
	if (m_eFishStatus == eFS_DIED_SHOW)
	{
		if (m_pMonsterAnimation[m_eFishStatus])
		{
			if (m_pMonsterAnimation[m_eFishStatus]->IsAnimationDone())
				m_eFishStatus = eFS_WAITING_FOR_CLEAN;
		}
		else
		{
			m_eFishStatus = eFS_WAITING_FOR_CLEAN;
		}
	}
	InternalUpdate(e_fElpaseTime);
}

void	cFishBase::Render()
{
	if (m_pMonsterAnimation[m_eFishStatus])
		m_pMonsterAnimation[m_eFishStatus]->Render();
}

bool	cFishBase::IsVisible(Vector4 e_vWall)
{
	Vector3 l_vPos = this->GetWorldPosition();
	return SphereCollideRect(e_vWall, l_vPos, m_fRadius);
}

bool	cFishBase::IsStatusAllowToDied()
{
	if( m_eFishBodyType == eFS_DIED_SHOW || m_eFishBodyType == eFS_WAITING_FOR_CLEAN )
		return false;
	return true;
}

void	cFishBase::StatusChange(eFishStatus e_eFishStatus)
{
	m_eFishStatus = e_eFishStatus;
	if (!m_pMonsterAnimation[e_eFishStatus])
		return;
	if (m_eFishStatus == eFS_STRUGGLE)
	{
		m_StruggleTC.SetTargetTime(m_StruggleTime.Rand());
		m_StruggleTC.Start();
	}
}

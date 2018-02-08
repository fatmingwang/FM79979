#include "stdafx.h"
#include "FishBase.h"

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
	m_eFishBodyType = eFBT_Total;
	for (int i = 0; i < eFS_MAX; ++i)
		m_pMonsterAnimation[i] = nullptr;;
	m_StruggleTC;
	m_iID = -1;
	m_fScale = 1.f;
	m_fRadius = -1.f;
	m_eFishStatus = eFS_NONE;
}

cFishBase::~cFishBase()
{

}

void	cFishBase::ProcessStatusData(TiXmlElement*e_pTiXmlElement)
{

}

cFishBase::cFishBase(cFishBase*e_pMonsterBase)
{

}

void	cFishBase::Init()
{
	m_eFishStatus = eFS_NONE;
	InternalInit();
}

void	cFishBase::Update(float e_fElpaseTime)
{
	m_pMonsterAnimation[m_eFishStatus]->Update(e_fElpaseTime);
}

void	cFishBase::Render()
{

}

bool	cFishBase::IsStatusAllowToDied()
{
	return false;
}

void	cFishBase::StatusChange(eFishStatus e_eFishStatus)
{
	if (!m_pMonsterAnimation[e_eFishStatus])
		return;
	m_eFishStatus = e_eFishStatus;
	if (m_eFishStatus == eFS_STRUGGLE)
	{
		m_StruggleTC.SetTargetTime(m_StruggleTime.Rand());
		m_StruggleTC.Start();
	}
}

#include "stdafx.h"
#include "MonsterBase.h"

sMinMaxData<float>	cMonsterBase::m_StruggleTime = Vector2(1,3);


eBodyType	GetBodyType(const wchar_t *e_str)
{
	if (wcscmp(e_str, L"Small") == 0 || wcscmp(e_str, L"small") == 0)
		return eBT_Small;
	if (wcscmp(e_str, L"Medium") == 0 || wcscmp(e_str, L"medium") == 0)
		return eBT_Medium;
	if (wcscmp(e_str, L"Big") == 0 || wcscmp(e_str, L"big") == 0)
		return eBT_Big;
	if (wcscmp(e_str, L"Enormous") == 0 || wcscmp(e_str, L"enormous") == 0)
		return eBT_Enormous;
	return eBT_Total;
}

cMonsterBase::cMonsterBase()
{
	m_eBodyType = eBT_Total;
	for (int i = 0; i < eMS_MAX; ++i)
		m_pMonsterAnimation[i] = nullptr;;
	m_StruggleTC;
	m_iID = -1;
	m_fScale = 1.f;
	m_fRadius = -1.f;
	m_eMonsterStatus = eMS_NONE;
}

cMonsterBase::~cMonsterBase()
{

}

void	cMonsterBase::ProcessStatusData(TiXmlElement*e_pTiXmlElement)
{

}

cMonsterBase::cMonsterBase(cMonsterBase*e_pMonsterBase)
{

}

void	cMonsterBase::Init()
{
	m_eMonsterStatus = eMS_NONE;
	InternalInit();
}

void	cMonsterBase::Update(float e_fElpaseTime)
{
	m_pMonsterAnimation[m_eMonsterStatus]->Update(e_fElpaseTime);
}

void	cMonsterBase::Render()
{

}

bool	cMonsterBase::IsStatusAllowToDied()
{
	return false;
}

void	cMonsterBase::StatusChange(eMonsterStatus e_eMonsterStatus)
{
	if (!m_pMonsterAnimation[e_eMonsterStatus])
		return;
	m_eMonsterStatus = e_eMonsterStatus;
	if (m_eMonsterStatus == eMS_STRUGGLE)
	{
		m_StruggleTC.SetTargetTime(m_StruggleTime.Rand());
		m_StruggleTC.Start();
	}
}

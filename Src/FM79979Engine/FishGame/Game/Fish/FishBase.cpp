#include "stdafx.h"
#include "FishBase.h"
#include "FishLockPoints.h"
sMinMaxData<float>	cFishBase::m_StruggleTime = Vector2(1,3);


eFishBodyType	GetFishBodyTypeByString(const wchar_t *e_str)
{
	if (wcscmp(e_str, L"Small") == 0 || wcscmp(e_str, L"small") == 0)
		return eFBT_Small;
	if (wcscmp(e_str, L"Medium") == 0 || wcscmp(e_str, L"medium") == 0)
		return eFBT_Medium;
	if (wcscmp(e_str, L"Big") == 0 || wcscmp(e_str, L"big") == 0)
		return eFBT_Big;
	if (wcscmp(e_str, L"Enormous") == 0 || wcscmp(e_str, L"enormous") == 0)
		return eFBT_Enormous;
	assert("0 && GetFishBodyType wrong data");
	return eFBT_Total;
}

cFishBase::cFishBase()
{
	m_pCurrentbtShapeCollision = nullptr;
	m_pAnimationMPDIList = nullptr;
	m_eFishBodyType = eFBT_Total;
	for (int i = 0; i < eFS_MAX; ++i)
		m_pStatusAnimation[i] = nullptr;;
	m_iID = -1;
	m_fScale = 1.f;
	m_fRadius = -1.f;
	m_eFishStatus = eFS_WAIT_FOR_FETCH;
}

cFishBase::cFishBase(cFishBase*e_pFishBase)
{
	m_pCurrentbtShapeCollision = nullptr;
	m_pAnimationMPDIList = e_pFishBase->m_pAnimationMPDIList;
	m_eFishBodyType = e_pFishBase->m_eFishBodyType;
	for (int i = 0; i < eFS_MAX; ++i)
	{
		if (e_pFishBase->m_pStatusAnimation[i])
		{
			m_pStatusAnimation[i] = new cMPDI(e_pFishBase->m_pStatusAnimation[i]);
			m_pStatusAnimation[i]->SetParent(this);
		}
		else
			m_pStatusAnimation[i] = nullptr;
	}
	//m_pAnimationMPDIList
	for (auto l_Iterator = e_pFishBase->m_CollisionDataMap.begin(); l_Iterator != e_pFishBase->m_CollisionDataMap.end(); ++l_Iterator)
	{
		c2DImageCollisionData*l_pCollisionData = new c2DImageCollisionData(l_Iterator->second);
		m_CollisionDataMap.insert(std::make_pair(l_Iterator->first, l_pCollisionData));
	}
	m_iID = e_pFishBase->m_iID;
	m_fScale = e_pFishBase->m_fScale;
	m_fRadius = e_pFishBase->m_fRadius;
	m_eFishStatus = eFS_WAIT_FOR_FETCH;
}

cFishBase::~cFishBase()
{
	for (int i = 0; i < eFS_MAX; ++i)
	{
		SAFE_DELETE(m_pStatusAnimation[i]);
	}
	DELETE_MAP(m_CollisionDataMap);
}

void	cFishBase::ProcessCollisionlData(TiXmlElement*e_pElement)
{
	PARSE_ELEMENT_START(e_pElement)
		COMPARE_NAME("CollisionFile")
		{
			std::string	l_strFileName = UT::WcharToChar(l_strValue);
			c2DImageCollisionData*l_pCollisionData = new c2DImageCollisionData();
			if (!l_pCollisionData->Parse(l_strFileName.c_str()))
			{
				SAFE_DELETE(l_pCollisionData);
				UT::ErrorMsg(l_strFileName.c_str(), "parse failed");
			}
			assert(m_pAnimationMPDIList&&"m_pAnimationMPDIList not exist!?");
			if (l_pCollisionData )
			{
				int l_iPullzeImageIndex = m_pAnimationMPDIList->GetPIList()->GetObjectIndexByName(l_pCollisionData->GetName());
				assert(m_CollisionDataMap.find(l_iPullzeImageIndex) == m_CollisionDataMap.end() && "collision file added!?");
				m_CollisionDataMap.insert(std::make_pair(l_iPullzeImageIndex, l_pCollisionData));
			}
		}
	PARSE_NAME_VALUE_END
}
//<StatusAnimation MPDIList="" Moving="" DiedShow="" Struggle="" Scale="" RenderOrder="1" />
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
			m_pAnimationMPDIList = l_pMPDIList;
		}
		else
		COMPARE_NAME("Moving")
		{
			cMPDI*l_pMPDI = l_pMPDIList->GetObject(l_strValue);
			if (l_pMPDI)
			{
				m_pStatusAnimation[eFS_MOVING] = new cMPDI(l_pMPDI);
				m_pStatusAnimation[eFS_MOVING]->SetDoPositionOffsetToCenter(true);
				Vector2	l_vSize = m_pStatusAnimation[eFS_MOVING]->GetDrawSize();
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
				m_pStatusAnimation[eFS_DIED_SHOW] = new cMPDI(l_pMPDI);
				m_pStatusAnimation[eFS_DIED_SHOW]->SetDoPositionOffsetToCenter(true);
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
				m_pStatusAnimation[eFS_STRUGGLE] = new cMPDI(l_pMPDI);
				m_pStatusAnimation[eFS_STRUGGLE]->SetDoPositionOffsetToCenter(true);
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
		else
		COMPARE_NAME("RenderOrder")
		{
			this->SetLocalPosition(Vector3(0,0, VALUE_TO_FLOAT));
		}
	PARSE_NAME_VALUE_END
	for (int i = 0; i < eFS_MAX; ++i)
	{
		if (m_pStatusAnimation[i])
		{
			m_pStatusAnimation[i]->Init();
			m_pStatusAnimation[i]->SetParent(this);
		}
	}
}

void	cFishBase::Init()
{
	m_eFishStatus = eFS_MOVING;
	InternalInit();
	if (m_pStatusAnimation[eFS_DIED_SHOW])
	{
		m_pStatusAnimation[eFS_DIED_SHOW]->Init();
	}
}

void	cFishBase::SetTransform(Vector3 e_vPos, float e_fAngle, Vector3 e_vOffsetPos)
{
	cMatrix44 l_mat = cMatrix44::TranslationMatrix(e_vPos + e_vOffsetPos)*cMatrix44::ZAxisRotationMatrix(e_fAngle)*cMatrix44::ScaleMatrix(Vector3(this->m_fScale, this->m_fScale, 1.f));
	this->SetLocalTransform(l_mat);
	SetTransformCollision(l_mat);
}

void	cFishBase::SetTransformCollision(cMatrix44 e_mat)
{
	cMPDI*l_pMPDI = this->m_pStatusAnimation[this->m_eFishStatus];
	if (l_pMPDI)
	{
		cSubMPDI*l_pSubMPDI = l_pMPDI->GetObject(0);
		if (l_pSubMPDI)
		{
			sTexBehaviorDataWithImageIndexData*l_pCurrentPointData = l_pSubMPDI->GetCurrentPointData();
			if (m_pAnimationMPDIList)
			{
				int l_iIndex = l_pSubMPDI->GetPIList()->GetObjectIndexByPointer(l_pCurrentPointData->pPI);
				assert(l_iIndex != -1 && "SetTransformCollision index is -1 !!??");
				if (l_iIndex != -1)
				{
					assert(m_CollisionDataMap.find(l_iIndex) != m_CollisionDataMap.end() && " m_CollisionDataMap cannt find data!");
					c2DImageCollisionData*l_2DImageCollisionData = m_CollisionDataMap[l_iIndex];
					m_pCurrentbtShapeCollision = l_2DImageCollisionData->GetObject(l_pCurrentPointData->iImageIndex);
					m_pCurrentbtShapeCollision->SetTransform(e_mat);
				}
			}
		}
	}
}

void	cFishBase::Update(float e_fElpaseTime)
{
	if (!m_pStatusAnimation[m_eFishStatus])
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
	if(m_pStatusAnimation[m_eFishStatus])
		m_pStatusAnimation[m_eFishStatus]->Update(e_fElpaseTime);
	if (m_eFishStatus == eFS_DIED_SHOW)
	{
		if (m_pStatusAnimation[m_eFishStatus])
		{
			if (m_pStatusAnimation[m_eFishStatus]->IsAnimationDone())
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
	if (m_pStatusAnimation[m_eFishStatus])
		m_pStatusAnimation[m_eFishStatus]->Render();
}

void	cFishBase::DebugRender()
{
	if (m_pCurrentbtShapeCollision)
	{
		m_pCurrentbtShapeCollision->DebugRender();
	}
}

bool	cFishBase::IsCollide(cbtShapeCollision*e_pbtShapeCollision)
{
	if (m_pCurrentbtShapeCollision)
	{
		return m_pCurrentbtShapeCollision->Collide(e_pbtShapeCollision);
	}
	return false;
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
	if (!m_pStatusAnimation[e_eFishStatus])
		return;
	if (m_eFishStatus == eFS_STRUGGLE)
	{
		m_StruggleTC.SetTargetTime(m_StruggleTime.Rand());
		m_StruggleTC.Start();
	}
}


void	cFishBase::SetFishStatusToWaitForFetch()
{
	assert(eFS_WAITING_FOR_CLEAN == this->m_eFishStatus&&"only fish manager can call this while fish was died");
	m_eFishStatus = eFS_WAIT_FOR_FETCH;
}

eFishStatus		cFishBase::GetFishStatus()
{
	return m_eFishStatus;
}
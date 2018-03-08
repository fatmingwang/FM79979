#include "stdafx.h"
#include "FAIFish.h"
#include "FishManager.h"

TYPDE_DEFINE_MARCO(cFAIFish);

cFAIFish::cFAIFish()
{
	m_pFAIMachine = nullptr;
}

cFAIFish::~cFAIFish()
{
	SAFE_DELETE(m_pFAIMachine);
}
cFAIFish::cFAIFish(cFAIFish*e_pFAIFish):cFishBase(e_pFAIFish)
{
	m_pFAIMachine = nullptr;
	if (e_pFAIFish->m_pFAIMachine)
	{
		m_pFAIMachine = new cFAIMachine(e_pFAIFish->m_pFAIMachine);
	}
}

cFAIFish*cFAIFish::GetMe(TiXmlElement*e_pElement)
{
	FISH_LAZY_GET_ME_START(e_pElement, cFAIFish,
		COMPARE_TARGET_ELEMENT_VALUE(e_pElement, "FAI")
		{
			l_pObject->ProcessFAIMachineData(e_pElement);
		}
	);
}

void	cFAIFish::ProcessFAIMachineData(TiXmlElement*e_pTiXmlElement)
{
	SAFE_DELETE(m_pFAIMachine);

	auto l_str = e_pTiXmlElement->Attribute(L"File");
	std::string	l_strFileName = UT::WcharToChar(l_str);
	//WCHAR*l_strAIMachineName = UT::GetFileNameWithoutFullPath(l_str);
	if (l_str)
	{
		auto l_pFishManager = cFishManager::GetInstance();
		if (l_pFishManager)
		{
			auto l_pFAIMachine = l_pFishManager->GetFAIBehaviorParser()->GetFAIMachineByFileName(l_str);
			if (l_pFAIMachine)
			{
				this->m_pFAIMachine = dynamic_cast<cFAIMachine*>(l_pFAIMachine->Clone());
				cFAICharacterInterface*l_pFAICharacterInterface = m_pFAIMachine->GetSrcCharacter();
				sSphere l_sSphere(Vector3(0, 0, 0), this->m_fRadius);
				cBound l_Bound(l_sSphere);
				l_pFAICharacterInterface->SetLocalBound(&l_Bound);
			}
			else
			{
				UT::ErrorMsg(l_str, L"AI file error");
			}
		}
	}
}


Vector3		cFAIFish::GetRandomFishShowPos()
{
	sMinMaxData<float>	l_fPosX;
	sMinMaxData<float>	l_fPosY;
	Vector2 l_vWallSize = cGameApp::m_svGameResolution;

	l_fPosX.Max = l_vWallSize.x;		l_fPosX.Min = 0.f;
	l_fPosY.Max = l_vWallSize.y;		l_fPosY.Min = 0.f;
	int	l_iDirection = rand()%4;
	Vector3	l_vPos;
	switch (l_iDirection)
	{
	case 0://eD_LEFT
		l_vPos.x = -m_fRadius;
		l_vPos.y = l_fPosY.Rand();
		break;
	case 1://eD_UP
		l_vPos.x = l_fPosX.Rand();
		l_vPos.y = -m_fRadius;
		break;
	case 2://eD_RIGHT:
		l_vPos.x = l_vWallSize.x + m_fRadius;
		l_vPos.y = l_fPosY.Rand();
		break;
	case 3://eD_DOWN:
		l_vPos.x = l_fPosX.Rand();
		l_vPos.y = l_vWallSize.y + m_fRadius;
		break;
	}
	l_vPos.z = 0.f;
	return l_vPos;
}

void	cFAIFish::InternalInit()
{
	assert(m_pFAIMachine&&"cFAIFish not FAI?");
	if (m_pFAIMachine)
	{
		Vector3 l_vFishPos = GetRandomFishShowPos();
		cFAICharacterInterface*l_pFAICharacterInterface = m_pFAIMachine->GetSrcCharacter();
		l_pFAICharacterInterface->SetLocalPosition(l_vFishPos);
		m_pFAIMachine->Init();
	}
}

void	cFAIFish::InternalUpdate(float e_fElpaseTime)
{
	if (m_pFAIMachine)
	{
		m_pFAIMachine->Update(e_fElpaseTime);
		cFAICharacterInterface*l_pFAICharacterInterface = m_pFAIMachine->GetSrcCharacter();
		cMatrix44 l_mat = l_pFAICharacterInterface->GetWorldTransform();
		if (m_pFAIMachine->IsDone())
		{
			Vector4 l_vWall(0, 0, cGameApp::m_svGameResolution.x, cGameApp::m_svGameResolution.y);
			if(SphereCollideRect(l_vWall, l_mat.GetTranslation(),m_fRadius))
			{ 
				MonsterLeave();
			}
			else
			{//not in the scene 

				this->m_eFishStatus = eFS_WAITING_FOR_CLEAN;
			}
		}
		Vector3 l_vAngle = *l_pFAICharacterInterface->GetAngle();
		this->SetTransform(l_mat.GetTranslation(), l_vAngle.z- D3DXToRadian(90),Vector3::Zero);
	}
}


cFAIMachine*	cFAIFish::GetAIMachine()
{
	return m_pFAIMachine;
}

void			cFAIFish::MonsterLeave()
{
	m_pFAIMachine->SetCurrentWorkingObject(L"Leave");
}

void			cFAIFish::SetAIMachine(cFAIMachine* e_pFAIMachine)
{
	SAFE_DELETE(m_pFAIMachine);
	m_pFAIMachine = e_pFAIMachine;
}

void	cFAIFish::DebugRender()
{
	if (m_pFAIMachine)
	{
		m_pFAIMachine->Render();
		m_pFAIMachine->DebugRender();
		cFishBase::DebugRender();
	}
}
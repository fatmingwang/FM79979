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
		m_pFAIMachine = new cFAIMachine(e_pFAIFish->m_pFAIMachine);
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
			}
			else
			{
				UT::ErrorMsg(l_str, L"AI file error");
			}
		}
	}
}


void	cFAIFish::InternalInit()
{
	if (m_pFAIMachine)
	{
		m_pFAIMachine->Init();
	}
}

void	cFAIFish::InternalUpdate(float e_fElpaseTime)
{
	if (m_pFAIMachine)
	{
		m_pFAIMachine->Update(e_fElpaseTime);
	}
}


cFAIMachine*	cFAIFish::GetAIMachine()
{
	return m_pFAIMachine;
}

void			cFAIFish::MonsterLeave()
{

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
	}
}
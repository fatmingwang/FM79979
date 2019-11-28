#include "../../stdafx.h"
#include "MPDIToGameObject.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cMPDIToGameObject)
		cMPDIToGameObject::cMPDIToGameObject()
	{
		m_pBGMPDI = nullptr;
		m_pClonedMPDI = nullptr;
		m_pRenderObject = nullptr;
	}

	cMPDIToGameObject::cMPDIToGameObject(cMPDI * e_pMPDI, std::vector<std::wstring> e_strButtonName, bool e_bKeepMPDIAsBG)
	{
		m_pBGMPDI = nullptr;
		m_pClonedMPDI = nullptr;
		m_pRenderObject = nullptr;
		SetupMPDIAndButtonNameVector(e_pMPDI, e_strButtonName, e_bKeepMPDIAsBG);
	}

	cMPDIToGameObject::~cMPDIToGameObject()
	{
		this->Destroy();
	}

	bool cMPDIToGameObject::SetupMPDIAndButtonNameVector(cMPDI * e_pMPDI, std::vector<std::wstring> e_strButtonName, bool e_bKeepMPDIAsBG)
	{
		if (e_pMPDI)
		{
			if (m_pClonedMPDI || m_pRenderObject)
				return false;
			m_pClonedMPDI = new cMPDI(e_pMPDI);
			m_pRenderObject = new cRenderObject();
			//m_pRenderObject->SetIgnoreChildrenUpdate(true);
			std::wstring l_strName = e_pMPDI->GetName(); l_strName += L"_RenderObject";
			std::wstring l_strName2 = e_pMPDI->GetName(); l_strName2 += L"_MPDIToGameObject";
			m_pRenderObject->SetName(l_strName.c_str());
			this->SetName(l_strName2.c_str());
			auto l_uiSize = e_strButtonName.size();
			for (size_t i = 0; i < l_uiSize; ++i)
			{
				auto l_pSubMPDI = m_pClonedMPDI->GetObject(e_strButtonName[i].c_str());
				if (l_pSubMPDI)
				{
					l_pSubMPDI->SetAnimationLoop(true);
					auto l_pData = AddDefaultRenderClickBehaviorButton(l_pSubMPDI, nullptr, nullptr);
					m_pRenderObject->AddChild(std::get<1>(l_pData));
				}
			}
			if (e_bKeepMPDIAsBG)
			{
				m_pBGMPDI = m_pClonedMPDI;
				m_pBGMPDI->SetIgnoreChildrenUpdate(true);
				m_pRenderObject->AddChild(m_pClonedMPDI);
			}
			m_pRenderObject->DumpDebugInfo();
			return true;
		}
		return false;
	}

	void cMPDIToGameObject::AddClickObject(cClickBehaviorGroup* e_pClickBehaviorGroup)
	{
		cClickBehaviorGroup::AddObjectNeglectExist(e_pClickBehaviorGroup);
		e_pClickBehaviorGroup->Init();
	}

	void cMPDIToGameObject::AddRenderObject(cRenderObject*e_pRenderObject)
	{
		if (m_pRenderObject)
		{
			m_pRenderObject->AddChildToLast(e_pRenderObject);
			e_pRenderObject->InitNodes();
		}
	}

	void cMPDIToGameObject::AddObject(cMPDIToGameObject * e_pcMPDIToGameObject)
	{
		AddClickObject(e_pcMPDIToGameObject);
		AddRenderObject(e_pcMPDIToGameObject->m_pRenderObject);
	}

	bool cMPDIToGameObject::CreateRenderObject()
	{
		if (m_pRenderObject)
			return false;
		m_pRenderObject = new cRenderObject();
		return true;
	}

	cClickBehavior* cMPDIToGameObject::AddRenderObjectandGenerateClickBehavior(cRenderObject * e_pRenderObject, bool e_bEnableClickScale)
	{
		auto l_pClickBehavior = AddDefaultRenderClickBehaviorButton(e_pRenderObject, nullptr, nullptr, e_bEnableClickScale);
		if (m_pRenderObject)
			m_pRenderObject->AddChild(e_pRenderObject);
		return l_pClickBehavior;
	}

	bool cMPDIToGameObject::EnableBGCollide(bool e_bFullScreen)
	{
		if (e_bFullScreen)
		{
			CreateFullScreenCollide();
		}
		else
			if (m_pBGMPDI && m_pBGMPDI->Count() > 0)
			{
				m_CollideFunction = [this](int e_iPosX, int e_iPosY)
				{
					return this->m_pBGMPDI->Collide(e_iPosX, e_iPosY);
				};
			}
			else
			{
				return false;
			}
		return true;
	}

	void cMPDIToGameObject::DisConnectFromParent()
	{
		auto l_pOwner = GetOwner();
		if (l_pOwner)
		{
			cMPDIToGameObject*l_pClickBehaviorDispatcher = dynamic_cast<cMPDIToGameObject*>(l_pOwner);
			if (l_pClickBehaviorDispatcher)
				l_pClickBehaviorDispatcher->RemoveObjectWithoutDelete(this);
		}
		if (l_pOwner && m_pRenderObject)
		{
			m_pRenderObject->SetParent(nullptr);
		}
	}

	void cMPDIToGameObject::Init()
	{
		cClickBehaviorGroup::Init();
		if (m_pRenderObject)
		{
			m_pRenderObject->InitNodes();
		}
	}

	void cMPDIToGameObject::SetHide(bool e_bHide)
	{
		if (m_pRenderObject)
		{
			m_pRenderObject->SetVisible(!e_bHide);
		}
		this->SetEnable(!e_bHide);
	}

	void cMPDIToGameObject::Update(float e_fElpaseTime)
	{
		cClickBehaviorGroup::Update(e_fElpaseTime);
		if (this->m_pBGMPDI)
		{
			m_pBGMPDI->Update(e_fElpaseTime);
		}
	}

	void cMPDIToGameObject::Render()
	{
		if (m_pRenderObject)
		{
			m_pRenderObject->RenderNodes();
		}
	}

	void cMPDIToGameObject::Destroy()
	{
		cMPDIToGameObject*l_oOwner = dynamic_cast<cMPDIToGameObject*>(this->GetOwner());
		if (l_oOwner)
		{
			l_oOwner->RemoveObjectWithoutDelete(this);
		}
		cClickBehaviorGroup::Destroy();
		if (!m_pBGMPDI)
		{
			SAFE_DELETE(m_pClonedMPDI);
		}
		if (m_pRenderObject)
		{
			m_pRenderObject->SetParent(nullptr);
			Frame::DestoryWithChildren(m_pRenderObject);
			m_pRenderObject = nullptr;
		}
	}

	bool cMPDIToGameObject::AssignButtonClickFunction(const wchar_t * e_strButtonName, ClickFunction e_ClickFunction, cBasicSound*e_pSound)
	{
		auto l_pObject = GetObject(e_strButtonName);
		if (l_pObject)
		{
			l_pObject->SetMouseFunction(nullptr, nullptr, nullptr, e_ClickFunction, nullptr, nullptr);
			l_pObject->SetClickSound(e_pSound);
			return true;
		}
		return false;
	}
	//void cMPDIToGameObject::InscreasePirority(int e_iPiority)
	//{
	//	auto l_pOwner = m_pClickBehaviorDispatcher->GetOwner();
	//	if (l_pOwner)
	//	{
	//		cMPDIToGameObject*l_pMPDIToGameObject = (cMPDIToGameObject*)l_pOwner;
	//	}
	//}
	//
	//void cMPDIToGameObject::DecreasePirority(int e_iPiority)
	//{
	//	auto l_pOwner = m_pClickBehaviorDispatcher->GetOwner();
	//	if (l_pOwner)
	//	{
	//		cMPDIToGameObject*l_pMPDIToGameObject = (cMPDIToGameObject*)l_pOwner;
	//	}
	//}
//end namespace FATMING_CORE
}
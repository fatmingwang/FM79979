#include "ScrollBox.h"
#include "ScrollBar.h"
#include "../../../GameApp/GameApp.h"
#include "../../RenderFrameEffect/RenderFrameEffect.h"
namespace FATMING_CORE
{
	float	sSmoothSlide::sfMaxTime = 10.f;

	float	sSmoothSlide::m_sfSmoothSlideTC = 0.5f;
	float	sSmoothSlide::m_sfMinimumDistanceToDoSmoothSlide = 10.f;
	float	cScrollBox::sDataForScrollBox::m_sfRollbackTC = 0.3f;

	sSmoothSlide::sSmoothSlide()
	{
		m_LazySmoothCurve.AddPoint(Vector3(0, 0, 0), 0.0f);
		m_LazySmoothCurve.AddPoint(Vector3(35, 0, 0), 0.2f);
		m_LazySmoothCurve.AddPoint(Vector3(60, 0, 0), 0.4f);
		m_LazySmoothCurve.AddPoint(Vector3(80, 0, 0), 0.6f);
		m_LazySmoothCurve.AddPoint(Vector3(90, 0, 0), 0.8f);
		m_LazySmoothCurve.AddPoint(Vector3(100, 0, 0), 1.0f);
		m_LazySmoothCurve.SetLOD(3);

		//m_LazySmoothCurve.AddPoint(Vector3(5, 0, 0), 0.0f);
		//m_LazySmoothCurve.AddPoint(Vector3(12, 0, 0), 0.1f);
		//m_LazySmoothCurve.AddPoint(Vector3(26, 0, 0), 0.2f);
		//m_LazySmoothCurve.AddPoint(Vector3(47, 0, 0), 0.3f);
		//m_LazySmoothCurve.AddPoint(Vector3(64, 0, 0), 0.4f);
		//m_LazySmoothCurve.AddPoint(Vector3(70, 0, 0), 0.5f);//0.7
		//m_LazySmoothCurve.AddPoint(Vector3(80, 0, 0), 0.6f);
		//m_LazySmoothCurve.AddPoint(Vector3(88, 0, 0), 0.7f);
		//m_LazySmoothCurve.AddPoint(Vector3(94, 0, 0), 0.8f);//0.9
		//m_LazySmoothCurve.AddPoint(Vector3(98, 0, 0), 0.9f);
		//m_LazySmoothCurve.AddPoint(Vector3(100, 0, 0), 1.0f);
	}

	sSmoothSlide::~sSmoothSlide()
	{
	}

	void sSmoothSlide::Init(Vector3 e_vStartPos, Vector3 e_vMoveDistance, float e_fMouseDownUpTime)
	{
		float l_fTimeForSmoothSlide = (1.f - (m_sfSmoothSlideTC - e_fMouseDownUpTime))*2.f;
		float l_fMoveLength = e_vMoveDistance.Length();
		float l_fTimeLength = l_fMoveLength / 1000;
		while (l_fTimeLength > 10.f)
			l_fTimeLength /= 10.f;
		l_fTimeLength *= l_fTimeForSmoothSlide;
		if (l_fTimeLength == 0.f)
		{
			l_fTimeLength = EPSIONAL;
		}
		TC.SetTargetTime(l_fTimeLength);
		vTargetPos = e_vStartPos + e_vMoveDistance;
		vStartPos = vCurrentPos = e_vStartPos;
		vGapDistance = e_vMoveDistance;
		m_LazySmoothCurve.Init();
	}

	void sSmoothSlide::Update(float e_fElpaseTime)
	{
		TC.Update(e_fElpaseTime);
		float l_fLERP = TC.GetLERP();
		if (l_fLERP < 0.f)
			l_fLERP = 0.f;
		auto l_fLERPValue = m_LazySmoothCurve.GetPositionByTime(l_fLERP).x / 100.f;
		Vector3 l_vPos = l_fLERPValue * vGapDistance;
		vCurrentPos = vStartPos + l_vPos;
	}

	bool sSmoothSlide::IsDone()
	{
		return TC.bTragetTimrReached;
	}

	cScrollBox::sDataForScrollBox::sDataForScrollBox()
	{
		m_pSelectedObject = nullptr;
		m_vMouseUpRenderObjectPos = Vector3(0, 0, 0);
		m_vRollbackForProperPos = Vector3(0, 0, 0);
		m_fMouseDownUpCounter = 0.f;
		m_bRollBackToProperPosition = false;
		m_bDoSmoothSlide = false;
		m_RollBackTC.SetTargetTime(m_sfRollbackTC);
		m_vLocalViewRect = cGameApp::m_spOpenGLRender->m_vViewPortSize;
		m_vScrollerSizeOfAllObject = Vector2(0.f, 0.f);
		m_iColumnCountForMultiArrange = 0;
	}

	cScrollBox::sDataForScrollBox::~sDataForScrollBox()
	{
	}


	TYPDE_DEFINE_MARCO(cScrollBox)
		cScrollBox::cScrollBox()
	{
		//m_pHorizontalScrollBar = nullptr;
		//m_pVerticalScrollBar = nullptr;;
		this->CreateRenderObject();
		m_eOrientation = eOrientation::eO_HORIZONTAL;
		m_bArrangeWithMultiLine = false;
		m_vNextObjectGap = Vector2(5.f, 5.f);
		m_vRenderObjectOffsetPos = Vector2(5.f, 5.f);
		m_bArrangeWithMultiLine = false;
		m_iLeftTopObjectIndex = -1;
	}

	cScrollBox::~cScrollBox()
	{
	}

	bool cScrollBox::DoSmoothSlideCheck()
	{
		if (m_DataForScrollBox.m_fMouseDownUpCounter <= sSmoothSlide::m_sfSmoothSlideTC)
		{
			auto l_MouseMovePosition = m_DataForScrollBox.m_MouseMovePosition - m_DataForScrollBox.m_MouseDownPosition;
			POINT l_ABSMouseMovePosition = { abs(l_MouseMovePosition.x),abs(l_MouseMovePosition.y) };
			if (m_eOrientation == eOrientation::eO_HORIZONTAL && (float)l_ABSMouseMovePosition.x >= sSmoothSlide::m_sfMinimumDistanceToDoSmoothSlide)
			{
				m_DataForScrollBox.m_bDoSmoothSlide = true;
			}
			else
			if (m_eOrientation == eOrientation::eO_VERTICAL && (float)l_ABSMouseMovePosition.y >= sSmoothSlide::m_sfMinimumDistanceToDoSmoothSlide)
			{
				m_DataForScrollBox.m_bDoSmoothSlide = true;
			}
			else
			if (m_eOrientation == eOrientation::eO_BOTH)
			{
				if ((float)l_ABSMouseMovePosition.x >= sSmoothSlide::m_sfMinimumDistanceToDoSmoothSlide || (float)l_ABSMouseMovePosition.y >= sSmoothSlide::m_sfMinimumDistanceToDoSmoothSlide)
				{
					m_DataForScrollBox.m_bDoSmoothSlide = true;
				}
			}
			if (m_DataForScrollBox.m_bDoSmoothSlide)
			{
				float l_fTimeOffset = m_DataForScrollBox.m_fMouseDownUpCounter;
				Vector3 l_vSmoothMoveDistance((float)l_MouseMovePosition.x, (float)l_MouseMovePosition.y, 0.f);
				l_vSmoothMoveDistance *= 1.f / l_fTimeOffset;
				//check boundry
				Vector3 l_vBoundry = Vector3::Zero;
				if (IsHitBoundary(this->m_eOrientation, this->m_pObjectsMovingRoot->GetLocalPosition() + l_vSmoothMoveDistance, l_vBoundry))
				{
					l_vSmoothMoveDistance += l_vBoundry;
				}
				if (this->m_eOrientation == eOrientation::eO_HORIZONTAL)
					l_vSmoothMoveDistance.y = 0.f;
				else
				if (this->m_eOrientation == eOrientation::eO_VERTICAL)
					l_vSmoothMoveDistance.x = 0.f;
				m_SmoothSlide.Init(this->m_pObjectsMovingRoot->GetLocalPosition(), l_vSmoothMoveDistance, m_DataForScrollBox.m_fMouseDownUpCounter);
			}
			return true;
		}
		return false;
	}

	void cScrollBox::DoSmoothSlideUpdate(float e_fElpaseTime)
	{
		if (m_DataForScrollBox.m_bDoSmoothSlide)
		{
			m_SmoothSlide.Update(e_fElpaseTime);
			this->m_pObjectsMovingRoot->SetLocalPosition(m_SmoothSlide.vCurrentPos);
			if (m_SmoothSlide.IsDone())
			{
				m_DataForScrollBox.m_bDoSmoothSlide = false;
				m_eObjectMouseBehavior = eOMB_NONE;
			}
			ObjectUpdateVisible();
			for (auto l_CallbackFunction : m_ScorllMovingCallbackVecor)
				l_CallbackFunction(GetPercentageByCurrentRootRenderPosOfScrollerSizeOfAllObject());
		}
	}

	void cScrollBox::DoRollBackToProperPositionUpdate(float e_fElpaseTime)
	{
		if (m_DataForScrollBox.m_bRollBackToProperPosition)
		{
			if (!m_DataForScrollBox.m_RollBackTC.bTragetTimrReached)
			{
				m_DataForScrollBox.m_RollBackTC.Update(e_fElpaseTime);
				auto l_vMovePos = (m_DataForScrollBox.m_RollBackTC.GetLERP()*m_DataForScrollBox.m_vRollbackForProperPos) + m_DataForScrollBox.m_vMouseUpRenderObjectPos;
				this->m_pObjectsMovingRoot->SetLocalPosition(l_vMovePos);
				ObjectUpdateVisible();
				if (m_DataForScrollBox.m_RollBackTC.bTragetTimrReached)
				{
					m_DataForScrollBox.m_bRollBackToProperPosition = false;
					m_eObjectMouseBehavior = eOMB_NONE;
				}
			}
			for (auto l_CallbackFunction : m_ScorllMovingCallbackVecor)
				l_CallbackFunction(GetPercentageByCurrentRootRenderPosOfScrollerSizeOfAllObject());
		}
	}

	void cScrollBox::ObjectUpdateVisible()
	{
		if (m_pRenderObject)
		{
			int l_iIndex = 0;
			this->m_iLeftTopObjectIndex = -1;
			auto l_vRect = m_pScissorRenderObject->GetWorldPosScissorRect();
			RECT l_Rect = { (long)l_vRect.x, (long)l_vRect.y, (long)l_vRect.z, (long)l_vRect.w };
			GoThoughFirstChildLevel(
				[this, l_Rect](void*e_pData, Frame*e_pFrame)
			{
				int*l_piIndex = (int*)e_pData;
				cClickBehavior*l_pOwner = dynamic_cast<cClickBehavior*>(e_pFrame->GetOwner());
				auto l_pBound = e_pFrame->GetWorldBound();
				if(!l_pBound || l_pBound->Collide(l_Rect))
				{
					e_pFrame->SetVisible(true);
					if (l_pOwner)
						l_pOwner->SetEnable(true);
					if (m_iLeftTopObjectIndex == -1)
					{
						m_iLeftTopObjectIndex = *l_piIndex;
					}
				}
				else
				{
					e_pFrame->SetVisible(false);
					if (l_pOwner)
						l_pOwner->SetEnable(false);
				}
				++(*l_piIndex);
			}
			, m_pObjectsMovingRoot, &l_iIndex);
		}
	}

	void cScrollBox::InternalMouseMoveUp(int e_iPosX, int e_iPosY, bool e_bMouseMove)
	{
		if (m_DataForScrollBox.m_pSelectedObject && m_DataForScrollBox.m_pSelectedObject != this)
		{
			Vector2 l_vMoveDis((float)e_iPosX - (float)m_DataForScrollBox.m_MouseDownPosition.x, (float)e_iPosY - (float)m_DataForScrollBox.m_MouseDownPosition.y);
			if (l_vMoveDis.Length() >= 30.f)
			{
				m_DataForScrollBox.m_pSelectedObject->MouseMove(-79979, -79979);
				m_DataForScrollBox.m_pSelectedObject->Init();
				m_DataForScrollBox.m_pSelectedObject = nullptr;
			}
			if (m_DataForScrollBox.m_pSelectedObject)
			{
				cClickBehavior *l_pSelectedObject = nullptr;
				if (e_bMouseMove)
					l_pSelectedObject = cClickBehaviorGroup::MouseMove(e_iPosX, e_iPosY);
				else
					l_pSelectedObject = cClickBehaviorGroup::MouseUp(e_iPosX, e_iPosY);
				if (m_DataForScrollBox.m_pSelectedObject != l_pSelectedObject)
				{
					m_DataForScrollBox.m_pSelectedObject->MouseMove(-79979, -79979);
					m_DataForScrollBox.m_pSelectedObject->Init();
					m_DataForScrollBox.m_pSelectedObject = nullptr;
				}
			}
		}
	}

	bool cScrollBox::GetObjectPosByBisibleIndex(int e_iIndex, Vector3&e_vPos)
	{
		int l_iIndex = 0;
		auto l_pChild = this->m_pObjectsMovingRoot->GetFirstChild();
		while (l_pChild && l_iIndex != e_iIndex)
		{
			l_pChild = l_pChild->GetNextSibling();
			++l_iIndex;
		}
		if (!l_pChild)
			return false;
		e_vPos = l_pChild->GetWorldPosition();
		return true;
	}

	void cScrollBox::AddRenderObject(cRenderObject * e_pRenderObject)
	{
		if (m_pObjectsMovingRoot && e_pRenderObject)
		{
			m_pObjectsMovingRoot->AddChildToLast(e_pRenderObject);
			e_pRenderObject->InitNodes();
		}
	}

	bool cScrollBox::IsHitBoundary(eOrientation e_eOrientation, Vector3 e_vPos, Vector3 &e_vOverdDistance)
	{
		bool l_bRollBackToProperPosition = false;
		auto l_vSissorRect = m_pScissorRenderObject->GetWorldPosScissorRect();
		if (e_eOrientation == eOrientation::eO_HORIZONTAL)
		{
			if (e_vPos.x > 0.f)
			{
				l_bRollBackToProperPosition = true;
				e_vOverdDistance.x = -e_vPos.x;
			}
			float l_fValue = m_DataForScrollBox.m_vScrollerSizeOfAllObject.x - l_vSissorRect.Width() / 2.f;
			if (e_vPos.x < -l_fValue)
			{
				l_bRollBackToProperPosition = true;
				e_vOverdDistance.x = -(l_fValue + e_vPos.x);
			}
		}
		else
			if (e_eOrientation == eOrientation::eO_VERTICAL)
			{
				if (e_vPos.y > 0.f)
				{
					l_bRollBackToProperPosition = true;
					e_vOverdDistance.y = -e_vPos.y;
				}
				float l_fValue = m_DataForScrollBox.m_vScrollerSizeOfAllObject.y - l_vSissorRect.Height() / 2.f;
				if (e_vPos.y < -l_fValue)
				{
					l_bRollBackToProperPosition = true;
					e_vOverdDistance.y = -(l_fValue + e_vPos.y);
				}
			}
			else
			if (e_eOrientation == eOrientation::eO_BOTH)
			{
				l_bRollBackToProperPosition = IsHitBoundary(eOrientation::eO_HORIZONTAL, e_vPos, e_vOverdDistance) | IsHitBoundary(eOrientation::eO_VERTICAL, e_vPos, e_vOverdDistance);
			}
		return l_bRollBackToProperPosition;
	}

	void cScrollBox::ArrangeObjectPos(Vector2 e_vStartOffsetPos, Vector2 e_vObjectGap, bool e_bMultiLines)
	{
		Vector3 l_vPos = e_vStartOffsetPos;
		auto l_vSissorRect = m_pScissorRenderObject->GetWorldPosScissorRect();
		if (this->m_pObjectsMovingRoot)
		{
			//assert(m_eOrientation != eOrientation::eO_BOTH &&"ArrangeObjectPos with eOrientation::eO_BOTH !?.");
			eOrientation l_eOrientation = this->m_eOrientation;
			m_DataForScrollBox.m_iColumnCountForMultiArrange = 0;
			int l_iBothDirectionWithMiltiLineRoot = 1;
			int l_iCurrentCountForBothDirection = 0;
			if (l_eOrientation == eOrientation::eO_BOTH)
			{
				int l_iCount = this->Count();
				while (l_iBothDirectionWithMiltiLineRoot*l_iBothDirectionWithMiltiLineRoot < l_iCount)
				{
					++l_iBothDirectionWithMiltiLineRoot;
				}
				m_DataForScrollBox.m_iColumnCountForMultiArrange = l_iBothDirectionWithMiltiLineRoot;
			}
			auto l_pChild = m_pObjectsMovingRoot->GetFirstChild();
			Vector2 l_ViewRectSize(l_vSissorRect.Width(), l_vSissorRect.Height());
			int l_iMultiLinesCount = 0;
			while (l_pChild)
			{
				l_pChild->SetLocalPosition(l_vPos);
				cRenderObject*l_pRenderObject = dynamic_cast<cRenderObject*>(l_pChild);
				POINT l_Size = l_pRenderObject->GetSize();
				if (l_eOrientation == eOrientation::eO_HORIZONTAL)
				{
					if (e_bMultiLines)
					{
						++l_iMultiLinesCount;
						l_vPos.y += (e_vObjectGap.y + l_Size.y);
						if (l_vPos.y + l_Size.y >= l_ViewRectSize.y)
						{
							if (m_DataForScrollBox.m_iColumnCountForMultiArrange == 0)
								m_DataForScrollBox.m_iColumnCountForMultiArrange = l_iMultiLinesCount;
							l_vPos.y = e_vStartOffsetPos.y;
							l_vPos.x += e_vObjectGap.x + l_Size.x;
						}
					}
					else
					{
						l_vPos.x += e_vObjectGap.x + l_Size.x;
					}
				}
				else
				if (l_eOrientation == eOrientation::eO_VERTICAL)
				{
					if (e_bMultiLines)
					{
						++l_iMultiLinesCount;
						l_vPos.x += e_vObjectGap.x + l_Size.x;
						if (l_vPos.x + l_Size.x >= l_ViewRectSize.x)
						{
							if (m_DataForScrollBox.m_iColumnCountForMultiArrange == 0)
								m_DataForScrollBox.m_iColumnCountForMultiArrange = l_iMultiLinesCount;
							l_vPos.x = e_vStartOffsetPos.x;
							l_vPos.y += e_vObjectGap.y + l_Size.y;
						}
					}
					else
					{
						l_vPos.y += e_vObjectGap.y + l_Size.y;
					}
				}
				else
				if (l_eOrientation == eOrientation::eO_BOTH)
				{
					++l_iCurrentCountForBothDirection;
					l_vPos.x += e_vObjectGap.x + l_Size.x;
					if (l_iCurrentCountForBothDirection == l_iBothDirectionWithMiltiLineRoot)
					{
						l_vPos.x = e_vStartOffsetPos.x;
						l_vPos.y += e_vObjectGap.y + l_Size.y;
						l_iCurrentCountForBothDirection = 0;
					}

				}
				Vector2 l_vDrawSize(l_vPos.x + l_Size.x, l_vPos.y + l_Size.y);
				if (m_DataForScrollBox.m_vScrollerSizeOfAllObject.x < l_vDrawSize.x)
					m_DataForScrollBox.m_vScrollerSizeOfAllObject.x = l_vDrawSize.x;
				if (m_DataForScrollBox.m_vScrollerSizeOfAllObject.y < l_vDrawSize.y)
					m_DataForScrollBox.m_vScrollerSizeOfAllObject.y = l_vDrawSize.y;
				l_pChild = l_pChild->GetNextSibling();
			}
			ObjectUpdateVisible();
		}
	}

	void cScrollBox::Init()
	{
		cMPDIToGameObject::Init();
		if (this->Count())
			m_iLeftTopObjectIndex = 0;
		else
			m_iLeftTopObjectIndex = -1;
		m_DataForScrollBox.m_vScrollerSizeOfAllObject = Vector2(0.f, 0.f);
		if (m_pRenderObject)
		{
			m_pRenderObject->InitNodes();
		}
		this->ArrangeObjectPos(m_vRenderObjectOffsetPos, m_vNextObjectGap, m_bArrangeWithMultiLine);
	}

	cClickBehavior* cScrollBox::AddRenderObjectandGenerateClickBehavior(cRenderObject * e_pRenderObject, bool e_bEnableClickScale)
	{
		auto l_pClickBehavior = AddDefaultRenderClickBehaviorButton(e_pRenderObject, nullptr, nullptr, e_bEnableClickScale);
		e_pRenderObject->SetOwner(l_pClickBehavior);
		if (this->m_pObjectsMovingRoot)
			m_pObjectsMovingRoot->AddChild(e_pRenderObject);
		return l_pClickBehavior;
	}

	void cScrollBox::Update(float e_fElpaseTime)
	{
		m_DataForScrollBox.m_fMouseDownUpCounter += e_fElpaseTime;
		DoRollBackToProperPositionUpdate(e_fElpaseTime);
		DoSmoothSlideUpdate(e_fElpaseTime);
		if (m_DataForScrollBox.m_bDoSmoothSlide || m_DataForScrollBox.m_bRollBackToProperPosition)
			ObjectUpdateVisible();
		else
		if (m_DataForScrollBox.m_pSelectedObject && m_DataForScrollBox.m_pSelectedObject != this)
		{
			m_DataForScrollBox.m_pSelectedObject->Update(e_fElpaseTime);
		}
		cMPDIToGameObject::Update(e_fElpaseTime);
	}

	void cScrollBox::Destroy()
	{
		cMPDIToGameObject::Destroy();
		m_pScissorRenderObject = nullptr;
		m_pObjectsMovingRoot = nullptr;
	}

	bool cScrollBox::CreateRenderObject()
	{
		if (!m_pRenderObject)
		{
			this->m_pRenderObject = new cRenderObject();
			m_pScissorRenderObject = new cScissorRenderObject();
			m_pObjectsMovingRoot = new cRenderObject();
			m_pRenderObject->SetName(L"cScrollBoxRenderObject");
			m_pScissorRenderObject->SetName(L"ScrollBoxScissorRenderObject");
			m_pObjectsMovingRoot->SetName(L"ScrollBoxMovingRoot");
			m_pRenderObject->AddChild(m_pScissorRenderObject);
			m_pScissorRenderObject->AddChild(m_pObjectsMovingRoot);
			return true;
		}
		return false;
	}

	bool cScrollBox::AddBG(cMPDI * e_pBGMPDI)
	{
		if (m_pBGMPDI)
		{
			return false;
		}
		this->m_pClonedMPDI = e_pBGMPDI;
		this->m_pBGMPDI = e_pBGMPDI;
		if (m_pRenderObject)
		{
			this->m_pRenderObject->AddChild(m_pBGMPDI);
			return true;
		}
		return false;
	}

	void cScrollBox::SetScrollerPositionAndViewRect(Vector3 e_vPos, Vector4 e_vLocalViewRect)
	{
		if (this->m_pScissorRenderObject)
		{
			assert(m_pRenderObject&&"cScrollBox::SetScrollerPositionAndViewRect is nullptr");
			if (m_pRenderObject)
			{
				m_pRenderObject->SetLocalPosition(e_vPos);
			}
			m_pScissorRenderObject->SetScissorRect(e_vLocalViewRect);
			m_DataForScrollBox.m_vLocalViewRect = e_vLocalViewRect;
			m_CollideFunction = std::bind(&cScissorRenderObject::Collide, m_pScissorRenderObject, std::placeholders::_1, std::placeholders::_2);
		}
	}

	void cScrollBox::SetOrientation(eOrientation e_eOrientation)
	{
		m_eOrientation = e_eOrientation;
	}

	bool cScrollBox::ScrollMoveToTargetItemIndex(int e_iTargetIndex, bool e_bDoSmooth)
	{
		int l_iCount = this->Count();
		if (e_iTargetIndex < 0)
		{
			e_iTargetIndex = 0;
		}
		if (e_iTargetIndex >= l_iCount)
		{
			e_iTargetIndex = l_iCount - 1;
		}
		if (this->m_pObjectsMovingRoot)
		{
			if (m_iLeftTopObjectIndex == -1)
			{//should't happen!?
				this->ObjectUpdateVisible();
				if (m_iLeftTopObjectIndex == -1)
					return false;
			}
			int l_iLeftTopObjectIndex = m_iLeftTopObjectIndex;
			Vector3 l_vCurrentPos;
			Vector3 l_vTargetPos;
			if (l_iLeftTopObjectIndex <= 0 && e_iTargetIndex <= 0)
			{
				m_pObjectsMovingRoot->SetLocalPosition(Vector3::Zero);
			}
			else
			if (GetObjectPosByBisibleIndex(l_iLeftTopObjectIndex, l_vCurrentPos) &&
				GetObjectPosByBisibleIndex(e_iTargetIndex, l_vTargetPos))
			{
				Vector3 l_vMoveDistance = l_vCurrentPos - l_vTargetPos;
				m_SmoothSlide.Init(this->m_pObjectsMovingRoot->GetLocalPosition(), l_vMoveDistance, sSmoothSlide::m_sfSmoothSlideTC - 0.1f);
				m_DataForScrollBox.m_bDoSmoothSlide = true;
				if (!e_bDoSmooth)
				{
					m_SmoothSlide.TC.SetTargetTime(0.0016f);
				}
				return true;
			}
		}
		return false;
	}

	bool cScrollBox::ScrollMoveToNextItem(bool e_bDoSmooth)
	{
		if (this->m_DataForScrollBox.m_iColumnCountForMultiArrange != 0)
		{
			if (this->m_iLeftTopObjectIndex + 1 < m_DataForScrollBox.m_iColumnCountForMultiArrange)
				return ScrollMoveToTargetItemIndex(this->m_iLeftTopObjectIndex + 1, e_bDoSmooth);
			return false;
		}
		return ScrollMoveToTargetItemIndex(this->m_iLeftTopObjectIndex + 1, e_bDoSmooth);
	}

	bool cScrollBox::ScrollMoveToPrevioudItem(bool e_bDoSmooth)
	{
		if (this->m_DataForScrollBox.m_iColumnCountForMultiArrange != 0)
		{
			if (this->m_iLeftTopObjectIndex - 1 > -1)
				return ScrollMoveToTargetItemIndex(this->m_iLeftTopObjectIndex - 1, e_bDoSmooth);
			return false;
		}
		return ScrollMoveToTargetItemIndex(this->m_iLeftTopObjectIndex - 1, e_bDoSmooth);
	}

	bool cScrollBox::MiltilineScrollMove(int e_iRowMoveIndex, int e_iColumnMoveIndex, bool e_bDoSmooth)
	{
		if (this->m_DataForScrollBox.m_iColumnCountForMultiArrange == 0)
		{
			if (e_iRowMoveIndex != 0)
				return ScrollMoveToTargetItemIndex(this->m_iLeftTopObjectIndex + e_iRowMoveIndex, e_bDoSmooth);
			if (e_iColumnMoveIndex != 0)
				return ScrollMoveToTargetItemIndex(this->m_iLeftTopObjectIndex + e_iColumnMoveIndex, e_bDoSmooth);
		}
		if (this->m_eOrientation == eOrientation::eO_BOTH)
		{
			int l_iIndexX = m_iLeftTopObjectIndex % m_DataForScrollBox.m_iColumnCountForMultiArrange + e_iColumnMoveIndex;
			int l_iIndexY = m_iLeftTopObjectIndex / m_DataForScrollBox.m_iColumnCountForMultiArrange + e_iRowMoveIndex;
			if (l_iIndexY >= m_DataForScrollBox.m_iColumnCountForMultiArrange || l_iIndexX >= m_DataForScrollBox.m_iColumnCountForMultiArrange)
				return false;
		}
		int l_iTargetIndex = (e_iRowMoveIndex * m_DataForScrollBox.m_iColumnCountForMultiArrange) + e_iColumnMoveIndex + this->m_iLeftTopObjectIndex;
		return ScrollMoveToTargetItemIndex(l_iTargetIndex, e_bDoSmooth);
	}

	bool cScrollBox::CreateScrollBar(const wchar_t*e_strPIFileName, eOrientation e_eOrientation)
	{
		return false;
		cMPDIToGameObject*l_pScrollBar = nullptr;
		//if (e_eOrientation == eOrientation::eO_HORIZONTAL)
		//{
		//	if (m_pHorizontalScrollBar)
		//		return false;
		//	l_pScrollBar = m_pHorizontalScrollBar = cScrollBar::GetMe(this, e_eOrientation, e_strPIFileName);
		//}
		//else
		//if (e_eOrientation == eOrientation::eO_VERTICAL)
		//{
		//	if (m_pVerticalScrollBar)
		//		return false;
		//	l_pScrollBar = m_pVerticalScrollBar = cScrollBar::GetMe(this, e_eOrientation, e_strPIFileName);
		//}
		//if (!l_pScrollBar)
		//	return false;
		AddClickObject(l_pScrollBar);
		if (this->m_pRenderObject)
		{
			m_pRenderObject->AddChildToLast(l_pScrollBar->GetRenderObject());
		}
		return true;
	}

	bool cScrollBox::MoveScrollByScrollBarPercent(float e_fXPercentage, float e_fYPercentage)
	{
		if (e_fXPercentage > 1.f)e_fXPercentage = 1.f; else if (e_fXPercentage < 0.f)	e_fXPercentage = 0.f;
		if (e_fYPercentage > 1.f)e_fYPercentage = 1.f; else if (e_fYPercentage < 0.f)	e_fYPercentage = 0.f;
		auto l_fPosX = e_fXPercentage * m_DataForScrollBox.m_vScrollerSizeOfAllObject.x;
		auto l_fPosY = e_fYPercentage * m_DataForScrollBox.m_vScrollerSizeOfAllObject.y;
		Vector3 l_vTargetPos(l_fPosX, l_fPosY, 0.f);
		Vector3 l_vCurrentPos = this->m_pObjectsMovingRoot->GetLocalPosition();
		Vector3 l_vMoveDistance = -l_vTargetPos - l_vCurrentPos;
		m_SmoothSlide.Init(this->m_pObjectsMovingRoot->GetLocalPosition(), l_vMoveDistance, sSmoothSlide::m_sfSmoothSlideTC - 0.1f);
		m_SmoothSlide.TC.SetTargetTime(0.008f);
		m_DataForScrollBox.m_bDoSmoothSlide = true;
		return true;
		return false;
	}

	Vector2 cScrollBox::GetPercentageByCurrentRootRenderPosOfScrollerSizeOfAllObject()
	{
		Vector2 l_vPercentage(0, 0);
		auto l_vPos = this->m_pObjectsMovingRoot->GetLocalPosition();
		if (l_vPos.x > 0.f)
		{
			l_vPercentage.x = 0.f;
		}
		else
		{
			l_vPos.x = abs(l_vPos.x);
			l_vPercentage.x = l_vPos.x / this->m_DataForScrollBox.m_vScrollerSizeOfAllObject.x;
		}
		if (l_vPos.y > 0.f)
		{
			l_vPercentage.y = 0.f;
		}
		else
		{
			l_vPos.y = abs(l_vPos.y);
			l_vPercentage.y = l_vPos.y / this->m_DataForScrollBox.m_vScrollerSizeOfAllObject.y;
		}
		return l_vPercentage;
	}

	void cScrollBox::SetScissorRectColor(bool e_bRender, Vector4 e_vColor)
	{
		if (m_pScissorRenderObject)
		{
			m_pScissorRenderObject->SetRenderScissor(e_bRender);
			m_pScissorRenderObject->SetScissorRectColor(e_vColor);
		}
	}

	cClickBehavior * cScrollBox::MouseDown(int e_iPosX, int e_iPosY)
	{
		if (!this->m_bEnable)
			return nullptr;
		if (m_pScissorRenderObject->Collide(e_iPosX, e_iPosY))
		{
			this->m_eObjectMouseBehavior = eObjectMouseBehavior::eOMB_FIRST_TIME_INTO;
			if (!m_DataForScrollBox.m_bDoSmoothSlide)
				m_DataForScrollBox.m_pSelectedObject = this->ChildrenMouseDown(e_iPosX, e_iPosY);
			m_DataForScrollBox.m_bDoSmoothSlide = false;
			m_DataForScrollBox.m_fMouseDownUpCounter = 0.f;
			m_DataForScrollBox.m_MouseDownPosition.x = e_iPosX;
			m_DataForScrollBox.m_MouseDownPosition.y = e_iPosY;
			m_DataForScrollBox.m_MouseMovePosition = m_DataForScrollBox.m_MouseDownPosition;
			return this;
		}
		//if (m_pHorizontalScrollBar)
		//{
		//	cClickBehavior*l_pClickBehavior = m_pHorizontalScrollBar->MouseDown(e_iPosX, e_iPosY);
		//	if (l_pClickBehavior)
		//		return l_pClickBehavior;
		//}
		//if (m_pVerticalScrollBar)
		//{
		//	auto l_pClickBehavior = m_pVerticalScrollBar->MouseDown(e_iPosX, e_iPosY);
		//	if (l_pClickBehavior)
		//		return l_pClickBehavior;
		//}

		return nullptr;
	}

	cClickBehavior * cScrollBox::MouseMove(int e_iPosX, int e_iPosY)
	{
		if (!this->m_bEnable || m_DataForScrollBox.m_bRollBackToProperPosition)
			return nullptr;
		if (m_pScissorRenderObject->Collide(e_iPosX, e_iPosY))
		{
			this->m_eObjectMouseBehavior = eObjectMouseBehavior::eOMB_HORVER;
			Vector3 l_vMovedPos((float)e_iPosX - m_DataForScrollBox.m_MouseMovePosition.x, (float)e_iPosY - m_DataForScrollBox.m_MouseMovePosition.y, 0.f);
			m_DataForScrollBox.m_MouseMovePosition.x = e_iPosX;
			m_DataForScrollBox.m_MouseMovePosition.y = e_iPosY;
			if (this->m_pObjectsMovingRoot)
			{
				if (m_eOrientation == eOrientation::eO_HORIZONTAL)
				{
					l_vMovedPos.y = 0.f;
				}
				else
				if (m_eOrientation == eOrientation::eO_VERTICAL)
				{
					l_vMovedPos.x = 0.f;
				}
				else
				if (m_eOrientation == eOrientation::eO_BOTH)
				{

				}
				Vector3 l_vRenderPos = m_pObjectsMovingRoot->GetLocalPosition();
				m_pObjectsMovingRoot->SetLocalPosition(l_vRenderPos + l_vMovedPos);
			}
			InternalMouseMoveUp(e_iPosX, e_iPosY, true);
			ObjectUpdateVisible();
			for (auto l_CallbackFunction : m_ScorllMovingCallbackVecor)
				l_CallbackFunction(GetPercentageByCurrentRootRenderPosOfScrollerSizeOfAllObject());
			return this;
		}
		return nullptr;
	}

	cClickBehavior * cScrollBox::MouseUp(int e_iPosX, int e_iPosY)
	{
		if (!this->m_bEnable || m_DataForScrollBox.m_bRollBackToProperPosition)
			return nullptr;
		//if (m_DataForScrollBox.m_vWorldViewRect.CollidePoint(e_iPosX, e_iPosY))
		{
			this->m_eObjectMouseBehavior = eObjectMouseBehavior::eOMB_UP;
			m_DataForScrollBox.m_vMouseUpRenderObjectPos = this->m_pObjectsMovingRoot->GetLocalPosition();
			DoSmoothSlideCheck();
			m_DataForScrollBox.m_vRollbackForProperPos = Vector3(0.f, 0.f, 0.f);
			m_DataForScrollBox.m_bRollBackToProperPosition = IsHitBoundary(m_eOrientation, m_pObjectsMovingRoot->GetLocalPosition(), m_DataForScrollBox.m_vRollbackForProperPos);
			if (m_DataForScrollBox.m_bRollBackToProperPosition)
			{
				m_DataForScrollBox.m_bDoSmoothSlide = false;
				m_DataForScrollBox.m_RollBackTC.Start();
			}
			InternalMouseMoveUp(e_iPosX, e_iPosY, false);
			ObjectUpdateVisible();
			return this;
		}
		return this;
	}
//end namespace FATMING_CORE
}
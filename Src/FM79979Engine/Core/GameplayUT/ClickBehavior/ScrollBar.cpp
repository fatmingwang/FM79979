#include "../../stdafx.h"
#include "ScrollBar.h"
#include "ScrollBox.h"
#include "../GameApp.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cScrollBar);
	cScrollBar::cScrollBar(cScrollBox*e_pScrollBox)
	{
		this->SetName(cScrollBar::TypeID);
		m_pSliderImage = nullptr;
		m_pScrollBox = e_pScrollBox;
		m_eOrientation = eOrientation::eO_HORIZONTAL;
		m_bDebugRender = false;
		m_pBGImage = nullptr;
		e_pScrollBox->SetScrollMovingCallback(std::bind(&cScrollBar::ScrollMoving, this, std::placeholders::_1));
	}

	cScrollBar::~cScrollBar()
	{
	}

	cScrollBar * cScrollBar::GetMe(cScrollBox * e_pScrollBox, eOrientation e_eOrientation, const wchar_t * e_strPIFileName)
	{
		cPuzzleImage*l_pPI = cGameApp::GetPuzzleImageByFileName(e_strPIFileName);
		cBaseImage*l_pButtonPrevious = nullptr;
		cBaseImage*l_pButtonNext = nullptr;
		cBaseImage*l_pScrollBarBG = nullptr;
		cBaseImage*l_pSlider = nullptr;
		const wchar_t*l_strName = nullptr;
		if (e_eOrientation == eOrientation::eO_HORIZONTAL)
		{
			l_strName = L"HScrollBar";
			l_pButtonPrevious = l_pPI->GetObject(L"ButtonLeft");
			l_pButtonNext = l_pPI->GetObject(L"ButtonRight");
			l_pScrollBarBG = l_pPI->GetObject(L"HorizontalScrollBarBG");
			l_pSlider = l_pPI->GetObject(L"HorizontalSlider");
		}
		else
			if (e_eOrientation == eOrientation::eO_VERTICAL)
			{
				l_strName = L"VScrollBar";
				l_pButtonPrevious = l_pPI->GetObject(L"ButtonUp");
				l_pButtonNext = l_pPI->GetObject(L"ButtonDown");
				l_pScrollBarBG = l_pPI->GetObject(L"VerticalScrollBarBG");
				l_pSlider = l_pPI->GetObject(L"VerticalSlider");
			}
		if (!l_pButtonPrevious ||
			!l_pButtonNext ||
			!l_pScrollBarBG ||
			!l_pSlider)
			return nullptr;
		auto l_pCloneButtonPrevious = new cBaseImage(l_pButtonPrevious);
		auto l_pCloneButtonNext = new cBaseImage(l_pButtonNext);
		auto l_pCloneScrollBarBG = new cBaseImage(l_pScrollBarBG);
		auto l_pCloneSlider = new cBaseImage(l_pSlider);
		cScrollBar*l_pScrollBar = new cScrollBar(e_pScrollBox);
		//l_pScrollBar->SetName(l_strName);
		l_pScrollBar->SetupUI(e_eOrientation, l_pCloneButtonPrevious, l_pCloneButtonNext, l_pCloneSlider, l_pCloneScrollBarBG);
		return l_pScrollBar;
	}

	bool cScrollBar::SetupUI(eOrientation e_eOrientation,
		cBaseImage* e_pPreviousButton, cBaseImage* e_pNextButton, cBaseImage* e_pSlider, cBaseImage* e_pBG)
	{
		this->Destroy();
		if (!e_pPreviousButton ||
			!e_pNextButton ||
			!e_pSlider ||
			!e_pBG)
			return false;
		m_pRenderObject = new cRenderObject();
		Vector4 l_vScrollLocalViewRect = this->m_pScrollBox->GetDataForScrollBox()->m_vLocalViewRect;
		const float l_fPosOffset = 5.f;
		Vector2 l_vScrollAreaSize(l_vScrollLocalViewRect.Width(), l_vScrollLocalViewRect.Height());
		m_eOrientation = e_eOrientation;
		auto l_ButtonSize = e_pNextButton->GetSize();
		auto l_vBGSize = e_pBG->GetSize();
		float l_fScaleX = 1.f;
		float l_fScaleY = 1.f;
		Vector3 l_vShowPos;
		if (m_eOrientation == eOrientation::eO_HORIZONTAL)
		{
			l_vShowPos = Vector3(l_vScrollLocalViewRect.x, l_vScrollLocalViewRect.w + l_fPosOffset, 0.f);
			Vector3 l_vPos = l_vShowPos;
			e_pPreviousButton->SetLocalPosition(l_vShowPos);
			l_fScaleX = (l_vScrollAreaSize.x - l_ButtonSize.x * 2) / l_vBGSize.x;
			l_fScaleY = (float)l_ButtonSize.x / l_vBGSize.y;
			l_vPos.x += l_ButtonSize.x;
			e_pSlider->SetLocalPosition(l_vPos);
			l_vShowPos.x = l_vScrollLocalViewRect.z - l_ButtonSize.x;
			e_pNextButton->SetLocalPosition(l_vShowPos);
			e_pBG->SetLocalPosition(Vector3(l_vScrollLocalViewRect.x + l_ButtonSize.x, l_vShowPos.y, 0.f));
		}
		else
		if (m_eOrientation == eOrientation::eO_VERTICAL)
		{
			l_vShowPos = Vector3(l_vScrollLocalViewRect.z + l_fPosOffset, l_vScrollLocalViewRect.y, 0.f);
			Vector3 l_vPos = l_vShowPos;
			e_pPreviousButton->SetLocalPosition(l_vShowPos);
			l_fScaleX = l_vBGSize.x / (float)l_ButtonSize.x;
			l_fScaleY = (l_vScrollAreaSize.y - l_ButtonSize.y) / l_vBGSize.y;
			l_vPos.y += l_ButtonSize.y;
			e_pSlider->SetLocalPosition(l_vPos);
			l_vShowPos.y = l_vScrollLocalViewRect.w - l_ButtonSize.y;
			e_pNextButton->SetLocalPosition(l_vShowPos);
			e_pBG->SetLocalPosition(Vector3(l_vScrollLocalViewRect.z + l_fPosOffset, l_vScrollLocalViewRect.y + l_ButtonSize.y, 0.f));
		}
		e_pBG->SetWidth((int)(l_fScaleX*l_vBGSize.x));
		e_pBG->SetHeight(((int)l_fScaleY*l_vBGSize.y));
		auto l_pBGClickBehavior = this->AddDefaultRenderClickBehaviorButton(e_pBG, nullptr, nullptr,false);
		this->AddDefaultRenderClickBehaviorButton(e_pPreviousButton,
			std::bind(&cScrollBar::ButtonPreviousClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), nullptr);
		this->AddDefaultRenderClickBehaviorButton(e_pNextButton,
			std::bind(&cScrollBar::ButtonNextCliclk, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), nullptr);
		//this->AddDefaultRenderClickBehaviorButton(e_pSlider,nullptr, nullptr, false);
		auto l_BGMouseEvent = std::bind(&cScrollBar::SliderMove, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		l_pBGClickBehavior->SetMouseFunction(nullptr, l_BGMouseEvent, l_BGMouseEvent, l_BGMouseEvent, nullptr, nullptr);

		if (m_pRenderObject)
			m_pRenderObject->AddChild(e_pPreviousButton);
		if (m_pRenderObject)
			m_pRenderObject->AddChild(e_pNextButton);
		if (m_pRenderObject)
			m_pRenderObject->AddChild(e_pSlider);
		if (m_pRenderObject)
			m_pRenderObject->AddChild(e_pBG);
		m_pSliderImage = e_pSlider;
		m_pBGImage = e_pBG;
		return true;
	}

	void cScrollBar::ButtonNextCliclk(int e_iPosX, int e_iPosY, cClickBehavior * e_pObject)
	{
		if (m_pScrollBox)
		{
			if (m_pScrollBox->GetOrientation() == eOrientation::eO_BOTH)
			{
				if (m_eOrientation == eOrientation::eO_HORIZONTAL)
					m_pScrollBox->MiltilineScrollMove(0, 1, false);
				else
					if (m_eOrientation == eOrientation::eO_VERTICAL)
						m_pScrollBox->MiltilineScrollMove(1, 0, false);
			}
			else
			{
				m_pScrollBox->MiltilineScrollMove(1, 0, false);
			}
		}
	}

	void cScrollBar::ButtonPreviousClick(int e_iPosX, int e_iPosY, cClickBehavior * e_pObject)
	{
		if (m_pScrollBox)
		{
			if (m_pScrollBox->GetOrientation() == eOrientation::eO_BOTH)
			{
				if (m_eOrientation == eOrientation::eO_HORIZONTAL)
				{
					m_pScrollBox->MiltilineScrollMove(0, -1, false);
				}
				else
					if (m_eOrientation == eOrientation::eO_VERTICAL)
					{
						m_pScrollBox->MiltilineScrollMove(-1, 0, false);
					}
			}
			else
			{
				m_pScrollBox->MiltilineScrollMove(-1, 0, false);
			}
		}
	}

	void cScrollBar::SliderMove(int e_iPosX, int e_iPosY, cClickBehavior * e_pObject)
	{
		if (!m_pScrollBox)
			return;
		Vector3 l_vBGPos = m_pBGImage->GetWorldPosition();
		Vector4 l_vBGRect(l_vBGPos.x, l_vBGPos.y, l_vBGPos.x + m_pBGImage->GetWidth(), l_vBGPos.y + m_pBGImage->GetHeight());
		if (m_eOrientation == eOrientation::eO_HORIZONTAL)
		{
			if (e_iPosX > l_vBGRect.x&&e_iPosX < l_vBGRect.z)
			{
				float l_fPercentage = (e_iPosX - l_vBGRect.x) / l_vBGRect.Width();
				m_pScrollBox->MoveScrollByScrollBarPercent(l_fPercentage, 0.f);
				if (m_pSliderImage)
				{
					auto l_vPos = m_pSliderImage->GetWorldPosition();
					l_vPos.x = (float)e_iPosX;
					m_pSliderImage->SetWorldPosition(l_vPos);
				}
			}
		}
		else
			if (m_eOrientation == eOrientation::eO_VERTICAL)
			{
				if (e_iPosY > l_vBGRect.y&&e_iPosY < l_vBGRect.w)
				{
					float l_fPercentage = (e_iPosY - l_vBGRect.y) / l_vBGRect.Height();
					m_pScrollBox->MoveScrollByScrollBarPercent(0.f, l_fPercentage);
					if (m_pSliderImage)
					{
						auto l_vPos = m_pSliderImage->GetWorldPosition();
						l_vPos.y = (float)e_iPosY;
						m_pSliderImage->SetWorldPosition(l_vPos);
					}
				}
			}
	}

	void cScrollBar::BGClick(int e_iPosX, int e_iPosY, cClickBehavior * e_pObject)
	{

	}

	void cScrollBar::ScrollMoving(Vector2 e_vPercentage)
	{
		if (m_pSliderImage)
		{
			Vector3 l_vBGPos = m_pBGImage->GetWorldPosition();
			Vector4 l_vBGRect(l_vBGPos.x, l_vBGPos.y, l_vBGPos.x + m_pBGImage->GetWidth(), l_vBGPos.y + m_pBGImage->GetHeight());
			auto l_vPos = m_pSliderImage->GetWorldPosition();
			if (m_eOrientation == eOrientation::eO_HORIZONTAL)
				l_vPos.x = e_vPercentage.x*l_vBGRect.Width() + l_vBGRect.x;
			else
				l_vPos.y = e_vPercentage.y*l_vBGRect.Height() + l_vBGRect.y;
			m_pSliderImage->SetWorldPosition(l_vPos);
		}
	}
//end namespace FATMING_CORE
}
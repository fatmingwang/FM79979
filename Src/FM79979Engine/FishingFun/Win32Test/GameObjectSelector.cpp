#include "stdafx.h"
#include "GameObjectSelector.h"
#include "FishRodAndFishThreadData.h"

const wchar_t*        cGameObjectSelector::TypeID = L"cGameObjectSelector";

void	FishRodAndFishThreadObjectInfo(NamedTypedObject*e_pSrc,cGameItemInfoButton*e_pGameItemInfoButton,int e_iType)
{
	if( e_iType == 0 )
	{
		cFishRodData*l_pFishRodData = (cFishRodData*)e_pSrc;
		e_pGameItemInfoButton->SetName(l_pFishRodData->GetName());
		e_pGameItemInfoButton->ChangeText(0,ValueToStringW((int)l_pFishRodData->GetCurrentLVData()->fHookRisePercent).c_str());
		e_pGameItemInfoButton->ChangeText(1,ValueToStringW((int)l_pFishRodData->GetCurrentLVData()->iFishLeaveCost).c_str());
		e_pGameItemInfoButton->ChangeText(2,ValueToStringW((int)l_pFishRodData->GetCurrentLVData()->fPowerTime).c_str());
	}
	else
	if( e_iType == 1 )
	{
		cFishThread*l_pFishThread = (cFishThread*)e_pSrc;
		e_pGameItemInfoButton->SetName(l_pFishThread->GetName());
		float	l_fDepth = l_pFishThread->GetTotalDepth();
		float	l_fEnergy = l_pFishThread->m_fEnergy;
		float	l_fCost = l_pFishThread->m_fCost;
		const WCHAR*l_strName = l_pFishThread->GetName();
		e_pGameItemInfoButton->ChangeText(0,ValueToStringW((int)l_fDepth).c_str());
		e_pGameItemInfoButton->ChangeText(1,ValueToStringW((int)l_fEnergy).c_str());
		e_pGameItemInfoButton->ChangeText(2,ValueToStringW((int)l_fCost).c_str());
		e_pGameItemInfoButton->ChangeText(3,l_strName);
	}
}

cGameItemInfoButton::cGameItemInfoButton()
{
	m_pDescriptionFontFrameVector = 0;
}

cGameItemInfoButton::cGameItemInfoButton(cGameItemInfoButton*e_pGameItemInfoButton):cButtonDialog(e_pGameItemInfoButton)
{
	m_pDescriptionFontFrameVector = 0;
	if(e_pGameItemInfoButton->m_pDescriptionFontFrameVector)
	{
		m_pDescriptionFontFrameVector = dynamic_cast<cRenderObjectVector*>(e_pGameItemInfoButton->m_pDescriptionFontFrameVector->Clone());
		m_pDescriptionFontFrameVector->SetParent(this,false);
	}
}

cGameItemInfoButton::~cGameItemInfoButton()
{
	SAFE_DELETE(m_pDescriptionFontFrameVector);
}

cGameItemInfoButton::cGameItemInfoButton(TiXmlElement*e_pElement)
{
	m_pDescriptionFontFrameVector = 0;
	assert(!wcscmp(e_pElement->Value(),L"cGameItemInfoButton"));
	const WCHAR*l_strCollisionRange = e_pElement->Attribute(L"CollisionRange");
	e_pElement = e_pElement->FirstChildElement();
	while( e_pElement )
	{
		const WCHAR*l_strValue = e_pElement->Value();
		COMPARE_VALUE_WITH_DEFINE(cButtonDialog::TypeID)
		{
			ProcessData(e_pElement);
		}
		else
		COMPARE_VALUE_WITH_DEFINE(cRenderObjectVector::TypeID)
		{
			assert(m_pDescriptionFontFrameVector == 0);
			m_pDescriptionFontFrameVector = cRenderObjectVector::GetMe(e_pElement);
			m_pDescriptionFontFrameVector->SetParent(this,false);
		}
		else
		COMPARE_VALUE("Name")
		{
			SetName(l_strValue);
		}
		e_pElement = e_pElement->NextSiblingElement();
	}
	if( l_strCollisionRange )
		m_vCollisionRange = GetVector4(l_strCollisionRange);
}

cGameItemInfoButton*cGameItemInfoButton::GetMe(TiXmlElement*e_pElement)
{
	cGameItemInfoButton*l_pGameItemInfoButton = new cGameItemInfoButton(e_pElement);
	return l_pGameItemInfoButton;
}

void	cGameItemInfoButton::ChangeText(int e_iIndex,std::wstring e_strText)
{
	ChangeText(e_iIndex,e_strText.c_str());
}

void	cGameItemInfoButton::ChangeText(int e_iIndex,const WCHAR*e_strText)
{
	if( m_pDescriptionFontFrameVector )
	{
		m_pDescriptionFontFrameVector->ChangeData(e_iIndex,e_strText);
	}
}

void	cGameItemInfoButton::Update(float e_fElpaseTime)
{
	cButtonDialog::Update(e_fElpaseTime);
	if( m_pDescriptionFontFrameVector )
		m_pDescriptionFontFrameVector->Update(e_fElpaseTime);
}

void	cGameItemInfoButton::Render()
{
	cButtonDialog::Render();
	if( m_pDescriptionFontFrameVector )
	{
		cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(0,0,1,1));
		m_pDescriptionFontFrameVector->Render();
		cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1,1,1,1));
	}
}

void	cGameItemInfoButton::ChangeBG(const WCHAR*e_strPuzzeleImageUnitName,const WCHAR*e_strPuzzleImageName)
{
	if(m_pBG)
	{
		cSubMPDI*l_pSubMPDI = dynamic_cast<cSubMPDI*>(m_pBG);
		cPuzzleImage*l_pPuzzleImage = l_pSubMPDI->GetPuzzleImage(e_strPuzzleImageName);
		if( !l_pPuzzleImage )
			l_pPuzzleImage = l_pSubMPDI->GetPuzzleImage();
		int	l_iImageIndex = l_pPuzzleImage->GetObjectIndexByName(e_strPuzzeleImageUnitName);
		assert(l_iImageIndex != -1);
		l_pSubMPDI->ChangeAllImageByImageIndex(l_iImageIndex);
		l_pSubMPDI->SetAnimationLoop(true);
	}
}

cGameObjectSelector::cGameObjectSelector(TiXmlElement*e_pTemplateElement)
{
	m_pGameItemInfoReferenceData = 0;
	AssignGameObjectInfoFun = FishRodAndFishThreadObjectInfo;
	m_pButtonDialog = 0;
	m_pTemplateElement = new TiXmlElement(*e_pTemplateElement);
	m_pGameItemInfoButtonTemplateElement = GetXmlElementByNameFromElement(cGameItemInfoButton::TypeID,e_pTemplateElement);
	TiXmlElement*l_pXmlElement = m_pTemplateElement->FirstChildElement();
	const WCHAR*l_strName = e_pTemplateElement->Attribute(L"Name");
	if( l_strName )
		this->SetName(l_strName);
	while( l_pXmlElement )
	{
		const WCHAR*l_strValue = l_pXmlElement->Value();
		COMPARE_VALUE_WITH_DEFINE(cButtonDialog::TypeID)
		{
			assert(m_pButtonDialog == 0);
			m_pButtonDialog = cButtonDialog::GetMe(l_pXmlElement);
		}
		else
		COMPARE_VALUE("cGameItemInfoButton")
		{
			cGameItemInfoButton*l_pGameItemInfoButton = cGameItemInfoButton::GetMe(l_pXmlElement);
			if( l_pGameItemInfoButton )
				this->AddObjectNeglectExist(l_pGameItemInfoButton);		
		}
		l_pXmlElement = l_pXmlElement->NextSiblingElement();
	}
}

cGameObjectSelector::cGameObjectSelector(cGameObjectSelector*e_pGameObjectSelector)
{
	m_pButtonDialog = 0;
	m_pTemplateElement = 0;
}

cGameObjectSelector::~cGameObjectSelector()
{
	SAFE_DELETE(m_pTemplateElement);
	SAFE_DELETE(m_pButtonDialog);
	Destroy();
}

void	cGameObjectSelector::Destroy()
{
	cFMWorkingObjectChanger<cGameItemInfoButton>::Destroy();
}

void	cGameObjectSelector::SetGameItemInfoReferenceData(cNamedTypedObjectVector<NamedTypedObject>*e_pTargetSource,int e_iType)
{
	m_pGameItemInfoReferenceData = e_pTargetSource;
	if( m_pGameItemInfoReferenceData )
	{
		cGameItemInfoButton*l_pGameItemInfoButton = this->GetObject(0);
		this->RemoveObjectWithoutDelete(0);
		int	l_iCount = e_pTargetSource->Count();
		for( int i=0;i<l_iCount;++i )
		{
			cGameItemInfoButton*l_pCloneGameItemInfoButton = new cGameItemInfoButton(l_pGameItemInfoButton);
			l_pCloneGameItemInfoButton->ChangeBG(e_pTargetSource->GetObject(i)->GetName());
			this->AddObjectNeglectExist(l_pCloneGameItemInfoButton);
			if( AssignGameObjectInfoFun )
			{
				if( AssignGameObjectInfoFun && l_pGameItemInfoButton )
					AssignGameObjectInfoFun(m_pGameItemInfoReferenceData->GetObject(i),l_pCloneGameItemInfoButton,e_iType);
			}
		}
		SAFE_DELETE(l_pGameItemInfoButton);
		this->SetCurrentWorkingObject(0);
	}
}

void	cGameObjectSelector::Update(float e_fElpaseTime)
{
	if( m_pButtonDialog )
	{
		m_pButtonDialog->Update(e_fElpaseTime);
		if( m_pButtonDialog->IsSatisfiedCondition() )
		{
			int	l_iCurrentSelectIndex = this->GetObjectIndexByPointer(m_pCurrentWorkingObject);
			int	l_iCount = this->Count();
			int	l_iIndex = -1;
			if( m_pButtonDialog->GetYesImageButton()->IsSatisfiedCondition() )
			{
				l_iIndex = UT::GetLoopIndex(l_iCurrentSelectIndex+1,l_iCount);
			}
			else
			{
				l_iIndex = UT::GetLoopIndex(l_iCurrentSelectIndex-1,l_iCount);
			}
			this->SetCurrentWorkingObject(l_iIndex);
			m_pButtonDialog->Init();
		}
	}
	if(this->m_pCurrentWorkingObject)
	{
		m_pCurrentWorkingObject->Update(e_fElpaseTime);
	}
}

void	cGameObjectSelector::Render()
{
	if( m_pButtonDialog )
		m_pButtonDialog->Render();
	if(this->m_pCurrentWorkingObject)
		m_pCurrentWorkingObject->Render();
}

void    cGameObjectSelector::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_pButtonDialog )
		m_pButtonDialog->MouseDown(e_iPosX,e_iPosY);
	if(this->m_pCurrentWorkingObject)
		m_pCurrentWorkingObject->MouseDown(e_iPosX,e_iPosY);
}

void    cGameObjectSelector::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pButtonDialog )
		m_pButtonDialog->MouseMove(e_iPosX,e_iPosY);
	if(this->m_pCurrentWorkingObject)
		m_pCurrentWorkingObject->MouseMove(e_iPosX,e_iPosY);
}

void    cGameObjectSelector::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pButtonDialog )
		m_pButtonDialog->MouseUp(e_iPosX,e_iPosY);
	if(this->m_pCurrentWorkingObject)
		m_pCurrentWorkingObject->MouseUp(e_iPosX,e_iPosY);
}

bool	cGameObjectSelector::IsSatisfiedCondition()
{
	return false;
}

const WCHAR*	cGameObjectSelector::GetSelectItemName()
{
	if( m_pCurrentWorkingObject )
		return m_pCurrentWorkingObject->GetName();
	return NULL;
}
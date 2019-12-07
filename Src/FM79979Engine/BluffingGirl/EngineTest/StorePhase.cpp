#include "stdafx.h"
#include "StorePhase.h"
#include "IABSetup.h"
#include "AllPhaseDefine.h"
#include "MyJni.h"
#include "PlayerData.h"
#include "BluffingGirlApp.h"
#include "ChoiceGirlPhase.h"
#include "HeartRecoverSelectGirlPhase.h"
#include "StageClearFile.h"
//#include "StageClearFile.h"
const char*g_strFirstBuyFileName = "xzuupoi3958304.bin";
void	BuyGirl(void*e_pData)
{
	cSimplePhase*l_pPhase = cBluffingGirlApp::GetCurrentPhase();
	if( l_pPhase )
	{
		const WCHAR*l_strCurrentSelectGirlName = 0;
		if(l_pPhase->IsSameName(HEART_RECOVER_SELECTGIRL_PHASE_NAME))
		{
			cHeartRecoverSelectGirlPhase*l_pHeartRecoverSelectGirlPhase = (cHeartRecoverSelectGirlPhase*)l_pPhase;
			l_strCurrentSelectGirlName = l_pHeartRecoverSelectGirlPhase->CurrentSelectGirlName();
		}
		else
		if(l_pPhase->IsSameName(CHOICE_GIRL_PHASE_NAME))
		{
			cChoiceGirlPhase*l_pChoiceGirlPhase = (cChoiceGirlPhase*)l_pPhase;
			l_strCurrentSelectGirlName = l_pChoiceGirlPhase->CurrentSelectGirlName();
		}
		if( l_strCurrentSelectGirlName )
		{
			std::string	l_strProductName = UT::WcharToChar(l_strCurrentSelectGirlName);
			PurchaseProduct(l_strProductName.c_str(),ePT_MANAGED,"Test");
			//IsProductsPurchased(char*e_strProductionID,ePurchaseType e_ePurchaseType);
			//std::wstring	l_strFileName = l_strCurrentSelectGirlName;
			//l_strFileName += ".xml";
		}
	}
}

cStorePhase::cStorePhase()
{
	cBluffingGirlApp::AddExternalFunction(BuyGirl,L"BuyGirl");
	m_pBGMPDI = 0;
	m_bWaitForRespond = false;
	m_pIABSetup = 0;
	m_pIABPI = 0;
	Vector4	l_vCollideRange(0,0,cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	m_pScroller = new cScroller((cPuzzleImage*)0,l_vCollideRange,0,0,false,0);
	m_pScroller->SetOutOfWorking(false);
	this->SetName(STORE_PHASE_NAME);
}

cStorePhase::~cStorePhase()
{
	SAFE_DELETE(m_pScroller);
	SAFE_DELETE(m_pIABSetup);
	if( m_pIABPI )
		cGameApp::m_spImageParser->RemoveObject(m_pIABPI);
	m_pIABPI = 0;
}
//<cStorePhase PI="aabb.pi" IABSetupFile="">
//	<ProductData Image="AABB" ProductName="" />
//</cStorePhase>

void	cStorePhase::DisableFirstBuy()
{
	if(UT::IsFileExists(g_strFirstBuyFileName))
	{
		int	l_iCount = m_pScroller->Count();
		for(int i=0;i<l_iCount;++i  )
		{
			cClickMouseBehavior*l_p = m_pScroller->GetObject(i);
			if( l_p->IsSameName(L"FirstBuy") )
			{
				l_p->SetEnable(false);
				return;
			}
		}
	}
}

void	cStorePhase::ProcessIABMenuData(TiXmlElement*e_pTiXmlElement)
{
	assert(m_pIABPI == 0);
	assert(m_pIABSetup== 0);
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("PI")
		{
			m_pIABPI = cGameApp::GetPuzzleImageByFileName(l_strValue);
		}
		else
		COMPARE_NAME("IABSetupFile")
		{
			m_pIABSetup = new cIABSetup(UT::WcharToChar(l_strValue).c_str());
		}
		else
		COMPARE_NAME("ScrollRect")
		{
			m_pScroller->SetCollisionRange(VALUE_TO_VECTOR4);
		}
		else
		COMPARE_NAME("ItemGap")
		{
			m_pScroller->SetObjectGap(VALUE_TO_FLOAT);
		}
	PARSE_NAME_VALUE_END
}

void	cStorePhase::ProcessScrollerData(TiXmlElement*e_pTiXmlElement)
{
	cPuzzleImageUnit*l_pPIUnit = 0;
	cPuzzleImageUnit*l_pPIUnitClone = 0;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("Image")
		{
			if( m_pIABPI )
			{
				l_pPIUnit = m_pIABPI->GetObject(l_strValue);
				l_pPIUnitClone = new cPuzzleImageUnit(l_pPIUnit);
			}
		}
		else
		COMPARE_NAME("ProductName")
		{
			if( l_pPIUnitClone )
				l_pPIUnitClone->SetName(l_strValue);
		}
	PARSE_NAME_VALUE_END
	if( l_pPIUnitClone )
	{
		m_pScroller->AddObject(dynamic_cast<cBaseImage*>(l_pPIUnitClone),true);
	}
	else
	{//test code
		//cTextButton*l_pTextButton = new cTextButton(Vector2(l_vPos.x,l_vPos.y),cGameApp::m_spGlyphFontRender,Vector4(1,0,0,1),L"Test11111");
		//l_pTextButton->SetLocalPosition(l_vPos);
		//m_pScroller->AddData(l_pTextButton);
	}
}

void	cStorePhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("IABMenu")
	{
		ProcessIABMenuData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("ProductData")
	{
		ProcessScrollerData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("BG")
	{
		if( e_pTiXmlElement->FirstChildElement() )
			m_pBGMPDI = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
		if( m_pBGMPDI )
			m_pBGMPDI->Init();
	}
	else
	COMPARE_VALUE("CancelButton")
	{
		if( e_pTiXmlElement->FirstChildElement() )
		{
			cSubMPDI*l_pSubMPDI = cSubMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
			m_pScroller->SetCancelButton(l_pSubMPDI);
		}
	}
}


void    cStorePhase::Init()
{
	if( m_pScroller )
	{
		m_pScroller->Init();
		DisableFirstBuy();
	}
}

void	cStorePhase::SetWaitForRespnd(bool e_bEnable)
{
	this->m_bWaitForRespond = e_bEnable;
}

void    cStorePhase::Update(float e_fElpaseTime)
{
	if( m_pBGMPDI )
		m_pBGMPDI->Update(e_fElpaseTime);
	if( m_bWaitForRespond )
		return;
	if( m_pScroller )
	{
		if(m_pScroller->IsChancelButtonSatisfied())
		{
			this->m_bSatisfiedCondition = true;
			return;
		}
		if( m_pScroller->IsSatisfiedCondition() )
		{
			m_bWaitForRespond = true;
			const WCHAR*l_strItemName = m_pScroller->GetCurrentWorkingObjectName();
			sProductInfo*l_pProductInfo = m_pIABSetup->GetProductInfo(l_strItemName);
			if( l_pProductInfo )
			{
				m_strSelectedItemName = m_pScroller->GetCurrentWorkingObjectName();
				PurchaseProduct(UT::WcharToChar(l_pProductInfo->strName.c_str()).c_str(),l_pProductInfo->PurchaseType,"79979");
				m_pScroller->Init();
			}
			else
			{
				UT::ErrorMsg(l_strItemName,L"no such item!!");
				this->m_bWaitForRespond = false;
			}
		}
		else
			m_pScroller->Update(e_fElpaseTime);
	}
}

void	cStorePhase::AddHeart(int e_iCount)
{
	cBluffingGirlApp::m_spPlayerData->HeartAdd(e_iCount,true);
}

void	cStorePhase::AddClairvoyant(int e_iCount)
{
	cBluffingGirlApp::m_spPlayerData->ClairvoyantAdd(e_iCount);
}

void	cStorePhase::MakeItemPurchase()
{
	const WCHAR*l_strItemName = m_strSelectedItemName.c_str();//m_pScroller->GetCurrentWorkingObjectName();
	MakeItemPurchase(l_strItemName);
}


void	cStorePhase::MakeItemPurchase(const WCHAR*e_strProductName)
{
	cGameApp::OutputDebugInfoString(L"MakeItemPurchase");
	cGameApp::OutputDebugInfoString(e_strProductName);
	if( !wcscmp(e_strProductName,L"Glasses") )
	{
		this->AddClairvoyant(1);
	}
	else
	if( !wcscmp(e_strProductName,L"Glasses10") )
	{
		this->AddClairvoyant(10);
	}
	else
	if( !wcscmp(e_strProductName,L"Glasses25") )
	{
		this->AddClairvoyant(25);
	}
	else
	if( !wcscmp(e_strProductName,L"Glasses50") )
	{
		this->AddClairvoyant(50);
	}
	else
	if( !wcscmp(e_strProductName,L"Heart") )
	{
		this->AddHeart(10);
	}
	else
	if( !wcscmp(e_strProductName,L"FirstBuy") )
	{
		if(!UT::IsFileExists(g_strFirstBuyFileName))
		{
			cSkipHeaderBinaryFile*l_pFile = new cSkipHeaderBinaryFile(15);
			l_pFile->Writefile(g_strFirstBuyFileName,true,true);
			SAFE_DELETE(l_pFile);
			DisableFirstBuy();
			this->AddHeart(20);
			this->AddClairvoyant(3);
		}
	}
	else
	{
		if( wcslen(e_strProductName) > 4 )
		{
			WCHAR l_strTemp[MAX_PATH];
			//firl#
			memcpy(l_strTemp,e_strProductName,sizeof(WCHAR)*4);
			if( l_strTemp[0] == L'G' &&
				l_strTemp[1] == L'i' &&
				l_strTemp[2] == L'r' &&
				l_strTemp[3] == L'l')
			{//unlock girl
				cStageClearFile::GirlUnlock(e_strProductName);
				cSimplePhase*l_pPhase = cBluffingGirlApp::GetCurrentPhase();
				if(l_pPhase->IsSameName(HEART_RECOVER_SELECTGIRL_PHASE_NAME))
				{
					cHeartRecoverSelectGirlPhase*l_pHeartRecoverSelectGirlPhase = (cHeartRecoverSelectGirlPhase*)l_pPhase;
					l_pHeartRecoverSelectGirlPhase->RecheckUnLockGirl();
				}
				else
				if(l_pPhase->IsSameName(CHOICE_GIRL_PHASE_NAME))
				{
					cChoiceGirlPhase*l_pChoiceGirlPhase = (cChoiceGirlPhase*)l_pPhase;
					l_pChoiceGirlPhase->RecheckUnLockGirl();
				}
			}
			else
			{
				UT::ErrorMsg(e_strProductName,L"item name no match IAB function");
			}
		}
		else
		{
			UT::ErrorMsg(e_strProductName,L"item name no match IAB function");
		}
	}
	cGameApp::OutputDebugInfoString(L"MakeItemPurchaseDone");
}

bool	cStorePhase::IsPurchaseItem()
{
	return m_pScroller->IsSatisfiedCondition();
}

//const	WCHAR*cStorePhase::GetPurchaseProductName()
//{
//	if( m_pScroller->IsSatisfiedCondition() )
//		return m_pScroller->GetCurrentWorkingObjectName();
//	return 0;
//}

void    cStorePhase::Render()
{
	//if( !m_bEnable )
	//	return;
	if( m_pBGMPDI )
		m_pBGMPDI->Render();
	if( m_pScroller )
		m_pScroller->Render();
}

void    cStorePhase::DebugRender()
{
	if( m_bWaitForRespond )
		return;
	if( m_pScroller )
		m_pScroller->DebugRender();
}

void    cStorePhase::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_bWaitForRespond )
		return;
	if( m_pScroller )
		m_pScroller->MouseMove(e_iPosX,e_iPosY);
}

void    cStorePhase::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_bWaitForRespond )
		return;
	if( m_pScroller )
		m_pScroller->MouseDown(e_iPosX,e_iPosY);
}

void    cStorePhase::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_bWaitForRespond )
		return;
	if( m_pScroller )
		m_pScroller->MouseUp(e_iPosX,e_iPosY);
}

void	cStorePhase::FetchData(const WCHAR*e_strPhaseName,void*e_pData)
{
	m_strLastPhaseName = e_strPhaseName;
}

const   WCHAR*	cStorePhase::GetNextPhaseName()
{
	return m_strLastPhaseName.c_str();
}
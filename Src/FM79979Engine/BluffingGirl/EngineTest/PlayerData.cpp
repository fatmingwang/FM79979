#include "stdafx.h"
#include "PlayerData.h"
//<Item Name="Clairvoyant" Maxium="99" Cosumble="1">
//	<cImageButton PI="Image/Item/Item.pi" Image="Clairvoyant" Pos="900,0">
//	<cNumeralImage PI="" Image0="" Image9=""/>
//</Item>
cCosumbleItem::cCosumbleItem(TiXmlElement*e_pElement)
{
	m_pTimeCounter = 0;
	m_iLessThanToRecovery = 0;
	m_iRecoverReachedTimeAddItemCount = 1;
	m_iCount = 0;
	m_fRestRecoverTime = 999999.f;
	m_iMaximumCount = 0;
	m_fRecoverTime = 99999.f;
	gettimeofday(&m_LastTimeGetItem,0);
	m_bCosumbleable = false;
	m_bItemChanged = false;
	m_pNumeralImage = 0;
	m_pImageButton = 0;

	PARSE_ELEMENT_START(e_pElement)
		COMPARE_NAME("Name")
		{
			this->SetName(l_strValue);
		}
		else
		COMPARE_NAME("Maximum")
		{
			m_iMaximumCount = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("Cosumble")
		{
			m_bCosumbleable = VALUE_TO_BOOLEAN;
		}
		else
		COMPARE_NAME("RecoverReachedTimeAddItemCount")
		{
			m_iRecoverReachedTimeAddItemCount = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("RecoverTime")
		{
			m_fRecoverTime = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("LessThanToRecovery")
		{
			m_iLessThanToRecovery = VALUE_TO_INT;
		}
	PARSE_NAME_VALUE_END
	e_pElement = e_pElement->FirstChildElement();
	while( e_pElement )
	{
		COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pElement,cImageButton::TypeID)
		{
			m_pImageButton = dynamic_cast<cImageButton*>(GetRenderObjectBehavior(e_pElement));
		}
		else
		COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pElement,cNumeralImage::TypeID)
		{
			m_pNumeralImage = dynamic_cast<cNumeralImage*>(GetRenderObjectBehavior(e_pElement));
		}
		else
		COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pElement,cTimeNumerialImage::TypeID)
		{
			m_pTimeCounter = cTimeNumerialImage::GetMe(e_pElement);
			if( m_pTimeCounter )
				m_pTimeCounter->Init();
		}
		e_pElement = e_pElement->NextSiblingElement();
	}
	
}

cCosumbleItem::cCosumbleItem(int e_iMaximumCount,float e_fRecoverTime)
{
	m_pTimeCounter = 0;
	m_iLessThanToRecovery = 0;
	m_iRecoverReachedTimeAddItemCount = 1;
	m_iCount = 0;
	m_fRestRecoverTime = 999999.f;
	m_iMaximumCount = e_iMaximumCount;
	m_fRecoverTime = e_fRecoverTime;
	gettimeofday(&m_LastTimeGetItem,0);
	m_pImageButton = 0;
	m_pNumeralImage = 0;
	m_bCosumbleable = false;;
	m_bItemChanged = false;
	m_pImageButton = 0;
	m_pNumeralImage = 0;
}

cCosumbleItem::~cCosumbleItem()
{
	SAFE_DELETE(m_pNumeralImage);
	SAFE_DELETE(m_pImageButton);
	SAFE_DELETE(m_pTimeCounter);
}

bool	cCosumbleItem::ItemAdd(int e_iCount,bool e_bItemChangeByTime)
{
	if(!IsAddable(e_iCount))
		return false;
	m_iCount += e_iCount;
	if( m_iCount > m_iMaximumCount )
		m_iCount = m_iMaximumCount;
	if(e_bItemChangeByTime)
		gettimeofday(&m_LastTimeGetItem,0);
	m_bItemChanged = true;
	return true;
}

bool	cCosumbleItem::ItemMinus(int e_iCount,bool e_bItemChangeByTime)
{
	if( !IsMinusable(e_iCount) )
		return false;
	m_iCount -= e_iCount;
	if( e_bItemChangeByTime || m_iCount == this->m_iLessThanToRecovery-1 )
		gettimeofday(&m_LastTimeGetItem,0);
	m_bItemChanged = true;
	return true;
}

bool	cCosumbleItem::IsAddable(int e_iCount)
{
	if( m_iCount == this->m_iMaximumCount )
		return false;
	return true;
}

bool	cCosumbleItem::IsMinusable(int e_iCount)
{
	if( m_iCount-e_iCount < 0)
		return false;
	return true;
}

void    cCosumbleItem::Init()
{
	if( m_pImageButton )
		m_pImageButton->Init();
	if(this->m_iCount >= this->m_iMaximumCount)
		m_iCount = m_iMaximumCount;
}

void	cCosumbleItem::Update(float e_fElpaseTime)
{
	if( m_pImageButton )
		m_pImageButton->Update(e_fElpaseTime);
	if( m_fRecoverTime <= 0.f || m_iCount >= m_iLessThanToRecovery )
	{
		return;
	}
	float	l_fTimeDiff = cGameApp::m_sTimeAndFPS.GetSecondDiff(m_LastTimeGetItem);
	m_fRestRecoverTime = m_fRecoverTime-l_fTimeDiff;
	if( m_fRestRecoverTime <= 0.f )
	{
		if( -m_fRestRecoverTime > m_fRecoverTime )
		{
			int	l_iCount = (int)(-m_fRestRecoverTime/m_fRecoverTime);
			if( l_iCount > m_iLessThanToRecovery )
				l_iCount = m_iLessThanToRecovery-this->m_iCount;
			for( int i=0;i<l_iCount;++i )
			{
				this->ItemAdd(m_iRecoverReachedTimeAddItemCount,true);
			}
		}
		else
			this->ItemAdd(m_iRecoverReachedTimeAddItemCount,true);
		m_fRestRecoverTime = m_fRecoverTime;
		return;
	}
}

void	cCosumbleItem::Render()
{
	if( m_pImageButton )
		m_pImageButton->Render();
	if( m_pNumeralImage )
	{
		m_pNumeralImage->SetValue(this->m_iCount);
		m_pNumeralImage->Render();
	}
	if( m_fRecoverTime > 0.f && m_iCount < this->m_iLessThanToRecovery )
	{
		if( m_pTimeCounter )
		{
			if( m_fRestRecoverTime > 0.f )
				m_pTimeCounter->SetValue((int64)m_fRestRecoverTime);
			else
				m_pTimeCounter->SetValue((int64)0);
			m_pTimeCounter->Render();
		}
		//RenderRestTime();
	}
	else
	{
		if( m_pTimeCounter )
		{
			m_pTimeCounter->SetValue(0);
			m_pTimeCounter->SetColor(Vector4::HalfBrightWithHalfAlpha);
			m_pTimeCounter->Render();
			m_pTimeCounter->SetColor(Vector4::One);
		}
	}
}

void	cCosumbleItem::RenderRestTime()
{
	Vector3	l_vPos = m_pNumeralImage->GetWorldPosition();
	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1,1,0,1));
	cGameApp::RenderFont(l_vPos.x,l_vPos.y,ValueToStringW(m_fRestRecoverTime).c_str());
	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1,1,1,1));
}

void    cCosumbleItem::MouseDown(int e_iPosX,int e_iPosY)
{
	if( this->m_bCosumbleable )
	{
		if( m_pImageButton )
			m_pImageButton->MouseDown(e_iPosX,e_iPosY);
	}
}

void    cCosumbleItem::MouseMove(int e_iPosX,int e_iPosY)
{
	if( this->m_bCosumbleable )
	{
		if( m_pImageButton )
			m_pImageButton->MouseMove(e_iPosX,e_iPosY);
	}
}

void    cCosumbleItem::MouseUp(int e_iPosX,int e_iPosY)
{
	if( this->m_bCosumbleable )
	{
		if( m_pImageButton )
			m_pImageButton->MouseUp(e_iPosX,e_iPosY);
	}
}

cClickMouseBehavior*g_pDebugHeartIncrease = 0;
cClickMouseBehavior*g_pDebugShakeIncrease = 0;
cClickMouseBehavior*g_pDebugClairvoyantIncrease = 0;

cPlayerData::cPlayerData(char*e_strPlayerDataFileName,char*e_strPlayerSaveFileName)
{
	m_bEnableRender = true;
	g_pDebugHeartIncrease = new cClickMouseBehavior();
	g_pDebugHeartIncrease->SetKeyData(49);
	g_pDebugShakeIncrease = new cClickMouseBehavior();
	g_pDebugShakeIncrease->SetKeyData(50);
	g_pDebugClairvoyantIncrease = new cClickMouseBehavior();
	g_pDebugClairvoyantIncrease->SetKeyData(51);
	m_AllItems.SetFromResource(true);
	m_pUIBar = 0;
	m_pHeart = 0;
	m_pClairvoynat = 0;
	m_pShockTime = 0;
	m_bUsingClairvoyant = false;
	m_bItemClickEnable = true;
	m_strPlayerSaveFileName = e_strPlayerSaveFileName;
	PARSE_FAILED_MESSAGE_BOX(this,e_strPlayerDataFileName);
	int	l_iMagicId = 79979;
#ifdef ANDROID
	l_iMagicId = GetDeviceIDToNumber();
#endif
	m_pItemFile = new cSkipHeaderBinaryFile(l_iMagicId);
	bool	l_bParseOk = false;
	if(m_pItemFile->Openfile(m_strPlayerSaveFileName.c_str(),"rb+"))
	{
		l_bParseOk = ParsePlayerSaveFile();
	}
	if( !l_bParseOk )
	{//file lost...set to default.
		//write num hearts,default is 5
		//write num Clairvoyant,default is 0
		m_pItemFile->CloseFile();
		gettimeofday(&m_pHeart->m_LastTimeGetItem,0);
		if( m_pClairvoynat )
			m_pClairvoynat->m_iCount = 1;
		if( m_pHeart )
			m_pHeart->m_iCount = 5;
		if( m_pShockTime )
			m_pShockTime->m_iCount = 3;
		gettimeofday(&m_pShockTime->m_LastTimeGetItem,0);
		Write();
	}
	m_pItemFile->CloseFile();
}

cPlayerData::~cPlayerData()
{
	Write();
	SAFE_DELETE(m_pUIBar);
	SAFE_DELETE(m_pHeart);
	SAFE_DELETE(m_pClairvoynat);
	SAFE_DELETE(m_pItemFile);
	SAFE_DELETE(m_pShockTime);
	SAFE_DELETE(g_pDebugHeartIncrease);
	SAFE_DELETE(g_pDebugShakeIncrease);
	SAFE_DELETE(g_pDebugClairvoyantIncrease);
}

void	cPlayerData::ProcessItemData(TiXmlElement*e_pTiXmlElement)
{
	cCosumbleItem*l_pCosumbleItem = new cCosumbleItem(e_pTiXmlElement);
	m_AllItems.AddObjectNeglectExist(l_pCosumbleItem);
	if(l_pCosumbleItem->IsSameName(L"Heart"))
	{
		assert( m_pHeart == 0 );
		this->m_pHeart = l_pCosumbleItem;
	}
	else
	if(l_pCosumbleItem->IsSameName(L"clairvoyant"))
	{
		assert( m_pClairvoynat == 0 );
		this->m_pClairvoynat = l_pCosumbleItem;
	}
	else
	if(l_pCosumbleItem->IsSameName(L"ShockTime"))
	{
		assert( m_pShockTime == 0 );
		m_pShockTime = l_pCosumbleItem;
	}
	else
	{
		UT::ErrorMsg(l_pCosumbleItem->GetName(),L"this name not support cPlayerData::ProcessItemData");
	}
}

void	cPlayerData::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	if( !wcscmp(l_strValue,L"Item") )
	{
		ProcessItemData(e_pTiXmlElement);
	}
	else
	if( !wcscmp(l_strValue,L"UIBar") )
	{
		if( e_pTiXmlElement->FirstChildElement() )
			m_pUIBar = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement());
	}
}

void	cPlayerData::Init()
{
	m_bUsingClairvoyant = false;
	if( m_pUIBar )
	{
		m_pUIBar->Init();
	}
}

bool	cPlayerData::ParsePlayerSaveFile()
{
	char*l_pData = (char*)m_pItemFile->GetDataFile(0);
	if( l_pData )
	{
		memcpy(&m_pHeart->m_LastTimeGetItem.tv_sec,l_pData,sizeof(long));			l_pData += sizeof(long);
		memcpy(&m_pHeart->m_LastTimeGetItem.tv_usec,l_pData,sizeof(long));			l_pData += sizeof(long);

		memcpy(&m_pHeart->m_iCount,l_pData,sizeof(int));					l_pData += sizeof(int);
		if( m_pHeart->m_iCount > 99999 || m_pHeart->m_iCount < -1 )
			m_pHeart->m_iCount = 5;
		memcpy(&m_pClairvoynat->m_iCount,l_pData,sizeof(int));				l_pData += sizeof(int);
		if( m_pClairvoynat->m_iCount > 99999 || m_pClairvoynat->m_iCount < -1 )
			m_pClairvoynat->m_iCount = 5;

		memcpy(&m_pShockTime->m_LastTimeGetItem.tv_sec,l_pData,sizeof(long));		l_pData += sizeof(long);
		memcpy(&m_pShockTime->m_LastTimeGetItem.tv_usec,l_pData,sizeof(long));		l_pData += sizeof(long);

		memcpy(&m_pShockTime->m_iCount,l_pData,sizeof(int));
		if( m_pShockTime->m_iCount > 5 || m_pShockTime->m_iCount < -1 )
			m_pShockTime->m_iCount = 5;
		return true;
	}
	return false;
}
//date,UINT
//num Clairvoyant int
//num heart int
void	cPlayerData::Write()
{
	if( m_pItemFile )
	{
		if( !m_pItemFile->GetFile() )
		{
			m_pItemFile->Writefile(m_strPlayerSaveFileName.c_str(),true,true);
		}
		if( m_pItemFile->GetFile() )
		{
			m_pItemFile->SetFilePos(0);
			//write current time
			m_pItemFile->WriteToFile(m_pHeart->m_LastTimeGetItem.tv_sec);
			m_pItemFile->WriteToFile(m_pHeart->m_LastTimeGetItem.tv_usec);

			m_pItemFile->WriteToFile(m_pHeart->m_iCount);
			m_pItemFile->WriteToFile(m_pClairvoynat->m_iCount);

			m_pItemFile->WriteToFile(m_pShockTime->m_LastTimeGetItem.tv_sec);
			m_pItemFile->WriteToFile(m_pShockTime->m_LastTimeGetItem.tv_usec);

			m_pItemFile->WriteToFile(m_pShockTime->m_iCount);
			m_pItemFile->AddGarbageEndFile();
			m_pItemFile->Flush();
			m_pItemFile->CloseFile();
		}
	}
}

bool	cPlayerData::UsingClairvoyant()
{
	if(this->m_pClairvoynat->ItemMinus(1,false))
	{
		m_bUsingClairvoyant = true;
		return true;
	}
	m_bUsingClairvoyant = false;
	return false;
}

bool	cPlayerData::HeartAdd(int e_iCount,bool e_bWriteTimeStame)
{
	if( this->m_pHeart->ItemAdd(e_iCount,e_bWriteTimeStame) )
	{
		return true;
	}
	return false;
}

int	cPlayerData::HeartCount()
{
	return m_pHeart->m_iCount;
}

bool	cPlayerData::HeartMinus()
{
	if( this->m_pHeart->ItemMinus(1,false) )
	{
		return true;
	}
	return false;
}

bool	cPlayerData::ShockTimeMinus()
{
	if( this->m_pShockTime->ItemMinus(1,false) )
	{
		return true;
	}
	return false;
}

void	cPlayerData::Update(float e_fElpaseTime)
{
	m_AllItems.Update(e_fElpaseTime);
	int	l_iCount = m_AllItems.Count();
	bool	l_bItemChanged = false;
	for( int i=0;i<l_iCount;++i )
	{
		if(m_AllItems[i]->m_bItemChanged)
		{
			l_bItemChanged = true;
			m_AllItems[i]->m_bItemChanged = false;
		}
	}
	if( l_bItemChanged )
		this->Write();
	if( m_pUIBar )
		m_pUIBar->Update(e_fElpaseTime);
	if(g_pDebugHeartIncrease)
	{
		g_pDebugHeartIncrease->SingnalProcess();
		if(g_pDebugHeartIncrease->IsSatisfiedCondition())
		{
			g_pDebugHeartIncrease->Init();
			m_pHeart->ItemAdd(1,false);
		}
	}
	if(g_pDebugShakeIncrease)
	{
		g_pDebugShakeIncrease->SingnalProcess();
		if(g_pDebugShakeIncrease->IsSatisfiedCondition())
		{
			g_pDebugShakeIncrease->Init();
			m_pShockTime->ItemAdd(1,false);
		}
	}
	if(g_pDebugClairvoyantIncrease)
	{
		g_pDebugClairvoyantIncrease->SingnalProcess();
		if(g_pDebugClairvoyantIncrease->IsSatisfiedCondition())
		{
			g_pDebugClairvoyantIncrease->Init();
			m_pClairvoynat->ItemAdd(1,false);
		}
	}
}

void	cPlayerData::RenderBG()
{
	if( m_bEnableRender && m_pUIBar )
		m_pUIBar->Render();
}

void	cPlayerData::Render()
{
	if( m_bEnableRender )
		m_AllItems.Render();
}

void	cPlayerData::MouseDown(int e_iX,int e_iY)
{
	if( m_bItemClickEnable )
		m_AllItems.MouseDown(e_iX,e_iY);
}

void	cPlayerData::MouseMove(int e_iX,int e_iY)
{
	if( m_bItemClickEnable )
		m_AllItems.MouseMove(e_iX,e_iY);
}

void	cPlayerData::MouseUp(int e_iX,int e_iY)
{
	if( m_bItemClickEnable )
		m_AllItems.MouseUp(e_iX,e_iY);
}

bool	cPlayerData::IsUsingClairvoyant()
{
	return m_bUsingClairvoyant;
}

int		cPlayerData::ClairvoyantCount()
{
	return m_pClairvoynat->m_iCount;
}

bool	cPlayerData::ClairvoyantAdd(int e_iCount)
{
	if( this->m_pClairvoynat->ItemAdd(e_iCount,false) )
	{
		return true;
	}
	return false;
}
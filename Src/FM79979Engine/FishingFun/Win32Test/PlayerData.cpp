#include "stdafx.h"
#include "PlayerData.h"
#include "GameApp.h"

cPlayerData::cPlayerData()
{
	m_i64Money = 9999;
	m_i64Gold = 9999;
	m_i64Reel = 5;
	m_pNumeralImage = 0;
	m_FishRodDataVector.SetFromResource(true);
}

cPlayerData::~cPlayerData()
{
	SAFE_DELETE(m_pNumeralImage);
}

bool	cPlayerData::ParsePlayerData()
{
	for( int i=0;i<cFishingFunApp::m_spFishRodAndFishThreadData->m_FishRodDataVector.Count();++i )
		m_FishRodDataVector.AddObjectNeglectExist(cFishingFunApp::m_spFishRodAndFishThreadData->m_FishRodDataVector.GetObject(i));
	return true;
}


void	cPlayerData::SetRenderPos(Vector3	e_vGoldShowPos,Vector3	e_vMonetShowPos,Vector3	e_vReelShowPos,cNumeralImage*e_pNumeralImage)
{
	m_vGoldShowPos = e_vGoldShowPos;
	m_vMonetShowPos = e_vMonetShowPos;
	m_vReelShowPos = e_vReelShowPos;
	SAFE_DELETE(m_pNumeralImage);
	m_pNumeralImage = e_pNumeralImage;
}

bool	cPlayerData::MoneyMinus(int e_iValue)
{
	if( m_i64Money >= e_iValue )
	{
		m_i64Money -= e_iValue;
		return true;
	}
	return false;
}

void	cPlayerData::MoneyAdded(int e_iValue)
{
	m_i64Money	+= e_iValue;
}

bool	cPlayerData::GoldMinus(int e_iValue)
{	
	if( m_i64Gold >= e_iValue )
	{
		m_i64Gold -= e_iValue;
		return true;
	}
	return false;
}

void	cPlayerData::GoldAdded(int e_iValue)
{
	m_i64Gold	+= e_iValue;
}

bool	cPlayerData::ReelMinus(int e_iValue)
{	
	if( m_i64Reel >= e_iValue )
	{
		m_i64Reel -= e_iValue;
		return true;
	}
	return false;
}

void	cPlayerData::ReelAdded(int e_iValue)
{
	m_i64Reel	+= e_iValue;
}

void	cPlayerData::Render()
{
	if( m_pNumeralImage )
	{
		glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
		m_pNumeralImage->Draw(this->m_i64Gold,(int)m_vGoldShowPos.x,(int)m_vGoldShowPos.y);
		m_pNumeralImage->Draw(this->m_i64Money,(int)m_vMonetShowPos.x,(int)m_vMonetShowPos.y);
		m_pNumeralImage->Draw(this->m_i64Reel,(int)m_vReelShowPos.x,(int)m_vReelShowPos.y);
	}

}
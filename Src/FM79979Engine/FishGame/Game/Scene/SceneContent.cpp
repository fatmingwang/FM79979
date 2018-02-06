#include "stdafx.h"
#include "SceneContent.h"

bool	cBaseScene::m_bGenerateMonster = true;
cBaseScene::cBaseScene(TiXmlElement*e_pTiXmlElement)
{
	m_pBG = nullptr;
}
cBaseScene::~cBaseScene()
{
}

void	cBaseScene::Init()
{
}

void	cBaseScene::Update(float e_fElpaseTime)
{
}

void	cBaseScene::Render()
{

}

cBossScene::cBossScene(TiXmlElement*e_pTiXmlElement):cBaseScene(e_pTiXmlElement)
{
	
}

cBossScene::~cBossScene()
{

}

void	cBossScene::Init()
{

}

void	cBossScene::Update(float e_fElpaseTime)
{

}

void	cBossScene::Render()
{

}